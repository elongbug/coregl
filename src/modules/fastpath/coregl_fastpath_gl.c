#include "coregl_fastpath.h"

#include <stdlib.h>

#define CURR_STATE_COMPARE(curr_state, state ) \
   if ((current_ctx->curr_state[0]) != (state))

#define IF_GL_SUCCESS(orig_gl_func) \
	_get_gl_error(); \
	orig_gl_func; \
	if (_get_gl_error() == GL_NO_ERROR)

#define DEFINE_FASTPAH_GL_FUNC() \
   MY_MODULE_TSTATE *tstate = NULL; \
   GLGlueContext *current_ctx = NULL;

#define INIT_FASTPATH_GL_FUNC() \
   GET_MY_TSTATE(tstate, get_current_thread_state()); \
   if (tstate == NULL || tstate->cstate == NULL) \
   { \
		ERR("\E[0;31;1mWARNING : '%s' called when GLES2 context is not binded (Check MakeCurrent)!\E[0m\n", __func__); \
		goto finish; \
   } \
   current_ctx = (GLGlueContext *)tstate->cstate->data; \
   AST(current_ctx != NULL);

#define GET_REAL_OBJ(type, glue_handle, real_handle) \
	_get_real_obj(current_ctx->sostate, type, glue_handle, real_handle)
#define GET_GLUE_OBJ(type, real_handle, glue_handle) \
	_get_glue_obj(current_ctx->sostate, type, real_handle, glue_handle)

static inline int
_get_real_obj(GL_Shared_Object_State *sostate, GL_Object_Type type, GLuint glue_handle, GLuint *real_handle)
{
	if (glue_handle == 0)
	{
		*real_handle = 0;
	}
	else
	{
		AST(sostate != NULL);
		*real_handle = fastpath_sostate_get_object(sostate, type, glue_handle);
		if (*real_handle == 0)
			return 0;
	}
	return 1;
}

static inline int
_get_glue_obj(GL_Shared_Object_State *sostate, GL_Object_Type type, GLuint real_handle, GLuint *glue_handle)
{
	if (real_handle == 0)
	{
		*glue_handle = 0;
	}
	else
	{
		AST(sostate != NULL);
		*glue_handle = fastpath_sostate_find_object(sostate, type, real_handle);
		if (*glue_handle == 0)
			return 0;
	}
	return 1;
}

static inline GLenum
_get_gl_error()
{
	GLenum glerror = GL_NONE;
	DEFINE_FASTPAH_GL_FUNC();
	INIT_FASTPATH_GL_FUNC();

	glerror = _orig_fastpath_glGetError();

	if (current_ctx->gl_error == GL_NO_ERROR && glerror != GL_NO_ERROR)
	{
		current_ctx->gl_error = glerror;
	}
	goto finish;

finish:
	return glerror;
}

static inline void
_set_gl_error(GLenum error)
{
	DEFINE_FASTPAH_GL_FUNC();
	INIT_FASTPATH_GL_FUNC();

	_get_gl_error();

	if (current_ctx->gl_error == GL_NO_ERROR && error != GL_NO_ERROR)
	{
		current_ctx->gl_error = error;
	}
	goto finish;

finish:
	return;
}

typedef struct
{
	GLsizei shader_count;
	GLuint shaders[10];
	GLuint is_deleting;
} Program_object_attached_tag;

GLuint
_create_program_object(GL_Shared_Object_State *sostate, int is_program, GLenum shader_type)
{
	GLuint ret = 0;
	GLuint real_obj = 0;

	if (is_program == 1)
		real_obj = _orig_fastpath_glCreateProgram();
	else
		real_obj = _orig_fastpath_glCreateShader(shader_type);

	if (real_obj != 0)
	{
		ret = fastpath_sostate_create_object(sostate, GL_OBJECT_TYPE_PROGRAM, real_obj);

		Program_object_attached_tag *poat = NULL;
		poat = (Program_object_attached_tag *)calloc(1, sizeof(Program_object_attached_tag));
		AST(poat != NULL);

		poat->is_deleting = 0;
		poat->shader_count = 0;

		fastpath_sostate_set_object_tag(sostate, GL_OBJECT_TYPE_PROGRAM, ret, poat);
	}

	return ret;
}

static void
_update_program_attach_info(GL_Shared_Object_State *sostate, GLuint program)
{
	Program_object_attached_tag *poat = NULL;
	GLuint real_program = _COREGL_INT_INIT_VALUE;

	poat = (Program_object_attached_tag *)fastpath_sostate_get_object_tag(sostate, GL_OBJECT_TYPE_PROGRAM, program);
	AST(poat != NULL);

	real_program = fastpath_sostate_get_object(sostate, GL_OBJECT_TYPE_PROGRAM, program);
	AST(real_program > 0);

	_orig_fastpath_glGetAttachedShaders(real_program, 10, &poat->shader_count, poat->shaders);
}

static void
_attach_program_object(GL_Shared_Object_State *sostate, GLuint object)
{
	if (object != 0)
	{
		fastpath_sostate_use_object(sostate, GL_OBJECT_TYPE_PROGRAM, object);
	}
}

static int
_is_deleted_program_object(GL_Shared_Object_State *sostate, GLuint glue_object)
{
	Program_object_attached_tag *poat = NULL;
	poat = (Program_object_attached_tag *)fastpath_sostate_get_object_tag(sostate, GL_OBJECT_TYPE_PROGRAM, glue_object);
	AST(poat != NULL);
	return poat->is_deleting;
}

static void
_detach_program_object(GL_Shared_Object_State *sostate, GLuint real_object, int is_program, int is_deleting)
{
	if (real_object != 0)
	{
		GLuint object = _COREGL_INT_INIT_VALUE;
		Program_object_attached_tag *poat = NULL;

		object = fastpath_sostate_find_object(sostate, GL_OBJECT_TYPE_PROGRAM, real_object);
		AST(object != 0);

		poat = (Program_object_attached_tag *)fastpath_sostate_get_object_tag(sostate, GL_OBJECT_TYPE_PROGRAM, object);
		AST(poat != NULL);

		if (is_deleting == 1)
		{
			if (poat->is_deleting == 0)
			{
				poat->is_deleting = 1;
				fastpath_sostate_remove_object(sostate, GL_OBJECT_TYPE_PROGRAM, object);
			}
		}
		else
		{
			fastpath_sostate_remove_object(sostate, GL_OBJECT_TYPE_PROGRAM, object);
		}

		if (fastpath_sostate_get_object(sostate, GL_OBJECT_TYPE_PROGRAM, object) == 0)
		{
			// Is completely removed. De-referencing attached shader objects
			int i;
			for (i = 0; i < poat->shader_count; i++)
			{
				GLuint glue_shader = fastpath_sostate_find_object(sostate, GL_OBJECT_TYPE_PROGRAM, poat->shaders[i]);
				fastpath_sostate_remove_object(sostate, GL_OBJECT_TYPE_PROGRAM, glue_shader);
			}

			poat = NULL;
			free(poat);

			if (is_program == 1)
				_orig_fastpath_glDeleteProgram(real_object);
			else
				_orig_fastpath_glDeleteShader(real_object);
		}
	}
}


void
fastpath_release_gl_context(GLGlueContext *gctx)
{
	// Release program
	if (gctx->gl_current_program[0] != 0)
	{
		_detach_program_object(gctx->sostate, gctx->gl_current_program[0], 1, 0);
		gctx->gl_current_program[0] = 0;
	}
}

GLenum
fastpath_glGetError(void)
{
	GLenum ret = GL_NONE;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (current_ctx->gl_error != GL_NO_ERROR)
	{
		ret = current_ctx->gl_error;
		current_ctx->gl_error = GL_NO_ERROR;
		_orig_fastpath_glGetError();
	}
	else
	{
		ret = _orig_fastpath_glGetError();
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return ret;
}

////////////////////////////////////////////////////////////////////////

void
fastpath_glActiveTexture(GLenum texture)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	CURR_STATE_COMPARE(gl_active_texture, texture)
	{
		IF_GL_SUCCESS(_orig_fastpath_glActiveTexture(texture))
		{
			current_ctx->_tex_flag1 |= FLAG_BIT_1;
			current_ctx->gl_active_texture[0] = texture;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glGenTextures(GLsizei n, GLuint* textures)
{
	int i;
	GLuint *objid_array = NULL;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (n < 0)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}
	if (n == 0) goto finish;
	if (textures == NULL) goto finish;

	AST(current_ctx->sostate != NULL);

	objid_array = (GLuint *)calloc(1, sizeof(GLuint) * n);

	_orig_fastpath_glGenTextures(n, objid_array);

	for (i = 0; i < n; i++)
	{
		textures[i] = fastpath_sostate_create_object(current_ctx->sostate, GL_OBJECT_TYPE_TEXTURE, objid_array[i]);
	}

	goto finish;

finish:
	if (objid_array != NULL)
	{
		free(objid_array);
		objid_array = NULL;
	}
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glBindTexture(GLenum target, GLuint texture)
{
	int active_idx;
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	active_idx = current_ctx->gl_active_texture[0] - GL_TEXTURE0;

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_TEXTURE, texture, &real_obj) != 1)
	{
		_set_gl_error(GL_OUT_OF_MEMORY);
		goto finish;
	}

	if (target == GL_TEXTURE_2D)
	{
		if (current_ctx->gl_tex_2d_state[active_idx] != real_obj)
		{
			IF_GL_SUCCESS(_orig_fastpath_glBindTexture(target, real_obj))
			{
				current_ctx->_tex_flag1 |= FLAG_BIT_3;
				current_ctx->gl_tex_2d_state[active_idx] = real_obj;
			}
		}
	}
	else if (target == GL_TEXTURE_CUBE_MAP)
	{
		if (current_ctx->gl_tex_cube_state[active_idx] != real_obj)
		{
			IF_GL_SUCCESS(_orig_fastpath_glBindTexture(target, real_obj))
			{
				current_ctx->_tex_flag1 |= FLAG_BIT_4;
				current_ctx->gl_tex_cube_state[active_idx] = real_obj;
			}
		}
	}
	else
	{
		_set_gl_error(GL_INVALID_ENUM);
		goto finish;
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_TEXTURE, texture, &real_obj) != 1)
	{
		_set_gl_error(GL_OUT_OF_MEMORY);
		goto finish;
	}

	_orig_fastpath_glFramebufferTexture2D(target, attachment, textarget, real_obj, level);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glFramebufferTexture2DMultisampleEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_TEXTURE, texture, &real_obj) != 1)
	{
		_set_gl_error(GL_OUT_OF_MEMORY);
		goto finish;
	}

	_orig_fastpath_glFramebufferTexture2DMultisampleEXT(target, attachment, textarget, real_obj, level, samples);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

