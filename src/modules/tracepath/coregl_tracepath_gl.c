#include "coregl_tracepath.h"


#include <stdlib.h>

struct _Glbuf_Data {
	int obj_idx;
	int width;
	int height;
	int bpp;
	char format[80];

	struct _Glbuf_Data *next;
};

static void
__addhash_glbuf_object(Glbuf_Data **glbuf, Glbuf_Data *target)
{
	Glbuf_Data *data = glbuf[target->obj_idx % MTD_GLBUF_HASH_ARRAY];
	if (data == NULL) {
		glbuf[target->obj_idx % MTD_GLBUF_HASH_ARRAY] = target;
	} else {
		while (data->next != NULL) {
			AST(data->obj_idx != target->obj_idx);
			data = data->next;
		}
		AST(data->obj_idx != target->obj_idx);
		data->next = target;
	}
	goto finish;

finish:
	return;
}

static void
__removehash_glbuf_object(Glbuf_Data **glbuf, Glbuf_Data **target)
{
	Glbuf_Data *data = glbuf[(*target)->obj_idx % MTD_GLBUF_HASH_ARRAY];
	Glbuf_Data *prev = NULL;
	while (data != NULL) {
		if (data->obj_idx == (*target)->obj_idx) {
			if (prev != NULL)
				prev->next = data->next;
			else
				glbuf[(*target)->obj_idx % MTD_GLBUF_HASH_ARRAY] = data->next;

			free(*target);
			*target = NULL;
			break;
		}
		prev = data;
		data = data->next;
	}
	goto finish;

finish:
	return;
}

static Glbuf_Data *
__findhash_glbuf_object(Glbuf_Data **glbuf, int obj_idx)
{
	Glbuf_Data *data = glbuf[obj_idx % MTD_GLBUF_HASH_ARRAY];
	while (data != NULL) {
		if (data->obj_idx == obj_idx)
			break;
		data = data->next;
	}
	goto finish;

finish:
	return data;
}

void
tracepath_glbuf_clear(Glbuf_Data **glbuf)
{
	int i;

	for (i = 0; i < MTD_GLBUF_HASH_ARRAY; i++) {
		Glbuf_Data *data = glbuf[i];

		while (data) {
			Glbuf_Data *delitm = data;
			data = data->next;
			free(delitm);
			delitm = NULL;
		}
	}
}

static void
_add_glbuf_object(Glbuf_Data **glbuf, int obj_idx, const char *obj_type,
		  int width, int height, int bpp, const char *format)
{
	Glbuf_Data *data = __findhash_glbuf_object(glbuf, obj_idx);
	if (data == NULL) {
		data = (Glbuf_Data *)calloc(1, sizeof(Glbuf_Data));
		data->obj_idx = obj_idx;
		__addhash_glbuf_object(glbuf, data);
	} else {
		// Update
		char ment[MAX_TRACE_NAME_LENGTH];
		snprintf(ment, MAX_TRACE_NAME_LENGTH, "%s(%4dx%4d %s)", obj_type, data->width,
			 data->height,
			 data->format);
		_COREGL_TRACE_MEM_REMOVE(ment, data->width * data->height * data->bpp);
	}

	data->width = width;
	data->height = height;
	data->bpp = bpp;
	snprintf(data->format, 80, "%s", format);
	{
		char ment[MAX_TRACE_NAME_LENGTH];
		snprintf(ment, MAX_TRACE_NAME_LENGTH, "%s(%4dx%4d %s)", obj_type, data->width,
			 data->height,
			 data->format);
		_COREGL_TRACE_MEM_ADD(ment, data->width * data->height * data->bpp);
	}
	goto finish;

finish:
	return;
}

static void
_remove_glbuf_object(Glbuf_Data **glbuf, int obj_idx, const char *obj_type)
{
	Glbuf_Data *data = __findhash_glbuf_object(glbuf, obj_idx);
	if (data == NULL) {
		COREGL_WRN("\E[40;31;1mError deleting %s object %d. (is not created)\E[0m\n",
			   obj_type, obj_idx);
		return;
	}

	{
		char ment[MAX_TRACE_NAME_LENGTH];
		snprintf(ment, MAX_TRACE_NAME_LENGTH, "%s(%4dx%4d %s)", obj_type, data->width,
			 data->height,
			 data->format);
		_COREGL_TRACE_MEM_REMOVE(ment, data->width * data->height * data->bpp);
	}

	__removehash_glbuf_object(glbuf, &data);
	goto finish;

finish:
	return;
}


#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
void
_surface_trace_set(int set, GLint fbname, GLenum attachment,
		   MY_MODULE_TSTATE *tstate)
{
	int atttype = _COREGL_INT_INIT_VALUE;
	_orig_tracepath_glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &atttype);
	AST(atttype != _COREGL_INT_INIT_VALUE);
	int attname = _COREGL_INT_INIT_VALUE;
	_orig_tracepath_glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
			attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &attname);
	AST(attname != _COREGL_INT_INIT_VALUE);
	if (set == 1) {
		switch (atttype) {
		case GL_TEXTURE:
			//COREGL_LOG("FBO DUMPING BEGIN = (TEX)0x%X\n", attname);
		{
			char name[256];
			snprintf(name, sizeof(name), "FBOTEX_0x%X", attname);
			tracepath_surface_trace_add(name, tstate->ctx->dpy, tstate->ctx->handle,
						    tstate->surf_draw, fbname, attname, 0, -1, -1, -1, NULL);
		}
		break;
		case GL_RENDERBUFFER:
			//COREGL_LOG("FBO DUMPING BEGIN = (RB)0x%X\n", attname);
		{
			char name[256];
			snprintf(name, sizeof(name), "FBORB_0x%X", attname);
			tracepath_surface_trace_add(name, tstate->ctx->dpy, tstate->ctx->handle,
						    tstate->surf_draw, fbname, 0, attname, -1, -1, -1, NULL);
		}
		break;
		}
	} else {
		switch (atttype) {
		case GL_TEXTURE:
			//COREGL_LOG("FBO DUMPING END = (TEX)0x%X\n", attname);
		{
			char name[256];
			snprintf(name, sizeof(name), "FBOTEX_0x%X", attname);
			tracepath_surface_trace_add(name, tstate->ctx->dpy, tstate->ctx->handle,
						    tstate->surf_draw, 0, attname, 0, -1, -1, -1, NULL);
		}
		break;
		case GL_RENDERBUFFER:
			//COREGL_LOG("FBO DUMPING END = (RB)0x%X\n", attname);
		{
			char name[256];
			snprintf(name, sizeof(name), "FBORB_0x%X", attname);
			tracepath_surface_trace_add(name, tstate->ctx->dpy, tstate->ctx->handle,
						    tstate->surf_draw, 0, 0, attname, -1, -1, -1, NULL);
		}
		break;
		}
	}
}

void
tracepath_fbdump_update(GLint set)
{
	if (trace_surface_flag == 1) {
		GLint fbname = _COREGL_INT_INIT_VALUE;
		_orig_tracepath_glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbname);
		AST(fbname != _COREGL_INT_INIT_VALUE);
		if (fbname != 0) {
			MY_MODULE_TSTATE *tstate = NULL;

			GET_MY_TSTATE(tstate, get_current_thread_state());
			AST(tstate != NULL);
			if (tstate->ctx != NULL) {
				_surface_trace_set(set, fbname, GL_COLOR_ATTACHMENT0, tstate);
//				_surface_trace_set(set, fbname, GL_DEPTH_ATTACHMENT, tstate);
//				_surface_trace_set(set, fbname, GL_STENCIL_ATTACHMENT, tstate);
			}
		}
	}
}
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

