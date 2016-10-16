#include <stdio.h>
#include <dlfcn.h>

# include <GLES/glplatform.h>
# include "../headers/gl.h"
# include "../coregl_internal.h"

#define COREGL_API           __attribute__((visibility("default")))

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     COREGL_API extern RET_TYPE FUNC_NAME PARAM_LIST;
#define _COREGL_EXT_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)
# include "../headers/sym_gl1.h"
#undef _COREGL_EXT_SYMBOL
#undef _COREGL_SYMBOL

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     RET_TYPE (*ovr_##FUNC_NAME) PARAM_LIST = NULL;
#define _COREGL_EXT_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)
# include "../headers/sym_gl1.h"

#undef _COREGL_EXT_SYMBOL
#undef _COREGL_SYMBOL

#define INIT_EXPORT()

void *lib_handle = NULL;
void (* set_driver_gl_version)(int version);

__attribute__((constructor))
int
coregl_glwrap_init()
{
	lib_handle = dlopen(_COREGL_LIB, RTLD_NOW);
	if (!lib_handle) {
		COREGL_ERR("%s", dlerror());
		COREGL_ERR("Invalid library link! (Check linkage of libEGL -> libCOREGL)");
		return 0;
	}

	set_driver_gl_version = dlsym(lib_handle, "set_driver_gl_version");
	if (set_driver_gl_version)
		set_driver_gl_version(1);
	else
		printf("%s\n", dlerror());

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST) \
   ovr_##FUNC_NAME = (__typeof__(ovr_##FUNC_NAME))dlsym(lib_handle, "coregl_api_"#FUNC_NAME); \
	if (ovr_##FUNC_NAME == NULL) \
	{ \
		COREGL_ERR("Can't find a symbol '%s'!", #FUNC_NAME); \
		COREGL_ERR("Invalid library link! (Check linkage of libGLESv1_CM -> libCOREGL)"); \
	}

#define _COREGL_EXT_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)
#include "../headers/sym_gl1.h"
#undef _COREGL_EXT_SYMBOL
#undef _COREGL_SYMBOL

	return 1;
}

void
glAlphaFunc(GLenum func, GLclampf ref)
{
	ovr_glAlphaFunc(func, ref);
}

void
glClipPlanef(GLenum plane, const GLfloat *equation)
{
	ovr_glClipPlanef(plane, equation);
}

void
glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	ovr_glColor4f(red, green, blue, alpha);
}

void
glFogf(GLenum pname, GLfloat param)
{
	ovr_glFogf(pname, param);
}

void
glFogfv(GLenum pname, const GLfloat *params)
{
	ovr_glFogfv(pname, params);
}

void
glFrustumf(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,
		   GLfloat zNear, GLfloat zFar)
{
	ovr_glFrustumf(left, right, bottom, top, zNear, zFar);
}

void
glGetClipPlanef(GLenum pname, GLfloat eqn[4])
{
	ovr_glGetClipPlanef(pname, eqn);
}

void
glGetLightfv(GLenum light, GLenum pname, GLfloat *params)
{
	ovr_glGetLightfv(light, pname, params);
}

void
glGetMaterialfv(GLenum face, GLenum pname, GLfloat *params)
{
	ovr_glGetMaterialfv(face, pname, params);
}

void
glGetTexEnvfv(GLenum env, GLenum pname, GLfloat *params)
{
	ovr_glGetTexEnvfv(env, pname, params);
}

void
glLightModelf(GLenum pname, GLfloat param)
{
	ovr_glLightModelf(pname, param);
}

void
glLightModelfv(GLenum pname, const GLfloat *params)
{
	ovr_glLightModelfv(pname, params);
}

void
glLightf(GLenum light, GLenum pname, GLfloat param)
{
	ovr_glLightf(light, pname, param);
}

void
glLightfv(GLenum light, GLenum pname, const GLfloat *params)
{
	ovr_glLightfv(light, pname, params);
}

void
glLoadMatrixf(const GLfloat *m)
{
	ovr_glLoadMatrixf(m);
}

void
glMaterialf(GLenum face, GLenum pname, GLfloat param)
{
	ovr_glMaterialf(face, pname, param);
}

void
glMaterialfv(GLenum face, GLenum pname, const GLfloat *params)
{
	ovr_glMaterialfv(face, pname, params);
}

void
glMultMatrixf(const GLfloat *m)
{
	ovr_glMultMatrixf(m);
}

void
glMultiTexCoord4f(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
	ovr_glMultiTexCoord4f(target, s, t, r, q);
}

void
glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)
{
	ovr_glNormal3f(nx, ny, nz);
}