GLboolean
fastpath_glIsTexture(GLuint texture)
{
	GLboolean ret = GL_FALSE;
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_TEXTURE, texture, &real_obj) != 1)
	{
		ret = GL_FALSE;
		goto finish;
	}

	ret = _orig_fastpath_glIsTexture(real_obj);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return ret;
}


void
fastpath_glDeleteTextures(GLsizei n, const GLuint* textures)
{
	int i, j;
	GLuint *objid_array = NULL;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (n < 0)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}
	if (n == 0) goto finish;
	if (textures == NULL) goto finish;

	AST(current_ctx->sostate != NULL);

	objid_array = (GLuint *)calloc(1, sizeof(GLuint) * n);
	{
		int real_n = 0;

		for (i = 0; i < n; i++)
		{
			int real_objid = _COREGL_INT_INIT_VALUE;
			if (textures[i] == 0) continue;

			real_objid = fastpath_sostate_get_object(current_ctx->sostate, GL_OBJECT_TYPE_TEXTURE, textures[i]);
			if (real_objid == 0) continue;

			AST(fastpath_sostate_remove_object(current_ctx->sostate, GL_OBJECT_TYPE_TEXTURE, textures[i]) == 1);
			objid_array[real_n++] = real_objid;
		}

		IF_GL_SUCCESS(_orig_fastpath_glDeleteTextures(real_n, objid_array))
		{
			for (i = 0; i < real_n; i++)
			{
				General_Trace_List *current = NULL;
				current = current_ctx->sostate->using_gctxs;

				while (current != NULL)
				{
					GLGlueContext *cur_gctx = (GLGlueContext *)current->value;

					for (j = 0; j < cur_gctx->gl_num_tex_units[0]; j++)
					{
						if (cur_gctx->gl_tex_2d_state[j] == objid_array[i])
							cur_gctx->gl_tex_2d_state[j] = 0;
						if (cur_gctx->gl_tex_cube_state[j] == objid_array[i])
							cur_gctx->gl_tex_cube_state[j] = 0;
					}

					current = current->next;
				}
			}
		}
	}

	goto finish;

finish:
	if (objid_array != NULL)
	{
		free(objid_array);
		objid_array = NULL;
	}
	_COREGL_FASTPATH_FUNC_END();
}

////////////////////////////////////////////////////////////////////////

void
fastpath_glGenBuffers(GLsizei n, GLuint* buffers)
{
	int i;
	GLuint *objid_array = NULL;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (n < 0)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}
	if (n == 0) goto finish;
	if (buffers == NULL) goto finish;

	AST(current_ctx->sostate != NULL);

	objid_array = (GLuint *)calloc(1, sizeof(GLuint) * n);

	IF_GL_SUCCESS(_orig_fastpath_glGenBuffers(n, objid_array))
	{
		for (i = 0; i < n; i++)
		{
			buffers[i] = fastpath_sostate_create_object(current_ctx->sostate, GL_OBJECT_TYPE_BUFFER, objid_array[i]);
		}
	}

	goto finish;

finish:
	if (objid_array != NULL)
	{
		free(objid_array);
		objid_array = NULL;
	}
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glBindBuffer(GLenum target, GLuint buffer)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_BUFFER, buffer, &real_obj) != 1)
	{
		_set_gl_error(GL_OUT_OF_MEMORY);
		goto finish;
	}

	if (target == GL_ARRAY_BUFFER)
	{
		CURR_STATE_COMPARE(gl_array_buffer_binding, real_obj)
		{
			IF_GL_SUCCESS(_orig_fastpath_glBindBuffer(target, real_obj))
			{
				if (real_obj == 0)
					current_ctx->_bind_flag &= (~FLAG_BIT_0);
				else
					current_ctx->_bind_flag |= FLAG_BIT_0;

				current_ctx->gl_array_buffer_binding[0] = real_obj;
			}
		}
	}
	else if (target == GL_ELEMENT_ARRAY_BUFFER)
	{
		CURR_STATE_COMPARE(gl_element_array_buffer_binding, real_obj)
		{
			IF_GL_SUCCESS(_orig_fastpath_glBindBuffer(target, real_obj))
			{
				if (real_obj == 0)
					current_ctx->_bind_flag &= (~FLAG_BIT_1);
				else
					current_ctx->_bind_flag |= FLAG_BIT_1;

				current_ctx->gl_element_array_buffer_binding[0] = real_obj;
			}
		}
	}
	else
	{
		_set_gl_error(GL_INVALID_ENUM);
		goto finish;
	}

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

GLboolean
fastpath_glIsBuffer(GLuint buffer)
{
	GLboolean ret = GL_FALSE;
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_BUFFER, buffer, &real_obj) != 1)
	{
		ret = GL_FALSE;
		goto finish;
	}

	ret = _orig_fastpath_glIsBuffer(real_obj);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return ret;
}


void
fastpath_glDeleteBuffers(GLsizei n, const GLuint* buffers)
{
	int i;
	GLuint *objid_array = NULL;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (n < 0)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}
	if (n == 0) goto finish;
	if (buffers == NULL) goto finish;

	AST(current_ctx->sostate != NULL);

	objid_array = (GLuint *)calloc(1, sizeof(GLuint) * n);
	{
		int real_n = 0;

		for (i = 0; i < n; i++)
		{
			int real_objid = _COREGL_INT_INIT_VALUE;
			if (buffers[i] == 0) continue;

			real_objid = fastpath_sostate_get_object(current_ctx->sostate, GL_OBJECT_TYPE_BUFFER, buffers[i]);
			if (real_objid == 0) continue;

			AST(fastpath_sostate_remove_object(current_ctx->sostate, GL_OBJECT_TYPE_BUFFER, buffers[i]) == 1);
			objid_array[real_n++] = real_objid;
		}

		IF_GL_SUCCESS(_orig_fastpath_glDeleteBuffers(real_n, objid_array))
		{
			for (i = 0; i < real_n; i++)
			{
				General_Trace_List *current = NULL;
				current = current_ctx->sostate->using_gctxs;

				while (current != NULL)
				{
					GLGlueContext *cur_gctx = (GLGlueContext *)current->value;

					if (cur_gctx->gl_array_buffer_binding[0] == objid_array[i])
					{
						cur_gctx->_bind_flag &= (~FLAG_BIT_0);
						cur_gctx->gl_array_buffer_binding[0] = 0;
					}
					if (cur_gctx->gl_element_array_buffer_binding[0] == objid_array[i])
					{
						cur_gctx->_bind_flag &= (~FLAG_BIT_1);
						cur_gctx->gl_element_array_buffer_binding[0] = 0;
					}

					current = current->next;
				}
			}
		}
	}

	goto finish;

finish:
	if (objid_array != NULL)
	{
		free(objid_array);
		objid_array = NULL;
	}
	_COREGL_FASTPATH_FUNC_END();
}

//////////////////////////////////////////////////////////////////////////////////

void
fastpath_glGenFramebuffers(GLsizei n, GLuint* framebuffers)
{
	int i;
	GLuint *objid_array = NULL;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (n < 0)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}
	if (n == 0) goto finish;
	if (framebuffers == NULL) goto finish;

	AST(current_ctx->sostate != NULL);

	objid_array = (GLuint *)calloc(1, sizeof(GLuint) * n);

	IF_GL_SUCCESS(_orig_fastpath_glGenFramebuffers(n, objid_array))
	{
		for (i = 0; i < n; i++)
		{
			framebuffers[i] = fastpath_sostate_create_object(current_ctx->sostate, GL_OBJECT_TYPE_FRAMEBUFFER, objid_array[i]);
		}
	}

	goto finish;