/* GLES 1.1 */
void
tracepath_glAlphaFunc(GLenum func, GLclampf ref)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glAlphaFunc(func, ref);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glClipPlanef(GLenum plane, const GLfloat *equation)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClipPlanef(plane, equation);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glColor4f(red, green, blue, alpha);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glFogf(GLenum pname, GLfloat param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glFogf(pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glFogfv(GLenum pname, const GLfloat *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glFogfv(pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glFrustumf(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glFrustumf(left, right, bottom, top, zNear, zFar);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetClipPlanef(GLenum pname, GLfloat eqn[4])
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetClipPlanef(pname, eqn);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetLightfv(GLenum light, GLenum pname, GLfloat *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetLightfv(light, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetMaterialfv(GLenum face, GLenum pname, GLfloat *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetMaterialfv(face, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetTexEnvfv(GLenum env, GLenum pname, GLfloat *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTexEnvfv(env, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glLightModelf(GLenum pname, GLfloat param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLightModelf(pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glLightModelfv(GLenum pname, const GLfloat *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLightModelfv(pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glLightf(GLenum light, GLenum pname, GLfloat param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLightf(light, pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glLightfv(GLenum light, GLenum pname, const GLfloat *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLightfv(light, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glLoadMatrixf(const GLfloat *m)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLoadMatrixf(m);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glMaterialf(GLenum face, GLenum pname, GLfloat param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMaterialf(face, pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glMaterialfv(GLenum face, GLenum pname, const GLfloat *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMaterialfv(face, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glMultMatrixf(const GLfloat *m)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMultMatrixf(m);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glMultiTexCoord4f(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMultiTexCoord4f(target, s, t, r, q);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glNormal3f(nx, ny, nz);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glOrthof(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glOrthof(left, right, bottom, top, zNear, zFar);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPointParameterf(GLenum pname, GLfloat param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPointParameterf(pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPointParameterfv(GLenum pname, const GLfloat *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPointParameterfv(pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPointSize(GLfloat size)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPointSize(size);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glRotatef(angle, x, y, z);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glScalef(x, y, z);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexEnvf(GLenum target, GLenum pname, GLfloat param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexEnvf(target, pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexEnvfv(GLenum target, GLenum pname, const GLfloat *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexEnvfv(target, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTranslatef(x, y, z);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glAlphaFuncx(GLenum func, GLclampx ref)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glAlphaFuncx(func, ref);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glClearColorx(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClearColorx(red, green, blue, alpha);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glClearDepthx(GLclampx depth)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClearDepthx(depth);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glClientActiveTexture(GLenum texture)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClientActiveTexture(texture);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glClipPlanex(GLenum plane, const GLfixed *equation)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClipPlanex(plane, equation);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glColor4ub(red, green, blue, alpha);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glColor4x(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glColor4x(red, green, blue, alpha);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glColorPointer(size, type, stride, pointer);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDepthRangex(GLclampx zNear, GLclampx zFar)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDepthRangex(zNear, zFar);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDisableClientState(GLenum array)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDisableClientState(array);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glEnableClientState(GLenum array)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glEnableClientState(array);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glFogx(GLenum pname, GLfixed param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glFogx(pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glFogxv(GLenum pname, const GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glFogxv(pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glFrustumx(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glFrustumx(left, right, bottom, top, zNear, zFar);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetClipPlanex(GLenum pname, GLfixed eqn[4])
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetClipPlanex(pname, eqn);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetFixedv(GLenum pname, GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetFixedv(pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetLightxv(GLenum light, GLenum pname, GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetLightxv(light, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetMaterialxv(GLenum face, GLenum pname, GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetMaterialxv(face, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetTexEnviv(GLenum env, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTexEnviv(env, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetTexEnvxv(GLenum env, GLenum pname, GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTexEnvxv(env, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetTexParameterxv(GLenum target, GLenum pname, GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTexParameterxv(target, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glLightModelx(GLenum pname, GLfixed param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLightModelx(pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glLightModelxv(GLenum pname, const GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLightModelxv(pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glLightx(GLenum light, GLenum pname, GLfixed param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLightx(light, pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glLightxv(GLenum light, GLenum pname, const GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLightxv(light, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glLineWidthx(GLfixed width)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLineWidthx(width);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glLoadIdentity(void)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLoadIdentity();
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glLoadMatrixx(const GLfixed *m)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLoadMatrixx(m);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glLogicOp(GLenum opcode)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLogicOp(opcode);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glMaterialx(GLenum face, GLenum pname, GLfixed param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMaterialx(face, pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glMaterialxv(GLenum face, GLenum pname, const GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMaterialxv(face, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glMatrixMode(GLenum mode)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMatrixMode(mode);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glMultMatrixx(const GLfixed *m)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMultMatrixx(m);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glMultiTexCoord4x(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMultiTexCoord4x(target, s, t, r, q);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glNormal3x(GLfixed nx, GLfixed ny, GLfixed nz)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glNormal3x(nx, ny, nz);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glNormalPointer(GLenum type, GLsizei stride, const GLvoid *pointer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glNormalPointer(type, stride, pointer);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glOrthox(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glOrthox(left, right, bottom, top, zNear, zFar);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPointParameterx(GLenum pname, GLfixed param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPointParameterx(pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPointParameterxv(GLenum pname, const GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPointParameterxv(pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPointSizex(GLfixed size)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPointSizex(size);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPolygonOffsetx(GLfixed factor, GLfixed units)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPolygonOffsetx(factor, units);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPopMatrix(void)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPopMatrix();
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPushMatrix(void)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPushMatrix();
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glRotatex(GLfixed angle, GLfixed x, GLfixed y, GLfixed z)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glRotatex (angle, x, y, z);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glSampleCoveragex(GLclampx value, GLboolean invert)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glSampleCoveragex(value, invert);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glScalex(GLfixed x, GLfixed y, GLfixed z)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glScalex (x, y, z);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glShadeModel(GLenum mode)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glShadeModel(mode);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexCoordPointer(size, type, stride, pointer);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexEnvi(GLenum target, GLenum pname, GLint param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexEnvi(target, pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexEnvx(GLenum target, GLenum pname, GLfixed param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexEnvx(target, pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexEnviv(GLenum target, GLenum pname, const GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexEnviv(target, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexEnvxv(GLenum target, GLenum pname, const GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexEnvxv(target, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexParameterx(GLenum target, GLenum pname, GLfixed param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexParameterx(target, pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexParameterxv(GLenum target, GLenum pname, const GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexParameterxv(target, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTranslatex(GLfixed x, GLfixed y, GLfixed z)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTranslatex(x, y, z);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexPointer(size, type, stride, pointer);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

/* Khronos GLES 1.1 CTS requires glPointSizePointerOES as shared library symbol.*/
void
tracepath_glPointSizePointerOES(GLenum type, GLsizei stride, const GLvoid *pointer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPointSizePointerOES(type, stride, pointer);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

/* end of GLES 1.1*/

/* GLES 1.1 extensions */
void
tracepath_glDrawTexsOES(GLshort x, GLshort y, GLshort z, GLshort width, GLshort height)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDrawTexsOES(x, y, z, width, height);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDrawTexiOES(GLint x, GLint y, GLint z, GLint width, GLint height)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDrawTexiOES(x, y, z, width, height);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDrawTexxOES(GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDrawTexxOES(x, y, z, width, height);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDrawTexsvOES(const GLshort *coords)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDrawTexsvOES(coords);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDrawTexivOES(const GLint *coords)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDrawTexivOES(coords);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDrawTexxvOES(const GLfixed *coords)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDrawTexxvOES(coords);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDrawTexfOES(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDrawTexfOES(x, y, z, width, height);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDrawTexfvOES(const GLfloat *coords)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDrawTexfvOES(coords);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glCurrentPaletteMatrixOES(GLuint matrixpaletteindex)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glCurrentPaletteMatrixOES(matrixpaletteindex);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glLoadPaletteFromModelViewMatrixOES(void)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLoadPaletteFromModelViewMatrixOES();

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glMatrixIndexPointerOES(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMatrixIndexPointerOES(size, type, stride, pointer);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glWeightPointerOES(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glWeightPointerOES(size, type, stride, pointer);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLbitfield
tracepath_glQueryMatrixxOES(GLfixed mantissa[16], GLint exponent[16])
{
	GLuint ret = _COREGL_INT_INIT_VALUE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glQueryMatrixxOES(mantissa, exponent);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glTexGenfOES(GLenum coord, GLenum pname, GLfloat param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexGenfOES(coord, pname, param);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexGenfvOES(GLenum coord, GLenum pname, const GLfloat *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexGenfvOES(coord, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexGeniOES(GLenum coord, GLenum pname, GLint param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexGeniOES(coord, pname, param);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexGenivOES(GLenum coord, GLenum pname, const GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexGenivOES(coord, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexGenxOES(GLenum coord, GLenum pname, GLfixed param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexGenxOES(coord, pname, param);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexGenxvOES(GLenum coord, GLenum pname, const GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexGenxvOES(coord, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetTexGenfvOES(GLenum coord, GLenum pname, GLfloat *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTexGenfvOES(coord, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetTexGenivOES(GLenum coord, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTexGenivOES(coord, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetTexGenxvOES(GLenum coord, GLenum pname, GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTexGenxvOES(coord, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glClipPlanefIMG(GLenum plane, const GLfloat *equation)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClipPlanefIMG(plane, equation);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glClipPlanexIMG(GLenum plane, const GLfixed *equation)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClipPlanexIMG(plane, equation);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

/* end of GLES 1.1 extensions */

/* common extensions in GLES1.1 and GLES2.0 */
void
tracepath_glRenderbufferStorageMultisampleAPPLE(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glRenderbufferStorageMultisampleAPPLE(target, samples, internalformat, width, height);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glResolveMultisampleFramebufferAPPLE(void)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glResolveMultisampleFramebufferAPPLE();

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glMultiDrawArraysEXT(GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMultiDrawArraysEXT(mode, first, count, primcount);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glMultiDrawElementsEXT(GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei primcount)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMultiDrawElementsEXT(mode, count, type, indices, primcount);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glRenderbufferStorageMultisampleIMG(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glRenderbufferStorageMultisampleIMG(target, samples, internalformat, width, height);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glFramebufferTexture2DMultisampleIMG(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glFramebufferTexture2DMultisampleIMG(target, attachment, textarget, texture, level, samples);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}
/* end of common extensions in GLES1.1 and GLES2.0 */

void
tracepath_glActiveTexture(GLenum texture)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glActiveTexture(texture);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glAttachShader(GLuint program, GLuint shader)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glAttachShader(program, shader);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBindAttribLocation(GLuint program, GLuint index, const char *name)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBindAttribLocation(program, index, name);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBindBuffer(GLenum target, GLuint buffer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBindBuffer(target, buffer);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBindFramebuffer(GLenum target, GLuint framebuffer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	if (unlikely(trace_surface_flag == 1)) {
		GLint oldfb;
		_orig_tracepath_glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldfb);
		if (oldfb != 0) {
			_COREGL_TRACE_SURFACE(0, 2, "GLBINDFBO");
		}

		tracepath_fbdump_update(0);
	}
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	_orig_tracepath_glBindFramebuffer(target, framebuffer);

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	if (unlikely(trace_surface_flag == 1)) {
		tracepath_fbdump_update(1);
	}
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBindRenderbuffer(target, renderbuffer);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBindTexture(GLenum target, GLuint texture)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

	_COREGL_TRACE_SURFACE(0, 1, "GLBINDTEXTURE");
	_orig_tracepath_glBindTexture(target, texture);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBlendColor(GLclampf red, GLclampf green, GLclampf blue,
		       GLclampf alpha)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBlendColor(red, green, blue, alpha);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBlendEquation(GLenum mode)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBlendEquation(mode);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBlendEquationSeparate(modeRGB, modeAlpha);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBlendFunc(GLenum sfactor, GLenum dfactor)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBlendFunc(sfactor, dfactor);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha,
			      GLenum dstAlpha)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBufferData(GLenum target, GLsizeiptr size, const void *data,
		       GLenum usage)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBufferData(target, size, data, usage);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
			  const void *data)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBufferSubData(target, offset, size, data);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLenum
tracepath_glCheckFramebufferStatus(GLenum target)
{
	GLenum ret = GL_NONE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glCheckFramebufferStatus(target);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glClear(GLbitfield mask)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClear(mask);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glClearColor(GLclampf red, GLclampf green, GLclampf blue,
		       GLclampf alpha)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClearColor(red, green, blue, alpha);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glClearDepthf(GLclampf depth)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClearDepthf(depth);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glClearStencil(GLint s)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClearStencil(s);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glColorMask(GLboolean red, GLboolean green, GLboolean blue,
		      GLboolean alpha)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glColorMask(red, green, blue, alpha);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glCompileShader(GLuint shader)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glCompileShader(shader);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glCompressedTexImage2D(GLenum target, GLint level,
				 GLenum internalformat, GLsizei width, GLsizei height, GLint border,
				 GLsizei imageSize, const void *data)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glCompressedTexImage2D(target, level, internalformat, width,
					       height, border, imageSize, data);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset,
				    GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize,
				    const void *data)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glCompressedTexSubImage2D(target, level, xoffset, yoffset,
			width, height, format, imageSize, data);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat,
			   GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glCopyTexImage2D(target, level, internalformat, x, y, width,
					 height, border);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset,
			      GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y,
					    width, height);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLuint
tracepath_glCreateProgram(void)
{
	GLuint ret = _COREGL_INT_INIT_VALUE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glCreateProgram();

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

GLuint
tracepath_glCreateShader(GLenum type)
{
	GLuint ret = _COREGL_INT_INIT_VALUE;
	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glCreateShader(type);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glCullFace(GLenum mode)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glCullFace(mode);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDeleteBuffers(GLsizei n, const GLuint *buffers)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDeleteBuffers(n, buffers);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(0);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	_orig_tracepath_glDeleteFramebuffers(n, framebuffers);

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(1);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDeleteProgram(GLuint program)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDeleteProgram(program);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(0);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	_orig_tracepath_glDeleteRenderbuffers(n, renderbuffers);

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(1);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
#ifdef COREGL_TRACEPATH_TRACE_MEMUSE_INFO
	if (trace_mem_flag == 1) {
		MY_MODULE_TSTATE *tstate = NULL;

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
		if (tstate->ctx != NULL) {
			for (int i = 0; i < n; i++) {
				if (renderbuffers[i] == 0) continue;
				_remove_glbuf_object(tstate->ctx->sostate->glbuf_rb, renderbuffers[i],
						     "Renderbuffer");
			}
		}
	}
#endif // COREGL_TRACEPATH_TRACE_MEMUSE_INFO
}

void
tracepath_glDeleteShader(GLuint shader)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDeleteShader(shader);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDeleteTextures(GLsizei n, const GLuint *textures)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(0);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	_orig_tracepath_glDeleteTextures(n, textures);

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(1);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
#ifdef COREGL_TRACEPATH_TRACE_MEMUSE_INFO
	if (trace_mem_flag == 1) {
		MY_MODULE_TSTATE *tstate = NULL;

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
		if (tstate->ctx != NULL) {
			for (int i = 0; i < n; i++) {
				if (textures[i] == 0) continue;
				_remove_glbuf_object(tstate->ctx->sostate->glbuf_tex, textures[i], "Texture");
			}
		}
	}
#endif // COREGL_TRACEPATH_TRACE_MEMUSE_INFO
}

void
tracepath_glDepthFunc(GLenum func)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDepthFunc(func);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDepthMask(GLboolean flag)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDepthMask(flag);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDepthRangef(GLclampf zNear, GLclampf zFar)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDepthRangef(zNear, zFar);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDetachShader(GLuint program, GLuint shader)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDetachShader(program, shader);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDisable(GLenum cap)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDisable(cap);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDisableVertexAttribArray(GLuint index)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDisableVertexAttribArray(index);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDrawArrays(mode, first, count);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	if (trace_surface_flag == 1) {
		GLint fbname = _COREGL_INT_INIT_VALUE;
		_orig_tracepath_glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbname);
		AST(fbname != _COREGL_INT_INIT_VALUE);
		if (fbname == 0) {
			char name[256];
			EGLint btype;
			_orig_tracepath_eglQuerySurface(_orig_tracepath_eglGetCurrentDisplay(),
							_orig_tracepath_eglGetCurrentSurface(EGL_DRAW), EGL_RENDER_BUFFER, &btype);
			if (btype == EGL_SINGLE_BUFFER)
				snprintf(name, sizeof(name), "EGLPIXMAP_%p",
					 _orig_tracepath_eglGetCurrentSurface(EGL_DRAW));
			else
				snprintf(name, sizeof(name), "EGLWINDOW_%p",
					 _orig_tracepath_eglGetCurrentSurface(EGL_DRAW));
			tracepath_surface_trace_add(name, _orig_tracepath_eglGetCurrentDisplay(),
						    _orig_tracepath_eglGetCurrentContext(),
						    _orig_tracepath_eglGetCurrentSurface(EGL_DRAW), 0, 0, 0, 0, 0, 0, NULL);
		}
	}
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO
}

void
tracepath_glDrawElements(GLenum mode, GLsizei count, GLenum type,
			 const void *indices)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDrawElements(mode, count, type, indices);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	if (trace_surface_flag == 1) {
		GLint fbname = _COREGL_INT_INIT_VALUE;
		_orig_tracepath_glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbname);
		AST(fbname != _COREGL_INT_INIT_VALUE);
		if (fbname == 0) {
			char name[256];
			EGLint btype;
			_orig_tracepath_eglQuerySurface(_orig_tracepath_eglGetCurrentDisplay(),
							_orig_tracepath_eglGetCurrentSurface(EGL_DRAW), EGL_RENDER_BUFFER, &btype);
			if (btype == EGL_SINGLE_BUFFER)
				snprintf(name, sizeof(name), "EGLPIXMAP_%p",
					 _orig_tracepath_eglGetCurrentSurface(EGL_DRAW));
			else
				snprintf(name, sizeof(name), "EGLWINDOW_%p",
					 _orig_tracepath_eglGetCurrentSurface(EGL_DRAW));
			tracepath_surface_trace_add(name, _orig_tracepath_eglGetCurrentDisplay(),
						    _orig_tracepath_eglGetCurrentContext(),
						    _orig_tracepath_eglGetCurrentSurface(EGL_DRAW), 0, 0, 0, 0, 0, 0, NULL);
		}
	}
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO
}

void
tracepath_glEnable(GLenum cap)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glEnable(cap);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glEnableVertexAttribArray(GLuint index)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glEnableVertexAttribArray(index);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glFinish(void)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glFinish();

	_COREGL_TRACE_SURFACE(0, 0, "GLFINISH");

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	_COREGL_TRACE_API_OUTPUT(0);
	_COREGL_TRACE_MEM_OUTPUT(0);
}

void
tracepath_glFlush(void)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glFlush();

	_COREGL_TRACE_SURFACE(0, 0, "GLFLUSH");

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	_COREGL_TRACE_API_OUTPUT(0);
	_COREGL_TRACE_MEM_OUTPUT(0);
}

void
tracepath_glFramebufferRenderbuffer(GLenum target, GLenum attachment,
				    GLenum renderbuffertarget, GLuint renderbuffer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(0);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	_orig_tracepath_glFramebufferRenderbuffer(target, attachment,
			renderbuffertarget, renderbuffer);

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(1);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glFramebufferTexture2D(GLenum target, GLenum attachment,
				 GLenum textarget, GLuint texture, GLint level)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(0);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	_orig_tracepath_glFramebufferTexture2D(target, attachment, textarget, texture,
					       level);

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(1);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glFrontFace(GLenum mode)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glFrontFace(mode);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetVertexAttribfv(index, pname, params);


	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetVertexAttribiv(GLuint index, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetVertexAttribiv(index, pname, params);


	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexBufferOES(GLenum target, GLenum internalformat, GLuint buffer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexBufferOES(target, internalformat, buffer);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetVertexAttribPointerv(GLuint index, GLenum pname, void **pointer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetVertexAttribPointerv(index, pname, pointer);



	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glHint(GLenum target, GLenum mode)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glHint(target, mode);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGenBuffers(GLsizei n, GLuint *buffers)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGenBuffers(n, buffers);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGenerateMipmap(GLenum target)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGenerateMipmap(target);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGenFramebuffers(GLsizei n, GLuint *framebuffers)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGenFramebuffers(n, framebuffers);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGenRenderbuffers(GLsizei n, GLuint *renderbuffers)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGenRenderbuffers(n, renderbuffers);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
#ifdef COREGL_TRACEPATH_TRACE_MEMUSE_INFO
	if (trace_mem_flag == 1) {
		MY_MODULE_TSTATE *tstate = NULL;

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
		if (tstate->ctx != NULL) {
			for (int i = 0; i < n; i++) {
				if (renderbuffers[i] == 0) continue;
				_add_glbuf_object(tstate->ctx->sostate->glbuf_rb, renderbuffers[i],
						  "Renderbuffer", 0, 0, 0, "Unknown");
			}
		}
	}
#endif // COREGL_TRACEPATH_TRACE_MEMUSE_INFO
}

void
tracepath_glGenTextures(GLsizei n, GLuint *textures)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGenTextures(n, textures);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
#ifdef COREGL_TRACEPATH_TRACE_MEMUSE_INFO
	if (trace_mem_flag == 1) {
		MY_MODULE_TSTATE *tstate = NULL;

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
		if (tstate->ctx != NULL) {
			for (int i = 0; i < n; i++) {
				if (textures[i] == 0) continue;
				_add_glbuf_object(tstate->ctx->sostate->glbuf_tex, textures[i], "Texture", 0, 0,
						  0, "Unknown");
			}
		}
	}
#endif // COREGL_TRACEPATH_TRACE_MEMUSE_INFO
}

void
tracepath_glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize,
			    GLsizei *length, GLint *size, GLenum *type, char *name)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetActiveAttrib(program, index, bufsize, length, size, type,
					  name);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize,
			     GLsizei *length, GLint *size, GLenum *type, char *name)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetActiveUniform(program, index, bufsize, length, size, type,
					   name);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei *count,
			       GLuint *shaders)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetAttachedShaders(program, maxcount, count, shaders);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

int
tracepath_glGetAttribLocation(GLuint program, const char *name)
{
	int ret = _COREGL_INT_INIT_VALUE;
	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glGetAttribLocation(program, name);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glGetBooleanv(GLenum pname, GLboolean *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetBooleanv(pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetBufferParameteriv(GLenum target, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetBufferParameteriv(target, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLenum
tracepath_glGetError(void)
{
	GLenum ret = GL_NONE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glGetError();
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glGetFloatv(GLenum pname, GLfloat *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetFloatv(pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetFramebufferAttachmentParameteriv(GLenum target,
		GLenum attachment, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetFramebufferAttachmentParameteriv(target, attachment, pname,
			params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetIntegerv(GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetIntegerv(pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetProgramiv(GLuint program, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetProgramiv(program, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei *length,
			      char *infolog)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetProgramInfoLog(program, bufsize, length, infolog);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetRenderbufferParameteriv(GLenum target, GLenum pname,
				       GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetRenderbufferParameteriv(target, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetShaderiv(shader, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei *length,
			     char *infolog)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetShaderInfoLog(shader, bufsize, length, infolog);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype,
				     GLint *range, GLint *precision)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetShaderPrecisionFormat(shadertype, precisiontype, range,
			precision);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetShaderSource(GLuint shader, GLsizei bufsize, GLsizei *length,
			    char *source)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetShaderSource(shader, bufsize, length, source);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

const GLubyte *
tracepath_glGetString(GLenum name)
{
	const GLubyte *ret = NULL;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glGetString(name);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTexParameterfv(target, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetTexParameteriv(GLenum target, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTexParameteriv(target, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetUniformfv(GLuint program, GLint location, GLfloat *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetUniformfv(program, location, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetUniformiv(GLuint program, GLint location, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetUniformiv(program, location, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}
int
tracepath_glGetUniformLocation(GLuint program, const char *name)
{
	int ret = _COREGL_INT_INIT_VALUE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glGetUniformLocation(program, name);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

GLboolean
tracepath_glIsBuffer(GLuint buffer)
{
	GLboolean ret = GL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glIsBuffer(buffer);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

GLboolean
tracepath_glIsEnabled(GLenum cap)
{
	GLboolean ret = GL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glIsEnabled(cap);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

GLboolean
tracepath_glIsFramebuffer(GLuint framebuffer)
{
	GLboolean ret = GL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glIsFramebuffer(framebuffer);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

GLboolean
tracepath_glIsProgram(GLuint program)
{
	GLboolean ret;
	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glIsProgram(program);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

GLboolean
tracepath_glIsRenderbuffer(GLuint renderbuffer)
{
	GLboolean ret;
	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glIsRenderbuffer(renderbuffer);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

GLboolean
tracepath_glIsShader(GLuint shader)
{
	GLboolean ret;
	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glIsShader(shader);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

GLboolean
tracepath_glIsTexture(GLuint texture)
{
	GLboolean ret;
	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glIsTexture(texture);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glLineWidth(GLfloat width)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLineWidth(width);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glLinkProgram(GLuint program)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLinkProgram(program);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPixelStorei(GLenum pname, GLint param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPixelStorei(pname, param);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPolygonOffset(GLfloat factor, GLfloat units)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPolygonOffset(factor, units);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height,
		       GLenum format, GLenum type, void *pixels)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glReadPixels(x, y, width, height, format, type, pixels);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glReleaseShaderCompiler(void)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glReleaseShaderCompiler();

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glRenderbufferStorage(GLenum target, GLenum internalformat,
				GLsizei width, GLsizei height)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

	_orig_tracepath_glRenderbufferStorage(target, internalformat, width, height);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
#ifdef COREGL_TRACEPATH_TRACE_MEMUSE_INFO
	if (trace_mem_flag == 1) {
		MY_MODULE_TSTATE *tstate = NULL;

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
		if (tstate->ctx != NULL) {
			int objidx = _COREGL_INT_INIT_VALUE;
			_orig_tracepath_glGetIntegerv(GL_RENDERBUFFER_BINDING, &objidx);
			AST(objidx != _COREGL_INT_INIT_VALUE);

			// Detect byte per pixel
			int bpp = 0;
			char formatment[80];
			switch (internalformat) {
			case GL_ALPHA:
				snprintf(formatment, sizeof(formatment), "ALPHA");
				bpp = 1;
				break;
			case GL_LUMINANCE:
				snprintf(formatment, sizeof(formatment), "LUMINANCE");
				bpp = 1;
				break;
			case GL_LUMINANCE_ALPHA:
				snprintf(formatment, sizeof(formatment), "LUMINANCE_ALPHA");
				bpp = 1;
				break;
			case GL_RGB:
				snprintf(formatment, sizeof(formatment), "RGB");
				bpp = 2;
				break;
			case GL_RGBA:
				snprintf(formatment, sizeof(formatment), "RGBA");
				bpp = 4;
				break;
			case 0x80E1:
				snprintf(formatment, sizeof(formatment), "BGRA_EXT");
				bpp = 4;
				break;
			case 0x84F9:
				snprintf(formatment, sizeof(formatment), "DEPTH_STENCIL_OES");
				bpp = 4;
				break;
			case GL_DEPTH_COMPONENT :
				snprintf(formatment, sizeof(formatment), "DEPTH_COMPONENT");
				bpp = 1;
				break;
			case 0x81A5:
				snprintf(formatment, sizeof(formatment), "DEPTH_COMPONENT16_ARB");
				bpp = 2;
				break;
			case 0x81A6:
				snprintf(formatment, sizeof(formatment), "DEPTH_COMPONENT24_ARB");
				bpp = 3;
				break;
			case 0x81A7:
				snprintf(formatment, sizeof(formatment), "DEPTH_COMPONENT32_ARB");
				bpp = 4;
				break;
			case 0x8D46 :
				snprintf(formatment, sizeof(formatment), "STENCIL_INDEX1_OES");
				bpp = 1;
				break;
			case 0x8D47 :
				snprintf(formatment, sizeof(formatment), "STENCIL_INDEX4_OES");
				bpp = 1;
				break;
			case 0x8D48 :
				snprintf(formatment, sizeof(formatment), "STENCIL_INDEX8_OES");
				bpp = 1;
				break;
			default:
				snprintf(formatment, sizeof(formatment), "0x%X", internalformat);
				bpp = 0;
				break;
			}

			_add_glbuf_object(tstate->ctx->sostate->glbuf_rb, objidx, "Renderbuffer", width,
					  height, bpp, formatment);
		}
	}
#endif // COREGL_TRACEPATH_TRACE_MEMUSE_INFO
#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	if (trace_surface_flag == 1) {
		MY_MODULE_TSTATE *tstate = NULL;

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
		if (tstate->ctx != NULL) {
			int objidx = _COREGL_INT_INIT_VALUE;
			_orig_tracepath_glGetIntegerv(GL_RENDERBUFFER_BINDING, &objidx);
			AST(objidx != _COREGL_INT_INIT_VALUE);

			{
				int channel = 0;
				switch (internalformat) {
				case GL_ALPHA:
				case GL_LUMINANCE:
				case GL_DEPTH_COMPONENT :
				case 0x81A5:
				case 0x81A6:
				case 0x81A7:
				case 0x8D46 :
				case 0x8D47 :
				case 0x8D48 :
					channel = 1;
					break;
				case GL_LUMINANCE_ALPHA:
				case 0x84F9:
					channel = 2;
					break;
				case GL_RGB:
					channel = 3;
					break;
				case GL_RGBA:
				case 0x80E1:
					channel = 4;
					break;
				}

				char name[256];
				snprintf(name, sizeof(name), "FBORB_%d", objidx);
				tracepath_surface_trace_add(name, tstate->ctx->dpy, tstate->ctx->handle,
							    tstate->surf_draw, -1, 0, objidx, width, height, channel, NULL);
			}
		}
	}
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO
}

void
tracepath_glSampleCoverage(GLclampf value, GLboolean invert)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glSampleCoverage(value, invert);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glScissor(x, y, width, height);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glShaderBinary(GLsizei n, const GLuint *shaders, GLenum binaryformat,
			 const void *binary, GLsizei length)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glShaderBinary(n, shaders, binaryformat, binary, length);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glShaderSource(GLuint shader, GLsizei count, const char **string,
			 const GLint *length)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glShaderSource(shader, count, string, length);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glStencilFunc(GLenum func, GLint ref, GLuint mask)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glStencilFunc(func, ref, mask);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glStencilFuncSeparate(GLenum face, GLenum func, GLint ref,
				GLuint mask)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glStencilFuncSeparate(face, func, ref, mask);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glStencilMask(GLuint mask)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glStencilMask(mask);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glStencilMaskSeparate(GLenum face, GLuint mask)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glStencilMaskSeparate(face, mask);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glStencilOp(fail, zfail, zpass);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail,
			      GLenum zpass)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glStencilOpSeparate(face, fail, zfail, zpass);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexImage2D(GLenum target, GLint level, GLint internalformat,
		       GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type,
		       const void *pixels)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexImage2D(target, level, internalformat, width, height,
				     border, format, type, pixels);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
#ifdef COREGL_TRACEPATH_TRACE_MEMUSE_INFO
	if (trace_mem_flag == 1) {
		MY_MODULE_TSTATE *tstate = NULL;

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
		if (tstate->ctx != NULL) {
			int objidx = _COREGL_INT_INIT_VALUE;
			_orig_tracepath_glGetIntegerv(GL_TEXTURE_BINDING_2D, &objidx);
			AST(objidx != _COREGL_INT_INIT_VALUE);

			// Detect byte per pixel
			int bpp = 0;
			char formatment[80];
			switch (internalformat) {
			case GL_ALPHA:
				snprintf(formatment, sizeof(formatment), "ALPHA");
				bpp = 1;
				break;
			case GL_LUMINANCE:
				snprintf(formatment, sizeof(formatment), "LUMINANCE");
				bpp = 1;
				break;
			case GL_LUMINANCE_ALPHA:
				snprintf(formatment, sizeof(formatment), "LUMINANCE_ALPHA");
				bpp = 1;
				break;
			case GL_RGB:
				snprintf(formatment, sizeof(formatment), "RGB");
				bpp = 2;
				break;
			case GL_RGBA:
				snprintf(formatment, sizeof(formatment), "RGBA");
				bpp = 4;
				break;
			case 0x80E1:
				snprintf(formatment, sizeof(formatment), "BGRA_EXT");
				bpp = 4;
				break;
			case 0x84F9:
				snprintf(formatment, sizeof(formatment), "DEPTH_STENCIL_OES");
				bpp = 4;
				break;
			case GL_DEPTH_COMPONENT :
				snprintf(formatment, sizeof(formatment), "DEPTH_COMPONENT");
				bpp = 1;
				break;
			case 0x81A5:
				snprintf(formatment, sizeof(formatment), "DEPTH_COMPONENT16_ARB");
				bpp = 2;
				break;
			case 0x81A6:
				snprintf(formatment, sizeof(formatment), "DEPTH_COMPONENT24_ARB");
				bpp = 3;
				break;
			case 0x81A7:
				snprintf(formatment, sizeof(formatment), "DEPTH_COMPONENT32_ARB");
				bpp = 4;
				break;
			case 0x8D46 :
				snprintf(formatment, sizeof(formatment), "STENCIL_INDEX1_OES");
				bpp = 1;
				break;
			case 0x8D47 :
				snprintf(formatment, sizeof(formatment), "STENCIL_INDEX4_OES");
				bpp = 1;
				break;
			case 0x8D48 :
				snprintf(formatment, sizeof(formatment), "STENCIL_INDEX8_OES");
				bpp = 1;
				break;
			default:
				snprintf(formatment, sizeof(formatment), "0x%X", internalformat);
				bpp = 0;
				break;
			}

			_add_glbuf_object(tstate->ctx->sostate->glbuf_tex, objidx, "Texture", width,
					  height, bpp, formatment);
		}
	}
#endif // COREGL_TRACEPATH_TRACE_MEMUSE_INFO
#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	if (trace_surface_flag == 1) {
		MY_MODULE_TSTATE *tstate = NULL;

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
		if (tstate->ctx != NULL) {
			int objidx = _COREGL_INT_INIT_VALUE;
			_orig_tracepath_glGetIntegerv(GL_TEXTURE_BINDING_2D, &objidx);
			AST(objidx != _COREGL_INT_INIT_VALUE);

			{
				int channel = 0;
				switch (internalformat) {
				case GL_ALPHA:
				case GL_LUMINANCE:
				case GL_DEPTH_COMPONENT :
				case 0x81A5:
				case 0x81A6:
				case 0x81A7:
				case 0x8D46 :
				case 0x8D47 :
				case 0x8D48 :
					channel = 1;
					break;
				case GL_LUMINANCE_ALPHA:
				case 0x84F9:
					channel = 2;
					break;
				case GL_RGB:
					channel = 3;
					break;
				case GL_RGBA:
				case 0x80E1:
					channel = 4;
					break;
				}

				char name[256];
				snprintf(name, sizeof(name), "FBOTEX_0x%X", objidx);
				tracepath_surface_trace_add(name, tstate->ctx->dpy, tstate->ctx->handle,
							    tstate->surf_draw, -1, objidx, 0, width, height, channel, NULL);
			}
		}
	}
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO
}

void
tracepath_glTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexParameterf(target, pname, param);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexParameterfv(GLenum target, GLenum pname, const GLfloat *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexParameterfv(target, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexParameteri(GLenum target, GLenum pname, GLint param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexParameteri(target, pname, param);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexParameteriv(GLenum target, GLenum pname, const GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexParameteriv(target, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexSubImage2D(GLenum target, GLint level, GLint xoffset,
			  GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type,
			  const void *pixels)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexSubImage2D(target, level, xoffset, yoffset, width, height,
					format, type, pixels);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform1f(GLint location, GLfloat x)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform1f(location, x);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform1fv(GLint location, GLsizei count, const GLfloat *v)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform1fv(location, count, v);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform1i(GLint location, GLint x)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform1i(location, x);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform1iv(GLint location, GLsizei count, const GLint *v)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform1iv(location, count, v);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform2f(GLint location, GLfloat x, GLfloat y)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform2f(location, x, y);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform2fv(GLint location, GLsizei count, const GLfloat *v)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform2fv(location, count, v);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform2i(GLint location, GLint x, GLint y)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform2i(location, x, y);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform2iv(GLint location, GLsizei count, const GLint *v)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform2iv(location, count, v);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform3f(location, x, y, z);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform3fv(GLint location, GLsizei count, const GLfloat *v)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform3fv(location, count, v);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform3i(GLint location, GLint x, GLint y, GLint z)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform3i(location, x, y, z);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform3iv(GLint location, GLsizei count, const GLint *v)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform3iv(location, count, v);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z,
		      GLfloat w)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform4f(location, x, y, z, w);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform4fv(GLint location, GLsizei count, const GLfloat *v)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform4fv(location, count, v);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform4i(GLint location, GLint x, GLint y, GLint z, GLint w)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform4i(location, x, y, z, w);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform4iv(GLint location, GLsizei count, const GLint *v)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform4iv(location, count, v);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose,
			     const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniformMatrix2fv(location, count, transpose, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose,
			     const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniformMatrix3fv(location, count, transpose, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose,
			     const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniformMatrix4fv(location, count, transpose, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUseProgram(GLuint program)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUseProgram(program);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glValidateProgram(GLuint program)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glValidateProgram(program);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glVertexAttrib1f(GLuint index, GLfloat x)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexAttrib1f(index, x);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glVertexAttrib1fv(GLuint index, const GLfloat *values)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexAttrib1fv(index, values);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glVertexAttrib2f(GLuint index, GLfloat x, GLfloat y)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexAttrib2f(index, x, y);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glVertexAttrib2fv(GLuint index, const GLfloat *values)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexAttrib2fv(index, values);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glVertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexAttrib3f(index, x, y, z);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glVertexAttrib3fv(GLuint index, const GLfloat *values)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexAttrib3fv(index, values);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z,
			   GLfloat w)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexAttrib4f(index, x, y, z, w);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glVertexAttrib4fv(GLuint index, const GLfloat *values)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexAttrib4fv(index, values);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glVertexAttribPointer(GLuint index, GLint size, GLenum type,
				GLboolean normalized, GLsizei stride, const void *pointer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexAttribPointer(index, size, type, normalized, stride,
					      pointer);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glViewport(x, y, width, height);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}


/* OpenGL ES 3.0 */
void
tracepath_glReadBuffer(GLenum mode)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glReadBuffer(mode);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDrawRangeElements(GLenum mode, GLuint start, GLuint end,
			      GLsizei count, GLenum type, const GLvoid *indices)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDrawRangeElements(mode, start, end, count, type, indices);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexImage3D(GLenum target, GLint level, GLint GLinternalFormat,
		       GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format,
		       GLenum type, const GLvoid *pixels)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexImage3D(target, level, GLinternalFormat, width, height,
				     depth, border, format, type, pixels);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexImage3DOES(GLenum target, GLint level, GLenum GLinternalFormat,
			  GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format,
			  GLenum type, const void *pixels)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexImage3DOES(target, level, GLinternalFormat, width, height,
					depth, border, format, type, pixels);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexSubImage3D(GLenum target, GLint level, GLint xoffset,
			  GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,
			  GLenum format, GLenum type, const GLvoid *pixels)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexSubImage3D(target, level, xoffset, yoffset, zoffset, width,
					height, depth, format, type, pixels);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexSubImage3DOES(GLenum target, GLint level, GLint xoffset,
			     GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,
			     GLenum format, GLenum type, const GLvoid *pixels)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexSubImage3DOES(target, level, xoffset, yoffset, zoffset,
					   width, height, depth, format, type, pixels);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glCopyTexSubImage3D(GLenum target, GLint level, GLint xoffset,
			      GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glCopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x,
					    y, width, height);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glCopyTexSubImage3DOES(GLenum target, GLint level, GLint xoffset,
				 GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glCopyTexSubImage3DOES(target, level, xoffset, yoffset, zoffset,
					       x, y, width, height);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glCompressedTexImage3D(GLenum target, GLint level,
				 GLenum GLinternalformat, GLsizei width, GLsizei height, GLsizei depth,
				 GLint border, GLsizei imageSize, const GLvoid *data)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glCompressedTexImage3D(target, level, GLinternalformat, width,
					       height, depth, border, imageSize, data);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glCompressedTexImage3DOES(GLenum target, GLint level,
				    GLenum GLinternalformat, GLsizei width, GLsizei height, GLsizei depth,
				    GLint border, GLsizei imageSize, const GLvoid *data)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glCompressedTexImage3DOES(target, level, GLinternalformat,
			width, height, depth, border, imageSize, data);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glCompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset,
				    GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,
				    GLenum format, GLsizei imageSize, const GLvoid *data)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glCompressedTexSubImage3D(target, level, xoffset, yoffset,
			zoffset, width, height, depth, format, imageSize, data);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glCompressedTexSubImage3DOES(GLenum target, GLint level,
				       GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height,
				       GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glCompressedTexSubImage3DOES(target, level, xoffset, yoffset,
			zoffset, width, height, depth, format, imageSize, data);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGenQueries(GLsizei n, GLuint *ids)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGenQueries(n, ids);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDeleteQueries(GLsizei n, const GLuint *ids)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDeleteQueries(n, ids);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLboolean
tracepath_glIsQuery(GLuint id)
{
	GLboolean ret = GL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glIsQuery(id);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glBeginQuery(GLenum target, GLuint id)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBeginQuery(target, id);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glEndQuery(GLenum target)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glEndQuery(target);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetQueryiv(GLenum target, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetQueryiv(target, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetQueryObjectuiv(id, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLboolean
tracepath_glUnmapBuffer(GLenum target)
{
	GLboolean ret = GL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glUnmapBuffer(target);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glGetBufferPointerv(GLenum target, GLenum pname, GLvoid **params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetBufferPointerv(target, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDrawBuffers(GLsizei n, const GLenum *bufs)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDrawBuffers(n, bufs);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniformMatrix2x3fv(GLint location, GLsizei count,
			       GLboolean transpose, const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniformMatrix2x3fv(location, count, transpose, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniformMatrix3x2fv(GLint location, GLsizei count,
			       GLboolean transpose, const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniformMatrix3x2fv(location, count, transpose, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniformMatrix2x4fv(GLint location, GLsizei count,
			       GLboolean transpose, const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniformMatrix2x4fv(location, count, transpose, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniformMatrix4x2fv(GLint location, GLsizei count,
			       GLboolean transpose, const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniformMatrix4x2fv(location, count, transpose, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniformMatrix3x4fv(GLint location, GLsizei count,
			       GLboolean transpose, const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniformMatrix3x4fv(location, count, transpose, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniformMatrix4x3fv(GLint location, GLsizei count,
			       GLboolean transpose, const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniformMatrix4x3fv(location, count, transpose, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
			    GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask,
			    GLenum filter)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0,
					  dstX1, dstY1, mask, filter);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glRenderbufferStorageMultisample(GLenum target, GLsizei samples,
		GLenum internalformat, GLsizei width, GLsizei height)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glRenderbufferStorageMultisample(target, samples,
			internalformat, width, height);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
#ifdef COREGL_TRACEPATH_TRACE_MEMUSE_INFO
	if (trace_mem_flag == 1) {
		MY_MODULE_TSTATE *tstate = NULL;

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
		if (tstate->ctx != NULL) {
			int objidx = _COREGL_INT_INIT_VALUE;
			_orig_tracepath_glGetIntegerv(GL_RENDERBUFFER_BINDING, &objidx);
			AST(objidx != _COREGL_INT_INIT_VALUE);

			// Detect byte per pixel
			int bpp = 0;
			char formatment[80];
			switch (internalformat) {
			case GL_ALPHA:
				snprintf(formatment, sizeof(formatment), "ALPHA");
				bpp = 1;
				break;
			case GL_LUMINANCE:
				snprintf(formatment, sizeof(formatment), "LUMINANCE");
				bpp = 1;
				break;
			case GL_LUMINANCE_ALPHA:
				snprintf(formatment, sizeof(formatment), "LUMINANCE_ALPHA");
				bpp = 1;
				break;
			case GL_RGB:
				snprintf(formatment, sizeof(formatment), "RGB");
				bpp = 2;
				break;
			case GL_RGBA:
				snprintf(formatment, sizeof(formatment), "RGBA");
				bpp = 4;
				break;
			case 0x80E1:
				snprintf(formatment, sizeof(formatment), "BGRA_EXT");
				bpp = 4;
				break;
			case 0x84F9:
				snprintf(formatment, sizeof(formatment), "DEPTH_STENCIL_OES");
				bpp = 4;
				break;
			case GL_DEPTH_COMPONENT :
				snprintf(formatment, sizeof(formatment), "DEPTH_COMPONENT");
				bpp = 1;
				break;
			case 0x81A5:
				snprintf(formatment, sizeof(formatment), "DEPTH_COMPONENT16_ARB");
				bpp = 2;
				break;
			case 0x81A6:
				snprintf(formatment, sizeof(formatment), "DEPTH_COMPONENT24_ARB");
				bpp = 3;
				break;
			case 0x81A7:
				snprintf(formatment, sizeof(formatment), "DEPTH_COMPONENT32_ARB");
				bpp = 4;
				break;
			case 0x8D46 :
				snprintf(formatment, sizeof(formatment), "STENCIL_INDEX1_OES");
				bpp = 1;
				break;
			case 0x8D47 :
				snprintf(formatment, sizeof(formatment), "STENCIL_INDEX4_OES");
				bpp = 1;
				break;
			case 0x8D48 :
				snprintf(formatment, sizeof(formatment), "STENCIL_INDEX8_OES");
				bpp = 1;
				break;
			default:
				snprintf(formatment, sizeof(formatment), "0x%X", internalformat);
				bpp = 0;
				break;
			}

			_add_glbuf_object(tstate->ctx->sostate->glbuf_rb, objidx, "Renderbuffer", width,
					  height, bpp, formatment);
		}
	}
#endif // COREGL_TRACEPATH_TRACE_MEMUSE_INFO
#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	if (trace_surface_flag == 1) {
		MY_MODULE_TSTATE *tstate = NULL;

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
		if (tstate->ctx != NULL) {
			int objidx = _COREGL_INT_INIT_VALUE;
			_orig_tracepath_glGetIntegerv(GL_RENDERBUFFER_BINDING, &objidx);
			AST(objidx != _COREGL_INT_INIT_VALUE);

			{
				int channel = 0;
				switch (internalformat) {
				case GL_ALPHA:
				case GL_LUMINANCE:
				case GL_DEPTH_COMPONENT :
				case 0x81A5:
				case 0x81A6:
				case 0x81A7:
				case 0x8D46 :
				case 0x8D47 :
				case 0x8D48 :
					channel = 1;
					break;
				case GL_LUMINANCE_ALPHA:
				case 0x84F9:
					channel = 2;
					break;
				case GL_RGB:
					channel = 3;
					break;
				case GL_RGBA:
				case 0x80E1:
					channel = 4;
					break;
				}

				char name[256];
				snprintf(name, sizeof(name), "FBORB_0x%X", objidx);
				tracepath_surface_trace_add(name, tstate->ctx->dpy, tstate->ctx->handle,
							    tstate->surf_draw, -1, 0, objidx, width, height, channel, NULL);
			}
		}
	}
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO
}

void
tracepath_glRenderbufferStorageMultisampleEXT(GLenum target, GLsizei samples,
		GLenum internalformat, GLsizei width, GLsizei height)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glRenderbufferStorageMultisampleEXT(target, samples,
			internalformat, width, height);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
#ifdef COREGL_TRACEPATH_TRACE_MEMUSE_INFO
	if (trace_mem_flag == 1) {
		MY_MODULE_TSTATE *tstate = NULL;

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
		if (tstate->ctx != NULL) {
			int objidx = _COREGL_INT_INIT_VALUE;
			_orig_tracepath_glGetIntegerv(GL_RENDERBUFFER_BINDING, &objidx);
			AST(objidx != _COREGL_INT_INIT_VALUE);

			// Detect byte per pixel
			int bpp = 0;
			char formatment[80];
			switch (internalformat) {
			case GL_ALPHA:
				snprintf(formatment, sizeof(formatment), "ALPHA");
				bpp = 1;
				break;
			case GL_LUMINANCE:
				snprintf(formatment, sizeof(formatment), "LUMINANCE");
				bpp = 1;
				break;
			case GL_LUMINANCE_ALPHA:
				snprintf(formatment, sizeof(formatment), "LUMINANCE_ALPHA");
				bpp = 1;
				break;
			case GL_RGB:
				snprintf(formatment, sizeof(formatment), "RGB");
				bpp = 2;
				break;
			case GL_RGBA:
				snprintf(formatment, sizeof(formatment), "RGBA");
				bpp = 4;
				break;
			case 0x80E1:
				snprintf(formatment, sizeof(formatment), "BGRA_EXT");
				bpp = 4;
				break;
			case 0x84F9:
				snprintf(formatment, sizeof(formatment), "DEPTH_STENCIL_OES");
				bpp = 4;
				break;
			case GL_DEPTH_COMPONENT :
				snprintf(formatment, sizeof(formatment), "DEPTH_COMPONENT");
				bpp = 1;
				break;
			case 0x81A5:
				snprintf(formatment, sizeof(formatment), "DEPTH_COMPONENT16_ARB");
				bpp = 2;
				break;
			case 0x81A6:
				snprintf(formatment, sizeof(formatment), "DEPTH_COMPONENT24_ARB");
				bpp = 3;
				break;
			case 0x81A7:
				snprintf(formatment, sizeof(formatment), "DEPTH_COMPONENT32_ARB");
				bpp = 4;
				break;
			case 0x8D46 :
				snprintf(formatment, sizeof(formatment), "STENCIL_INDEX1_OES");
				bpp = 1;
				break;
			case 0x8D47 :
				snprintf(formatment, sizeof(formatment), "STENCIL_INDEX4_OES");
				bpp = 1;
				break;
			case 0x8D48 :
				snprintf(formatment, sizeof(formatment), "STENCIL_INDEX8_OES");
				bpp = 1;
				break;
			default:
				snprintf(formatment, sizeof(formatment), "0x%X", internalformat);
				bpp = 0;
				break;
			}

			_add_glbuf_object(tstate->ctx->sostate->glbuf_rb, objidx, "Renderbuffer", width,
					  height, bpp, formatment);
		}
	}
#endif // COREGL_TRACEPATH_TRACE_MEMUSE_INFO
#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	if (trace_surface_flag == 1) {
		MY_MODULE_TSTATE *tstate = NULL;

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
		if (tstate->ctx != NULL) {
			int objidx = _COREGL_INT_INIT_VALUE;
			_orig_tracepath_glGetIntegerv(GL_RENDERBUFFER_BINDING, &objidx);
			AST(objidx != _COREGL_INT_INIT_VALUE);

			{
				int channel = 0;
				switch (internalformat) {
				case GL_ALPHA:
				case GL_LUMINANCE:
				case GL_DEPTH_COMPONENT :
				case 0x81A5:
				case 0x81A6:
				case 0x81A7:
				case 0x8D46 :
				case 0x8D47 :
				case 0x8D48 :
					channel = 1;
					break;
				case GL_LUMINANCE_ALPHA:
				case 0x84F9:
					channel = 2;
					break;
				case GL_RGB:
					channel = 3;
					break;
				case GL_RGBA:
				case 0x80E1:
					channel = 4;
					break;
				}

				char name[256];
				snprintf(name, sizeof(name), "FBORB_0x%X", objidx);
				tracepath_surface_trace_add(name, tstate->ctx->dpy, tstate->ctx->handle,
							    tstate->surf_draw, -1, 0, objidx, width, height, channel, NULL);
			}
		}
	}
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO
}

void
tracepath_glFramebufferTextureLayer(GLenum target, GLenum attachment,
				    GLuint texture, GLint level, GLint layer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(0);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	_orig_tracepath_glFramebufferTextureLayer(target, attachment, texture, level,
			layer);

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(1);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLvoid *
tracepath_glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length,
			   GLbitfield access)
{
	GLvoid *ret = NULL;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glMapBufferRange(target, offset, length, access);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glFlushMappedBufferRange(GLenum target, GLintptr offset,
				   GLsizeiptr length)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glFlushMappedBufferRange(target, offset, length);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBindVertexArray(GLuint array)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBindVertexArray(array);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDeleteVertexArrays(GLsizei n, const GLuint *arrays)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDeleteVertexArrays(n, arrays);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGenVertexArrays(GLsizei n, GLuint *arrays)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGenVertexArrays(n, arrays);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLboolean
tracepath_glIsVertexArray(GLuint array)
{
	GLboolean ret = GL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glIsVertexArray(array);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glGetIntegeri_v(GLenum target, GLuint index, GLint *data)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetIntegeri_v(target, index, data);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBeginTransformFeedback(GLenum primitiveMode)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBeginTransformFeedback(primitiveMode);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glEndTransformFeedback()
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glEndTransformFeedback();

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBindBufferRange(GLenum target, GLuint index, GLuint buffer,
			    GLintptr offset, GLsizeiptr size)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBindBufferRange(target, index, buffer, offset, size);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBindBufferBase(GLenum target, GLuint index, GLuint buffer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBindBufferBase(target, index, buffer);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTransformFeedbackVaryings(GLuint program, GLsizei count,
				      const GLchar *const *varyings, GLenum bufferMode)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTransformFeedbackVaryings(program, count, varyings,
			bufferMode);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetTransformFeedbackVarying(GLuint program, GLuint index,
					GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTransformFeedbackVarying(program, index, bufSize, length,
			size, type, name);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glVertexAttribIPointer(GLuint index, GLint size, GLenum type,
				 GLsizei stride, const GLvoid *pointer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexAttribIPointer(index, size, type, stride, pointer);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetVertexAttribIiv(GLuint index, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetVertexAttribIiv(index, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetVertexAttribIuiv(GLuint index, GLenum pname, GLuint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetVertexAttribIuiv(index, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glVertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexAttribI4i(index, x, y, z, w);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glVertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z,
			     GLuint w)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexAttribI4ui(index, x, y, z, w);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glVertexAttribI4iv(GLuint index, const GLint *v)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexAttribI4iv(index, v);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glVertexAttribI4uiv(GLuint index, const GLuint *v)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexAttribI4uiv(index, v);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetUniformuiv(GLuint program, GLint location, GLuint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetUniformuiv(program, location, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLint
tracepath_glGetFragDataLocation(GLuint program, const GLchar *name)
{
	GLint ret = _COREGL_INT_INIT_VALUE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glGetFragDataLocation(program, name);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glUniform1ui(GLint location, GLuint v0)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform1ui(location, v0);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform2ui(GLint location, GLuint v0, GLuint v1)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform2ui(location, v0, v1);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform3ui(location, v0, v1, v2);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2,
		       GLuint v3)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform4ui(location, v0, v1, v2, v3);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform1uiv(GLint location, GLsizei count, const GLuint *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform1uiv(location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform2uiv(GLint location, GLsizei count, const GLuint *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform2uiv(location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform3uiv(GLint location, GLsizei count, const GLuint *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform3uiv(location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniform4uiv(GLint location, GLsizei count, const GLuint *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniform4uiv(location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClearBufferiv(buffer, drawbuffer, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClearBufferuiv(buffer, drawbuffer, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClearBufferfv(buffer, drawbuffer, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth,
			  GLint stencil)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClearBufferfi(buffer, drawbuffer, depth, stencil);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

const GLubyte *
tracepath_glGetStringi(GLenum name, GLuint index)
{
	const GLubyte *ret = NULL;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glGetStringi(name, index);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glCopyBufferSubData(GLenum readTarget, GLenum writeTarget,
			      GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glCopyBufferSubData(readTarget, writeTarget, readOffset,
					    writeOffset, size);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetUniformIndices(GLuint program, GLsizei uniformCount,
			      const GLchar *const *uniformNames, GLuint *uniformIndices)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetUniformIndices(program, uniformCount, uniformNames,
					    uniformIndices);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetActiveUniformsiv(GLuint program, GLsizei uniformCount,
				const GLuint *uniformIndices, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetActiveUniformsiv(program, uniformCount, uniformIndices,
					      pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLuint
tracepath_glGetUniformBlockIndex(GLuint program, const GLchar *uniformBlockName)
{
	GLuint ret = _COREGL_INT_INIT_VALUE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glGetUniformBlockIndex(program, uniformBlockName);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex,
				    GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetActiveUniformBlockiv(program, uniformBlockIndex, pname,
			params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex,
				      GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetActiveUniformBlockName(program, uniformBlockIndex, bufSize,
			length, uniformBlockName);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex,
				GLuint uniformBlockBinding)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUniformBlockBinding(program, uniformBlockIndex,
					      uniformBlockBinding);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count,
				GLsizei instanceCount)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDrawArraysInstanced(mode, first, count, instanceCount);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type,
				  const GLvoid *indices, GLsizei instanceCount)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDrawElementsInstanced(mode, count, type, indices,
						instanceCount);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLsync
tracepath_glFenceSync(GLenum condition, GLbitfield flags)
{
	GLsync ret = NULL;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glFenceSync(condition, flags);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

GLboolean
tracepath_glIsSync(GLsync sync)
{
	GLboolean ret = GL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glIsSync(sync);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glDeleteSync(GLsync sync)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDeleteSync(sync);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLenum
tracepath_glClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
	GLenum ret = _COREGL_INT_INIT_VALUE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glClientWaitSync(sync, flags, timeout);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glWaitSync(sync, flags, timeout);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetInteger64v(GLenum pname, GLint64 *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetInteger64v(pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetSynciv(GLsync sync, GLenum pname, GLsizei bufSize,
		      GLsizei *length, GLint *values)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetSynciv(sync, pname, bufSize, length, values);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetInteger64i_v(GLenum target, GLuint index, GLint64 *data)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetInteger64i_v(target, index, data);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetBufferParameteri64v(GLenum target, GLenum pname, GLint64 *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetBufferParameteri64v(target, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGenSamplers(GLsizei n, GLuint *samplers)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGenSamplers(n, samplers);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDeleteSamplers(GLsizei n, const GLuint *samplers)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDeleteSamplers(n, samplers);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLboolean
tracepath_glIsSampler(GLuint sampler)
{
	GLboolean ret = GL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glIsSampler(sampler);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glBindSampler(GLuint unit, GLuint sampler)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBindSampler(unit, sampler);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glSamplerParameteri(GLuint sampler, GLenum pname, GLint param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glSamplerParameteri(sampler, pname, param);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glSamplerParameteriv(GLuint sampler, GLenum pname, const GLint *param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glSamplerParameteriv(sampler, pname, param);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glSamplerParameterf(GLuint sampler, GLenum pname, GLfloat param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glSamplerParameterf(sampler, pname, param);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glSamplerParameterfv(GLuint sampler, GLenum pname,
			       const GLfloat *param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glSamplerParameterfv(sampler, pname, param);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetSamplerParameteriv(GLuint sampler, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetSamplerParameteriv(sampler, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetSamplerParameterfv(sampler, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glVertexAttribDivisor(GLuint index, GLuint divisor)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexAttribDivisor(index, divisor);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBindTransformFeedback(GLenum target, GLuint id)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBindTransformFeedback(target, id);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDeleteTransformFeedbacks(GLsizei n, const GLuint *ids)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDeleteTransformFeedbacks(n, ids);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGenTransformFeedbacks(GLsizei n, GLuint *ids)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGenTransformFeedbacks(n, ids);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLboolean
tracepath_glIsTransformFeedback(GLuint id)
{
	GLboolean ret = GL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glIsTransformFeedback(id);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glPauseTransformFeedback()
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPauseTransformFeedback();

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glResumeTransformFeedback()
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glResumeTransformFeedback();

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei *length,
			     GLenum *binaryFormat, GLvoid *binary)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetProgramBinary(program, bufSize, length, binaryFormat,
					   binary);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetProgramBinaryOES(GLuint program, GLsizei bufSize,
				GLsizei *length, GLenum *binaryFormat, GLvoid *binary)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetProgramBinaryOES(program, bufSize, length, binaryFormat,
					      binary);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramBinary(GLuint program, GLenum binaryFormat,
			  const GLvoid *binary, GLsizei length)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramBinary(program, binaryFormat, binary, length);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramBinaryOES(GLuint program, GLenum binaryFormat,
			     const GLvoid *binary, GLsizei length)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramBinaryOES(program, binaryFormat, binary, length);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramParameteri(GLuint program, GLenum pname, GLint value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramParameteri(program, pname, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glInvalidateFramebuffer(GLenum target, GLsizei numAttachments,
				  const GLenum *attachments)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glInvalidateFramebuffer(target, numAttachments, attachments);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDiscardFramebufferEXT(GLenum target, GLsizei numAttachments,
				  const GLenum *attachments)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDiscardFramebufferEXT(target, numAttachments, attachments);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glInvalidateSubFramebuffer(GLenum target, GLsizei numAttachments,
				     const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glInvalidateSubFramebuffer(target, numAttachments, attachments,
			x, y, width, height);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat,
			 GLsizei width, GLsizei height)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexStorage2D(target, levels, internalformat, width, height);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexStorage3D(GLenum target, GLsizei levels, GLenum internalformat,
			 GLsizei width, GLsizei height, GLsizei depth)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexStorage3D(target, levels, internalformat, width, height,
				       depth);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetInternalformativ(GLenum target, GLenum internalformat,
				GLenum pname, GLsizei bufSize, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetInternalformativ(target, internalformat, pname, bufSize,
					      params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}


/* GLES Extensions */
void
tracepath_glEGLImageTargetTexture2DOES(GLenum target, GLeglImageOES image)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_COREGL_TRACE_SURFACE(0, 1, "TEXTURE2DOES");
	_orig_tracepath_glEGLImageTargetTexture2DOES(target, image);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
#ifdef COREGL_TRACEPATH_TRACE_MEMUSE_INFO
	if (trace_mem_flag == 1) {
		MY_MODULE_TSTATE *tstate = NULL;

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
		if (tstate->ctx != NULL) {
			int objidx = _COREGL_INT_INIT_VALUE;
			_orig_tracepath_glGetIntegerv(GL_TEXTURE_BINDING_2D, &objidx);
			AST(objidx != _COREGL_INT_INIT_VALUE);

			_add_glbuf_object(tstate->ctx->sostate->glbuf_tex, objidx, "Texture", 0, 0, 0,
					  "Unknown");
		}
	}
#endif // COREGL_TRACEPATH_TRACE_MEMUSE_INFO
}

void
tracepath_glEGLImageTargetRenderbufferStorageOES(GLenum target,
		GLeglImageOES image)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glEGLImageTargetRenderbufferStorageOES(target, image);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
#ifdef COREGL_TRACEPATH_TRACE_MEMUSE_INFO
	if (trace_mem_flag == 1) {
		MY_MODULE_TSTATE *tstate = NULL;

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
		if (tstate->ctx != NULL) {
			int objidx = _COREGL_INT_INIT_VALUE;
			_orig_tracepath_glGetIntegerv(GL_RENDERBUFFER_BINDING, &objidx);
			AST(objidx != _COREGL_INT_INIT_VALUE);

			_add_glbuf_object(tstate->ctx->sostate->glbuf_rb, objidx, "Renderbuffer", 0, 0,
					  0, "Unknown");
		}
	}
#endif // COREGL_TRACEPATH_TRACE_MEMUSE_INFO
}

void
tracepath_glUseProgramStagesEXT(GLuint pipeline, GLbitfield stages,
				GLuint program)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUseProgramStagesEXT(pipeline, stages, program);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glActiveShaderProgramEXT(GLuint pipeline, GLuint program)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glActiveShaderProgramEXT(pipeline, program);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLuint
tracepath_glCreateShaderProgramvEXT(GLenum type, GLsizei count,
				    const char **strings)
{
	GLuint ret = _COREGL_INT_INIT_VALUE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glCreateShaderProgramvEXT(type, count, strings);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glBindProgramPipelineEXT(GLuint pipeline)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBindProgramPipelineEXT(pipeline);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDeleteProgramPipelinesEXT(GLsizei n, const GLuint *pipelines)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDeleteProgramPipelinesEXT(n, pipelines);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGenProgramPipelinesEXT(GLsizei n, GLuint *pipelines)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGenProgramPipelinesEXT(n, pipelines);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLboolean
tracepath_glIsProgramPipelineEXT(GLuint pipeline)
{
	GLboolean ret = _COREGL_INT_INIT_VALUE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glIsProgramPipelineEXT(pipeline);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glProgramParameteriEXT(GLuint program, GLenum pname, GLint value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramParameteriEXT(program, pname, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetProgramPipelineivEXT(GLuint pipeline, GLenum pname,
				    GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetProgramPipelineivEXT(pipeline, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform1iEXT(GLuint program, GLint location, GLint x)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform1iEXT(program, location, x);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform2iEXT(GLuint program, GLint location, GLint x,
				GLint y)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform2iEXT(program, location, x, y);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform3iEXT(GLuint program, GLint location, GLint x,
				GLint y, GLint z)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform3iEXT(program, location, x, y, z);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform4iEXT(GLuint program, GLint location, GLint x,
				GLint y, GLint z, GLint w)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform4iEXT(program, location, x, y, z, w);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform1fEXT(GLuint program, GLint location, GLfloat x)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform1fEXT(program, location, x);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform2fEXT(GLuint program, GLint location, GLfloat x,
				GLfloat y)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform2fEXT(program, location, x, y);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform3fEXT(GLuint program, GLint location, GLfloat x,
				GLfloat y, GLfloat z)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform3fEXT(program, location, x, y, z);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform4fEXT(GLuint program, GLint location, GLfloat x,
				GLfloat y, GLfloat z, GLfloat w)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform4fEXT(program, location, x, y, z, w);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform1ivEXT(GLuint program, GLint location, GLsizei count,
				 const GLint *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform1ivEXT(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform2ivEXT(GLuint program, GLint location, GLsizei count,
				 const GLint *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform2ivEXT(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform3ivEXT(GLuint program, GLint location, GLsizei count,
				 const GLint *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform3ivEXT(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform4ivEXT(GLuint program, GLint location, GLsizei count,
				 const GLint *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform4ivEXT(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform1fvEXT(GLuint program, GLint location, GLsizei count,
				 const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform1fvEXT(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform2fvEXT(GLuint program, GLint location, GLsizei count,
				 const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform2fvEXT(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform3fvEXT(GLuint program, GLint location, GLsizei count,
				 const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform3fvEXT(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform4fvEXT(GLuint program, GLint location, GLsizei count,
				 const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform4fvEXT(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniformMatrix2fvEXT(GLuint program, GLint location,
				       GLsizei count, GLboolean transpose, const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniformMatrix2fvEXT(program, location, count,
			transpose, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniformMatrix3fvEXT(GLuint program, GLint location,
				       GLsizei count, GLboolean transpose, const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniformMatrix3fvEXT(program, location, count,
			transpose, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniformMatrix4fvEXT(GLuint program, GLint location,
				       GLsizei count, GLboolean transpose, const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniformMatrix4fvEXT(program, location, count,
			transpose, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glValidateProgramPipelineEXT(GLuint pipeline)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glValidateProgramPipelineEXT(pipeline);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetProgramPipelineInfoLogEXT(GLuint pipeline, GLsizei bufSize,
		GLsizei *length, char *infoLog)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetProgramPipelineInfoLogEXT(pipeline, bufSize, length,
			infoLog);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}


void
tracepath_glFramebufferTexture2DMultisampleEXT(GLenum target, GLenum attachment,
		GLenum textarget, GLuint texture, GLint level, GLsizei samples)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(0);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	_orig_tracepath_glFramebufferTexture2DMultisampleEXT(target, attachment,
			textarget, texture, level, samples);

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(1);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glFramebufferTexture3DOES(GLenum target, GLenum attachment,
				    GLenum textarget, GLuint texture, GLint level, GLint zoffset)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glFramebufferTexture3DOES(target, attachment, textarget,
			texture, level, zoffset);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}
/*
void
tracepath_glBlitFramebufferANGLE(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBlitFramebufferANGLE(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glRenderbufferStorageMultisampleANGLE(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

	_orig_tracepath_glRenderbufferStorageMultisampleANGLE(target, samples, internalformat, width, height);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
#ifdef COREGL_TRACEPATH_TRACE_MEMUSE_INFO
	if (trace_mem_flag == 1)
	{
		MY_MODULE_TSTATE *tstate = NULL;

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
		if (tstate->ctx != NULL)
		{
			int objidx = _COREGL_INT_INIT_VALUE;
			_orig_tracepath_glGetIntegerv(GL_RENDERBUFFER_BINDING, &objidx);
			AST(objidx != _COREGL_INT_INIT_VALUE);

			// Detect byte per pixel
			int bpp = 0;
			char formatment[80];
			switch (internalformat)
			{
				case GL_ALPHA: sprintf(formatment, "ALPHA"); bpp = 1; break;
				case GL_LUMINANCE: sprintf(formatment, "LUMINANCE"); bpp = 1; break;
				case GL_LUMINANCE_ALPHA: sprintf(formatment, "LUMINANCE_ALPHA"); bpp = 1; break;
				case GL_RGB: sprintf(formatment, "RGB"); bpp = 2; break;
				case GL_RGBA: sprintf(formatment, "RGBA"); bpp = 4; break;
				case 0x80E1: sprintf(formatment, "BGRA_EXT"); bpp = 4; break;
				case 0x84F9: sprintf(formatment, "DEPTH_STENCIL_OES"); bpp = 4; break;
				case GL_DEPTH_COMPONENT : sprintf(formatment, "DEPTH_COMPONENT"); bpp = 1; break;
				case 0x81A5: sprintf(formatment, "DEPTH_COMPONENT16_ARB"); bpp = 2; break;
				case 0x81A6: sprintf(formatment, "DEPTH_COMPONENT24_ARB"); bpp = 3; break;
				case 0x81A7: sprintf(formatment, "DEPTH_COMPONENT32_ARB"); bpp = 4; break;
				case 0x8D46 : sprintf(formatment, "STENCIL_INDEX1_OES"); bpp = 1; break;
				case 0x8D47 : sprintf(formatment, "STENCIL_INDEX4_OES"); bpp = 1; break;
				case 0x8D48 : sprintf(formatment, "STENCIL_INDEX8_OES"); bpp = 1; break;
				default: sprintf(formatment, "0x%X", internalformat); bpp = 0; break;
			}

			_add_glbuf_object(tstate->ctx->sostate->glbuf_rb, objidx, "Renderbuffer", width, height, bpp, formatment);
		}
	}
#endif // COREGL_TRACEPATH_TRACE_MEMUSE_INFO
#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	if (trace_surface_flag == 1)
	{
		MY_MODULE_TSTATE *tstate = NULL;

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
		if (tstate->ctx != NULL)
		{
			int objidx = _COREGL_INT_INIT_VALUE;
			_orig_tracepath_glGetIntegerv(GL_RENDERBUFFER_BINDING, &objidx);
			AST(objidx != _COREGL_INT_INIT_VALUE);

			{
				int channel = 0;
				switch (internalformat)
				{
					case GL_ALPHA:
					case GL_LUMINANCE:
					case GL_DEPTH_COMPONENT :
					case 0x81A5:
					case 0x81A6:
					case 0x81A7:
					case 0x8D46 :
					case 0x8D47 :
					case 0x8D48 : channel = 1; break;
					case GL_LUMINANCE_ALPHA:
					case 0x84F9: channel = 2; break;
					case GL_RGB: channel = 3; break;
					case GL_RGBA:
					case 0x80E1: channel = 4; break;
				}

				char name[256];
				sprintf(name, "FBORB_%d", objidx);
				tracepath_surface_trace_add(name, tstate->ctx->dpy, tstate->ctx->handle, tstate->surf_draw, -1, 0, objidx, width, height, channel, NULL);
			}
		}
	}
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO
}
*/


void
tracepath_glGetBufferPointervOES(GLenum target, GLenum pname, GLvoid **params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetBufferPointervOES(target, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}


void *
tracepath_glMapBufferOES(GLenum target, GLenum access)
{
	void *ret = NULL;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glMapBufferOES(target, access);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}


GLboolean
tracepath_glUnmapBufferOES(GLenum target)
{
	GLboolean ret = GL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glUnmapBufferOES(target);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

/* GLES3.1 API */
void
tracepath_glGetProgramPipelineInfoLog(GLuint pipeline, GLsizei bufSize,
				      GLsizei *length, GLchar *infoLog)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetProgramPipelineInfoLog(pipeline, bufSize, length, infoLog);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLuint
tracepath_glCreateShaderProgramv(GLenum type,  GLsizei count,
				 const GLchar *const *strings)
{
	GLboolean ret = GL_FALSE;
	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glCreateShaderProgramv(type, count, strings);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glGenProgramPipelines( GLsizei n,  GLuint *pipelines)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGenProgramPipelines(n, pipelines);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBindProgramPipeline( GLuint pipeline)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBindProgramPipeline(pipeline);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetProgramPipelineiv(GLuint pipeline, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetProgramPipelineiv(pipeline, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDeleteProgramPipelines(GLsizei n, GLuint const *pipelines)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDeleteProgramPipelines(n, pipelines);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLboolean
tracepath_glIsProgramPipeline( GLuint pipeline)
{
	GLboolean ret = GL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glIsProgramPipeline(pipeline);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glValidateProgramPipeline(GLuint pipeline)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glValidateProgramPipeline(pipeline);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDispatchCompute(GLuint num_groups_x, GLuint num_groups_y,
			    GLuint num_groups_z)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDispatchComputeIndirect( GLintptr indirect)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDispatchComputeIndirect(indirect);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDrawArraysIndirect(GLenum mode, GLvoid const *indirect)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDrawArraysIndirect(mode, indirect);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDrawElementsIndirect(GLenum mode, GLenum type,
				 GLvoid const *indirect)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDrawElementsIndirect(mode, type, indirect);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glFramebufferParameteri(GLenum target, GLenum pname, GLint param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glFramebufferParameteri(target, pname, param);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetFramebufferParameteriv( GLenum target,  GLenum pname,
				       GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetFramebufferParameteriv(target,  pname,  params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetProgramInterfaceiv(GLuint program,  GLenum programInterface,
				  GLenum pname,  GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetProgramInterfaceiv(program, programInterface, pname,
						params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLuint
tracepath_glGetProgramResourceIndex( GLuint program,  GLenum programInterface,
				     const char *name)
{
	GLuint ret = GL_INVALID_INDEX;
	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret =  _orig_tracepath_glGetProgramResourceIndex(program,  programInterface,
			name);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glGetProgramResourceName(GLuint program, GLenum programInterface,
				   GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

	_orig_tracepath_glGetProgramResourceName(program, programInterface, index,
			bufSize, length, name);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetProgramResourceiv( GLuint program,  GLenum programInterface,
				  GLuint index,  GLsizei propCount,  const GLenum *props,  GLsizei bufSize,
				  GLsizei *length,  GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

	_orig_tracepath_glGetProgramResourceiv(program,  programInterface,  index,
					       propCount,  props,  bufSize,  length,  params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLint
tracepath_glGetProgramResourceLocation(GLuint program, GLenum programInterface,
				       GLchar const *name)
{
	GLuint ret = -1;
	_COREGL_TRACEPATH_FUNC_BEGIN();

	ret = _orig_tracepath_glGetProgramResourceLocation(program, programInterface,
			name);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glUseProgramStages(GLuint pipeline, GLbitfield stages, GLuint program)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glUseProgramStages(pipeline, stages, program);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glActiveShaderProgram(GLuint pipeline, GLuint program)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glActiveShaderProgram(pipeline, program);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform1iv(GLuint program, GLint location, GLsizei count,
			      const GLint *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform1iv(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform2iv(GLuint program, GLint location, GLsizei count,
			      const GLint *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform2iv(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform3iv(GLuint program, GLint location, GLsizei count,
			      const GLint *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform3iv(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform4iv(GLuint program, GLint location, GLsizei count,
			      const GLint *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform4iv(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform1fv(GLuint program, GLint location, GLsizei count,
			      const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform1fv(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform2fv(GLuint program, GLint location, GLsizei count,
			      const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform2fv(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform3fv(GLuint program, GLint location, GLsizei count,
			      const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform3fv(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform4fv(GLuint program, GLint location, GLsizei count,
			      const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform4fv(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniformMatrix2fv(GLuint program, GLint location,
				    GLsizei count, GLboolean transpose, const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniformMatrix2fv(program, location, count, transpose,
			value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniformMatrix3fv(GLuint program, GLint location,
				    GLsizei count, GLboolean transpose, const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniformMatrix3fv(program, location, count, transpose,
			value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniformMatrix4fv(GLuint program, GLint location,
				    GLsizei count, GLboolean transpose, const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniformMatrix4fv(program, location, count, transpose,
			value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform1i(GLuint program, GLint location, GLint x)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform1i(program, location, x);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform2i(GLuint program, GLint location, GLint x, GLint y)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform2i(program, location, x, y);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform3i(GLuint program, GLint location, GLint x, GLint y,
			     GLint z)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform3i(program, location, x, y, z);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform4i(GLuint program, GLint location, GLint x, GLint y,
			     GLint z, GLint w)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform4i(program, location, x, y, z, w);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform1f(GLuint program, GLint location, GLfloat x)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform1f(program, location, x);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform2f(GLuint program, GLint location, GLfloat x,
			     GLfloat y)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform2f(program, location, x, y);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform3f(GLuint program, GLint location, GLfloat x,
			     GLfloat y, GLfloat z)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform3f(program, location, x, y, z);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform4f(GLuint program, GLint location, GLfloat x,
			     GLfloat y, GLfloat z, GLfloat w)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform4f(program, location, x, y, z, w);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform1ui(GLuint program, GLint location, GLuint x)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform1ui(program, location, x);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform2ui(GLuint program, GLint location, GLuint x,
			      GLuint y)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform2ui(program, location, x, y);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform3ui(GLuint program, GLint location, GLuint x,
			      GLuint y, GLuint z)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform3ui(program, location, x, y, z);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform4ui(GLuint program, GLint location, GLuint x,
			      GLuint y, GLuint z, GLuint w)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform4ui(program, location, x, y, z, w);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform1uiv(GLuint program, GLint location, GLsizei count,
			       const GLuint *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform1uiv(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform2uiv(GLuint program, GLint location, GLsizei count,
			       const GLuint *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform2uiv(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform3uiv(GLuint program, GLint location, GLsizei count,
			       const GLuint *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform3uiv(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniform4uiv(GLuint program, GLint location, GLsizei count,
			       const GLuint *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniform4uiv(program, location, count, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniformMatrix2x3fv(GLuint program, GLint location,
				      GLsizei count, GLboolean transpose, const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniformMatrix2x3fv(program, location, count, transpose,
			value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniformMatrix3x2fv(GLuint program, GLint location,
				      GLsizei count, GLboolean transpose, const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniformMatrix3x2fv(program, location, count, transpose,
			value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniformMatrix4x2fv(GLuint program, GLint location,
				      GLsizei count, GLboolean transpose, const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniformMatrix4x2fv(program, location, count, transpose,
			value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniformMatrix2x4fv(GLuint program, GLint location,
				      GLsizei count, GLboolean transpose, const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniformMatrix2x4fv(program, location, count, transpose,
			value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniformMatrix3x4fv(GLuint program, GLint location,
				      GLsizei count, GLboolean transpose, const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniformMatrix3x4fv(program, location, count, transpose,
			value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glProgramUniformMatrix4x3fv(GLuint program, GLint location,
				      GLsizei count, GLboolean transpose, const GLfloat *value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glProgramUniformMatrix4x3fv(program, location, count, transpose,
			value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBindImageTexture(GLuint unit, GLuint texture, GLint level,
			     GLboolean layered, GLint layer, GLenum access, GLenum format)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

	_orig_tracepath_glBindImageTexture(unit, texture, level, layered, layer, access,
					   format);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetBooleani_v (GLenum target, GLuint index, GLboolean *data)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetBooleani_v (target, index, data);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glMemoryBarrier (GLbitfield barriers)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMemoryBarrier (barriers);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glMemoryBarrierByRegion (GLbitfield barriers)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMemoryBarrierByRegion (barriers);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexStorage2DMultisample (GLenum target, GLsizei samples,
				     GLenum internalformat, GLsizei width, GLsizei height,
				     GLboolean fixedsamplelocations)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexStorage2DMultisample (target, samples, internalformat,
			width, height, fixedsamplelocations);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetMultisamplefv (GLenum pname, GLuint index, GLfloat *val)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetMultisamplefv (pname, index, val);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glSampleMaski (GLuint maskNumber, GLbitfield mask)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glSampleMaski (maskNumber, mask);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetTexLevelParameteriv (GLenum target, GLint level, GLenum pname,
				    GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTexLevelParameteriv (target, level, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetTexLevelParameterfv (GLenum target, GLint level, GLenum pname,
				    GLfloat *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTexLevelParameterfv (target, level, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBindVertexBuffer (GLuint bindingindex, GLuint buffer,
			      GLintptr offset, GLsizei stride)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBindVertexBuffer (bindingindex, buffer, offset, stride);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}


void tracepath_glVertexAttribFormat (GLuint attribindex, GLint size,
				     GLenum type, GLboolean normalized, GLuint relativeoffset)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexAttribFormat (attribindex, size, type, normalized,
					      relativeoffset);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glVertexAttribIFormat (GLuint attribindex, GLint size,
				      GLenum type, GLuint relativeoffset)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexAttribIFormat (attribindex, size, type, relativeoffset);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glVertexAttribBinding (GLuint attribindex, GLuint bindingindex)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexAttribBinding (attribindex, bindingindex);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glVertexBindingDivisor (GLuint bindingindex, GLuint divisor)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glVertexBindingDivisor (bindingindex, divisor);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}
void
tracepath_glBindFramebufferOES(GLenum target, GLuint framebuffer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	if (unlikely(trace_surface_flag == 1)) {
		GLint oldfb;
		_orig_tracepath_glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldfb);
		if (oldfb != 0) {
			_COREGL_TRACE_SURFACE(0, 2, "GLBINDFBO");
		}

		tracepath_fbdump_update(0);
	}
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	_orig_tracepath_glBindFramebufferOES(target, framebuffer);

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	if (unlikely(trace_surface_flag == 1)) {
		tracepath_fbdump_update(1);
	}
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBindRenderbufferOES(GLenum target, GLuint renderbuffer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBindRenderbufferOES(target, renderbuffer);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDeleteFramebuffersOES(GLsizei n, const GLuint *framebuffers)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(0);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	_orig_tracepath_glDeleteFramebuffersOES(n, framebuffers);

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(1);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDeleteRenderbuffersOES(GLsizei n, const GLuint *renderbuffers)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(0);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	_orig_tracepath_glDeleteRenderbuffersOES(n, renderbuffers);

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(1);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
#ifdef COREGL_TRACEPATH_TRACE_MEMUSE_INFO
	if (trace_mem_flag == 1) {
		MY_MODULE_TSTATE *tstate = NULL;

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
		if (tstate->ctx != NULL) {
			for (int i = 0; i < n; i++) {
				if (renderbuffers[i] == 0) continue;
				_remove_glbuf_object(tstate->ctx->sostate->glbuf_rb, renderbuffers[i],
						     "Renderbuffer");
			}
		}
	}
#endif // COREGL_TRACEPATH_TRACE_MEMUSE_INFO
}

void
tracepath_glDepthRangefOES(GLclampf zNear, GLclampf zFar)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDepthRangefOES(zNear, zFar);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDepthRangexOES(GLclampx zNear, GLclampx zFar)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDepthRangexOES(zNear, zFar);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glFramebufferRenderbufferOES(GLenum target, GLenum attachment,
				       GLenum renderbuffertarget, GLuint renderbuffer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(0);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	_orig_tracepath_glFramebufferRenderbufferOES(target, attachment,
			renderbuffertarget, renderbuffer);

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(1);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glFramebufferTexture2DOES(GLenum target, GLenum attachment,
				    GLenum textarget, GLuint texture, GLint level)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(0);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	_orig_tracepath_glFramebufferTexture2DOES(target, attachment, textarget,
			texture, level);

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(1);
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGenFramebuffersOES(GLsizei n, GLuint *framebuffers)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGenFramebuffersOES(n, framebuffers);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGenRenderbuffersOES(GLsizei n, GLuint *renderbuffers)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGenRenderbuffersOES(n, renderbuffers);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
#ifdef COREGL_TRACEPATH_TRACE_MEMUSE_INFO
	if (trace_mem_flag == 1) {
		MY_MODULE_TSTATE *tstate = NULL;

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
		if (tstate->ctx != NULL) {
			for (int i = 0; i < n; i++) {
				if (renderbuffers[i] == 0) continue;
				_add_glbuf_object(tstate->ctx->sostate->glbuf_rb, renderbuffers[i],
						  "Renderbuffer", 0, 0, 0, "Unknown");
			}
		}
	}
#endif // COREGL_TRACEPATH_TRACE_MEMUSE_INFO
}

void
tracepath_glGetFramebufferAttachmentParameterivOES(GLenum target,
		GLenum attachment, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetFramebufferAttachmentParameterivOES(target, attachment,
			pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetQueryObjecti64vEXT(GLuint id, GLenum pname, GLint64 *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetQueryObjecti64vEXT(id, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetQueryObjectivEXT(GLuint id, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetQueryObjectivEXT(id, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetQueryObjectui64vEXT(GLuint id, GLenum pname, GLuint64 *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetQueryObjectui64vEXT(id, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetQueryObjectuivEXT(GLuint id, GLenum pname, GLuint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetQueryObjectuivEXT(id, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetQueryivEXT(GLenum target, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetQueryivEXT(target, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBeginQueryEXT(GLenum target, GLuint id)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBeginQueryEXT(target, id);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}


void
tracepath_glDeleteQueriesEXT(GLsizei n, const GLuint *ids)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDeleteQueriesEXT(n, ids);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGenQueriesEXT(GLsizei n, GLuint *ids)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGenQueriesEXT(n, ids);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLboolean
tracepath_glIsFramebufferOES(GLuint framebuffer)
{
	GLboolean ret = GL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glIsFramebufferOES(framebuffer);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

GLboolean
tracepath_glIsQueryEXT(GLuint id)
{
	GLboolean ret = GL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glIsQueryEXT(id);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

GLboolean
tracepath_glIsRenderbufferOES(GLuint renderbuffer)
{
	GLboolean ret;
	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glIsRenderbufferOES(renderbuffer);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glEndQueryEXT(GLenum target)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glEndQueryEXT(target);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glAlphaFuncxOES(GLenum func, GLclampx ref)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glAlphaFuncxOES(func, ref);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glBlendBarrierKHR()
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBlendBarrierKHR();

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glBlendEquationOES(GLenum mode)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBlendEquationOES(mode);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glBlendEquationSeparateOES(GLenum modeRGB, GLenum modeAlpha)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBlendEquationSeparateOES(modeRGB, modeAlpha);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glBlendFuncSeparateOES(GLenum srcRGB, GLenum dstRGB,
				      GLenum srcAlpha, GLenum dstAlpha)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBlendFuncSeparateOES(srcRGB, dstRGB, srcAlpha, dstAlpha);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLenum tracepath_glCheckFramebufferStatusOES(GLenum target)
{
	GLenum ret = _COREGL_INT_INIT_VALUE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glCheckFramebufferStatusOES(target);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}


void tracepath_glClearColorxOES(GLclampx red, GLclampx green, GLclampx blue,
				GLclampx alpha)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClearColorxOES(red, green, blue, alpha);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glClearDepthfOES(GLclampf depth)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClearDepthfOES(depth);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glClearDepthxOES(GLclampx depth)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClearDepthxOES(depth);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glClipPlanefOES(GLenum plane, GLfloat const *equation)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClipPlanefOES(plane, equation);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}


void tracepath_glClipPlanexOES(GLenum plane, GLfixed const *equation)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glClipPlanexOES(plane, equation);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glColor4xOES(GLfixed red, GLfixed green, GLfixed blue,
			    GLfixed alpha)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glColor4xOES(red, green, blue, alpha);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glFogxOES(GLenum pname, GLfixed param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glFogxOES(pname, param);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glFogxvOES(GLenum pname, GLfixed const *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glFogxvOES(pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glFrustumxOES(GLfixed left, GLfixed right, GLfixed bottom,
			     GLfixed top, GLfixed zNear, GLfixed zFar)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glFrustumxOES(left, right, bottom, top, zNear, zFar);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glFrustumfOES(GLfloat left, GLfloat right, GLfloat bottom,
			     GLfloat top, GLfloat zNear, GLfloat zFar)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glFrustumfOES(left, right, bottom, top, zNear, zFar);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glGenerateMipmapOES(GLenum target)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGenerateMipmapOES(target);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glGetClipPlanefOES(GLenum pname, GLfloat eqn[4])
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetClipPlanefOES(pname, eqn);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glGetClipPlanexOES(GLenum pname, GLfixed eqn[4])
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetClipPlanexOES(pname, eqn);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glGetFixedvOES(GLenum pname, GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetFixedvOES(pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glGetLightxvOES(GLenum light, GLenum pname, GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetLightxvOES(light, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glGetMaterialxvOES(GLenum face, GLenum pname, GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetMaterialxvOES(face, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glGetPointervKHR(GLenum pname, GLvoid **params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetPointervKHR(pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glGetRenderbufferParameterivOES(GLenum target, GLenum pname,
		GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetRenderbufferParameterivOES(target, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glGetTexEnvxvOES(GLenum env, GLenum pname, GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTexEnvxvOES(env, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glGetTexParameterxvOES(GLenum target, GLenum pname,
				      GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTexParameterxvOES(target, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}


void
tracepath_glLightModelxOES (GLenum pname, GLfixed param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLightModelxOES (pname, param);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}


void
tracepath_glLightModelxvOES (GLenum pname, const GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLightModelxvOES (pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}


void
tracepath_glLightxOES (GLenum light, GLenum pname, GLfixed param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLightxOES (light, pname, param);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}


void
tracepath_glLightxvOES (GLenum light, GLenum pname, const GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLightxvOES (light, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glLineWidthxOES (GLfixed width)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLineWidthxOES (width);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glLoadMatrixxOES (const GLfixed *m)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glLoadMatrixxOES (m);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glMaterialxOES (GLenum face, GLenum pname, GLfixed param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMaterialxOES (face, pname, param);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glMaterialxvOES (GLenum face, GLenum pname, const GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMaterialxvOES (face, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glMultMatrixxOES (const GLfixed *m)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMultMatrixxOES (m);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glMultiTexCoord4bOES (GLenum target, GLbyte s, GLbyte t, GLbyte r,
				GLbyte q)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMultiTexCoord4bOES (target, s, t, r, q);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glMultiTexCoord4xOES (GLenum target, GLfixed s, GLfixed t, GLfixed r,
				GLfixed q)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMultiTexCoord4xOES (target, s, t, r, q);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glNormal3xOES (GLfixed nx, GLfixed ny, GLfixed nz)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glNormal3xOES (nx, ny, nz);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glOrthofOES (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,
		       GLfloat zNear, GLfloat zFar)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glOrthofOES (left, right, bottom, top, zNear, zFar);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glOrthoxOES (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top,
		       GLfixed zNear, GLfixed zFar)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glOrthoxOES (left, right, bottom, top, zNear, zFar);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPointParameterxOES (GLenum pname, GLfixed param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPointParameterxOES (pname, param);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPointParameterxvOES (GLenum pname, const GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPointParameterxvOES (pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPointSizexOES (GLfixed size)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPointSizexOES (size);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPolygonOffsetxOES (GLfixed factor, GLfixed units)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPolygonOffsetxOES (factor, units);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glQueryCounterEXT (GLuint id, GLenum target)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glQueryCounterEXT (id, target);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glRenderbufferStorageOES (GLenum target, GLenum internalformat,
				    GLsizei width, GLsizei height)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glRenderbufferStorageOES (target, internalformat, width,
			height);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glRotatexOES (GLfixed angle, GLfixed x, GLfixed y, GLfixed z)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glRotatexOES (angle, x, y, z);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glSampleCoveragexOES (GLclampx value, GLboolean invert)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glSampleCoveragexOES (value, invert);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glScalexOES (GLfixed x, GLfixed y, GLfixed z)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glScalexOES (x, y, z);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexEnvxOES (GLenum target, GLenum pname, GLfixed param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexEnvxOES (target, pname, param);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexEnvxvOES (GLenum target, GLenum pname, const GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexEnvxvOES (target, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexParameterxOES (GLenum target, GLenum pname, GLfixed param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexParameterxOES (target, pname, param);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexParameterxvOES (GLenum target, GLenum pname,
			       const GLfixed *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexParameterxvOES (target, pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexStorage2DEXT (GLenum target, GLsizei levels,
			     GLenum internalformat, GLsizei width, GLsizei height)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexStorage2DEXT (target, levels, internalformat, width,
					   height);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexStorage3DEXT (GLenum target, GLsizei levels,
			     GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexStorage3DEXT (target, levels, internalformat, width,
					   height, depth);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexStorage3DMultisampleOES (GLenum target, GLsizei samples,
					GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth,
					GLboolean fixedsamplelocations)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexStorage3DMultisampleOES (target, samples, internalformat,
			width, height, depth, fixedsamplelocations);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTranslatexOES (GLfixed x, GLfixed y, GLfixed z)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTranslatexOES (x, y, z);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBindVertexArrayOES(GLuint array)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBindVertexArrayOES(array);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDeleteVertexArraysOES(GLsizei n, const GLuint *arrays)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDeleteVertexArraysOES(n, arrays);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGenVertexArraysOES(GLsizei n, GLuint *arrays)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGenVertexArraysOES(n, arrays);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLboolean
tracepath_glIsVertexArrayOES(GLuint array)
{
	GLboolean ret = GL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glIsVertexArrayOES(array);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void tracepath_glDebugMessageCallbackKHR(GLDEBUGPROCKHR callback,
		void const *userParam)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDebugMessageCallbackKHR(callback, userParam);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}


void tracepath_glDebugMessageControlKHR(GLenum source, GLenum type,
					GLenum severity, GLsizei count, GLuint const *ids, GLboolean enabled)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDebugMessageControlKHR(source, type, severity, count, ids,
			enabled);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glDebugMessageInsertKHR(GLenum source, GLenum type, GLuint id,
				       GLenum severity, GLsizei length, GLchar const *buf)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDebugMessageInsertKHR(source, type, id, severity, length,
						buf);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLuint tracepath_glGetDebugMessageLogKHR(GLuint count, GLsizei bufsize,
		GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities,
		GLsizei *lengths, GLchar *messageLog)
{
	GLuint ret = GL_INVALID_INDEX;
	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret =  _orig_tracepath_glGetDebugMessageLogKHR(count, bufsize, sources, types,
			ids, severities, lengths, messageLog);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void tracepath_glGetObjectLabelKHR(GLenum identifier, GLuint name,
				   GLsizei bufSize, GLsizei *length, GLchar *label)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetObjectLabelKHR(identifier, name, bufSize, length, label);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void tracepath_glGetObjectPtrLabelKHR(void const *ptr, GLsizei bufSize,
				      GLsizei *length, GLchar *label)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetObjectPtrLabelKHR(ptr, bufSize, length, label);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetPointerv(GLenum pname, GLvoid **params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetPointerv(pname, params);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glObjectLabelKHR (GLenum identifier, GLuint name, GLsizei length,
			    const GLchar *label)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glObjectLabelKHR (identifier, name, length, label);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glObjectPtrLabelKHR (const void *ptr, GLsizei length,
			       const GLchar *label)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glObjectPtrLabelKHR (ptr, length, label);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPopDebugGroupKHR (void)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPopDebugGroupKHR ();

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPushDebugGroupKHR (GLenum source, GLuint id, GLsizei length,
			       const GLchar *message)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPushDebugGroupKHR (source, id, length, message);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

/* GLES3.2 API*/
void
tracepath_glBlendBarrier(void)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBlendBarrier();
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glCopyImageSubData (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glCopyImageSubData(srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDebugMessageControl (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDebugMessageControl (source, type, severity, count, ids, enabled);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDebugMessageInsert(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDebugMessageInsert(source, type, id, severity, length, buf);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDebugMessageCallback(GLDEBUGPROC callback, const void *userParam)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDebugMessageCallback(callback, userParam);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLuint
tracepath_glGetDebugMessageLog(GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog)
{
	GLuint ret = _COREGL_INT_INIT_VALUE;
	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glGetDebugMessageLog(count, bufSize, sources, types, ids, severities, lengths, messageLog);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glPushDebugGroup(GLenum source, GLuint id, GLsizei length, const GLchar *message)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPushDebugGroup(source, id, length, message);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPopDebugGroup(void)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPopDebugGroup();
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glObjectLabel(GLenum identifier, GLuint name, GLsizei length, const GLchar *label)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glObjectLabel(identifier, name, length, label);
	goto finish;

	finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetObjectLabel(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetObjectLabel(identifier, name, bufSize, length, label);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glObjectPtrLabel(const void *ptr, GLsizei length, const GLchar *label)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glObjectPtrLabel(ptr, length, label);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetObjectPtrLabel(const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetObjectPtrLabel(ptr, bufSize, length, label);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glEnablei(GLenum target, GLuint index)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glEnablei(target, index);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDisablei(GLenum target, GLuint index)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDisablei(target, index);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBlendEquationi(GLuint buf, GLenum mode)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBlendEquationi(buf, mode);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBlendEquationSeparatei(GLuint buf, GLenum modeRGB, GLenum modeAlpha)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBlendEquationSeparatei(buf, modeRGB, modeAlpha);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBlendFunci(GLuint buf, GLenum src, GLenum dst)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBlendFunci(buf, src, dst);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glBlendFuncSeparatei(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glBlendFuncSeparatei(buf, srcRGB, dstRGB, srcAlpha, dstAlpha);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glColorMaski(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glColorMaski(index, r, g, b, a);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLboolean
tracepath_glIsEnabledi(GLenum target, GLuint index)
{
	GLboolean ret = GL_FALSE;
	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_glIsEnabledi(target, index);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDrawElementsBaseVertex(mode, count, type, indices, basevertex);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDrawRangeElementsBaseVertex(mode, start, end, count, type, indices, basevertex);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glDrawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glDrawElementsInstancedBaseVertex(mode, count, type, indices, instancecount, basevertex);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glFramebufferTexture(GLenum target, GLenum attachment, GLuint texture, GLint level)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glFramebufferTexture(target, attachment, texture, level);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPrimitiveBoundingBox(GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPrimitiveBoundingBox(minX, minY, minZ, minW, maxX, maxY, maxZ, maxW);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

GLenum
tracepath_glGetGraphicsResetStatus(void)
{
	GLenum ret = GL_NONE;
	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret =  _orig_tracepath_glGetGraphicsResetStatus();
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void
tracepath_glReadnPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glReadnPixels(x, y, width, height, format, type, bufSize, data);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetnUniformfv(GLuint program, GLint location, GLsizei bufSize, GLfloat *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetnUniformfv(program, location, bufSize, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetnUniformiv(GLuint program, GLint location, GLsizei bufSize, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetnUniformiv(program, location, bufSize, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetnUniformuiv(GLuint program, GLint location, GLsizei bufSize, GLuint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetnUniformuiv(program, location, bufSize, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glMinSampleShading(GLfloat value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glMinSampleShading(value);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPatchParameteri(GLenum pname, GLint value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPatchParameteri(pname, value);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexParameterIiv(GLenum target, GLenum pname, const GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexParameterIiv(target, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexParameterIuiv(GLenum target, GLenum pname, const GLuint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexParameterIuiv(target, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetTexParameterIiv(GLenum target, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTexParameterIiv(target, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetTexParameterIuiv(GLenum target, GLenum pname, GLuint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTexParameterIuiv(target, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glSamplerParameterIiv(GLuint sampler, GLenum pname, const GLint *param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glSamplerParameterIiv(sampler, pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glSamplerParameterIuiv(GLuint sampler, GLenum pname, const GLuint *param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glSamplerParameterIuiv(sampler, pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetSamplerParameterIiv(GLuint sampler, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetSamplerParameterIiv(sampler, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetSamplerParameterIuiv(GLuint sampler, GLenum pname, GLuint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetSamplerParameterIuiv(sampler, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexBuffer(GLenum target, GLenum internalformat, GLuint buffer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexBuffer(target, internalformat, buffer);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexBufferRange(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexBufferRange(target, internalformat, buffer, offset, size);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexStorage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexStorage3DMultisample(target, samples, internalformat, width, height, depth, fixedsamplelocations);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glFramebufferTextureOES(GLenum target, GLenum attachment, GLuint texture, GLint level)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(0);
#endif

	_orig_tracepath_glFramebufferTextureOES(target, attachment,texture, level);

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(1);
#endif

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexBufferRangeOES(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexBufferRangeOES(target, internalformat, buffer, offset, size);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glSamplerParameterIuivOES(GLuint sampler, GLenum pname, const GLuint *param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glSamplerParameterIuivOES(sampler, pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glSamplerParameterIivOES(GLuint sampler, GLenum pname, const GLint *param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glSamplerParameterIivOES(sampler, pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetSamplerParameterIuivOES(GLuint sampler, GLenum pname, GLuint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetSamplerParameterIuivOES(sampler, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetSamplerParameterIivOES(GLuint sampler, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetSamplerParameterIivOES(sampler, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetTexParameterIuivOES(GLenum target, GLenum pname, GLuint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTexParameterIuivOES(target, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexParameterIivOES(GLenum target, GLenum pname, const GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexParameterIivOES(target, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPatchParameteriOES(GLenum pname, GLint value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPatchParameteriOES(pname, value);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexParameterIuivOES(GLenum target, GLenum pname, const GLuint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexParameterIuivOES(target, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetTexParameterIivOES(GLenum target, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTexParameterIivOES(target, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetSamplerParameterIivEXT(GLuint sampler, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetSamplerParameterIivEXT(sampler, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetSamplerParameterIuivEXT(GLuint sampler, GLenum pname, GLuint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetSamplerParameterIuivEXT(sampler, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glSamplerParameterIivEXT(GLuint sampler, GLenum pname, const GLint *param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glSamplerParameterIivEXT(sampler, pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glFramebufferTextureEXT(GLenum target, GLenum attachment, GLuint texture, GLint level)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(0);
#endif

	_orig_tracepath_glFramebufferTextureEXT(target, attachment,texture, level);

#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	tracepath_fbdump_update(1);
#endif

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexBufferRangeEXT(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexBufferRangeEXT(target, internalformat, buffer, offset, size);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glSamplerParameterIuivEXT(GLuint sampler, GLenum pname, const GLuint *param)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glSamplerParameterIuivEXT(sampler, pname, param);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetTexParameterIuivEXT(GLenum target, GLenum pname, GLuint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTexParameterIuivEXT(target, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glGetTexParameterIivEXT(GLenum target, GLenum pname, GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glGetTexParameterIivEXT(target, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glPatchParameteriEXT(GLenum pname, GLint value)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glPatchParameteriEXT(pname, value);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexParameterIivEXT(GLenum target, GLenum pname, const GLint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexParameterIivEXT(target, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}

void
tracepath_glTexParameterIuivEXT(GLenum target, GLenum pname, const GLuint *params)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexParameterIuivEXT(target, pname, params);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}
void
tracepath_glTexBufferEXT(GLenum target, GLenum internalformat, GLuint buffer)
{
	_COREGL_TRACEPATH_FUNC_BEGIN();
	_orig_tracepath_glTexBufferOES(target, internalformat, buffer);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
}