void
glOrthof(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,
		 GLfloat zNear, GLfloat zFar)
{
	ovr_glOrthof(left, right, bottom, top, zNear, zFar);
}

void
glPointParameterf(GLenum pname, GLfloat param)
{
	ovr_glPointParameterf(pname, param);
}

void
glPointParameterfv(GLenum pname, const GLfloat *params)
{
	ovr_glPointParameterfv(pname, params);
}

void
glPointSize(GLfloat size)
{
	ovr_glPointSize(size);
}

void
glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	ovr_glRotatef(angle, x, y, z);
}

void
glScalef(GLfloat x, GLfloat y, GLfloat z)
{
	ovr_glScalef(x, y, z);
}

void
glTexEnvf(GLenum target, GLenum pname, GLfloat param)
{
	ovr_glTexEnvf(target, pname, param);
}

void
glTexEnvfv(GLenum target, GLenum pname, const GLfloat *params)
{
	ovr_glTexEnvfv(target, pname, params);
}

void
glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
	ovr_glTranslatef(x, y, z);
}

void
glAlphaFuncx(GLenum func, GLclampx ref)
{
	ovr_glAlphaFuncx(func, ref);
}

void
glClearColorx(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha)
{
	ovr_glClearColorx(red, green, blue, alpha);
}

void
glClearDepthx(GLclampx depth)
{
	ovr_glClearDepthx(depth);
}

void
glClientActiveTexture(GLenum texture)
{
	ovr_glClientActiveTexture(texture);
}

void
glClipPlanex(GLenum plane, const GLfixed *equation)
{
	ovr_glClipPlanex(plane, equation);
}

void
glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
	ovr_glColor4ub(red, green, blue, alpha);
}

void
glColor4x(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)
{
	ovr_glColor4x(red, green, blue, alpha);
}

void
glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	ovr_glColorPointer(size, type, stride, pointer);
}

void
glDepthRangex(GLclampx zNear, GLclampx zFar)
{
	ovr_glDepthRangex(zNear, zFar);
}

void
glDisableClientState(GLenum array)
{
	ovr_glDisableClientState(array);
}

void
glEnableClientState(GLenum array)
{
	ovr_glEnableClientState(array);
}

void
glFogx(GLenum pname, GLfixed param)
{
	ovr_glFogx(pname, param);
}

void
glFogxv(GLenum pname, const GLfixed *params)
{
	ovr_glFogxv(pname, params);
}

void
glFrustumx(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top,
		   GLfixed zNear, GLfixed zFar)
{
	ovr_glFrustumx(left, right, bottom, top, zNear, zFar);
}

void
glGetClipPlanex(GLenum pname, GLfixed eqn[4])
{
	ovr_glGetClipPlanex(pname, eqn);
}

void
glGetFixedv(GLenum pname, GLfixed *params)
{
	ovr_glGetFixedv(pname, params);
}

void
glGetLightxv(GLenum light, GLenum pname, GLfixed *params)
{
	ovr_glGetLightxv(light, pname, params);
}

void
glGetMaterialxv(GLenum face, GLenum pname, GLfixed *params)
{
	ovr_glGetMaterialxv(face, pname, params);
}

void
glGetTexEnviv(GLenum env, GLenum pname, GLint *params)
{
	ovr_glGetTexEnviv(env, pname, params);
}

void
glGetTexEnvxv(GLenum env, GLenum pname, GLfixed *params)
{
	ovr_glGetTexEnvxv(env, pname, params);
}

void
glGetTexParameterxv(GLenum target, GLenum pname, GLfixed *params)
{
	ovr_glGetTexParameterxv(target, pname, params);
}

void
glLightModelx(GLenum pname, GLfixed param)
{
	ovr_glLightModelx(pname, param);
}

void
glLightModelxv(GLenum pname, const GLfixed *params)
{
	ovr_glLightModelxv(pname, params);
}

void
glLightx(GLenum light, GLenum pname, GLfixed param)
{
	ovr_glLightx(light, pname, param);
}