finish:
	if (objid_array != NULL)
	{
		free(objid_array);
		objid_array = NULL;
	}
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glBindFramebuffer(GLenum target, GLuint framebuffer)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_FRAMEBUFFER, framebuffer, &real_obj) != 1)
	{
		_set_gl_error(GL_OUT_OF_MEMORY);
		goto finish;
	}

	if (target == GL_FRAMEBUFFER)
	{
		CURR_STATE_COMPARE(gl_framebuffer_binding, real_obj)
		{
			IF_GL_SUCCESS(_orig_fastpath_glBindFramebuffer(target, real_obj))
			{
				if (real_obj == 0)
					current_ctx->_bind_flag &= (~FLAG_BIT_2);
				else
					current_ctx->_bind_flag |= FLAG_BIT_2;
				current_ctx->gl_framebuffer_binding[0] = real_obj;
			}
		}
	}
	else
	{
		_set_gl_error(GL_INVALID_ENUM);
		goto finish;
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

GLboolean
fastpath_glIsFramebuffer(GLuint framebuffer)
{
	GLboolean ret = GL_FALSE;
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_FRAMEBUFFER, framebuffer, &real_obj) != 1)
	{
		ret = GL_FALSE;
		goto finish;
	}

	ret = _orig_fastpath_glIsFramebuffer(real_obj);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return ret;
}


void
fastpath_glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers)
{
	int i;
	GLuint *objid_array = NULL;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (n < 0)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}
	if (n == 0) goto finish;
	if (framebuffers == NULL) goto finish;

	AST(current_ctx->sostate != NULL);

	objid_array = (GLuint *)calloc(1, sizeof(GLuint) * n);
	{
		int real_n = 0;

		for (i = 0; i < n; i++)
		{
			int real_objid = _COREGL_INT_INIT_VALUE;
			if (framebuffers[i] == 0) continue;

			real_objid = fastpath_sostate_get_object(current_ctx->sostate, GL_OBJECT_TYPE_FRAMEBUFFER, framebuffers[i]);
			if (real_objid == 0) continue;

			AST(fastpath_sostate_remove_object(current_ctx->sostate, GL_OBJECT_TYPE_FRAMEBUFFER, framebuffers[i]) == 1);
			objid_array[real_n++] = real_objid;
		}

		IF_GL_SUCCESS(_orig_fastpath_glDeleteFramebuffers(real_n, objid_array))
		{
			for (i = 0; i < real_n; i++)
			{
				General_Trace_List *current = NULL;
				current = current_ctx->sostate->using_gctxs;

				while (current != NULL)
				{
					GLGlueContext *cur_gctx = (GLGlueContext *)current->value;

					if (cur_gctx->gl_framebuffer_binding[0] == objid_array[i])
					{
						cur_gctx->_bind_flag &= (~FLAG_BIT_2);
						cur_gctx->gl_framebuffer_binding[0] = 0;
					}

					current = current->next;
				}
			}
		}
	}

	goto finish;

finish:
	if (objid_array != NULL)
	{
		free(objid_array);
		objid_array = NULL;
	}
	_COREGL_FASTPATH_FUNC_END();
}

//////////////////////////////////////////////////////////////////////////////////

void
fastpath_glGenRenderbuffers(GLsizei n, GLuint* renderbuffers)
{
	int i;
	GLuint *objid_array = NULL;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (n < 0)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}
	if (n == 0) goto finish;
	if (renderbuffers == NULL) goto finish;

	AST(current_ctx->sostate != NULL);

	objid_array = (GLuint *)calloc(1, sizeof(GLuint) * n);

	IF_GL_SUCCESS(_orig_fastpath_glGenRenderbuffers(n, objid_array))
	{
		for (i = 0; i < n; i++)
		{
			renderbuffers[i] = fastpath_sostate_create_object(current_ctx->sostate, GL_OBJECT_TYPE_RENDERBUFFER, objid_array[i]);
		}
	}

	goto finish;

finish:
	if (objid_array != NULL)
	{
		free(objid_array);
		objid_array = NULL;
	}
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_RENDERBUFFER, renderbuffer, &real_obj) != 1)
	{
		_set_gl_error(GL_OUT_OF_MEMORY);
		goto finish;
	}

	if (target == GL_RENDERBUFFER)
	{
		CURR_STATE_COMPARE(gl_renderbuffer_binding, real_obj)
		{
			IF_GL_SUCCESS(_orig_fastpath_glBindRenderbuffer(target, real_obj))
			{
				if (real_obj == 0)
					current_ctx->_bind_flag &= (~FLAG_BIT_3);
				else
					current_ctx->_bind_flag |= FLAG_BIT_3;
				current_ctx->gl_renderbuffer_binding[0] = real_obj;
			}
		}
	}
	else
	{
		_set_gl_error(GL_INVALID_ENUM);
		goto finish;
	}

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_RENDERBUFFER, renderbuffer, &real_obj) != 1)
	{
		_set_gl_error(GL_OUT_OF_MEMORY);
		goto finish;
	}

	_orig_fastpath_glFramebufferRenderbuffer(target, attachment, renderbuffertarget, real_obj);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

GLboolean
fastpath_glIsRenderbuffer(GLuint renderbuffer)
{
	GLboolean ret = GL_FALSE;
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_RENDERBUFFER, renderbuffer, &real_obj) != 1)
	{
		ret = GL_FALSE;
		goto finish;
	}

	ret = _orig_fastpath_glIsRenderbuffer(real_obj);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return ret;
}


void
fastpath_glDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers)
{
	int i;
	GLuint *objid_array = NULL;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (n < 0)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}
	if (n == 0) goto finish;
	if (renderbuffers == NULL) goto finish;

	AST(current_ctx->sostate != NULL);

	objid_array = (GLuint *)calloc(1, sizeof(GLuint) * n);
	{
		int real_n = 0;

		for (i = 0; i < n; i++)
		{
			int real_objid = _COREGL_INT_INIT_VALUE;
			if (renderbuffers[i] == 0) continue;

			real_objid = fastpath_sostate_get_object(current_ctx->sostate, GL_OBJECT_TYPE_RENDERBUFFER, renderbuffers[i]);
			if (real_objid == 0) continue;

			AST(fastpath_sostate_remove_object(current_ctx->sostate, GL_OBJECT_TYPE_RENDERBUFFER, renderbuffers[i]) == 1);
			objid_array[real_n++] = real_objid;
		}

		IF_GL_SUCCESS(_orig_fastpath_glDeleteRenderbuffers(real_n, objid_array))
		{
			for (i = 0; i < real_n; i++)
			{
				General_Trace_List *current = NULL;
				current = current_ctx->sostate->using_gctxs;

				while (current != NULL)
				{
					GLGlueContext *cur_gctx = (GLGlueContext *)current->value;

					if (cur_gctx->gl_renderbuffer_binding[0] == objid_array[i])
					{
						cur_gctx->_bind_flag &= (~FLAG_BIT_3);
						cur_gctx->gl_renderbuffer_binding[0] = 0;
					}

					current = current->next;
				}
			}
		}
	}

	goto finish;

finish:
	if (objid_array != NULL)
	{
		free(objid_array);
		objid_array = NULL;
	}
	_COREGL_FASTPATH_FUNC_END();
}

//////////////////////////////////////////////////////////////////////////////////

GLuint
fastpath_glCreateProgram(void)
{
	GLuint ret = 0;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	AST(current_ctx->sostate != NULL);

	ret = _create_program_object(current_ctx->sostate, 1, GL_NONE);

	_attach_program_object(current_ctx->sostate, ret);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return ret;
}

GLuint
fastpath_glCreateShader(GLenum type)
{
	GLuint ret = 0;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	AST(current_ctx->sostate != NULL);

	ret = _create_program_object(current_ctx->sostate, 0, type);

	_attach_program_object(current_ctx->sostate, ret);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return ret;
}

void
fastpath_glShaderSource(GLuint shader, GLsizei count, const char** string, const GLint* length)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, shader, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}
	_orig_fastpath_glShaderSource(real_obj, count, string, length);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glShaderBinary(GLsizei n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLsizei length)
{
	int i;
	GLuint *objid_array = NULL;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (n < 0)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}
	if (n == 0) goto finish;
	if (shaders == NULL) goto finish;

	AST(current_ctx->sostate != NULL);

	objid_array = (GLuint *)calloc(1, sizeof(GLuint) * n);

	for (i = 0; i < n; i++)
	{
		if (shaders[i] == 0) continue;
		objid_array[i] = fastpath_sostate_get_object(current_ctx->sostate, GL_OBJECT_TYPE_PROGRAM, shaders[i]);
	}

	_orig_fastpath_glShaderBinary(n, objid_array, binaryformat, binary, length);

	goto finish;

finish:
	if (objid_array != NULL)
	{
		free(objid_array);
		objid_array = NULL;
	}

	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glCompileShader(GLuint shader)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, shader, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glCompileShader(real_obj);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glBindAttribLocation(GLuint program, GLuint index, const char* name)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glBindAttribLocation(real_obj, index, name);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glAttachShader(GLuint program, GLuint shader)
{
	GLuint real_obj_program;
	GLuint real_obj_shader;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj_program) != 1 ||
	    GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, shader, &real_obj_shader) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	IF_GL_SUCCESS(_orig_fastpath_glAttachShader(real_obj_program, real_obj_shader))
	{
		_update_program_attach_info(current_ctx->sostate, program);
		_attach_program_object(current_ctx->sostate, shader);
	}

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glDetachShader(GLuint program, GLuint shader)
{
	GLuint real_obj_program;
	GLuint real_obj_shader;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj_program) != 1 ||
	    GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, shader, &real_obj_shader) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	IF_GL_SUCCESS(_orig_fastpath_glDetachShader(real_obj_program, real_obj_shader))
	{
		_update_program_attach_info(current_ctx->sostate, program);
		_detach_program_object(current_ctx->sostate, real_obj_shader, 0, 0);
	}

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

GLboolean
fastpath_glIsShader(GLuint shader)
{
	GLboolean ret = GL_FALSE;
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, shader, &real_obj) != 1)
	{
		ret = GL_FALSE;
		goto finish;
	}

	ret = _orig_fastpath_glIsShader(real_obj);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return ret;
}

GLboolean
fastpath_glIsProgram(GLuint program)
{
	GLboolean ret = GL_FALSE;
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		ret = GL_FALSE;
		goto finish;
	}

	ret = _orig_fastpath_glIsProgram(real_obj);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return ret;
}