void
glLightxv(GLenum light, GLenum pname, const GLfixed *params)
{
	ovr_glLightxv(light, pname, params);
}

void
glLineWidthx(GLfixed width)
{
	ovr_glLineWidthx(width);
}

void
glLoadIdentity(void)
{
	ovr_glLoadIdentity();
}

void
glLoadMatrixx(const GLfixed *m)
{
	ovr_glLoadMatrixx(m);
}

void
glLogicOp(GLenum opcode)
{
	ovr_glLogicOp(opcode);
}

void
glMaterialx(GLenum face, GLenum pname, GLfixed param)
{
	ovr_glMaterialx(face, pname, param);
}

void
glMaterialxv(GLenum face, GLenum pname, const GLfixed *params)
{
	ovr_glMaterialxv(face, pname, params);
}

void
glMatrixMode(GLenum mode)
{
	ovr_glMatrixMode(mode);
}

void
glMultMatrixx(const GLfixed *m)
{
	ovr_glMultMatrixx(m);
}

void
glMultiTexCoord4x(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q)
{
	ovr_glMultiTexCoord4x(target, s, t, r, q);
}

void
glNormal3x(GLfixed nx, GLfixed ny, GLfixed nz)
{
	ovr_glNormal3x(nx, ny, nz);
}

void
glNormalPointer(GLenum type, GLsizei stride, const GLvoid *pointer)
{
	ovr_glNormalPointer(type, stride, pointer);
}

void
glOrthox(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top,
		 GLfixed zNear, GLfixed zFar)
{
	ovr_glOrthox(left, right, bottom, top, zNear, zFar);
}

void
glPointParameterx(GLenum pname, GLfixed param)
{
	ovr_glPointParameterx(pname, param);
}

void
glPointParameterxv(GLenum pname, const GLfixed *params)
{
	ovr_glPointParameterxv(pname, params);
}

void
glPointSizex(GLfixed size)
{
	ovr_glPointSizex(size);
}

void
glPolygonOffsetx(GLfixed factor, GLfixed units)
{
	ovr_glPolygonOffsetx(factor, units);
}

void
glPopMatrix(void)
{
	ovr_glPopMatrix();
}

void
glPushMatrix(void)
{
	ovr_glPushMatrix();
}

void
glRotatex(GLfixed angle, GLfixed x, GLfixed y, GLfixed z)
{
	ovr_glRotatex (angle, x, y, z);
}

void
glSampleCoveragex(GLclampx value, GLboolean invert)
{
	ovr_glSampleCoveragex(value, invert);
}

void
glScalex(GLfixed x, GLfixed y, GLfixed z)
{
	ovr_glScalex (x, y, z);
}

void
glShadeModel(GLenum mode)
{
	ovr_glShadeModel(mode);
}

void
glTexCoordPointer(GLint size, GLenum type, GLsizei stride,
				  const GLvoid *pointer)
{
	ovr_glTexCoordPointer(size, type, stride, pointer);
}

void
glTexEnvi(GLenum target, GLenum pname, GLint param)
{
	ovr_glTexEnvi(target, pname, param);
}

void
glTexEnvx(GLenum target, GLenum pname, GLfixed param)
{
	ovr_glTexEnvx(target, pname, param);
}

void
glTexEnviv(GLenum target, GLenum pname, const GLint *params)
{
	ovr_glTexEnviv(target, pname, params);
}

void
glTexEnvxv(GLenum target, GLenum pname, const GLfixed *params)
{
	ovr_glTexEnvxv(target, pname, params);
}

void
glTexParameterx(GLenum target, GLenum pname, GLfixed param)
{
	ovr_glTexParameterx(target, pname, param);
}

void
glTexParameterxv(GLenum target, GLenum pname, const GLfixed *params)
{
	ovr_glTexParameterxv(target, pname, params);
}

void
glTranslatex(GLfixed x, GLfixed y, GLfixed z)
{
	ovr_glTranslatex(x, y, z);
}

void
glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	ovr_glVertexPointer(size, type, stride, pointer);
}

/* Khronos GLES 1.1 CTS requires glPointSizePointerOES as shared library symbol.*/
void
glPointSizePointerOES(GLenum type, GLsizei stride, const GLvoid *pointer)
{
	ovr_glPointSizePointerOES(type, stride, pointer);
}