void
fastpath_glLinkProgram(GLuint program)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glLinkProgram(real_obj);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glValidateProgram(GLuint program)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glValidateProgram(real_obj);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glUseProgram(GLuint program)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	CURR_STATE_COMPARE(gl_current_program, real_obj)
	{
		IF_GL_SUCCESS(_orig_fastpath_glUseProgram(real_obj))
		{
			_attach_program_object(current_ctx->sostate, program);
			_detach_program_object(current_ctx->sostate, current_ctx->gl_current_program[0], 1, 0);

			current_ctx->_clear_flag1 |= FLAG_BIT_1;
			current_ctx->gl_current_program[0] = real_obj;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glGetActiveAttrib(real_obj, index, bufsize, length, size, type, name);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glGetActiveUniform(real_obj, index, bufsize, length, size, type, name);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders)
{
	int i;
	GLsizei real_count = _COREGL_INT_INIT_VALUE;
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	IF_GL_SUCCESS(_orig_fastpath_glGetAttachedShaders(real_obj, maxcount, &real_count, shaders))
	{
		for (i = 0; i < real_count; i++)
		{
			if (shaders[i] != 0)
				shaders[i] = fastpath_sostate_find_object(current_ctx->sostate, GL_OBJECT_TYPE_PROGRAM, shaders[i]);
		}
		if (count != NULL) *count = real_count;
	}

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

int
fastpath_glGetAttribLocation(GLuint program, const char* name)
{
	int ret = _COREGL_INT_INIT_VALUE;
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	ret = _orig_fastpath_glGetAttribLocation(real_obj, name);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return ret;
}

void
fastpath_glGetShaderiv(GLuint shader, GLenum pname, GLint* params)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, shader, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	switch (pname)
	{
		case GL_DELETE_STATUS:
			*params = GL_FALSE;
			if (_is_deleted_program_object(current_ctx->sostate, shader) == 1)
				*params = GL_TRUE;
			break;
		default:
			_orig_fastpath_glGetShaderiv(real_obj, pname, params);
			break;
	}

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, shader, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glGetShaderInfoLog(real_obj, bufsize, length, infolog);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glGetProgramiv(GLuint program, GLenum pname, GLint* params)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	switch (pname)
	{
		case GL_DELETE_STATUS:
			*params = GL_FALSE;
			if (_is_deleted_program_object(current_ctx->sostate, program) == 1)
				*params = GL_TRUE;
			break;
		default:
			_orig_fastpath_glGetProgramiv(real_obj, pname, params);
			break;
	}

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei* length, char* infolog)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glGetProgramInfoLog(real_obj, bufsize, length, infolog);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glGetShaderSource(GLuint shader, GLsizei bufsize, GLsizei* length, char* source)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, shader, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glGetShaderSource(real_obj, bufsize, length, source);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glGetUniformfv(GLuint program, GLint location, GLfloat* params)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glGetUniformfv(real_obj, location, params);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glGetUniformiv(GLuint program, GLint location, GLint* params)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glGetUniformiv(real_obj, location, params);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glGetProgramBinaryOES(GLuint program, GLsizei bufsize, GLsizei* length, GLenum* binaryFormat, void* binary)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glGetProgramBinaryOES(real_obj, bufsize, length, binaryFormat, binary);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramBinaryOES(GLuint program, GLenum binaryFormat, const void* binary, GLint length)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramBinaryOES(real_obj, binaryFormat, binary, length);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glUseProgramStagesEXT(GLuint pipeline, GLbitfield stages, GLuint program)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glUseProgramStagesEXT(pipeline, stages, real_obj);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glActiveShaderProgramEXT(GLuint pipeline, GLuint program)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glActiveShaderProgramEXT(pipeline, real_obj);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

GLuint
fastpath_glCreateShaderProgramvEXT(GLenum type, GLsizei count, const char **strings)
{
	GLuint ret = _COREGL_INT_INIT_VALUE;

	_COREGL_FASTPATH_FUNC_BEGIN();
	ret = _orig_fastpath_glCreateShaderProgramvEXT(type, count, strings);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
   return ret;
}

void
fastpath_glBindProgramPipelineEXT(GLuint pipeline)
{
	_COREGL_FASTPATH_FUNC_BEGIN();
	_orig_fastpath_glBindProgramPipelineEXT(pipeline);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glDeleteProgramPipelinesEXT(GLsizei n, const GLuint *pipelines)
{
	_COREGL_FASTPATH_FUNC_BEGIN();
	_orig_fastpath_glDeleteProgramPipelinesEXT(n, pipelines);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glGenProgramPipelinesEXT(GLsizei n, GLuint *pipelines)
{
	_COREGL_FASTPATH_FUNC_BEGIN();
	_orig_fastpath_glGenProgramPipelinesEXT(n, pipelines);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

GLboolean
fastpath_glIsProgramPipelineEXT(GLuint pipeline)
{
	GLboolean ret = _COREGL_INT_INIT_VALUE;

	_COREGL_FASTPATH_FUNC_BEGIN();
	ret = _orig_fastpath_glIsProgramPipelineEXT(pipeline);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
   return ret;
}

void
fastpath_glProgramParameteriEXT(GLuint program, GLenum pname, GLint value)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramParameteriEXT(real_obj, pname, value);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glGetProgramPipelineivEXT(GLuint pipeline, GLenum pname, GLint *params)
{
	_COREGL_FASTPATH_FUNC_BEGIN();
	_orig_fastpath_glGetProgramPipelineivEXT(pipeline, pname, params);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramUniform1iEXT(GLuint program, GLint location, GLint x)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramUniform1iEXT(real_obj, location, x);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramUniform2iEXT(GLuint program, GLint location, GLint x, GLint y)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramUniform2iEXT(real_obj, location, x, y);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramUniform3iEXT(GLuint program, GLint location, GLint x, GLint y, GLint z)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramUniform3iEXT(real_obj, location, x, y, z);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramUniform4iEXT(GLuint program, GLint location, GLint x, GLint y, GLint z, GLint w)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramUniform4iEXT(real_obj, location, x, y, z, w);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramUniform1fEXT(GLuint program, GLint location, GLfloat x)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramUniform1fEXT(real_obj, location, x);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramUniform2fEXT(GLuint program, GLint location, GLfloat x, GLfloat y)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramUniform2fEXT(real_obj, location, x, y);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramUniform3fEXT(GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramUniform3fEXT(real_obj, location, x, y, z);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramUniform4fEXT(GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramUniform4fEXT(real_obj, location, x, y, z, w);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramUniform1ivEXT(GLuint program, GLint location, GLsizei count, const GLint *value)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramUniform1ivEXT(real_obj, location, count, value);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramUniform2ivEXT(GLuint program, GLint location, GLsizei count, const GLint *value)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramUniform2ivEXT(real_obj, location, count, value);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramUniform3ivEXT(GLuint program, GLint location, GLsizei count, const GLint *value)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramUniform3ivEXT(real_obj, location, count, value);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramUniform4ivEXT(GLuint program, GLint location, GLsizei count, const GLint *value)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramUniform4ivEXT(real_obj, location, count, value);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramUniform1fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramUniform1fvEXT(real_obj, location, count, value);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramUniform2fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramUniform2fvEXT(real_obj, location, count, value);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramUniform3fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramUniform3fvEXT(real_obj, location, count, value);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramUniform4fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramUniform4fvEXT(real_obj, location, count, value);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramUniformMatrix2fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramUniformMatrix2fvEXT(real_obj, location, count, transpose, value);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramUniformMatrix3fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramUniformMatrix3fvEXT(real_obj, location, count, transpose, value);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glProgramUniformMatrix4fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_orig_fastpath_glProgramUniformMatrix4fvEXT(real_obj, location, count, transpose, value);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glValidateProgramPipelineEXT(GLuint pipeline)
{
	_COREGL_FASTPATH_FUNC_BEGIN();
	_orig_fastpath_glValidateProgramPipelineEXT(pipeline);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glGetProgramPipelineInfoLogEXT(GLuint pipeline, GLsizei bufSize, GLsizei *length, char *infoLog)
{
	_COREGL_FASTPATH_FUNC_BEGIN();
	_orig_fastpath_glGetProgramPipelineInfoLogEXT(pipeline, bufSize, length, infoLog);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


int
fastpath_glGetUniformLocation(GLuint program, const char* name)
{
	int ret = _COREGL_INT_INIT_VALUE;
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		ret = -1;
		goto finish;
	}

	ret = _orig_fastpath_glGetUniformLocation(real_obj, name);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return ret;
}

void
fastpath_glDeleteShader(GLuint shader)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, shader, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_detach_program_object(current_ctx->sostate, real_obj, 0, 1);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glDeleteProgram(GLuint program)
{
	GLuint real_obj;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (GET_REAL_OBJ(GL_OBJECT_TYPE_PROGRAM, program, &real_obj) != 1)
	{
		_set_gl_error(GL_INVALID_VALUE);
		goto finish;
	}

	_detach_program_object(current_ctx->sostate, real_obj, 1, 1);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}



//////////////////////////////////////////////////////////////////////////////////

void
fastpath_glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if ((current_ctx->gl_blend_color[0] != red) ||
	    (current_ctx->gl_blend_color[1] != green) ||
	    (current_ctx->gl_blend_color[2] != blue) ||
	    (current_ctx->gl_blend_color[3] != alpha))
	{
		IF_GL_SUCCESS(_orig_fastpath_glBlendColor(red, green, blue, alpha))
		{
			current_ctx->_blend_flag |= FLAG_BIT_0;
			current_ctx->gl_blend_color[0] = red;
			current_ctx->gl_blend_color[1] = green;
			current_ctx->gl_blend_color[2] = blue;
			current_ctx->gl_blend_color[3] = alpha;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


//!!! Optimze?
void
fastpath_glBlendEquation(GLenum mode)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	IF_GL_SUCCESS(_orig_fastpath_glBlendEquation(mode))
	{
		current_ctx->_blend_flag |= (FLAG_BIT_5 | FLAG_BIT_6);
		_orig_fastpath_glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*) & (current_ctx->gl_blend_equation_rgb));
		_orig_fastpath_glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*) & (current_ctx->gl_blend_equation_alpha));
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if ((current_ctx->gl_blend_equation_rgb[0] != modeRGB) ||
	    (current_ctx->gl_blend_equation_alpha[0] != modeAlpha))
	{
		IF_GL_SUCCESS(_orig_fastpath_glBlendEquationSeparate(modeRGB, modeAlpha))
		{
			current_ctx->_blend_flag |= (FLAG_BIT_5 | FLAG_BIT_6);
			current_ctx->gl_blend_equation_rgb[0]    = modeRGB;
			current_ctx->gl_blend_equation_alpha[0]  = modeAlpha;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


//!!! Optimze?
void
fastpath_glBlendFunc(GLenum sfactor, GLenum dfactor)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	IF_GL_SUCCESS(_orig_fastpath_glBlendFunc(sfactor, dfactor))
	{
		current_ctx->_blend_flag |= (FLAG_BIT_1 | FLAG_BIT_2 | FLAG_BIT_3 | FLAG_BIT_4);
		_orig_fastpath_glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*) & (current_ctx->gl_blend_src_rgb));
		_orig_fastpath_glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*) & (current_ctx->gl_blend_src_alpha));
		_orig_fastpath_glGetIntegerv(GL_BLEND_DST_RGB, (GLint*) & (current_ctx->gl_blend_dst_rgb));
		_orig_fastpath_glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*) & (current_ctx->gl_blend_dst_alpha));
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if ((current_ctx->gl_blend_src_rgb[0] != srcRGB) ||
	    (current_ctx->gl_blend_dst_rgb[0] != dstRGB) ||
	    (current_ctx->gl_blend_src_alpha[0] != srcAlpha) ||
	    (current_ctx->gl_blend_dst_alpha[0] != dstAlpha))
	{
		IF_GL_SUCCESS(_orig_fastpath_glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha))
		{
			current_ctx->_blend_flag |= (FLAG_BIT_1 | FLAG_BIT_2 | FLAG_BIT_3 | FLAG_BIT_4);
			current_ctx->gl_blend_src_rgb[0]   = srcRGB;
			current_ctx->gl_blend_dst_rgb[0]   = dstRGB;
			current_ctx->gl_blend_src_alpha[0] = srcAlpha;
			current_ctx->gl_blend_dst_alpha[0] = dstAlpha;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if ((current_ctx->gl_color_clear_value[0] != red) ||
	    (current_ctx->gl_color_clear_value[1] != green) ||
	    (current_ctx->gl_color_clear_value[2] != blue) ||
	    (current_ctx->gl_color_clear_value[3] != alpha))
	{
		IF_GL_SUCCESS(_orig_fastpath_glClearColor(red, green, blue, alpha))
		{
			current_ctx->_clear_flag1 |= (FLAG_BIT_2);
			current_ctx->gl_color_clear_value[0] = red;
			current_ctx->gl_color_clear_value[1] = green;
			current_ctx->gl_color_clear_value[2] = blue;
			current_ctx->gl_color_clear_value[3] = alpha;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glClearDepthf(GLclampf depth)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	CURR_STATE_COMPARE(gl_depth_clear_value, depth)
	{
		IF_GL_SUCCESS(_orig_fastpath_glClearDepthf(depth))
		{
			current_ctx->_clear_flag2 |= FLAG_BIT_2;
			current_ctx->gl_depth_clear_value[0] = depth;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glClearStencil(GLint s)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	CURR_STATE_COMPARE(gl_stencil_clear_value, s)
	{
		IF_GL_SUCCESS(_orig_fastpath_glClearStencil(s))
		{
			current_ctx->_stencil_flag2 |= FLAG_BIT_7;
			current_ctx->gl_stencil_clear_value[0] = s;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if ((current_ctx->gl_color_writemask[0] != red) ||
	    (current_ctx->gl_color_writemask[1] != green) ||
	    (current_ctx->gl_color_writemask[2] != blue) ||
	    (current_ctx->gl_color_writemask[3] != alpha))
	{
		IF_GL_SUCCESS(_orig_fastpath_glColorMask(red, green, blue, alpha))
		{
			current_ctx->_clear_flag2 |= FLAG_BIT_0;
			current_ctx->gl_color_writemask[0] = red;
			current_ctx->gl_color_writemask[1] = green;
			current_ctx->gl_color_writemask[2] = blue;
			current_ctx->gl_color_writemask[3] = alpha;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glCullFace(GLenum mode)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	CURR_STATE_COMPARE(gl_cull_face_mode, mode)
	{
		IF_GL_SUCCESS(_orig_fastpath_glCullFace(mode))
		{
			current_ctx->_clear_flag2 |= FLAG_BIT_5;
			current_ctx->gl_cull_face_mode[0] = mode;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glDepthFunc(GLenum func)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	CURR_STATE_COMPARE(gl_depth_func, func)
	{
		IF_GL_SUCCESS(_orig_fastpath_glDepthFunc(func))
		{
			current_ctx->_clear_flag2 |= FLAG_BIT_3;
			current_ctx->gl_depth_func[0] = func;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glDepthMask(GLboolean flag)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	CURR_STATE_COMPARE(gl_depth_writemask, flag)
	{
		IF_GL_SUCCESS(_orig_fastpath_glDepthMask(flag))
		{
			current_ctx->_clear_flag2 |= FLAG_BIT_4;
			current_ctx->gl_depth_writemask[0] = flag;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glDepthRangef(GLclampf zNear, GLclampf zFar)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if ((current_ctx->gl_depth_range[0] != zNear) ||
	    (current_ctx->gl_depth_range[1] != zFar))
	{
		IF_GL_SUCCESS(_orig_fastpath_glDepthRangef(zNear, zFar))
		{
			current_ctx->_clear_flag2 |= FLAG_BIT_1;
			current_ctx->gl_depth_range[0] = zNear;
			current_ctx->gl_depth_range[1] = zFar;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glDisable(GLenum cap)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	switch (cap)
	{
		case GL_BLEND:
			CURR_STATE_COMPARE(gl_blend, GL_FALSE)
			{
				_orig_fastpath_glDisable(cap);
				current_ctx->_enable_flag1 &= (~FLAG_BIT_0);
				current_ctx->gl_blend[0] = GL_FALSE;
			}
			break;
		case GL_CULL_FACE:
			CURR_STATE_COMPARE(gl_cull_face, GL_FALSE)
			{
				_orig_fastpath_glDisable(cap);
				current_ctx->_enable_flag1 &= (~FLAG_BIT_1);
				current_ctx->gl_cull_face[0] = GL_FALSE;
			}
			break;
		case GL_DEPTH_TEST:
			CURR_STATE_COMPARE(gl_depth_test, GL_FALSE)
			{
				_orig_fastpath_glDisable(cap);
				current_ctx->_enable_flag1 &= (~FLAG_BIT_2);
				current_ctx->gl_depth_test[0] = GL_FALSE;
			}
			break;
		case GL_DITHER:
			CURR_STATE_COMPARE(gl_dither, GL_FALSE)
			{
				_orig_fastpath_glDisable(cap);
				current_ctx->_enable_flag1 &= (~FLAG_BIT_3);
				current_ctx->gl_dither[0] = GL_FALSE;
			}
			break;
		case GL_POLYGON_OFFSET_FILL:
			CURR_STATE_COMPARE(gl_polygon_offset_fill, GL_FALSE)
			{
				_orig_fastpath_glDisable(cap);
				current_ctx->_enable_flag2 &= (~FLAG_BIT_0);
				current_ctx->gl_polygon_offset_fill[0] = GL_FALSE;
			}
			break;
		case GL_SAMPLE_ALPHA_TO_COVERAGE:
			CURR_STATE_COMPARE(gl_sample_alpha_to_coverage, GL_FALSE)
			{
				_orig_fastpath_glDisable(cap);
				current_ctx->_enable_flag2 &= (~FLAG_BIT_1);
				current_ctx->gl_sample_alpha_to_coverage[0] = GL_FALSE;
			}
			break;
		case GL_SAMPLE_COVERAGE:
			CURR_STATE_COMPARE(gl_sample_coverage, GL_FALSE)
			{
				_orig_fastpath_glDisable(cap);
				current_ctx->_enable_flag2 &= (~FLAG_BIT_2);
				current_ctx->gl_sample_coverage[0] = GL_FALSE;
			}
			break;
		case GL_SCISSOR_TEST:
			CURR_STATE_COMPARE(gl_scissor_test, GL_FALSE)
			{
				_orig_fastpath_glDisable(cap);
				current_ctx->_enable_flag2 &= (~FLAG_BIT_3);
				current_ctx->gl_scissor_test[0] = GL_FALSE;
			}
			break;
		case GL_STENCIL_TEST:
			CURR_STATE_COMPARE(gl_stencil_test, GL_FALSE)
			{
				_orig_fastpath_glDisable(cap);
				current_ctx->_enable_flag2 &= (~FLAG_BIT_4);
				current_ctx->gl_stencil_test[0] = GL_FALSE;
			}
			break;
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glDisableVertexAttribArray(GLuint index)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	IF_GL_SUCCESS(_orig_fastpath_glDisableVertexAttribArray(index))
	{
		current_ctx->_vattrib_flag |= FLAG_BIT_1;
		current_ctx->gl_vertex_array_enabled[index] = GL_FALSE;
	}

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glEnable(GLenum cap)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	switch (cap)
	{
		case GL_BLEND:
			CURR_STATE_COMPARE(gl_blend, GL_TRUE)
			{
				_orig_fastpath_glEnable(cap);
				current_ctx->_enable_flag1 |= FLAG_BIT_0;
				current_ctx->gl_blend[0] = GL_TRUE;
			}
			break;
		case GL_CULL_FACE:
			CURR_STATE_COMPARE(gl_cull_face, GL_TRUE)
			{
				_orig_fastpath_glEnable(cap);
				current_ctx->_enable_flag1 |= FLAG_BIT_1;
				current_ctx->gl_cull_face[0] = GL_TRUE;
			}
			break;
		case GL_DEPTH_TEST:
			CURR_STATE_COMPARE(gl_depth_test, GL_TRUE)
			{
				_orig_fastpath_glEnable(cap);
				current_ctx->_enable_flag1 |= FLAG_BIT_2;
				current_ctx->gl_depth_test[0] = GL_TRUE;
			}
			break;
		case GL_DITHER:
			CURR_STATE_COMPARE(gl_dither, GL_TRUE)
			{
				_orig_fastpath_glEnable(cap);
				current_ctx->_enable_flag1 |= FLAG_BIT_3;
				current_ctx->gl_dither[0] = GL_TRUE;
			}
			break;
		case GL_POLYGON_OFFSET_FILL:
			CURR_STATE_COMPARE(gl_polygon_offset_fill, GL_TRUE)
			{
				_orig_fastpath_glEnable(cap);
				current_ctx->_enable_flag2 |= FLAG_BIT_0;
				current_ctx->gl_polygon_offset_fill[0] = GL_TRUE;
			}
			break;
		case GL_SAMPLE_ALPHA_TO_COVERAGE:
			CURR_STATE_COMPARE(gl_sample_alpha_to_coverage, GL_TRUE)
			{
				_orig_fastpath_glEnable(cap);
				current_ctx->_enable_flag2 |= FLAG_BIT_1;
				current_ctx->gl_sample_alpha_to_coverage[0] = GL_TRUE;
			}
			break;
		case GL_SAMPLE_COVERAGE:
			CURR_STATE_COMPARE(gl_sample_coverage, GL_TRUE)
			{
				_orig_fastpath_glEnable(cap);
				current_ctx->_enable_flag2 |= FLAG_BIT_2;
				current_ctx->gl_sample_coverage[0] = GL_TRUE;
			}
			break;
		case GL_SCISSOR_TEST:
			CURR_STATE_COMPARE(gl_scissor_test, GL_TRUE)
			{
				_orig_fastpath_glEnable(cap);
				current_ctx->_enable_flag2 |= FLAG_BIT_3;
				current_ctx->gl_scissor_test[0] = GL_TRUE;
			}
			break;
		case GL_STENCIL_TEST:
			CURR_STATE_COMPARE(gl_stencil_test, GL_TRUE)
			{
				_orig_fastpath_glEnable(cap);
				current_ctx->_enable_flag2 |= FLAG_BIT_4;
				current_ctx->gl_stencil_test[0] = GL_TRUE;
			}
			break;
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


// Optimze?
void
fastpath_glEnableVertexAttribArray(GLuint index)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	IF_GL_SUCCESS(_orig_fastpath_glEnableVertexAttribArray(index))
	{
		current_ctx->_vattrib_flag |= FLAG_BIT_1;
		current_ctx->gl_vertex_array_enabled[index] = GL_TRUE;
	}

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glFrontFace(GLenum mode)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	CURR_STATE_COMPARE(gl_front_face, mode)
	{
		IF_GL_SUCCESS(_orig_fastpath_glFrontFace(mode))
		{
			current_ctx->_misc_flag1 |= FLAG_BIT_0;
			current_ctx->gl_front_face[0] = mode;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glHint(GLenum target, GLenum mode)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	switch (target)
	{
		case GL_GENERATE_MIPMAP_HINT:
			CURR_STATE_COMPARE(gl_generate_mipmap_hint, mode)
			{
				IF_GL_SUCCESS(_orig_fastpath_glHint(target, mode))
				{
					current_ctx->_tex_flag1 |= FLAG_BIT_2;
					current_ctx->gl_generate_mipmap_hint[0] = mode;
				}
			}
			break;
		case GL_FRAGMENT_SHADER_DERIVATIVE_HINT_OES:
			CURR_STATE_COMPARE(gl_fragment_shader_derivative_hint, mode)
			{
				IF_GL_SUCCESS(_orig_fastpath_glHint(target, mode))
				{
					current_ctx->_misc_flag1 |= FLAG_BIT_6;
					current_ctx->gl_fragment_shader_derivative_hint[0] = mode;
				}
			}
			break;
		default:
			ERR("\E[0;31;1mERROR : Invalid (or not supported) hint target is specified!\E[0m\n");
			_set_gl_error(GL_INVALID_ENUM);
			break;
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glLineWidth(GLfloat width)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	CURR_STATE_COMPARE(gl_line_width, width)
	{
		IF_GL_SUCCESS(_orig_fastpath_glLineWidth(width))
		{
			current_ctx->_misc_flag1 |= FLAG_BIT_1;
			current_ctx->gl_line_width[0] = width;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glPixelStorei(GLenum pname, GLint param)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (pname == GL_PACK_ALIGNMENT)
	{
		CURR_STATE_COMPARE(gl_pack_alignment, param)
		{
			IF_GL_SUCCESS(_orig_fastpath_glPixelStorei(pname, param))
			{
				current_ctx->_misc_flag2 |= FLAG_BIT_1;
				current_ctx->gl_pack_alignment[0] = param;
			}
		}
	}
	else if (pname == GL_UNPACK_ALIGNMENT)
	{
		CURR_STATE_COMPARE(gl_unpack_alignment, param)
		{
			IF_GL_SUCCESS(_orig_fastpath_glPixelStorei(pname, param))
			{
				current_ctx->_misc_flag2 |= FLAG_BIT_2;
				current_ctx->gl_unpack_alignment[0] = param;
			}
		}
	}
	else
	{
		_set_gl_error(GL_INVALID_ENUM);
		goto finish;
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glPolygonOffset(GLfloat factor, GLfloat units)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if ((current_ctx->gl_polygon_offset_factor[0] != factor) ||
	    (current_ctx->gl_polygon_offset_units[0] != units))
	{
		IF_GL_SUCCESS(_orig_fastpath_glPolygonOffset(factor, units))
		{
			current_ctx->_misc_flag1 |= (FLAG_BIT_2 | FLAG_BIT_3);
			current_ctx->gl_polygon_offset_factor[0] = factor;
			current_ctx->gl_polygon_offset_units[0]  = units;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glSampleCoverage(GLclampf value, GLboolean invert)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if ((current_ctx->gl_sample_coverage_value[0] != value) ||
	    (current_ctx->gl_sample_coverage_invert[0] != invert))
	{
		IF_GL_SUCCESS(_orig_fastpath_glSampleCoverage(value, invert))
		{
			current_ctx->_misc_flag1 |= (FLAG_BIT_4 | FLAG_BIT_5);
			current_ctx->gl_sample_coverage_value[0]  = value;
			current_ctx->gl_sample_coverage_invert[0] = invert;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if ((current_ctx->gl_scissor_box[0] != x) ||
	    (current_ctx->gl_scissor_box[1] != y) ||
	    (current_ctx->gl_scissor_box[2] != width) ||
	    (current_ctx->gl_scissor_box[3] != height))
	{
		IF_GL_SUCCESS(_orig_fastpath_glScissor(x, y, width, height))
		{
			current_ctx->_misc_flag2 |= FLAG_BIT_0;
			current_ctx->gl_scissor_box[0] = x;
			current_ctx->gl_scissor_box[1] = y;
			current_ctx->gl_scissor_box[2] = width;
			current_ctx->gl_scissor_box[3] = height;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glStencilFunc(GLenum func, GLint ref, GLuint mask)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if ((current_ctx->gl_stencil_func[0] != func) ||
	    (current_ctx->gl_stencil_ref[0] != ref) ||
	    (current_ctx->gl_stencil_value_mask[0] != mask) ||
	    (current_ctx->gl_stencil_back_func[0] != func) ||
	    (current_ctx->gl_stencil_back_ref[0] != ref) ||
	    (current_ctx->gl_stencil_back_value_mask[0] != mask))
	{
		IF_GL_SUCCESS(_orig_fastpath_glStencilFunc(func, ref, mask))
		{
			current_ctx->_stencil_flag1 |= (FLAG_BIT_0 | FLAG_BIT_1 | FLAG_BIT_2);
			current_ctx->gl_stencil_func[0]             = func;
			current_ctx->gl_stencil_ref[0]              = ref;
			current_ctx->gl_stencil_value_mask[0]       = mask;

			current_ctx->_stencil_flag2 |= (FLAG_BIT_0 | FLAG_BIT_1 | FLAG_BIT_2);
			current_ctx->gl_stencil_back_func[0]        = func;
			current_ctx->gl_stencil_back_ref[0]         = ref;
			current_ctx->gl_stencil_back_value_mask[0]  = mask;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if ((face == GL_FRONT) || (face == GL_FRONT_AND_BACK))
	{
		if ((current_ctx->gl_stencil_func[0] != func) ||
		    (current_ctx->gl_stencil_ref[0] != ref) ||
		    (current_ctx->gl_stencil_value_mask[0] != mask))
		{
			IF_GL_SUCCESS(_orig_fastpath_glStencilFuncSeparate(face, func, ref, mask))
			{
				current_ctx->_stencil_flag1 |= (FLAG_BIT_0 | FLAG_BIT_1 | FLAG_BIT_2);

				current_ctx->gl_stencil_func[0]             = func;
				current_ctx->gl_stencil_ref[0]              = ref;
				current_ctx->gl_stencil_value_mask[0]       = mask;
			}
		}
	}
	else if ((face == GL_BACK) || (face == GL_FRONT_AND_BACK))
	{
		if ((current_ctx->gl_stencil_back_func[0] != func) ||
		    (current_ctx->gl_stencil_back_ref[0] != ref) ||
		    (current_ctx->gl_stencil_back_value_mask[0] != mask))
		{
			IF_GL_SUCCESS(_orig_fastpath_glStencilFuncSeparate(face, func, ref, mask))
			{
				current_ctx->_stencil_flag2 |= (FLAG_BIT_0 | FLAG_BIT_1 | FLAG_BIT_2);

				current_ctx->gl_stencil_back_func[0]        = func;
				current_ctx->gl_stencil_back_ref[0]         = ref;
				current_ctx->gl_stencil_back_value_mask[0]  = mask;
			}
		}
	}
	else
	{
		_set_gl_error(GL_INVALID_ENUM);
		goto finish;
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glStencilMask(GLuint mask)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if ((current_ctx->gl_stencil_writemask[0] != mask) ||
	    (current_ctx->gl_stencil_back_writemask[0] != mask))
	{
		IF_GL_SUCCESS(_orig_fastpath_glStencilMask(mask))
		{
			current_ctx->_stencil_flag1 |= (FLAG_BIT_6);
			current_ctx->_stencil_flag2 |= (FLAG_BIT_6);

			current_ctx->gl_stencil_writemask[0]        = mask;
			current_ctx->gl_stencil_back_writemask[0]   = mask;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glStencilMaskSeparate(GLenum face, GLuint mask)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if ((face == GL_FRONT) || (face == GL_FRONT_AND_BACK))
	{
		if (current_ctx->gl_stencil_writemask[0] != mask)
		{
			IF_GL_SUCCESS(_orig_fastpath_glStencilMaskSeparate(face, mask))
			{
				current_ctx->_stencil_flag1 |= (FLAG_BIT_6);
				current_ctx->gl_stencil_writemask[0] = mask;
			}
		}
	}
	else if ((face == GL_BACK) || (face == GL_FRONT_AND_BACK))
	{
		if (current_ctx->gl_stencil_back_writemask[0] != mask)
		{
			IF_GL_SUCCESS(_orig_fastpath_glStencilMaskSeparate(face, mask))
			{
				current_ctx->_stencil_flag2 |= (FLAG_BIT_6);
				current_ctx->gl_stencil_back_writemask[0]   = mask;
			}
		}
	}
	else
	{
		_set_gl_error(GL_INVALID_ENUM);
		goto finish;
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if ((current_ctx->gl_stencil_fail[0] != fail) ||
	    (current_ctx->gl_stencil_pass_depth_fail[0] != zfail) ||
	    (current_ctx->gl_stencil_pass_depth_pass[0] != zpass) ||
	    (current_ctx->gl_stencil_back_fail[0] != fail) ||
	    (current_ctx->gl_stencil_back_pass_depth_fail[0] != zfail) ||
	    (current_ctx->gl_stencil_back_pass_depth_pass[0] != zpass))
	{
		IF_GL_SUCCESS(_orig_fastpath_glStencilOp(fail, zfail, zpass))
		{
			current_ctx->_stencil_flag1 |= (FLAG_BIT_3 | FLAG_BIT_4 | FLAG_BIT_5);
			current_ctx->gl_stencil_fail[0]              = fail;
			current_ctx->gl_stencil_pass_depth_fail[0]   = zfail;
			current_ctx->gl_stencil_pass_depth_pass[0]   = zpass;

			current_ctx->_stencil_flag2 |= (FLAG_BIT_3 | FLAG_BIT_4 | FLAG_BIT_5);
			current_ctx->gl_stencil_back_fail[0]         = fail;
			current_ctx->gl_stencil_back_pass_depth_fail[0]   = zfail;
			current_ctx->gl_stencil_back_pass_depth_pass[0]   = zpass;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if ((face == GL_FRONT) || (face == GL_FRONT_AND_BACK))
	{
		if ((current_ctx->gl_stencil_fail[0] != fail) ||
		    (current_ctx->gl_stencil_pass_depth_fail[0] != zfail) ||
		    (current_ctx->gl_stencil_pass_depth_pass[0] != zpass))
		{
			IF_GL_SUCCESS(_orig_fastpath_glStencilOpSeparate(face, fail, zfail, zpass))
			{
				current_ctx->_stencil_flag1 |= (FLAG_BIT_3 | FLAG_BIT_4 | FLAG_BIT_5);
				current_ctx->gl_stencil_fail[0]              = fail;
				current_ctx->gl_stencil_pass_depth_fail[0]   = zfail;
				current_ctx->gl_stencil_pass_depth_pass[0]   = zpass;
			}
		}
	}
	else if ((face == GL_BACK) || (face == GL_FRONT_AND_BACK))
	{
		if ((current_ctx->gl_stencil_back_fail[0] != fail) ||
		    (current_ctx->gl_stencil_back_pass_depth_fail[0] != zfail) ||
		    (current_ctx->gl_stencil_back_pass_depth_pass[0] != zpass))
		{
			IF_GL_SUCCESS(_orig_fastpath_glStencilOpSeparate(face, fail, zfail, zpass))
			{
				current_ctx->_stencil_flag2 |= (FLAG_BIT_3 | FLAG_BIT_4 | FLAG_BIT_5);
				current_ctx->gl_stencil_back_fail[0]              = fail;
				current_ctx->gl_stencil_back_pass_depth_fail[0]   = zfail;
				current_ctx->gl_stencil_back_pass_depth_pass[0]   = zpass;
			}
		}
	}
	else
	{
		_set_gl_error(GL_INVALID_ENUM);
		goto finish;
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

// Optmize?
void
fastpath_glVertexAttrib1f(GLuint indx, GLfloat x)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	IF_GL_SUCCESS(_orig_fastpath_glVertexAttrib1f(indx, x))
	{
		current_ctx->_vattrib_flag |= FLAG_BIT_0;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 0] = x;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 1] = 0;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 2] = 0;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 3] = 1;
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


// Optmize?
void
fastpath_glVertexAttrib1fv(GLuint indx, const GLfloat* values)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	IF_GL_SUCCESS(_orig_fastpath_glVertexAttrib1fv(indx, values))
	{
		current_ctx->_vattrib_flag |= FLAG_BIT_0;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 0] = values[0];
		current_ctx->gl_vertex_attrib_value[indx * 4 + 1] = 0;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 2] = 0;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 3] = 1;
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


// Optmize?
void
fastpath_glVertexAttrib2f(GLuint indx, GLfloat x, GLfloat y)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	IF_GL_SUCCESS(_orig_fastpath_glVertexAttrib2f(indx, x, y))
	{
		current_ctx->_vattrib_flag |= FLAG_BIT_0;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 0] = x;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 1] = y;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 2] = 0;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 3] = 1;
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


// Optmize?
void
fastpath_glVertexAttrib2fv(GLuint indx, const GLfloat* values)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	IF_GL_SUCCESS(_orig_fastpath_glVertexAttrib2fv(indx, values))
	{
		current_ctx->_vattrib_flag |= FLAG_BIT_0;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 0] = values[0];
		current_ctx->gl_vertex_attrib_value[indx * 4 + 1] = values[1];
		current_ctx->gl_vertex_attrib_value[indx * 4 + 2] = 0;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 3] = 1;
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


// Optmize?
void
fastpath_glVertexAttrib3f(GLuint indx, GLfloat x, GLfloat y, GLfloat z)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	IF_GL_SUCCESS(_orig_fastpath_glVertexAttrib3f(indx, x, y, z))
	{
		current_ctx->_vattrib_flag |= FLAG_BIT_0;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 0] = x;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 1] = y;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 2] = z;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 3] = 1;
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


// Optmize?
void
fastpath_glVertexAttrib3fv(GLuint indx, const GLfloat* values)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	IF_GL_SUCCESS(_orig_fastpath_glVertexAttrib3fv(indx, values))
	{
		current_ctx->_vattrib_flag |= FLAG_BIT_0;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 0] = values[0];
		current_ctx->gl_vertex_attrib_value[indx * 4 + 1] = values[1];
		current_ctx->gl_vertex_attrib_value[indx * 4 + 2] = values[2];
		current_ctx->gl_vertex_attrib_value[indx * 4 + 3] = 1;
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


// Optmize?
void
fastpath_glVertexAttrib4f(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	IF_GL_SUCCESS(_orig_fastpath_glVertexAttrib4f(indx, x, y, z, w))
	{
		current_ctx->_vattrib_flag |= FLAG_BIT_0;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 0] = x;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 1] = y;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 2] = z;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 3] = w;
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


// Optmize?
void
fastpath_glVertexAttrib4fv(GLuint indx, const GLfloat* values)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	IF_GL_SUCCESS(_orig_fastpath_glVertexAttrib4fv(indx, values))
	{
		current_ctx->_vattrib_flag |= FLAG_BIT_0;
		current_ctx->gl_vertex_attrib_value[indx * 4 + 0] = values[0];
		current_ctx->gl_vertex_attrib_value[indx * 4 + 1] = values[1];
		current_ctx->gl_vertex_attrib_value[indx * 4 + 2] = values[2];
		current_ctx->gl_vertex_attrib_value[indx * 4 + 3] = values[3];
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}



// Optmize?
void
fastpath_glVertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	IF_GL_SUCCESS(_orig_fastpath_glVertexAttribPointer(indx, size, type, normalized, stride, ptr))
	{
		current_ctx->_vattrib_flag |= FLAG_BIT_1;

		current_ctx->gl_vertex_array_buf_id[indx]     = current_ctx->gl_array_buffer_binding[0];
		current_ctx->gl_vertex_array_size[indx]       = size;
		current_ctx->gl_vertex_array_type[indx]       = type;
		current_ctx->gl_vertex_array_normalized[indx] = normalized;
		current_ctx->gl_vertex_array_stride[indx]     = stride;
		current_ctx->gl_vertex_array_pointer[indx]    = (GLvoid *)ptr;
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}


void
fastpath_glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if ((current_ctx->gl_viewport[0] != x) ||
	    (current_ctx->gl_viewport[1] != y) ||
	    (current_ctx->gl_viewport[2] != width) ||
	    (current_ctx->gl_viewport[3] != height))
	{
		IF_GL_SUCCESS(_orig_fastpath_glViewport(x, y, width, height))
		{
			current_ctx->_clear_flag1 |= FLAG_BIT_0;
			current_ctx->gl_viewport[0] = x;
			current_ctx->gl_viewport[1] = y;
			current_ctx->gl_viewport[2] = width;
			current_ctx->gl_viewport[3] = height;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glEGLImageTargetTexture2DOES(GLenum target, GLeglImageOES image)
{
	int tex_idx;

	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	tex_idx = current_ctx->gl_active_texture[0] - GL_TEXTURE0;

	switch (target)
	{
		case GL_TEXTURE_2D:
			current_ctx->gl_tex_2d_state[tex_idx] = -1;
			break;
		case GL_TEXTURE_CUBE_MAP:
			current_ctx->gl_tex_cube_state[tex_idx] = -1;
			break;
	}

	_orig_fastpath_glEGLImageTargetTexture2DOES(target, image);

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

#define TRANS_VALUE(index, value) \
{ \
	switch (get_type) \
	{ \
		case GL_INT: ((GLint *)ptr)[index] = value; break; \
		case GL_FLOAT: ((GLfloat *)ptr)[index] = (GLfloat)value; break; \
		case GL_BOOL: ((GLboolean *)ptr)[index] = (value == 0) ? GL_FALSE : GL_TRUE; break; \
	} \
}

static GLboolean
_process_getfunc(GLenum pname, GLvoid *ptr, GLenum get_type)
{
	GLboolean ret = GL_FALSE;

	DEFINE_FASTPAH_GL_FUNC();
	INIT_FASTPATH_GL_FUNC();

	switch (pname)
	{
		case GL_TEXTURE_BINDING_2D:
		case GL_TEXTURE_BINDING_CUBE_MAP:
		case GL_ARRAY_BUFFER_BINDING:
		case GL_ELEMENT_ARRAY_BUFFER_BINDING:
		case GL_FRAMEBUFFER_BINDING:
		case GL_RENDERBUFFER_BINDING:
		case GL_CURRENT_PROGRAM:
		{
			GLint real_obj_id = _COREGL_INT_INIT_VALUE;
			GLuint glue_obj_id = _COREGL_INT_INIT_VALUE;
			GL_Object_Type obj_type = GL_OBJECT_TYPE_UNKNOWN;

			_orig_fastpath_glGetIntegerv(pname, (GLint *)&real_obj_id);

			switch(pname)
			{
				case GL_TEXTURE_BINDING_2D:
				case GL_TEXTURE_BINDING_CUBE_MAP:
					obj_type = GL_OBJECT_TYPE_TEXTURE;
					break;
				case GL_ARRAY_BUFFER_BINDING:
				case GL_ELEMENT_ARRAY_BUFFER_BINDING:
					obj_type = GL_OBJECT_TYPE_BUFFER;
					break;
				case GL_FRAMEBUFFER_BINDING:
					obj_type = GL_OBJECT_TYPE_FRAMEBUFFER;
					break;
				case GL_RENDERBUFFER_BINDING:
					obj_type = GL_OBJECT_TYPE_RENDERBUFFER;
					break;
				case GL_CURRENT_PROGRAM:
					obj_type = GL_OBJECT_TYPE_PROGRAM;
					break;
			}
			AST(obj_type != GL_OBJECT_TYPE_UNKNOWN);
			AST(GET_GLUE_OBJ(obj_type, real_obj_id, &glue_obj_id) == 1);
			TRANS_VALUE(0, glue_obj_id);

			ret = GL_TRUE;
			break;
		}
	}
	goto finish;

finish:
	return ret;
}


void
fastpath_glGetBooleanv(GLenum pname, GLboolean* params)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (_process_getfunc(pname, params, GL_BOOL) != GL_TRUE)
	{
		_orig_fastpath_glGetBooleanv(pname, params);
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glGetFloatv(GLenum pname, GLfloat* params)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (_process_getfunc(pname, params, GL_FLOAT) != GL_TRUE)
	{
		_orig_fastpath_glGetFloatv(pname, params);
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

void
fastpath_glGetIntegerv(GLenum pname, GLint* params)
{
	DEFINE_FASTPAH_GL_FUNC();
	_COREGL_FASTPATH_FUNC_BEGIN();
	INIT_FASTPATH_GL_FUNC();

	if (_process_getfunc(pname, params, GL_INT) != GL_TRUE)
	{
		_orig_fastpath_glGetIntegerv(pname, params);
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
}

