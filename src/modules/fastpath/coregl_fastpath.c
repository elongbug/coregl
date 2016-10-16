#include "coregl_fastpath.h"

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <sys/types.h>
#include <unistd.h>


#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     RET_TYPE (*_orig_fastpath_##FUNC_NAME) PARAM_LIST = NULL;
# include "../../headers/sym_gl1.h"
# include "../../headers/sym_gl2.h"
# include "../../headers/sym_gl_common.h"
# include "../../headers/sym_egl.h"
#undef _COREGL_SYMBOL

Fastpath_Opt_Flag   fp_opt = FP_UNKNOWN_PATH;

int                 debug_nofp = 0;
FILE               *trace_fp = NULL;

GLenum              FPGL_Error = GL_NO_ERROR;

GLGlueContext_List *gctx_list = NULL;

Mutex               init_context_mutex = MUTEX_INITIALIZER;
GLGlueContext      *initial_ctx = NULL;

Mutex               ctx_list_access_mutex = MUTEX_INITIALIZER;

GLContext_List     *glctx_list = NULL;
static int          api_gl_version = COREGL_GLAPI_2;

static void
_state_get_texture_states(GLenum pname, GLint *params)
{
	GLuint cur_active_tex = 0;

	AST(initial_ctx != NULL);

	_orig_fastpath_glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint *)&cur_active_tex);
	int i;
	for (i = 0; i < initial_ctx->gl_tex_units_num[0]; i++) {
		_orig_fastpath_glActiveTexture(GL_TEXTURE0 + i);
		_orig_fastpath_glGetIntegerv(pname, (GLint *)&params[i]);
	}
	_orig_fastpath_glActiveTexture(cur_active_tex);
}

static void
_state_get_draw_buffers(GLenum *params)
{
	AST(initial_ctx != NULL);

	int i;
	for (i = 0; i < initial_ctx->gl_draw_buffers_num[0]; i++) {
		_orig_fastpath_glGetIntegerv(GL_DRAW_BUFFER0 + i, (GLint *)&params[i]);
	}
}

static void
_state_get_transform_feedback_buffer_bindings(GLuint *params)
{
	AST(initial_ctx != NULL);

	int i;
	for (i = 0; i < initial_ctx->gl_transform_feedback_buffer_binding_num[0];
			i++) {
		_orig_fastpath_glGetIntegeri_v(GL_TRANSFORM_FEEDBACK_BUFFER_BINDING, i,
									   (GLint *)&params[i]);
	}
}

static void
_state_get_transform_feedback_buffer_bindings_offset(GLintptr *params)
{
	AST(initial_ctx != NULL);

	int i;
	for (i = 0; i < initial_ctx->gl_transform_feedback_buffer_binding_num[0];
			i++) {
		_orig_fastpath_glGetIntegeri_v(GL_TRANSFORM_FEEDBACK_BUFFER_START, i,
									   (GLint *)&params[i]);
	}
}

static void
_state_get_transform_feedback_buffer_bindings_size(GLsizeiptr *params)
{
	AST(initial_ctx != NULL);

	int i;
	for (i = 0; i < initial_ctx->gl_transform_feedback_buffer_binding_num[0];
			i++) {
		_orig_fastpath_glGetIntegeri_v(GL_TRANSFORM_FEEDBACK_BUFFER_SIZE, i,
									   (GLint *)&params[i]);
	}
}

static void
_state_get_uniform_buffer_bindings(GLuint *params)
{
	AST(initial_ctx != NULL);

	int i;
	for (i = 0; i < initial_ctx->gl_uniform_buffer_binding_num[0]; i++) {
/////////////////////////////////////////////////////////////////////////////////
// XXXX : AVOID SEGFAULT in ADRENO
		((GLint *)params)[i] = 0;
//		_orig_fastpath_glGetIntegeri_v(GL_UNIFORM_BUFFER_BINDING, i, (GLint *)&params[i]);
/////////////////////////////////////////////////////////////////////////////////
	}
}

static void
_state_get_uniform_buffer_bindings_offset(GLintptr *params)
{
	AST(initial_ctx != NULL);

	int i;
	for (i = 0; i < initial_ctx->gl_uniform_buffer_binding_num[0]; i++) {
		_orig_fastpath_glGetIntegeri_v(GL_UNIFORM_BUFFER_START, i, (GLint *)&params[i]);
	}
}

static void
_state_get_uniform_buffer_bindings_size(GLsizeiptr *params)
{
	AST(initial_ctx != NULL);

	int i;
	for (i = 0; i < initial_ctx->gl_uniform_buffer_binding_num[0]; i++) {
		_orig_fastpath_glGetIntegeri_v(GL_UNIFORM_BUFFER_SIZE, i, (GLint *)&params[i]);
	}
}

/* shader storage buffer */
static void
_state_get_shader_storage_buffer_bindings(GLuint *params)
{
	AST(initial_ctx != NULL);

	int i;
	for (i = 0; i < initial_ctx->gl_shader_storage_buffer_binding_num[0]; i++) {
		_orig_fastpath_glGetIntegeri_v(GL_SHADER_STORAGE_BUFFER_BINDING, i,
									   (GLint *)&params[i]);
	}
}

static void
_state_get_shader_storage_buffer_bindings_offset(GLintptr *params)
{
	AST(initial_ctx != NULL);

	int i;
	for (i = 0; i < initial_ctx->gl_shader_storage_buffer_binding_num[0]; i++) {
		_orig_fastpath_glGetIntegeri_v(GL_SHADER_STORAGE_BUFFER_START, i,
									   (GLint *)&params[i]);
	}
}

static void
_state_get_shader_storage_buffer_bindings_size(GLsizeiptr *params)
{
	AST(initial_ctx != NULL);

	int i;
	for (i = 0; i < initial_ctx->gl_shader_storage_buffer_binding_num[0]; i++) {
		_orig_fastpath_glGetIntegeri_v(GL_SHADER_STORAGE_BUFFER_SIZE, i,
									   (GLint *)&params[i]);
	}
}

/* atomic counter buffer */
static void
_state_get_atomic_counter_buffer_bindings(GLuint *params)
{
	AST(initial_ctx != NULL);

	int i;
	for (i = 0; i < initial_ctx->gl_atomic_counter_buffer_binding_num[0]; i++) {
		_orig_fastpath_glGetIntegeri_v(GL_ATOMIC_COUNTER_BUFFER_BINDING, i,
									   (GLint *)&params[i]);
	}
}

static void
_state_get_atomic_counter_buffer_bindings_offset(GLintptr *params)
{
	AST(initial_ctx != NULL);

	int i;
	for (i = 0; i < initial_ctx->gl_atomic_counter_buffer_binding_num[0]; i++) {
		_orig_fastpath_glGetIntegeri_v(GL_ATOMIC_COUNTER_BUFFER_START, i,
									   (GLint *)&params[i]);
	}
}

static void
_state_get_atomic_counter_buffer_bindings_size(GLsizeiptr *params)
{
	AST(initial_ctx != NULL);

	int i;
	for (i = 0; i < initial_ctx->gl_atomic_counter_buffer_binding_num[0]; i++) {
		_orig_fastpath_glGetIntegeri_v(GL_ATOMIC_COUNTER_BUFFER_SIZE, i,
									   (GLint *)&params[i]);
	}
}

void
fastpath_state_get_draw_buffers(GLenum *params)
{
	_state_get_draw_buffers(params);
}

void
init_modules_fastpath()
{
	int fastpath_opt = 0;
	int fastpath_force_off_opt = 0;

	COREGL_DBG("<Fastpath> : ");

	fastpath_opt = atoi(get_env_setting("COREGL_FASTPATH"));
	fastpath_force_off_opt = atoi(get_env_setting("COREGL_FASTPATH_FORCE_OFF"));

	if (fastpath_force_off_opt == 1) {
		COREGL_DBG("DISABLED by force option");
		fastpath_opt = 0;
	}

	switch (fastpath_opt) {
	case 1:
		COREGL_DBG("(%d) Fastpath enabled...", fastpath_opt);
		fp_opt = FP_FAST_PATH;
		break;
	default:
		COREGL_DBG("(%d) Default API path enabled...", fastpath_opt);
		fp_opt = FP_NORMAL_PATH;
		break;
	}

	debug_nofp = atoi(get_env_setting("COREGL_DEBUG_NOFP"));

}

void
deinit_modules_fastpath()
{
	GLContext_List *current = NULL;

	AST(mutex_lock(&ctx_list_access_mutex) == 1);

	// Destroy remained context & Detect leaks
	int retry_destroy = 0;

	while (1) {
		retry_destroy = 0;
		current = glctx_list;
		while (current) {
			if (current->cstate != NULL) {
				COREGL_WARN("Context attached to [dpy=%p|rctx=%p] has not been completely destroyed.(leak)",
							current->cstate->rdpy, current->cstate->rctx);

				_orig_fastpath_eglMakeCurrent(current->cstate->rdpy, EGL_NO_SURFACE,
											  EGL_NO_SURFACE, EGL_NO_CONTEXT);
				_orig_fastpath_eglDestroyContext(current->cstate->rdpy, current->cstate->rctx);

				fastpath_remove_context_states_from_list(current->cstate, NULL);
				retry_destroy = 1;
				break;
			}

			glctx_list = current->next;
			free(current);
			current = glctx_list;
		}
		if (retry_destroy == 0) break;
	}
	goto finish;

finish:
	AST(mutex_unlock(&ctx_list_access_mutex) == 1);
}

void
init_modules_tstate_fastpath(GLThreadState *tstate)
{
	MY_MODULE_TSTATE *tstate_mt = NULL;

	tstate_mt = (MY_MODULE_TSTATE *)calloc(1, sizeof(MY_MODULE_TSTATE));

	tstate_mt->binded_api = EGL_OPENGL_ES_API;

	tstate->module_data[MY_MODULE_ID] = tstate_mt;
}

void
deinit_modules_tstate_fastpath(GLThreadState *tstate)
{
	if (tstate->module_data[MY_MODULE_ID] != NULL) {
		free(tstate->module_data[MY_MODULE_ID]);
		tstate->module_data[MY_MODULE_ID] = NULL;
	}
}

void
fastpath_apply_overrides()
{
	switch (fp_opt) {
	case FP_FAST_PATH:
		fastpath_apply_overrides_egl(1);
		fastpath_apply_overrides_gl(1);
		break;
	case FP_NORMAL_PATH:
		break;
	default:
		COREGL_ERR("Invalide GL Override Option!!!");
		break;
	}
}


void
fastpath_apply_overrides_egl(int enable)
{
#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     COREGL_INIT_ORIGINAL(_orig_fastpath_, FUNC_NAME);
# include "../../headers/sym_egl.h"
#undef _COREGL_SYMBOL

	COREGL_OVERRIDE(fastpath_, eglGetProcAddress);

	COREGL_OVERRIDE(fastpath_, eglBindAPI);
	COREGL_OVERRIDE(fastpath_, eglQueryAPI);

	COREGL_OVERRIDE(fastpath_, eglCreateContext);
	COREGL_OVERRIDE(fastpath_, eglCreateImageKHR);
	COREGL_OVERRIDE(fastpath_, eglMakeCurrent);
	COREGL_OVERRIDE(fastpath_, eglDestroyContext);
	COREGL_OVERRIDE(fastpath_, eglQueryContext);
	COREGL_OVERRIDE(fastpath_, eglGetCurrentContext);
	COREGL_OVERRIDE(fastpath_, eglReleaseThread);
	COREGL_OVERRIDE(fastpath_, eglGetCurrentSurface);
	COREGL_OVERRIDE(fastpath_, eglTerminate);
	COREGL_OVERRIDE(fastpath_, eglGetCurrentDisplay);

}

void
fastpath_apply_overrides_gl(int enable)
{
#define _COREGL_START_API(version) api_gl_version = version;
#define _COREGL_END_API(version) api_gl_version = COREGL_GLAPI_2;
#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     \
   if(api_gl_version <= driver_gl_version) COREGL_INIT_ORIGINAL(_orig_fastpath_, FUNC_NAME);

# include "../../headers/sym_gl1.h"
# include "../../headers/sym_gl2.h"
# include "../../headers/sym_gl_common.h"
#undef _COREGL_SYMBOL
#undef _COREGL_START_API
#undef _COREGL_END_API

	if (debug_nofp != 1) {
		if (driver_gl_version >= COREGL_GLAPI_1) {
			COREGL_OVERRIDE(fastpath_, glClientActiveTexture);
			COREGL_OVERRIDE(fastpath_, glSampleCoveragex);
		}

		COREGL_OVERRIDE(fastpath_, glGetError);
		COREGL_OVERRIDE(fastpath_, glGetString);

		COREGL_OVERRIDE(fastpath_, glGetIntegerv);
		COREGL_OVERRIDE(fastpath_, glGetFloatv);

		COREGL_OVERRIDE(fastpath_, glActiveTexture);
		COREGL_OVERRIDE(fastpath_, glGenTextures);
		COREGL_OVERRIDE(fastpath_, glBindTexture);
		COREGL_OVERRIDE(fastpath_, glIsTexture);
		COREGL_OVERRIDE(fastpath_, glDeleteTextures);
		COREGL_OVERRIDE(fastpath_, glFramebufferTexture2D);

		COREGL_OVERRIDE(fastpath_, glGenBuffers);
		COREGL_OVERRIDE(fastpath_, glBindBuffer);
		COREGL_OVERRIDE(fastpath_, glIsBuffer);
		COREGL_OVERRIDE(fastpath_, glDeleteBuffers);

		COREGL_OVERRIDE(fastpath_, glGenFramebuffers);
		COREGL_OVERRIDE(fastpath_, glBindFramebuffer);
		COREGL_OVERRIDE(fastpath_, glIsFramebuffer);
		COREGL_OVERRIDE(fastpath_, glDeleteFramebuffers);
		COREGL_OVERRIDE(fastpath_, glGetFramebufferAttachmentParameteriv);

		COREGL_OVERRIDE(fastpath_, glGenRenderbuffers);
		COREGL_OVERRIDE(fastpath_, glBindRenderbuffer);
		COREGL_OVERRIDE(fastpath_, glFramebufferRenderbuffer);
		COREGL_OVERRIDE(fastpath_, glIsRenderbuffer);
		COREGL_OVERRIDE(fastpath_, glDeleteRenderbuffers);

		COREGL_OVERRIDE(fastpath_, glCreateShader);
		COREGL_OVERRIDE(fastpath_, glCreateProgram);
		COREGL_OVERRIDE(fastpath_, glAttachShader);
		COREGL_OVERRIDE(fastpath_, glCompileShader);
		COREGL_OVERRIDE(fastpath_, glShaderBinary);
		COREGL_OVERRIDE(fastpath_, glDeleteShader);
		COREGL_OVERRIDE(fastpath_, glDetachShader);
		COREGL_OVERRIDE(fastpath_, glGetShaderiv);
		COREGL_OVERRIDE(fastpath_, glGetShaderInfoLog);
		COREGL_OVERRIDE(fastpath_, glGetShaderSource);
		COREGL_OVERRIDE(fastpath_, glIsShader);
		COREGL_OVERRIDE(fastpath_, glShaderSource);
		COREGL_OVERRIDE(fastpath_, glBindAttribLocation);
		COREGL_OVERRIDE(fastpath_, glDeleteProgram);
		COREGL_OVERRIDE(fastpath_, glGetActiveAttrib);
		COREGL_OVERRIDE(fastpath_, glGetActiveUniform);
		COREGL_OVERRIDE(fastpath_, glGetAttachedShaders);
		COREGL_OVERRIDE(fastpath_, glGetAttribLocation);
		COREGL_OVERRIDE(fastpath_, glGetProgramiv);
		COREGL_OVERRIDE(fastpath_, glGetProgramInfoLog);
		COREGL_OVERRIDE(fastpath_, glGetUniformfv);
		COREGL_OVERRIDE(fastpath_, glGetUniformiv);
		COREGL_OVERRIDE(fastpath_, glGetUniformLocation);
		COREGL_OVERRIDE(fastpath_, glIsProgram);
		COREGL_OVERRIDE(fastpath_, glLinkProgram);
		COREGL_OVERRIDE(fastpath_, glUseProgram);
		COREGL_OVERRIDE(fastpath_, glValidateProgram);

		COREGL_OVERRIDE(fastpath_, glBlendColor);
		COREGL_OVERRIDE(fastpath_, glBlendEquation);
		COREGL_OVERRIDE(fastpath_, glBlendEquationSeparate);
		COREGL_OVERRIDE(fastpath_, glBlendFunc);
		COREGL_OVERRIDE(fastpath_, glBlendFuncSeparate);
		COREGL_OVERRIDE(fastpath_, glClearColor);
		COREGL_OVERRIDE(fastpath_, glClearDepthf);
		COREGL_OVERRIDE(fastpath_, glClearStencil);
		COREGL_OVERRIDE(fastpath_, glColorMask);
		COREGL_OVERRIDE(fastpath_, glColorMaski);
		COREGL_OVERRIDE(fastpath_, glColorMaskiOES);
		COREGL_OVERRIDE(fastpath_, glCullFace);
		COREGL_OVERRIDE(fastpath_, glDepthFunc);
		COREGL_OVERRIDE(fastpath_, glDepthMask);
		COREGL_OVERRIDE(fastpath_, glDepthRangef);
		COREGL_OVERRIDE(fastpath_, glDisable);
		COREGL_OVERRIDE(fastpath_, glDisableVertexAttribArray);
		COREGL_OVERRIDE(fastpath_, glEnable);
		COREGL_OVERRIDE(fastpath_, glEnableVertexAttribArray);
		COREGL_OVERRIDE(fastpath_, glFrontFace);
		COREGL_OVERRIDE(fastpath_, glHint);
		COREGL_OVERRIDE(fastpath_, glLineWidth);
		COREGL_OVERRIDE(fastpath_, glPixelStorei);
		COREGL_OVERRIDE(fastpath_, glPolygonOffset);
		COREGL_OVERRIDE(fastpath_, glSampleCoverage);
		COREGL_OVERRIDE(fastpath_, glScissor);
		COREGL_OVERRIDE(fastpath_, glStencilFunc);
		COREGL_OVERRIDE(fastpath_, glStencilFuncSeparate);
		COREGL_OVERRIDE(fastpath_, glStencilMask);
		COREGL_OVERRIDE(fastpath_, glStencilMaskSeparate);
		COREGL_OVERRIDE(fastpath_, glStencilOp);
		COREGL_OVERRIDE(fastpath_, glStencilOpSeparate);
		COREGL_OVERRIDE(fastpath_, glVertexAttrib1f);
		COREGL_OVERRIDE(fastpath_, glVertexAttrib1fv);
		COREGL_OVERRIDE(fastpath_, glVertexAttrib2f);
		COREGL_OVERRIDE(fastpath_, glVertexAttrib2fv);
		COREGL_OVERRIDE(fastpath_, glVertexAttrib3f);
		COREGL_OVERRIDE(fastpath_, glVertexAttrib3fv);
		COREGL_OVERRIDE(fastpath_, glVertexAttrib4f);
		COREGL_OVERRIDE(fastpath_, glVertexAttrib4fv);
		COREGL_OVERRIDE(fastpath_, glVertexAttribPointer);
		COREGL_OVERRIDE(fastpath_, glViewport);

		COREGL_OVERRIDE(fastpath_, glGetProgramBinaryOES);
		COREGL_OVERRIDE(fastpath_, glProgramBinaryOES);

		COREGL_OVERRIDE(fastpath_, glUseProgramStagesEXT);
		COREGL_OVERRIDE(fastpath_, glActiveShaderProgramEXT);
		COREGL_OVERRIDE(fastpath_, glProgramParameteriEXT);
		COREGL_OVERRIDE(fastpath_, glProgramUniform1iEXT);
		COREGL_OVERRIDE(fastpath_, glProgramUniform2iEXT);
		COREGL_OVERRIDE(fastpath_, glProgramUniform3iEXT);
		COREGL_OVERRIDE(fastpath_, glProgramUniform4iEXT);
		COREGL_OVERRIDE(fastpath_, glProgramUniform1fEXT);
		COREGL_OVERRIDE(fastpath_, glProgramUniform2fEXT);
		COREGL_OVERRIDE(fastpath_, glProgramUniform3fEXT);
		COREGL_OVERRIDE(fastpath_, glProgramUniform4fEXT);
		COREGL_OVERRIDE(fastpath_, glProgramUniform1ivEXT);
		COREGL_OVERRIDE(fastpath_, glProgramUniform2ivEXT);
		COREGL_OVERRIDE(fastpath_, glProgramUniform3ivEXT);
		COREGL_OVERRIDE(fastpath_, glProgramUniform4ivEXT);
		COREGL_OVERRIDE(fastpath_, glProgramUniform1fvEXT);
		COREGL_OVERRIDE(fastpath_, glProgramUniform2fvEXT);
		COREGL_OVERRIDE(fastpath_, glProgramUniform3fvEXT);
		COREGL_OVERRIDE(fastpath_, glProgramUniform4fvEXT);
		COREGL_OVERRIDE(fastpath_, glProgramUniformMatrix2fvEXT);
		COREGL_OVERRIDE(fastpath_, glProgramUniformMatrix3fvEXT);
		COREGL_OVERRIDE(fastpath_, glProgramUniformMatrix4fvEXT);

		COREGL_OVERRIDE(fastpath_, glFramebufferTexture2DMultisampleEXT);
		COREGL_OVERRIDE(fastpath_, glFramebufferTexture3DOES);

		COREGL_OVERRIDE(fastpath_, glBindFramebufferOES);
		COREGL_OVERRIDE(fastpath_, glBindRenderbufferOES);
		COREGL_OVERRIDE(fastpath_, glDeleteFramebuffersOES);
		COREGL_OVERRIDE(fastpath_, glDeleteRenderbuffersOES);
		COREGL_OVERRIDE(fastpath_, glDepthRangefOES);
		COREGL_OVERRIDE(fastpath_, glDepthRangexOES);
		COREGL_OVERRIDE(fastpath_, glFramebufferRenderbufferOES);
		COREGL_OVERRIDE(fastpath_, glFramebufferTexture2DOES);
		COREGL_OVERRIDE(fastpath_, glGenFramebuffersOES);
		COREGL_OVERRIDE(fastpath_, glGenRenderbuffersOES);
		COREGL_OVERRIDE(fastpath_, glGetFramebufferAttachmentParameterivOES);
		COREGL_OVERRIDE(fastpath_, glGetQueryObjecti64vEXT);
		COREGL_OVERRIDE(fastpath_, glGetQueryObjectivEXT);
		COREGL_OVERRIDE(fastpath_, glGetQueryObjectui64vEXT);
		COREGL_OVERRIDE(fastpath_, glGetQueryObjectuivEXT);
		COREGL_OVERRIDE(fastpath_, glGetQueryivEXT);
		COREGL_OVERRIDE(fastpath_, glBeginQueryEXT);
		COREGL_OVERRIDE(fastpath_, glDeleteQueriesEXT);
		COREGL_OVERRIDE(fastpath_, glGenQueriesEXT);
		COREGL_OVERRIDE(fastpath_, glIsFramebufferOES);
		COREGL_OVERRIDE(fastpath_, glIsQueryEXT);
		COREGL_OVERRIDE(fastpath_, glIsRenderbufferOES);
		COREGL_OVERRIDE(fastpath_, glBlendEquationOES);
		COREGL_OVERRIDE(fastpath_, glBlendEquationSeparateOES);
		COREGL_OVERRIDE(fastpath_, glBlendFuncSeparateOES);
		COREGL_OVERRIDE(fastpath_, glPolygonOffsetxOES);
		COREGL_OVERRIDE(fastpath_, glLineWidthxOES);
		COREGL_OVERRIDE(fastpath_, glSampleCoveragexOES);
		COREGL_OVERRIDE(fastpath_, glQueryCounterEXT);
		COREGL_OVERRIDE(fastpath_, glBindVertexArrayOES);
		COREGL_OVERRIDE(fastpath_, glDeleteVertexArraysOES);
		COREGL_OVERRIDE(fastpath_, glGenVertexArraysOES);
		COREGL_OVERRIDE(fastpath_, glIsVertexArrayOES);
		COREGL_OVERRIDE(fastpath_, glClearDepthfOES);
		COREGL_OVERRIDE(fastpath_, glClearDepthxOES);
		COREGL_OVERRIDE(fastpath_, glGetVertexAttribiv);
		COREGL_OVERRIDE(fastpath_, glTexBufferOES);
		COREGL_OVERRIDE(fastpath_, glFramebufferTextureEXT);
		COREGL_OVERRIDE(fastpath_, glFramebufferTextureOES);
		COREGL_OVERRIDE(fastpath_, glTexBufferRangeOES);
		COREGL_OVERRIDE(fastpath_, glSamplerParameterIuivOES);
		COREGL_OVERRIDE(fastpath_, glSamplerParameterIivOES);
		COREGL_OVERRIDE(fastpath_, glGetSamplerParameterIuivOES);
		COREGL_OVERRIDE(fastpath_, glGetSamplerParameterIivOES);
		COREGL_OVERRIDE(fastpath_, glGetSamplerParameterIivEXT);
		COREGL_OVERRIDE(fastpath_, glGetSamplerParameterIuivEXT);
		COREGL_OVERRIDE(fastpath_, glSamplerParameterIivEXT);
		COREGL_OVERRIDE(fastpath_, glSamplerParameterIuivEXT);
		COREGL_OVERRIDE(fastpath_, glTexBufferRangeEXT);
		COREGL_OVERRIDE(fastpath_, glTexBufferEXT);
		/* Start overriding GLES 3.0 */
		if (driver_gl_version >= COREGL_GLAPI_3) {
			COREGL_OVERRIDE(fastpath_, glReadBuffer);

			COREGL_OVERRIDE(fastpath_, glGenQueries);
			COREGL_OVERRIDE(fastpath_, glDeleteQueries);
			COREGL_OVERRIDE(fastpath_, glIsQuery);
			COREGL_OVERRIDE(fastpath_, glBeginQuery);
			COREGL_OVERRIDE(fastpath_, glGetQueryiv);
			COREGL_OVERRIDE(fastpath_, glGetQueryObjectuiv);
			COREGL_OVERRIDE(fastpath_, glDrawBuffers);
			COREGL_OVERRIDE(fastpath_, glFramebufferTextureLayer);

			COREGL_OVERRIDE(fastpath_, glBindVertexArray);
			COREGL_OVERRIDE(fastpath_, glDeleteVertexArrays);
			COREGL_OVERRIDE(fastpath_, glGenVertexArrays);
			COREGL_OVERRIDE(fastpath_, glIsVertexArray);

			COREGL_OVERRIDE(fastpath_, glGetIntegeri_v);

			COREGL_OVERRIDE(fastpath_, glBindTransformFeedback);
			COREGL_OVERRIDE(fastpath_, glBeginTransformFeedback);
			COREGL_OVERRIDE(fastpath_, glEndTransformFeedback);
			COREGL_OVERRIDE(fastpath_, glPauseTransformFeedback);
			COREGL_OVERRIDE(fastpath_, glResumeTransformFeedback);
			COREGL_OVERRIDE(fastpath_, glIsTransformFeedback);
			COREGL_OVERRIDE(fastpath_, glDeleteTransformFeedbacks);
			COREGL_OVERRIDE(fastpath_, glGenTransformFeedbacks);
			COREGL_OVERRIDE(fastpath_, glIsTransformFeedback);

			COREGL_OVERRIDE(fastpath_, glBindBufferRange);
			COREGL_OVERRIDE(fastpath_, glBindBufferBase);
			COREGL_OVERRIDE(fastpath_, glTransformFeedbackVaryings);
			COREGL_OVERRIDE(fastpath_, glGetTransformFeedbackVarying);
			COREGL_OVERRIDE(fastpath_, glVertexAttribIPointer);
			COREGL_OVERRIDE(fastpath_, glVertexAttribI4i);
			COREGL_OVERRIDE(fastpath_, glVertexAttribI4ui);
			COREGL_OVERRIDE(fastpath_, glVertexAttribI4iv);
			COREGL_OVERRIDE(fastpath_, glVertexAttribI4uiv);
			COREGL_OVERRIDE(fastpath_, glGetUniformuiv);
			COREGL_OVERRIDE(fastpath_, glGetFragDataLocation);
			COREGL_OVERRIDE(fastpath_, glGetStringi);
			COREGL_OVERRIDE(fastpath_, glGetUniformIndices);
			COREGL_OVERRIDE(fastpath_, glGetActiveUniformsiv);
			COREGL_OVERRIDE(fastpath_, glGetUniformBlockIndex);
			COREGL_OVERRIDE(fastpath_, glGetActiveUniformBlockiv);
			COREGL_OVERRIDE(fastpath_, glGetActiveUniformBlockName);
			COREGL_OVERRIDE(fastpath_, glUniformBlockBinding);
			COREGL_OVERRIDE(fastpath_, glGetInteger64v);
			COREGL_OVERRIDE(fastpath_, glGetInteger64i_v);
			COREGL_OVERRIDE(fastpath_, glGenSamplers);
			COREGL_OVERRIDE(fastpath_, glDeleteSamplers);
			COREGL_OVERRIDE(fastpath_, glIsSampler);
			COREGL_OVERRIDE(fastpath_, glBindSampler);
			COREGL_OVERRIDE(fastpath_, glSamplerParameteri);
			COREGL_OVERRIDE(fastpath_, glSamplerParameteriv);
			COREGL_OVERRIDE(fastpath_, glSamplerParameterf);
			COREGL_OVERRIDE(fastpath_, glSamplerParameterfv);
			COREGL_OVERRIDE(fastpath_, glGetSamplerParameteriv);
			COREGL_OVERRIDE(fastpath_, glGetSamplerParameterfv);
			COREGL_OVERRIDE(fastpath_, glVertexAttribDivisor);
			COREGL_OVERRIDE(fastpath_, glGetProgramBinary);
			COREGL_OVERRIDE(fastpath_, glProgramBinary);
			COREGL_OVERRIDE(fastpath_, glProgramParameteri);
		} // End of GLES 3.0

		if (driver_gl_version >= COREGL_GLAPI_31) {
			COREGL_OVERRIDE(fastpath_, glCreateShaderProgramv);
			COREGL_OVERRIDE(fastpath_, glGenProgramPipelines);
			COREGL_OVERRIDE(fastpath_, glGetProgramPipelineiv);
			COREGL_OVERRIDE(fastpath_, glBindProgramPipeline);
			COREGL_OVERRIDE(fastpath_, glDeleteProgramPipelines);
			COREGL_OVERRIDE(fastpath_, glIsProgramPipeline);
			COREGL_OVERRIDE(fastpath_, glValidateProgramPipeline);
			COREGL_OVERRIDE(fastpath_, glGetProgramPipelineInfoLog);
			COREGL_OVERRIDE(fastpath_, glDispatchCompute);
			COREGL_OVERRIDE(fastpath_, glDispatchComputeIndirect);
			COREGL_OVERRIDE(fastpath_, glGetProgramInterfaceiv);
			COREGL_OVERRIDE(fastpath_, glGetProgramResourceIndex);
			COREGL_OVERRIDE(fastpath_, glGetProgramResourceName);
			COREGL_OVERRIDE(fastpath_, glGetProgramResourceiv);
			COREGL_OVERRIDE(fastpath_, glGetProgramResourceLocation);
			COREGL_OVERRIDE(fastpath_, glUseProgramStages);
			COREGL_OVERRIDE(fastpath_, glActiveShaderProgram);
			COREGL_OVERRIDE(fastpath_, glProgramUniform1iv);
			COREGL_OVERRIDE(fastpath_, glProgramUniform2iv);
			COREGL_OVERRIDE(fastpath_, glProgramUniform3iv);
			COREGL_OVERRIDE(fastpath_, glProgramUniform4iv);
			COREGL_OVERRIDE(fastpath_, glProgramUniform1fv);
			COREGL_OVERRIDE(fastpath_, glProgramUniform2fv);
			COREGL_OVERRIDE(fastpath_, glProgramUniform3fv);
			COREGL_OVERRIDE(fastpath_, glProgramUniform4fv);
			COREGL_OVERRIDE(fastpath_, glProgramUniformMatrix2fv);
			COREGL_OVERRIDE(fastpath_, glProgramUniformMatrix3fv);
			COREGL_OVERRIDE(fastpath_, glProgramUniformMatrix4fv);
			COREGL_OVERRIDE(fastpath_, glProgramUniform1i);
			COREGL_OVERRIDE(fastpath_, glProgramUniform2i);
			COREGL_OVERRIDE(fastpath_, glProgramUniform3i);
			COREGL_OVERRIDE(fastpath_, glProgramUniform4i);
			COREGL_OVERRIDE(fastpath_, glProgramUniform1f);
			COREGL_OVERRIDE(fastpath_, glProgramUniform2f);
			COREGL_OVERRIDE(fastpath_, glProgramUniform3f);
			COREGL_OVERRIDE(fastpath_, glProgramUniform4f);
			COREGL_OVERRIDE(fastpath_, glProgramUniform1uiv);
			COREGL_OVERRIDE(fastpath_, glProgramUniform2uiv);
			COREGL_OVERRIDE(fastpath_, glProgramUniform3uiv);
			COREGL_OVERRIDE(fastpath_, glProgramUniform4uiv);
			COREGL_OVERRIDE(fastpath_, glProgramUniform1ui);
			COREGL_OVERRIDE(fastpath_, glProgramUniform2ui);
			COREGL_OVERRIDE(fastpath_, glProgramUniform3ui);
			COREGL_OVERRIDE(fastpath_, glProgramUniform4ui);
			COREGL_OVERRIDE(fastpath_, glProgramUniformMatrix2x3fv);
			COREGL_OVERRIDE(fastpath_, glProgramUniformMatrix3x2fv);
			COREGL_OVERRIDE(fastpath_, glProgramUniformMatrix4x2fv);
			COREGL_OVERRIDE(fastpath_, glProgramUniformMatrix2x4fv);
			COREGL_OVERRIDE(fastpath_, glProgramUniformMatrix3x4fv);
			COREGL_OVERRIDE(fastpath_, glProgramUniformMatrix4x3fv);
			COREGL_OVERRIDE(fastpath_, glBindImageTexture);
			COREGL_OVERRIDE(fastpath_, glGetBooleani_v);
			COREGL_OVERRIDE(fastpath_, glGetMultisamplefv);
			COREGL_OVERRIDE(fastpath_, glGetTexLevelParameteriv);
			COREGL_OVERRIDE(fastpath_, glGetTexLevelParameterfv);
			COREGL_OVERRIDE(fastpath_, glBindVertexBuffer);
		}

		if (driver_gl_version >= COREGL_GLAPI_32) {
			COREGL_OVERRIDE(fastpath_, glBlendEquationi);
			COREGL_OVERRIDE(fastpath_, glBlendEquationSeparatei);
			COREGL_OVERRIDE(fastpath_, glTexBuffer);
			COREGL_OVERRIDE(fastpath_, glTexBufferRange);
			COREGL_OVERRIDE(fastpath_, glFramebufferTexture);
			COREGL_OVERRIDE(fastpath_, glGetnUniformfv);
			COREGL_OVERRIDE(fastpath_, glGetnUniformiv);
			COREGL_OVERRIDE(fastpath_, glGetnUniformuiv);
			COREGL_OVERRIDE(fastpath_, glSamplerParameterIiv);
			COREGL_OVERRIDE(fastpath_, glSamplerParameterIuiv);
			COREGL_OVERRIDE(fastpath_, glGetSamplerParameterIiv);
			COREGL_OVERRIDE(fastpath_, glGetSamplerParameterIuiv);
		}
	} else {
		COREGL_DBG("SKIP GL FASTPATH...");
	}
}

#undef OVERRIDE

static inline GL_Object_Hash_Base *
_lock_gl_object_hash(GL_Object_State *ostate, GL_Object_Type type)
{
	switch (type) {
	case GL_OBJECT_TYPE_QUERY:
		return &ostate->query;
	case GL_OBJECT_TYPE_TEXTURE:
		AST(mutex_lock(&ostate->shared->access_mutex) == 1);
		return &ostate->shared->texture;
	case GL_OBJECT_TYPE_BUFFER:
		AST(mutex_lock(&ostate->shared->access_mutex) == 1);
		return &ostate->shared->buffer;
	case GL_OBJECT_TYPE_FRAMEBUFFER:
		return &ostate->framebuffer;
	case GL_OBJECT_TYPE_RENDERBUFFER:
		AST(mutex_lock(&ostate->shared->access_mutex) == 1);
		return &ostate->shared->renderbuffer;
	case GL_OBJECT_TYPE_PROGRAM:
		AST(mutex_lock(&ostate->shared->access_mutex) == 1);
		return &ostate->shared->program;
	case GL_OBJECT_TYPE_VERTEXARRAY:
		return &ostate->vertexarray;
	case GL_OBJECT_TYPE_SAMPLER:
		AST(mutex_lock(&ostate->shared->access_mutex) == 1);
		return &ostate->shared->sampler;
	case GL_OBJECT_TYPE_TRANSFORMFEEDBACK:
		return &ostate->transformfeedback;
	case GL_OBJECT_TYPE_PROGRAMPIPELINE:
		AST(mutex_lock(&ostate->shared->access_mutex) == 1);
		return &ostate->shared->programpipeline;
	default:
		return NULL;
	}
}

static inline void
_unlock_gl_object_hash(GL_Object_State *ostate, GL_Object_Type type)
{
	switch (type) {
	case GL_OBJECT_TYPE_TEXTURE:
	case GL_OBJECT_TYPE_BUFFER:
	case GL_OBJECT_TYPE_RENDERBUFFER:
	case GL_OBJECT_TYPE_PROGRAM:
	case GL_OBJECT_TYPE_SAMPLER:
	case GL_OBJECT_TYPE_PROGRAMPIPELINE:
		AST(mutex_unlock(&ostate->shared->access_mutex) == 1);
	default:
		break;
	}
}

static inline GL_Object_Hash_Base *
_lock_gl_object_hash_real(GL_Object_State *ostate, GL_Object_Type type)
{
	switch (type) {
	case GL_OBJECT_TYPE_QUERY:
		return &ostate->query_real;
	case GL_OBJECT_TYPE_TEXTURE:
		AST(mutex_lock(&ostate->shared->real_access_mutex) == 1);
		return &ostate->shared->texture_real;
	case GL_OBJECT_TYPE_BUFFER:
		AST(mutex_lock(&ostate->shared->real_access_mutex) == 1);
		return &ostate->shared->buffer_real;
	case GL_OBJECT_TYPE_FRAMEBUFFER:
		return &ostate->framebuffer_real;
	case GL_OBJECT_TYPE_RENDERBUFFER:
		AST(mutex_lock(&ostate->shared->real_access_mutex) == 1);
		return &ostate->shared->renderbuffer_real;
	case GL_OBJECT_TYPE_PROGRAM:
		AST(mutex_lock(&ostate->shared->real_access_mutex) == 1);
		return &ostate->shared->program_real;
	case GL_OBJECT_TYPE_VERTEXARRAY:
		return &ostate->vertexarray_real;
	case GL_OBJECT_TYPE_SAMPLER:
		AST(mutex_lock(&ostate->shared->real_access_mutex) == 1);
		return &ostate->shared->sampler_real;
	case GL_OBJECT_TYPE_TRANSFORMFEEDBACK:
		return &ostate->transformfeedback_real;
	case GL_OBJECT_TYPE_PROGRAMPIPELINE:
		AST(mutex_lock(&ostate->shared->real_access_mutex) == 1);
		return &ostate->shared->programpipeline_real;
	default:
		return NULL;
	}
}

static inline void
_unlock_gl_object_hash_real(GL_Object_State *ostate, GL_Object_Type type)
{
	switch (type) {
	case GL_OBJECT_TYPE_TEXTURE:
	case GL_OBJECT_TYPE_BUFFER:
	case GL_OBJECT_TYPE_RENDERBUFFER:
	case GL_OBJECT_TYPE_PROGRAM:
	case GL_OBJECT_TYPE_SAMPLER:
	case GL_OBJECT_TYPE_PROGRAMPIPELINE:
		AST(mutex_unlock(&ostate->shared->real_access_mutex) == 1);
		break;
	default:
		break;
	}
}

int
fastpath_add_context_state_to_list(const void *option, const int option_len,
								   GLContextState *cstate, Mutex *mtx)
{
	int ret = 0;
	int tid = 0;
	GLContext_List *current = NULL;
	GLContext_List *newitm = NULL;

	if (mtx != NULL) AST(mutex_lock(mtx) == 1);

	AST(cstate != NULL);

	tid = get_current_thread();

	current = glctx_list;
	while (current != NULL) {
		if (current->option_len == option_len &&
				memcmp(current->option, option, option_len) == 0 &&
				current->thread_id == tid) {
			AST(current->cstate == cstate);
			goto finish;
		}
		current = current->next;
	}

	newitm = (GLContext_List *)calloc(1, sizeof(GLContext_List));
	if (newitm == NULL) {
		COREGL_ERR("Failed to create context list.");
		goto finish;
	}

	newitm->cstate = cstate;
	newitm->thread_id = tid;
	newitm->option_len = option_len;
	newitm->option = (void *)malloc(option_len);
	memcpy(newitm->option, option, option_len);

	if (glctx_list != NULL)
		newitm->next = glctx_list;

	glctx_list = newitm;

	ret = 1;
	goto finish;

finish:
	if (ret != 1) {
		if (newitm != NULL) {
			free(newitm);
			newitm = NULL;
		}
		if (cstate != NULL) {
			free(cstate);
			cstate = NULL;
		}
	}
	if (mtx != NULL) AST(mutex_unlock(mtx) == 1);

	return ret;
}

GLContextState *
fastpath_get_context_state_from_list(const void *option, const int option_len,
									 Mutex *mtx)
{
	GLContextState *ret = NULL;
	GLContext_List *current = NULL;
	int tid = 0;

	if (mtx != NULL) AST(mutex_lock(mtx) == 1);

	tid = get_current_thread();

	current = glctx_list;
	while (current != NULL) {
		if (current->option_len == option_len &&
				memcmp(current->option, option, option_len) == 0 &&
				current->thread_id == tid) {
			ret = current->cstate;
			goto finish;
		}
		current = current->next;
	}
	goto finish;

finish:
	if (mtx != NULL) AST(mutex_unlock(mtx) == 1);
	return ret;
}

int
fastpath_remove_context_states_from_list(GLContextState *cstate, Mutex *mtx)
{
	int ret = 0;
	GLContext_List *olditm = NULL;
	GLContext_List *current = NULL;

	if (mtx != NULL) AST(mutex_lock(mtx) == 1);

	AST(cstate != NULL);

	current = glctx_list;

	while (current != NULL) {
		if (current->cstate == cstate) {
			GLContext_List *nextitm = NULL;
			if (olditm != NULL) {
				olditm->next = current->next;
				nextitm = olditm->next;
			} else {
				glctx_list = current->next;
				nextitm = glctx_list;
			}
			if (current->option != NULL) {
				AST(current->option_len > 0);
				free(current->option);
				current->option = NULL;
				current->option_len = 0;
			}
			free(current);
			ret = 1;
			current = nextitm;
			continue;
		}
		olditm = current;
		current = current->next;
	}
	goto finish;

finish:
	if (mtx != NULL) AST(mutex_unlock(mtx) == 1);
	return ret;
}



#define HASH_INIT(hash_base) \
	hash_base.hash_field = (GL_Object_Hash **)calloc(1, sizeof(GL_Object_Hash *) * GL_OBJECT_HASH_BASE); \
	hash_base.hash_size = GL_OBJECT_HASH_BASE;

void
fastpath_ostate_init(GL_Object_State *ostate)
{
	HASH_INIT(ostate->query);
	HASH_INIT(ostate->framebuffer);
	HASH_INIT(ostate->vertexarray);
	HASH_INIT(ostate->transformfeedback);

	HASH_INIT(ostate->query_real);
	HASH_INIT(ostate->framebuffer_real);
	HASH_INIT(ostate->vertexarray_real);
	HASH_INIT(ostate->transformfeedback_real);
}

void
fastpath_sostate_init(GL_Shared_Object_State *sostate)
{
	mutex_init(&sostate->access_mutex);

	HASH_INIT(sostate->texture);
	HASH_INIT(sostate->buffer);
	HASH_INIT(sostate->renderbuffer);
	HASH_INIT(sostate->program);
	HASH_INIT(sostate->sampler);
	HASH_INIT(sostate->programpipeline);

	HASH_INIT(sostate->texture_real);
	HASH_INIT(sostate->buffer_real);
	HASH_INIT(sostate->renderbuffer_real);
	HASH_INIT(sostate->program_real);
	HASH_INIT(sostate->sampler_real);
	HASH_INIT(sostate->programpipeline_real);
}

#undef HASH_INIT


static void
_add_hash(GL_Object_Hash_Base *hash_base, GL_Object_Hash *data)
{
	int array_idx = data->hash_key & (hash_base->hash_size - 1);
	if (hash_base->hash_field[array_idx] == NULL) {
		hash_base->hash_field[array_idx] = data;
	} else {
		GL_Object_Hash *current = hash_base->hash_field[array_idx];
		while (current->next) {
			AST(current->hash_key != data->hash_key);
			current = current->next;
		}
		current->next = data;
	}
	data->next = NULL;
	hash_base->item_size++;
}

static int
_remove_hash(GL_Object_Hash_Base *hash_base, GLuint hash)
{
	int ret = 0;
	int array_idx = hash & (hash_base->hash_size - 1);

	GL_Object_Hash *current = hash_base->hash_field[array_idx];
	GL_Object_Hash *prev = NULL;

	while (current) {
		if (current->hash_key == hash) {
			if (prev != NULL)
				prev->next = current->next;
			else
				hash_base->hash_field[array_idx] = current->next;
			hash_base->item_size--;
			ret = 1;
			break;
		}
		prev = current;
		current = current->next;
	}

	return ret;
}

static void
_free_hash_list(GL_Object_Hash_Base *hash_base, int free_data)
{
	if (hash_base->item_size == 0) return;

	for (int i = 0; i < hash_base->hash_size; i++) {
		if (hash_base->hash_field[i] != NULL) {
			GL_Object_Hash *current = hash_base->hash_field[i];

			while (current != NULL) {
				GL_Object_Hash *current_next = current->next;

				if (free_data == 1 && current->item != NULL) {
					free(current->item);
				}

				free(current);
				hash_base->item_size--;
				current = current_next;
			}
		}
	}
}



#define HASH_DEINIT(hash_base, free_data) \
	_free_hash_list(&hash_base, free_data); \
	free(hash_base.hash_field); \
	hash_base.hash_size = 0;

void
fastpath_ostate_deinit(GL_Object_State *ostate)
{
	HASH_DEINIT(ostate->query, 1);
	HASH_DEINIT(ostate->framebuffer, 1);
	HASH_DEINIT(ostate->vertexarray, 1);
	HASH_DEINIT(ostate->transformfeedback, 1);

	HASH_DEINIT(ostate->query_real, 0);
	HASH_DEINIT(ostate->framebuffer_real, 0);
	HASH_DEINIT(ostate->vertexarray_real, 0);
	HASH_DEINIT(ostate->transformfeedback_real, 0);
}

void
fastpath_sostate_deinit(GL_Shared_Object_State *sostate)
{
	HASH_DEINIT(sostate->texture, 1);
	HASH_DEINIT(sostate->buffer, 1);
	HASH_DEINIT(sostate->renderbuffer, 1);
	HASH_DEINIT(sostate->program, 1);
	HASH_DEINIT(sostate->sampler, 1);
	HASH_DEINIT(sostate->programpipeline, 1);

	HASH_DEINIT(sostate->texture_real, 0);
	HASH_DEINIT(sostate->buffer_real, 0);
	HASH_DEINIT(sostate->renderbuffer_real, 0);
	HASH_DEINIT(sostate->program_real, 0);
	HASH_DEINIT(sostate->sampler_real, 0);

	HASH_DEINIT(sostate->programpipeline_real, 0);
}

#undef HASH_DEINIT



#define FIND_HASH(hash_base, key, ret) \
{ \
	GL_Object_Hash *fh_current = hash_base->hash_field[(key) & (hash_base->hash_size - 1)]; \
	while(fh_current) \
	{ \
		if (fh_current->hash_key == (key)) \
		{ \
			ret = fh_current; \
			break; \
		} \
		fh_current = fh_current->next; \
	} \
}

void
_ostate_hash_check(GL_Object_Hash_Base *hash_base)
{
	if (hash_base->item_size + 1 < hash_base->hash_size)
		return;

	int oldsize = hash_base->hash_size;
	GL_Object_Hash **oldfield = hash_base->hash_field;

	hash_base->hash_size = oldsize << 1;
	hash_base->hash_field = (GL_Object_Hash **)calloc(1,
							sizeof(GL_Object_Hash *) * hash_base->hash_size);
	AST(hash_base->hash_field != NULL);

	for (int i = 0; i < oldsize; i++) {
		if (oldfield[i] != NULL) {
			GL_Object_Hash *current = oldfield[i];

			while (current != NULL) {
				GL_Object_Hash *current_next = current->next;
				_add_hash(hash_base, current);
				hash_base->item_size--;
				current = current_next;
			}
		}
	}
	free(oldfield);

}

GLuint
fastpath_ostate_create_object(GL_Object_State *ostate, GL_Object_Type type,
							  GLuint real_name)
{
	GLuint ret = _COREGL_INT_INIT_VALUE;

	GL_Object_Hash_Base *hash_base = NULL;
	GL_Object_Hash_Base *hash_base_real = NULL;
	int newid = _COREGL_INT_INIT_VALUE;

	hash_base = _lock_gl_object_hash(ostate, type);
	hash_base_real = _lock_gl_object_hash_real(ostate, type);

	newid = hash_base->last_id + 1;
	if (newid >= hash_base->hash_size) {
		hash_base->is_looped = 1;
		newid = 1;
		hash_base->last_id = 1;
	}

	if (hash_base->is_looped != 0) {
		int i;
		int findingid = newid;
		newid = -1;
		for (i = 0; i < hash_base->hash_size; i++) {
			GL_Object_Hash *exist_hash = NULL;
			FIND_HASH(hash_base, findingid, exist_hash);
			if (exist_hash == NULL) {
				newid = findingid;
				break;
			}
			findingid++;
			if (findingid >= hash_base->hash_size) findingid = 1;
		}
		AST(newid != -1);
	}
	hash_base->last_id = newid;

	{
		GL_Object *newobj = (GL_Object *)calloc(1, sizeof(GL_Object));
		if (newobj == NULL)
			goto finish;
		newobj->id = (int)type + newid;
		newobj->real_id = real_name;
		newobj->ref_count = 1;


		GL_Object_Hash *newobj_hash = (GL_Object_Hash *)calloc(1,
									  sizeof(GL_Object_Hash));
		if (newobj_hash == NULL) {
			free(newobj);
			goto finish;
		}
		newobj_hash->item = newobj;
		newobj_hash->hash_key = newid;
		_add_hash(hash_base, newobj_hash);

		GL_Object_Hash *newobj_hash_real = (GL_Object_Hash *)calloc(1,
										   sizeof(GL_Object_Hash));
		if (newobj_hash_real == NULL) {
			free(newobj);
			free(newobj_hash);
			goto finish;
		}
		newobj_hash_real->item = newobj;
		newobj_hash_real->hash_key = real_name;
		_add_hash(hash_base_real, newobj_hash_real);

		ret = newobj->id;
	}

	_ostate_hash_check(hash_base);
	_ostate_hash_check(hash_base_real);

	goto finish;

finish:
	_unlock_gl_object_hash(ostate, type);
	_unlock_gl_object_hash_real(ostate, type);
	return ret;
}

#define FIND_OBJ_FROM_HASH_WITH_VERIFY(hash_base, hash, object) \
	if (((int)(hash)) < 0) { ret = 0; goto finish; } \
	{ \
		GL_Object_Hash *object_hash = NULL; \
		FIND_HASH((hash_base), (int)(hash), object_hash); \
		if (object_hash == NULL) { ret = 0; goto finish; } \
		(object) = object_hash->item; \
		if ((object) == NULL) { ret = 0; goto finish; } \
	}

GLuint
fastpath_ostate_remove_object(GL_Object_State *ostate, GL_Object_Type type,
							  GLuint glue_name)
{
	GLuint ret = _COREGL_INT_INIT_VALUE;

	GL_Object_Hash_Base *hash_base = NULL;
	GL_Object_Hash_Base *hash_base_real = NULL;
	GL_Object *object = NULL;

	hash_base = _lock_gl_object_hash(ostate, type);
	hash_base_real = _lock_gl_object_hash_real(ostate, type);

	FIND_OBJ_FROM_HASH_WITH_VERIFY(hash_base, glue_name - (int)type, object);

	object->ref_count--;

	if (object->ref_count <= 0) {
		GL_Object_Hash *object_hash = NULL;

		FIND_HASH(hash_base, object->id - (int)type, object_hash);
		AST(object_hash != NULL);
		_remove_hash(hash_base, object->id - (int)type);
		free(object_hash);
		object_hash = NULL;

		FIND_HASH(hash_base_real, object->real_id, object_hash);
		AST(object_hash != NULL);
		_remove_hash(hash_base_real, object->real_id);
		free(object_hash);
		object_hash = NULL;

		free(object);
		object = NULL;
	}

	ret = 1;
	goto finish;

finish:
	_unlock_gl_object_hash(ostate, type);
	_unlock_gl_object_hash_real(ostate, type);
	return ret;
}

GLuint
fastpath_ostate_get_object(GL_Object_State *ostate, GL_Object_Type type,
						   GLuint glue_name)
{
	GLuint ret = _COREGL_INT_INIT_VALUE;

	GL_Object_Hash_Base *hash_base = NULL;
	GL_Object *object = NULL;

	hash_base = _lock_gl_object_hash(ostate, type);

	FIND_OBJ_FROM_HASH_WITH_VERIFY(hash_base, glue_name - (int)type, object);

	ret = object->real_id;
	goto finish;

finish:
	_unlock_gl_object_hash(ostate, type);
	return ret;
}

/* Check if the context's state contains object of a given type */
GLuint
fastpath_ostate_has_object_type(GL_Object_State *ostate, GL_Object_Type type)
{
	GLuint ret = _COREGL_INT_INIT_VALUE;

	GL_Object_Hash_Base *hash_base = NULL;
	GL_Object *object = NULL;
	hash_base = _lock_gl_object_hash(ostate, type);

	if (hash_base->hash_field == 0) {
		ret = 0;
		goto finish;
	}

	FIND_OBJ_FROM_HASH_WITH_VERIFY(hash_base, 1, object);

	ret = object->real_id;
	goto finish;

finish:
	_unlock_gl_object_hash(ostate, type);
	return ret;
}


GLint
fastpath_ostate_set_object_tag(GL_Object_State *ostate, GL_Object_Type type,
							   GLuint glue_name, GLvoid *tag)
{
	GLint ret = _COREGL_INT_INIT_VALUE;

	GL_Object_Hash_Base *hash_base = NULL;
	GL_Object *object = NULL;
	int hash = _COREGL_INT_INIT_VALUE;

	hash_base = _lock_gl_object_hash(ostate, type);

	hash = glue_name - (int)type;

	FIND_OBJ_FROM_HASH_WITH_VERIFY(hash_base, hash, object);

	AST(object->tag == NULL);
	object->tag = tag;
	ret = 1;
	goto finish;

finish:
	_unlock_gl_object_hash(ostate, type);
	return ret;
}

GLvoid *
fastpath_ostate_get_object_tag(GL_Object_State *ostate, GL_Object_Type type,
							   GLuint glue_name)
{
	GLvoid *ret = NULL;

	GL_Object_Hash_Base *hash_base = NULL;
	GL_Object *object = NULL;

	hash_base = _lock_gl_object_hash(ostate, type);

	FIND_OBJ_FROM_HASH_WITH_VERIFY(hash_base, glue_name - (int)type, object);

	ret = object->tag;
	goto finish;

finish:
	_unlock_gl_object_hash(ostate, type);
	return ret;
}

GLuint
fastpath_ostate_find_object(GL_Object_State *ostate, GL_Object_Type type,
							GLuint real_name)
{
	GLuint ret = _COREGL_INT_INIT_VALUE;

	GL_Object_Hash_Base *hash_base_real = NULL;
	GL_Object *object = NULL;

	hash_base_real = _lock_gl_object_hash_real(ostate, type);

	FIND_OBJ_FROM_HASH_WITH_VERIFY(hash_base_real, real_name, object);

	ret = object->id;
	goto finish;

finish:
	_unlock_gl_object_hash_real(ostate, type);
	return ret;
}

GLint
fastpath_ostate_use_object(GL_Object_State *ostate, GL_Object_Type type,
						   GLuint glue_name)
{
	GLint ret = _COREGL_INT_INIT_VALUE;

	GL_Object_Hash_Base *hash_base = NULL;
	GL_Object *object = NULL;

	hash_base = _lock_gl_object_hash(ostate, type);

	FIND_OBJ_FROM_HASH_WITH_VERIFY(hash_base, glue_name - (int)type, object);

	object->ref_count++;
	ret = 1;
	goto finish;

finish:
	_unlock_gl_object_hash(ostate, type);
	return ret;
}

void
fastpath_dump_context_states(GLGlueContext *ctx, int force_output)
{
	static struct timeval tv_last = { 0, 0 };

	if (unlikely(trace_state_flag != 1)) return;

	if (!force_output) {
		struct timeval tv_now = { 0, 0 };
		AST(gettimeofday(&tv_now, NULL) == 0);
		if (tv_now.tv_sec - tv_last.tv_sec < _COREGL_TRACE_OUTPUT_INTERVAL_SEC) {
			goto finish;
		}
		tv_last = tv_now;
	}

	TRACE("\n");
	TRACE("\E[0;40;34m========================================================================================================================\E[0m\n");
	TRACE("\E[40;32;1m  State info \E[1;37;1m: <PID = %d> GlueCTX = %p\E[0m\n",
		  getpid(), ctx);
	TRACE("\E[0;40;34m========================================================================================================================\E[0m\n");

#define PRINTF_CHAR_GLenum "0x%8X"
#define PRINTF_CHAR_GLboolean "%10d"
#define PRINTF_CHAR_GLint "%10d"
#define PRINTF_CHAR_GLsizei "%10u"
#define PRINTF_CHAR_GLuint "%10u"
#define PRINTF_CHAR_GLuintmask "0x%8X"
#define PRINTF_CHAR_GLintptr "%10ld"
#define PRINTF_CHAR_GLsizeiptr "%10ld"

#define PRINTF_CHAR_GLclampf "%10.6f"
#define PRINTF_CHAR_GLfloat "%10.6f"

#define PRINTF_CHAR_GLvoidptr "%10p"

#define PRINTF_CHAR(type) PRINTF_CHAR_##type

#define _COREGL_START_API(version) api_gl_version = version;
#define _COREGL_END_API(version) api_gl_version = COREGL_GLAPI_2;
#define INITIAL_CTX initial_ctx
#define GLUE_STATE(TYPE, NAME, SIZE, ARRAY_SIZE, DEFAULT_STMT, GET_STMT)  \
   { \
      TYPE valuedata[SIZE]; \
      TYPE *value = NULL; \
      if(api_gl_version <= driver_gl_version) { \
         value = valuedata; GET_STMT; value = valuedata; \
         _sym_glGetError();\
         TRACE("\E[40;37;1m %-30.30s : (\E[0m ", #NAME); \
         for (int i = 0; i < SIZE; i++) \
         { \
            if (i > 0) { \
               if (i % 4 == 0) \
                  TRACE("\n %-30.30s     ", "");\
               else \
                  TRACE(", "); \
            } \
            if (ctx->NAME[i] != value[i]) { TRACE("\E[40;31;1m"); } \
               TRACE(PRINTF_CHAR(TYPE), ctx->NAME[i]); \
               TRACE("["PRINTF_CHAR(TYPE)"]", value[i]); \
            if (ctx->NAME[i] != value[i]) { TRACE("\E[0m"); } \
         } \
         TRACE(" \E[40;37;1m)\E[0m\n"); \
      } \
   }
# include "coregl_fastpath_state.h"
#undef GLUE_STATE
#undef INITIAL_CTX
#undef _COREGL_START_API
#undef _COREGL_END_API

	TRACE("\E[0;40;34m========================================================================================================================\E[0m\n");
	TRACE("\n");

	TRACE_END();

finish:
	return;
}

int
fastpath_init_context_states(GLGlueContext *ctx)
{
	int ret = 0;

	AST(mutex_lock(&init_context_mutex) == 1);

	if (ctx == NULL) {
		COREGL_ERR("Context NULL");
		ret = 0;
		goto finish;
	}

	AST(ctx->initialized == 0);
	AST(ctx->ostate.shared != NULL);

	if (initial_ctx == NULL) {
		initial_ctx = (GLGlueContext *)calloc(1, sizeof(GLGlueContext));
		AST(initial_ctx != NULL);

//#define FORCE_DEFAULT_VALUE
#define _COREGL_START_API(version) api_gl_version = version;
#define _COREGL_END_API(version) api_gl_version = COREGL_GLAPI_2;
#ifdef FORCE_DEFAULT_VALUE
# define INITIAL_CTX initial_ctx
# define GLUE_STATE(TYPE, NAME, SIZE, ARRAY_SIZE, DEFAULT_STMT, GET_STMT)  \
      { \
         if(SIZE > 0) { \
             int i; \
             TYPE valuedata[SIZE]; \
             TYPE *value = NULL; \
             memset(valuedata, 0xcc, sizeof(TYPE) * SIZE); \
             initial_ctx->NAME = (TYPE *)calloc(SIZE, sizeof(TYPE));\
             initial_ctx->NAME##_updated = (GLboolean *)calloc(SIZE, sizeof(GLboolean)); \
             if(api_gl_version <= driver_gl_version) { \
                 value = valuedata; DEFAULT_STMT; value = valuedata; \
                 for (i = 0; i < SIZE; i++) \
                 { \
                    if (*((char *)(&value[i])) == 0xcc) \
                    { \
                        memset(&value[i], 0xaa, sizeof(TYPE)); \
                        value = valuedata; DEFAULT_STMT; value = valuedata; \
                        if (*((char *)(&value[i])) == 0xaa) \
                        { \
                            COREGL_WARN("GL-state '"#NAME"' cannot be retrieved"); \
                            break; \
                        } \
                    } \
                    initial_ctx->NAME[i] = value[i]; \
                    initial_ctx->NAME##_updated[i] = GL_FALSE; \
                 } \
             } \
        }\
      }
#  include "coregl_fastpath_state.h"
# undef GLUE_STATE
# undef INITIAL_CTX
#else
# define INITIAL_CTX initial_ctx
# define SET_GLUE_VALUE(DEFAULT_STMT, FALLBACK_STMT) \
      if (try_step == 1) \
      { \
         value = valuedata; DEFAULT_STMT; value = valuedata; \
      } \
      else \
      { \
         value = valuedata; FALLBACK_STMT; value = valuedata; \
      }

# define GLUE_STATE(TYPE, NAME, SIZE, ARRAY_SIZE, DEFAULT_STMT, GET_STMT)  \
      { \
         if (SIZE > 0) { \
             int i; \
             int try_step = 0;\
             TYPE valuedata[SIZE]; \
             TYPE *value = NULL; \
             _sym_glGetError(); \
             memset(valuedata, 0xcc, sizeof(TYPE) * SIZE); \
             initial_ctx->NAME = (TYPE *)calloc(SIZE, sizeof(TYPE));\
             initial_ctx->NAME##_updated = (GLboolean *)calloc(SIZE, sizeof(GLboolean)); \
             if(api_gl_version <= driver_gl_version) { \
                do { \
                   try_step++; \
                   SET_GLUE_VALUE(GET_STMT, DEFAULT_STMT); \
                   if (_sym_glGetError() == GL_INVALID_ENUM) \
                   { \
                      initial_ctx->NAME##_used = 0; \
                      value = valuedata; DEFAULT_STMT; value = valuedata; \
                      break; \
                   } \
                   initial_ctx->NAME##_used = 1; \
                   for (i = 0; i < SIZE; i++) \
                   { \
                      if (*((char *)(&value[i])) == 0xcc) \
                      { \
                         memset(&value[i], 0xaa, sizeof(TYPE)); \
                         SET_GLUE_VALUE(GET_STMT, DEFAULT_STMT); \
                         if (*((char *)(&value[i])) == 0xaa) \
                         { \
                            try_step++; \
                            if (try_step == 2) \
                            { \
                               COREGL_WARN("GL-state '"#NAME"' cannot be retrieved"); \
                            } \
                            break; \
                         } \
                      } \
                      initial_ctx->NAME[i] = value[i]; \
                      initial_ctx->NAME##_updated[i] = GL_FALSE; \
                   } \
                   if (try_step != 2) \
                   { \
                      value = valuedata; DEFAULT_STMT; value = valuedata; \
                      for (i = 0; i < SIZE; i++) \
                      { \
                         if (initial_ctx->NAME[i] != value[i]) \
                         { \
                            COREGL_WARN("GL-state '"#NAME"'[%d] value ["PRINTF_CHAR(TYPE)"] is different from SPEC-DEFAULT ["PRINTF_CHAR(TYPE)"]", i, initial_ctx->NAME[i], value[i]); \
                         } \
                      } \
                   } \
                } \
                while (try_step == 2); \
             }\
        }\
      }
#  include "coregl_fastpath_state.h"
# undef SET_GLUE_VALUE
# undef GLUE_STATE
# undef INITIAL_CTX
#endif
# undef _COREGL_END_API
# undef _COREGL_START_API

		if (initial_ctx->gl_vertex_attribs_num[0] > MAX_VERTEX_ATTRIBS) {
			COREGL_WARN("Number of vertex attrib is too big! (%d-%d)",
						MAX_VERTEX_ATTRIBS, initial_ctx->gl_vertex_attribs_num[0]);
		}
		if (initial_ctx->gl_tex_units_num[0] > MAX_TEXTURE_UNITS) {
			COREGL_WARN("Number of texture unit is too big! (%d-%d)",
						MAX_TEXTURE_UNITS, initial_ctx->gl_tex_units_num[0]);
		}
		if (initial_ctx->gl_transform_feedback_buffer_binding_num[0] >
				MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS) {
			COREGL_WARN("Number of transform feedback separate attrib is too big! (%d-%d)",
						MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS,
						initial_ctx->gl_transform_feedback_buffer_binding_num[0]);
		}
		if (initial_ctx->gl_uniform_buffer_binding_num[0] >
				MAX_UNIFORM_BUFFER_BINDINGS) {
			COREGL_WARN("Number of uniform buffer binding is too big! (%d-%d)",
						MAX_UNIFORM_BUFFER_BINDINGS, initial_ctx->gl_uniform_buffer_binding_num[0]);
		}
		if (initial_ctx->gl_shader_storage_buffer_binding_num[0] >
				MAX_SHADER_STORAGE_BUFFER_BINDINGS) {
			COREGL_WARN("Number of shader storage buffer binding is too big! (%d-%d)",
						MAX_SHADER_STORAGE_BUFFER_BINDINGS,
						initial_ctx->gl_shader_storage_buffer_binding_num[0]);
		}
		if (initial_ctx->gl_atomic_counter_buffer_binding_num[0] >
				MAX_ATOMIC_COUNTER_BUFFER_BINDING) {
			COREGL_WARN("Number of uniform buffer binding is too big! (%d-%d)",
						MAX_ATOMIC_COUNTER_BUFFER_BINDING,
						initial_ctx->gl_atomic_counter_buffer_binding_num[0]);
		}
	}

	{
		int i;
#define _COREGL_START_API(version) api_gl_version = version;
#define _COREGL_END_API(version) api_gl_version = COREGL_GLAPI_2;
#define INITIAL_CTX initial_ctx
#define GLUE_STATE(TYPE, NAME, SIZE, ARRAY_SIZE, DEFAULT_STMT, GET_STMT)  \
      { \
        if(SIZE > 0) { \
            ctx->NAME = (TYPE *)calloc(SIZE, sizeof(TYPE)); \
            ctx->NAME##_updated = (GLboolean *)calloc(SIZE, sizeof(GLboolean)); \
            if(api_gl_version <= driver_gl_version) { \
                for (i = 0; i < SIZE; i++) \
                { \
                   ctx->NAME[i] = initial_ctx->NAME[i]; \
                   ctx->NAME##_used = initial_ctx->NAME##_used; \
                   ctx->NAME##_updated[i] = initial_ctx->NAME##_updated[i]; \
                }\
            } \
        } \
      }
# include "coregl_fastpath_state.h"
#undef GLUE_STATE
#undef INITIAL_CTX
#undef _COREGL_START_API
#undef _COREGL_END_API
	}

	ctx->initialized = 1;
	ret = 1;
	goto finish;

finish:
	AST(mutex_unlock(&init_context_mutex) == 1);

	return ret;
}

#ifdef COREGL_USE_MODULE_TRACEPATH
extern void *tracepath_api_trace_begin(const char *name, void *hint,
									   int trace_total_time);
extern void *tracepath_api_trace_end(const char *name, void *hint,
									 int trace_total_time);
#endif

#define CHECK_GL_ERROR(func) \
	{ \
		func; \
		int err = _orig_fastpath_glGetError(); \
		if (err != GL_NO_ERROR) \
		{ \
			COREGL_ERR("(GL %p) : %s returns GL error 0x%X", oldctx->cstate, #func, err); \
			goto finish; \
		} \
	}

int
fastpath_make_context_current(GLGlueContext *oldctx, GLGlueContext *newctx)
{
	int ret = 0;
	unsigned char flag = 0;
	int i = 0;

	if (debug_nofp == 1) {
		ret = 1;
		goto finish;
	}

	// Return if they're the same
	if (oldctx == newctx) {
		ret = 1;
		goto finish;
	}

#define STATE_COMPARE(state) \
   ((oldctx->state) != (newctx->state))

#define STATES_COMPARE(state_ptr, bytes) \
   ((memcmp((oldctx->state_ptr), (newctx->state_ptr), (bytes))) != 0)


#ifdef COREGL_USE_MODULE_TRACEPATH
	static void *trace_hint_glfinish = NULL;
	trace_hint_glfinish = tracepath_api_trace_begin("eglMakeCurrent(FP glFinish)",
						  trace_hint_glfinish, 0);
#endif // COREGL_USE_MODULE_TRACEPATH

	{
		int err = _orig_fastpath_glGetError();
		if (err != GL_NO_ERROR && oldctx->gl_error == GL_NO_ERROR)
			oldctx->gl_error = err;
	}

	CHECK_GL_ERROR(_orig_fastpath_glFlush())

#ifdef COREGL_USE_MODULE_TRACEPATH
	tracepath_api_trace_end("eglMakeCurrent(FP glFinish)", trace_hint_glfinish, 0);
#endif // COREGL_USE_MODULE_TRACEPATH

	// _varray_flag
#ifdef COREGL_USE_MODULE_TRACEPATH
	static void *trace_hint_vertex_attrib = NULL;
	trace_hint_vertex_attrib =
		tracepath_api_trace_begin("eglMakeCurrent(FP vertex attrib)",
								  trace_hint_vertex_attrib, 0);
#endif // COREGL_USE_MODULE_TRACEPATH

	flag = oldctx->_vattrib_flag | newctx->_vattrib_flag;
	if (flag) {
		for (i = 0; i < oldctx->gl_vertex_attribs_num[0]; i++) {
			if (newctx->gl_vertex_array_buf_id[i] != oldctx->gl_vertex_array_buf_id[i]) {
				CHECK_GL_ERROR(_orig_fastpath_glBindBuffer(GL_ARRAY_BUFFER,
							   newctx->gl_vertex_array_buf_id[i]))
			} else {
				CHECK_GL_ERROR(_orig_fastpath_glBindBuffer(GL_ARRAY_BUFFER, 0))
			}

			if STATE_COMPARE(gl_vertex_array_divisor[i]) {
				CHECK_GL_ERROR(_orig_fastpath_glVertexAttribDivisor(i,
							   newctx->gl_vertex_array_divisor[i]))
			}

			if (newctx->gl_vertex_array_size[i] != 0) {
				CHECK_GL_ERROR(_orig_fastpath_glVertexAttribPointer(i,
							   newctx->gl_vertex_array_size[i],
							   newctx->gl_vertex_array_type[i],
							   newctx->gl_vertex_array_normalized[i],
							   newctx->gl_vertex_array_stride[i],
							   newctx->gl_vertex_array_pointer[i]))
			} else {
				if (newctx->gl_vertex_array_integer[0] == GL_TRUE) {
					if (newctx->gl_vertex_array_type[0] == GL_UNSIGNED_INT) {
						CHECK_GL_ERROR(_orig_fastpath_glVertexAttribI4uiv(i,
									   &newctx->gl_vertex_attrib_value_unsigned_integer[4 * i]))
					} else {
						CHECK_GL_ERROR(_orig_fastpath_glVertexAttribI4iv(i,
									   &newctx->gl_vertex_attrib_value_integer[4 * i]))
					}
				} else {
					CHECK_GL_ERROR(_orig_fastpath_glVertexAttrib4fv(i,
								   &newctx->gl_vertex_attrib_value[4 * i]))
				}
			}

			if (newctx->gl_vertex_array_enabled[i] == GL_TRUE) {
				CHECK_GL_ERROR(_orig_fastpath_glEnableVertexAttribArray(i))
			} else {
				CHECK_GL_ERROR(_orig_fastpath_glDisableVertexAttribArray(i))
			}
		}

	}

#ifdef COREGL_USE_MODULE_TRACEPATH
	tracepath_api_trace_end("eglMakeCurrent(FP vertex attrib)",
							trace_hint_vertex_attrib, 0);
#endif // COREGL_USE_MODULE_TRACEPATH


#ifdef COREGL_USE_MODULE_TRACEPATH
	static void *trace_hint_bindbuffers = NULL;
	trace_hint_bindbuffers =
		tracepath_api_trace_begin("eglMakeCurrent(FP bind buffers)",
								  trace_hint_bindbuffers, 0);
#endif // COREGL_USE_MODULE_TRACEPATH

	//------------------//
	// _bind_flag1
	flag = oldctx->_bind_flag1 | newctx->_bind_flag1;
	if (flag) {
		if STATE_COMPARE(gl_array_buffer_binding[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glBindBuffer(GL_ARRAY_BUFFER,
						   newctx->gl_array_buffer_binding[0]))
		}
		if STATE_COMPARE(gl_element_array_buffer_binding[0]) {
			if STATE_COMPARE(gl_vertex_array_binding[0]) {
				CHECK_GL_ERROR(_orig_fastpath_glBindVertexArray(
								   newctx->gl_vertex_array_binding[0]))
			}
			CHECK_GL_ERROR(_orig_fastpath_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
						   newctx->gl_element_array_buffer_binding[0]))
		}

		if (newctx->gl_framebuffer_binding_read_used == 1 &&
				newctx->gl_framebuffer_binding_draw_used == 1) {
			if STATE_COMPARE(gl_framebuffer_binding_read[0]) {
				if (driver_gl_version >= 2)
					CHECK_GL_ERROR(_orig_fastpath_glBindFramebuffer(GL_READ_FRAMEBUFFER,
								   newctx->gl_framebuffer_binding_read[0]))
					else
						CHECK_GL_ERROR(_orig_fastpath_glBindFramebufferOES(GL_READ_FRAMEBUFFER,
									   newctx->gl_framebuffer_binding_read[0]))
					}
			if STATE_COMPARE(gl_framebuffer_binding_draw[0]) {
				if (driver_gl_version >= 2)
					CHECK_GL_ERROR(_orig_fastpath_glBindFramebuffer(GL_DRAW_FRAMEBUFFER,
								   newctx->gl_framebuffer_binding_draw[0]))
					else
						CHECK_GL_ERROR(_orig_fastpath_glBindFramebufferOES(GL_DRAW_FRAMEBUFFER,
									   newctx->gl_framebuffer_binding_draw[0]))
					}
		} else {
			if STATE_COMPARE(gl_framebuffer_binding[0]) {
				if (driver_gl_version >= 2)
					CHECK_GL_ERROR(_orig_fastpath_glBindFramebuffer(GL_FRAMEBUFFER,
								   newctx->gl_framebuffer_binding[0]))
					else
						CHECK_GL_ERROR(_orig_fastpath_glBindFramebufferOES(GL_FRAMEBUFFER,
									   newctx->gl_framebuffer_binding[0]))
					}
		}
		if STATE_COMPARE(gl_renderbuffer_binding[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glBindRenderbuffer(GL_RENDERBUFFER,
						   newctx->gl_renderbuffer_binding[0]))
		}
	}

	//------------------//
	// _bind_flag2
	flag = oldctx->_bind_flag2 | newctx->_bind_flag2;
	if (flag) {
		if STATE_COMPARE(gl_copy_read_buffer_binding[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glBindBuffer(GL_COPY_READ_BUFFER,
						   newctx->gl_copy_read_buffer_binding[0]))
		}
		if STATE_COMPARE(gl_copy_write_buffer_binding[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glBindBuffer(GL_COPY_WRITE_BUFFER,
						   newctx->gl_copy_write_buffer_binding[0]))
		}
		if STATE_COMPARE(gl_pixel_pack_buffer_binding[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glBindBuffer(GL_PIXEL_PACK_BUFFER,
						   newctx->gl_pixel_pack_buffer_binding[0]))
		}
		if STATE_COMPARE(gl_pixel_unpack_buffer_binding[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glBindBuffer(GL_PIXEL_UNPACK_BUFFER,
						   newctx->gl_pixel_unpack_buffer_binding[0]))
		}
		if STATE_COMPARE(gl_transform_feedback_buffer_binding[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER,
						   newctx->gl_transform_feedback_buffer_binding[0]))
		}
		if STATE_COMPARE(gl_uniform_buffer_binding[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glBindBuffer(GL_UNIFORM_BUFFER,
						   newctx->gl_uniform_buffer_binding[0]))
		}
		/* handle states set in fastpath_glBindBufferBase() */
		for (i = 0; i < newctx->gl_transform_feedback_buffer_binding_num[0]; i++) {
			if STATE_COMPARE(gl_transform_feedback_buffer_binding_array[i]) {
				CHECK_GL_ERROR(_orig_fastpath_glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, i,
							   newctx->gl_transform_feedback_buffer_binding_array[i]))
			}
		}
		for (i = 0; i < newctx->gl_uniform_buffer_binding_num[0]; i++) {
			if STATE_COMPARE(gl_uniform_buffer_binding_array[i]) {
				CHECK_GL_ERROR(_orig_fastpath_glBindBufferBase(GL_UNIFORM_BUFFER, i,
							   newctx->gl_uniform_buffer_binding_array[i]))
			}
		}
	}

	flag = oldctx->_bind_flag3 | newctx->_bind_flag3;
	if (flag) {
		if STATE_COMPARE(gl_shader_storage_buffer_binding[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glBindBuffer(GL_SHADER_STORAGE_BUFFER,
						   newctx->gl_shader_storage_buffer_binding[0]))
		}
		if STATE_COMPARE(gl_atomic_counter_buffer_binding[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glBindBuffer(GL_ATOMIC_COUNTER_BUFFER,
						   newctx->gl_atomic_counter_buffer_binding[0]))
		}
		if STATE_COMPARE(gl_draw_indirect_buffer_binding[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glBindBuffer(GL_DRAW_INDIRECT_BUFFER,
						   newctx->gl_draw_indirect_buffer_binding[0]))
		}
		if STATE_COMPARE(gl_dispatch_indirect_buffer_binding[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER,
						   newctx->gl_dispatch_indirect_buffer_binding[0]))
		}
		if STATE_COMPARE(gl_texture_buffer_binding[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glBindBuffer(GL_TEXTURE_BUFFER,
						   newctx->gl_texture_buffer_binding[0]))
		}
		/* handle states set in fastpath_glBindBufferBase() */
		for (i = 0; i < newctx->gl_shader_storage_buffer_binding_num[0]; i++) {
			if STATE_COMPARE(gl_shader_storage_buffer_binding_array[i]) {
				CHECK_GL_ERROR(_orig_fastpath_glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i,
							   newctx->gl_shader_storage_buffer_binding_array[i]))
			}
		}
		for (i = 0; i < newctx->gl_atomic_counter_buffer_binding_num[0]; i++) {
			if STATE_COMPARE(gl_atomic_counter_buffer_binding_array[i]) {
				CHECK_GL_ERROR(_orig_fastpath_glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, i,
							   newctx->gl_atomic_counter_buffer_binding_array[i]))
			}
		}
	}

	flag = oldctx->_bind_flag4 | newctx->_bind_flag4;
	if (flag) {
		/* handle states set in fastpath_BindBufferRrange() */
		for (i = 0; i < newctx->gl_transform_feedback_buffer_binding_num[0]; i++) {
			if (STATE_COMPARE(gl_transform_feedback_buffer_range_binding_array[i]) ||
					STATE_COMPARE(gl_transform_feedback_buffer_range_binding_array_offset[i]) ||
					STATE_COMPARE(gl_transform_feedback_buffer_range_binding_array_size[i])) {
				CHECK_GL_ERROR(_orig_fastpath_glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, i,
							   newctx->gl_transform_feedback_buffer_range_binding_array[i],
							   newctx->gl_transform_feedback_buffer_range_binding_array_offset[i],
							   newctx->gl_transform_feedback_buffer_range_binding_array_size[i]))
			}
		}
		for (i = 0; i < newctx->gl_uniform_buffer_binding_num[0]; i++) {
			if (STATE_COMPARE(gl_uniform_buffer_range_binding_array[i]) ||
					STATE_COMPARE(gl_uniform_buffer_range_binding_array_offset[i]) ||
					STATE_COMPARE(gl_uniform_buffer_range_binding_array_size[i])) {
				CHECK_GL_ERROR(_orig_fastpath_glBindBufferRange(GL_UNIFORM_BUFFER, i,
							   newctx->gl_uniform_buffer_range_binding_array[i],
							   newctx->gl_uniform_buffer_range_binding_array_offset[i],
							   newctx->gl_uniform_buffer_range_binding_array_size[i]))
			}
		}
		for (i = 0; i < newctx->gl_shader_storage_buffer_binding_num[0]; i++) {
			if (STATE_COMPARE(gl_shader_storage_buffer_range_binding_array[i]) ||
					STATE_COMPARE(gl_shader_storage_buffer_range_binding_array_offset[i]) ||
					STATE_COMPARE(gl_shader_storage_buffer_range_binding_array_size[i])) {
				CHECK_GL_ERROR(_orig_fastpath_glBindBufferRange(GL_SHADER_STORAGE_BUFFER, i,
							   newctx->gl_shader_storage_buffer_range_binding_array[i],
							   newctx->gl_shader_storage_buffer_range_binding_array_offset[i],
							   newctx->gl_shader_storage_buffer_range_binding_array_size[i]))
			}
		}
		for (i = 0; i < newctx->gl_atomic_counter_buffer_binding_num[0]; i++) {
			if (STATE_COMPARE(gl_atomic_counter_buffer_range_binding_array[i]) ||
					STATE_COMPARE(gl_atomic_counter_buffer_range_binding_array_offset[i]) ||
					STATE_COMPARE(gl_atomic_counter_buffer_range_binding_array_size[i])) {
				CHECK_GL_ERROR(_orig_fastpath_glBindBufferRange(GL_ATOMIC_COUNTER_BUFFER, i,
							   newctx->gl_atomic_counter_buffer_range_binding_array[i],
							   newctx->gl_atomic_counter_buffer_range_binding_array_offset[i],
							   newctx->gl_atomic_counter_buffer_range_binding_array_size[i]))
			}
		}
	}

#ifdef COREGL_USE_MODULE_TRACEPATH
	tracepath_api_trace_end("eglMakeCurrent(FP bind buffers)",
							trace_hint_bindbuffers, 0);
#endif // COREGL_USE_MODULE_TRACEPATH


	//------------------//
	// Enable States
	// _enable_flag1
#ifdef COREGL_USE_MODULE_TRACEPATH
	static void *trace_hint_enable_states = NULL;
	trace_hint_enable_states =
		tracepath_api_trace_begin("eglMakeCurrent(FP enable states)",
								  trace_hint_enable_states, 0);
#endif // COREGL_USE_MODULE_TRACEPATH

	flag = oldctx->_enable_flag1 | newctx->_enable_flag1;
	if (flag) {
		if STATE_COMPARE(gl_blend[0]) {
			if (newctx->gl_blend[0]) {
				CHECK_GL_ERROR(_orig_fastpath_glEnable(GL_BLEND))
			} else
				CHECK_GL_ERROR(_orig_fastpath_glDisable(GL_BLEND))
			}
		if STATE_COMPARE(gl_cull_face[0]) {
			if (newctx->gl_cull_face[0]) {
				CHECK_GL_ERROR(_orig_fastpath_glEnable(GL_CULL_FACE))
			} else
				CHECK_GL_ERROR(_orig_fastpath_glDisable(GL_CULL_FACE))
			}
		if STATE_COMPARE(gl_depth_test[0]) {
			if (newctx->gl_depth_test[0]) {
				CHECK_GL_ERROR(_orig_fastpath_glEnable(GL_DEPTH_TEST))
			} else
				CHECK_GL_ERROR(_orig_fastpath_glDisable(GL_DEPTH_TEST))
			}
		if STATE_COMPARE(gl_dither[0]) {
			if (newctx->gl_dither[0]) {
				CHECK_GL_ERROR(_orig_fastpath_glEnable(GL_DITHER))
			} else
				CHECK_GL_ERROR(_orig_fastpath_glDisable(GL_DITHER))
			}
	}

	// _enable_flag2
	flag = oldctx->_enable_flag2 | newctx->_enable_flag2;
	if (flag) {
		if STATE_COMPARE(gl_polygon_offset_fill[0]) {
			if (newctx->gl_polygon_offset_fill[0]) {
				CHECK_GL_ERROR(_orig_fastpath_glEnable(GL_POLYGON_OFFSET_FILL))
			} else
				CHECK_GL_ERROR(_orig_fastpath_glDisable(GL_POLYGON_OFFSET_FILL))
			}
		if STATE_COMPARE(gl_sample_alpha_to_coverage[0]) {
			if (newctx->gl_sample_alpha_to_coverage[0]) {
				CHECK_GL_ERROR(_orig_fastpath_glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE))
			} else
				CHECK_GL_ERROR(_orig_fastpath_glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE))
			}
		if STATE_COMPARE(gl_sample_coverage[0]) {
			if (newctx->gl_sample_coverage[0]) {
				CHECK_GL_ERROR(_orig_fastpath_glEnable(GL_SAMPLE_COVERAGE))
			} else
				CHECK_GL_ERROR(_orig_fastpath_glDisable(GL_SAMPLE_COVERAGE))
			}
		if STATE_COMPARE(gl_scissor_test[0]) {
			if (newctx->gl_scissor_test[0]) {
				CHECK_GL_ERROR(_orig_fastpath_glEnable(GL_SCISSOR_TEST))
			} else
				CHECK_GL_ERROR(_orig_fastpath_glDisable(GL_SCISSOR_TEST))
			}
		if STATE_COMPARE(gl_stencil_test[0]) {
			if (newctx->gl_stencil_test[0]) {
				CHECK_GL_ERROR(_orig_fastpath_glEnable(GL_STENCIL_TEST))
			} else
				CHECK_GL_ERROR(_orig_fastpath_glDisable(GL_STENCIL_TEST))
			}
		if STATE_COMPARE(gl_sample_shading_oes[0]) {
			if (newctx->gl_sample_shading_oes[0]) {
				CHECK_GL_ERROR(_orig_fastpath_glEnable(GL_SAMPLE_SHADING_OES))
			} else
				CHECK_GL_ERROR(_orig_fastpath_glDisable(GL_SAMPLE_SHADING_OES))
			}
		if STATE_COMPARE(gl_sample_mask[0]) {
			if (newctx->gl_sample_mask[0]) {
				CHECK_GL_ERROR(_orig_fastpath_glEnable(GL_SAMPLE_MASK))
			} else
				CHECK_GL_ERROR(_orig_fastpath_glDisable(GL_SAMPLE_MASK))
			}
	}

	// _enable_flag3
	flag = oldctx->_enable_flag3 | newctx->_enable_flag3;
	if (flag) {
		if STATE_COMPARE(gl_primitive_restart_fixed_index[0]) {
			if (newctx->gl_primitive_restart_fixed_index[0]) {
				CHECK_GL_ERROR(_orig_fastpath_glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX))
			} else
				CHECK_GL_ERROR(_orig_fastpath_glDisable(GL_PRIMITIVE_RESTART_FIXED_INDEX))
			}
		if STATE_COMPARE(gl_rasterizer_discard[0]) {
			if (newctx->gl_rasterizer_discard[0]) {
				CHECK_GL_ERROR(_orig_fastpath_glEnable(GL_RASTERIZER_DISCARD))
			} else
				CHECK_GL_ERROR(_orig_fastpath_glDisable(GL_RASTERIZER_DISCARD))
			}
		if STATE_COMPARE(gl_blend_advanced_coherent_khr[0]) {
			if (newctx->gl_blend_advanced_coherent_khr[0]) {
				CHECK_GL_ERROR(_orig_fastpath_glEnable(GL_BLEND_ADVANCED_COHERENT_KHR))
			} else
				CHECK_GL_ERROR(_orig_fastpath_glDisable(GL_BLEND_ADVANCED_COHERENT_KHR))
			}
	}

#ifdef COREGL_USE_MODULE_TRACEPATH
	tracepath_api_trace_end("eglMakeCurrent(FP enable states)",
							trace_hint_enable_states, 0);
#endif // COREGL_USE_MODULE_TRACEPATH

	//------------------//
	// _clear_flag1
#ifdef COREGL_USE_MODULE_TRACEPATH
	static void *trace_hint_clear_viewport = NULL;
	trace_hint_clear_viewport =
		tracepath_api_trace_begin("eglMakeCurrent(FP clear/viewport)",
								  trace_hint_clear_viewport, 0);
#endif // COREGL_USE_MODULE_TRACEPATH

	flag = oldctx->_clear_flag1 | newctx->_clear_flag1;
	if (flag) {
		// Viewport.
		if STATES_COMPARE(gl_viewport, 4 * sizeof(GLint)) {
			CHECK_GL_ERROR(_orig_fastpath_glViewport(newctx->gl_viewport[0],
						   newctx->gl_viewport[1],
						   newctx->gl_viewport[2],
						   newctx->gl_viewport[3]))
		}

		if STATE_COMPARE(gl_current_program[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glUseProgram(newctx->gl_current_program[0]))
		}
		if STATES_COMPARE(gl_color_clear_value, 4 * sizeof(GLclampf)) {
			CHECK_GL_ERROR(_orig_fastpath_glClearColor(newctx->gl_color_clear_value[0],
						   newctx->gl_color_clear_value[1],
						   newctx->gl_color_clear_value[2],
						   newctx->gl_color_clear_value[3]))
		}
	}


	// _clear_flag2
	flag = oldctx->_clear_flag2 | newctx->_clear_flag2;
	if (flag) {
		if STATES_COMPARE(gl_color_writemask,
						  4 * newctx->gl_color_writemask_num[0] * sizeof(GLboolean)) {
			for (i = 0; i < newctx->gl_color_writemask_num[0]; i++) {
				CHECK_GL_ERROR(_orig_fastpath_glColorMaski(i,
							   (newctx->gl_color_writemask + i)[0],
							   (newctx->gl_color_writemask + i)[1],
							   (newctx->gl_color_writemask + i)[2],
							   (newctx->gl_color_writemask + i)[3]))
			}
		}
		if STATES_COMPARE(gl_color_writemask_for_glColorMask, 4 * sizeof(GLboolean)) {
			CHECK_GL_ERROR(_orig_fastpath_glColorMask(newctx->gl_color_writemask_for_glColorMask[0],
						   newctx->gl_color_writemask_for_glColorMask[1],
						   newctx->gl_color_writemask_for_glColorMask[2],
						   newctx->gl_color_writemask_for_glColorMask[3]))
		}
		if STATES_COMPARE(gl_depth_range, 2 * sizeof(GLclampf)) {
			CHECK_GL_ERROR(_orig_fastpath_glDepthRangef(newctx->gl_depth_range[0],
						   newctx->gl_depth_range[1]))
		}
		if STATE_COMPARE(gl_depth_clear_value[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glClearDepthf(newctx->gl_depth_clear_value[0]))
		}
		if STATE_COMPARE(gl_depth_func[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glDepthFunc(newctx->gl_depth_func[0]))
		}
		if STATE_COMPARE(gl_depth_writemask[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glDepthMask(newctx->gl_depth_writemask[0]))
		}
		if STATE_COMPARE(gl_cull_face_mode[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glCullFace(newctx->gl_cull_face_mode[0]))
		}

	}

#ifdef COREGL_USE_MODULE_TRACEPATH
	tracepath_api_trace_end("eglMakeCurrent(FP clear/viewport)",
							trace_hint_clear_viewport, 0);
#endif // COREGL_USE_MODULE_TRACEPATH

	//------------------//
	// Texture here...
#ifdef COREGL_USE_MODULE_TRACEPATH
	static void *trace_hint_bind_textures = NULL;
	trace_hint_bind_textures =
		tracepath_api_trace_begin("eglMakeCurrent(FP bind textures)",
								  trace_hint_bind_textures, 0);
#endif // COREGL_USE_MODULE_TRACEPATH

	flag = oldctx->_tex_flag1 | newctx->_tex_flag1;
	if (flag) {

		for (i = 0; i < oldctx->gl_tex_units_num[0]; i++) {
			if STATE_COMPARE(gl_tex_2d_state[i]) {
				CHECK_GL_ERROR(_orig_fastpath_glActiveTexture(GL_TEXTURE0 + i))
				CHECK_GL_ERROR(_orig_fastpath_glBindTexture(GL_TEXTURE_2D,
							   newctx->gl_tex_2d_state[i]))
			}
			if STATE_COMPARE(gl_tex_3d_state[i]) {
				CHECK_GL_ERROR(_orig_fastpath_glActiveTexture(GL_TEXTURE0 + i))
				CHECK_GL_ERROR(_orig_fastpath_glBindTexture(GL_TEXTURE_3D,
							   newctx->gl_tex_3d_state[i]))
			}
			if STATE_COMPARE(gl_tex_2d_array_state[i]) {
				CHECK_GL_ERROR(_orig_fastpath_glActiveTexture(GL_TEXTURE0 + i))
				CHECK_GL_ERROR(_orig_fastpath_glBindTexture(GL_TEXTURE_2D_ARRAY,
							   newctx->gl_tex_2d_array_state[i]))
			}
			if STATE_COMPARE(gl_tex_cube_state[i]) {
				CHECK_GL_ERROR(_orig_fastpath_glActiveTexture(GL_TEXTURE0 + i))
				CHECK_GL_ERROR(_orig_fastpath_glBindTexture(GL_TEXTURE_CUBE_MAP,
							   newctx->gl_tex_cube_state[i]))
			}
			if STATE_COMPARE(gl_tex_external_oes_state[i]) {
				CHECK_GL_ERROR(_orig_fastpath_glActiveTexture(GL_TEXTURE0 + i))
				CHECK_GL_ERROR(_orig_fastpath_glBindTexture(GL_TEXTURE_EXTERNAL_OES,
							   newctx->gl_tex_external_oes_state[i]))
			}
		}

		// Restore active texture
		CHECK_GL_ERROR(_orig_fastpath_glActiveTexture(newctx->gl_active_texture[0]))

		if STATE_COMPARE(gl_generate_mipmap_hint[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glHint(GL_GENERATE_MIPMAP_HINT,
												 newctx->gl_generate_mipmap_hint[0]))
		}
	}

	flag = oldctx->_tex_flag2 | newctx->_tex_flag2;
	if (flag) {
		for (i = 0; i < oldctx->gl_tex_units_num[0]; i++) {
			if STATE_COMPARE(gl_tex_buffer_ext_state[i]) {
				CHECK_GL_ERROR(_orig_fastpath_glActiveTexture(GL_TEXTURE0 + i))
				CHECK_GL_ERROR(_orig_fastpath_glBindTexture(GL_TEXTURE_BUFFER_EXT,
							   newctx->gl_tex_buffer_ext_state[i]))
			}
			if STATE_COMPARE(gl_tex_2d_multisample_state[i]) {
				CHECK_GL_ERROR(_orig_fastpath_glActiveTexture(GL_TEXTURE0 + i))
				CHECK_GL_ERROR(_orig_fastpath_glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,
							   newctx->gl_tex_2d_multisample_state[i]))
			}
			if STATE_COMPARE(gl_tex_cube_map_array_ext_state[i]) {
				CHECK_GL_ERROR(_orig_fastpath_glActiveTexture(GL_TEXTURE0 + i))
				CHECK_GL_ERROR(_orig_fastpath_glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY_EXT,
							   newctx->gl_tex_cube_map_array_ext_state[i]))
			}
			if STATE_COMPARE(gl_tex_2d_multisample_array_oes_state[i]) {
				CHECK_GL_ERROR(_orig_fastpath_glActiveTexture(GL_TEXTURE0 + i))
				CHECK_GL_ERROR(_orig_fastpath_glBindTexture(GL_TEXTURE_2D_MULTISAMPLE_ARRAY_OES,
							   newctx->gl_tex_2d_multisample_array_oes_state[i]))
			}
		}
	}
#ifdef COREGL_USE_MODULE_TRACEPATH
	tracepath_api_trace_end("eglMakeCurrent(FP bind textures)",
							trace_hint_bind_textures, 0);
#endif // COREGL_USE_MODULE_TRACEPATH

	//------------------//
#ifdef COREGL_USE_MODULE_TRACEPATH
	static void *trace_hint_etc = NULL;
	trace_hint_etc = tracepath_api_trace_begin("eglMakeCurrent(FP etc.)",
					 trace_hint_etc, 0);
#endif // COREGL_USE_MODULE_TRACEPATH

	flag = oldctx->_blend_flag | newctx->_blend_flag;
	if (flag) {
		if STATES_COMPARE(gl_blend_color, 4 * sizeof(GLclampf)) {
			CHECK_GL_ERROR(_orig_fastpath_glBlendColor(newctx->gl_blend_color[0],
						   newctx->gl_blend_color[1],
						   newctx->gl_blend_color[2],
						   newctx->gl_blend_color[3]))
		}
		if ((oldctx->gl_blend_src_rgb[0] != newctx->gl_blend_src_rgb[0]) ||
				(oldctx->gl_blend_dst_rgb[0] != newctx->gl_blend_dst_rgb[0]) ||
				(oldctx->gl_blend_src_alpha[0] != newctx->gl_blend_src_alpha[0]) ||
				(oldctx->gl_blend_dst_alpha[0] != newctx->gl_blend_dst_alpha[0])) {
			CHECK_GL_ERROR(_orig_fastpath_glBlendFuncSeparate(newctx->gl_blend_src_rgb[0],
						   newctx->gl_blend_dst_rgb[0],
						   newctx->gl_blend_src_alpha[0],
						   newctx->gl_blend_dst_alpha[0]))
		}
		if ((oldctx->gl_blend_equation_rgb[0] != newctx->gl_blend_equation_rgb[0]) ||
				(oldctx->gl_blend_equation_alpha[0] != newctx->gl_blend_equation_alpha[0])) {
			CHECK_GL_ERROR(_orig_fastpath_glBlendEquationSeparate(
							   newctx->gl_blend_equation_rgb[0], newctx->gl_blend_equation_alpha[0]))
		}

	}

	//------------------//
	// _stencil_flag1
	flag = oldctx->_stencil_flag1 | newctx->_stencil_flag1;
	if (flag) {
		if ((oldctx->gl_stencil_func[0] != newctx->gl_stencil_func[0]) ||
				(oldctx->gl_stencil_ref[0]  != newctx->gl_stencil_ref[0])  ||
				(oldctx->gl_stencil_value_mask[0] != newctx->gl_stencil_value_mask[0])) {
			CHECK_GL_ERROR(_orig_fastpath_glStencilFuncSeparate(GL_FRONT,
						   newctx->gl_stencil_func[0],
						   newctx->gl_stencil_ref[0],
						   newctx->gl_stencil_value_mask[0]))
		}
		if ((oldctx->gl_stencil_fail[0] != newctx->gl_stencil_fail[0]) ||
				(oldctx->gl_stencil_pass_depth_fail[0] != newctx->gl_stencil_pass_depth_fail[0])
				||
				(oldctx->gl_stencil_pass_depth_pass[0] !=
				 newctx->gl_stencil_pass_depth_pass[0])) {
			CHECK_GL_ERROR(_orig_fastpath_glStencilOpSeparate(GL_FRONT,
						   newctx->gl_stencil_fail[0],
						   newctx->gl_stencil_pass_depth_fail[0],
						   newctx->gl_stencil_pass_depth_pass[0]))
		}

		if STATE_COMPARE(gl_stencil_writemask[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glStencilMaskSeparate(GL_FRONT,
						   newctx->gl_stencil_writemask[0]))
		}
	}


	// _stencil_flag1
	flag = oldctx->_stencil_flag2 | newctx->_stencil_flag2;
	if (flag) {
		if ((oldctx->gl_stencil_back_func[0] != newctx->gl_stencil_back_func[0]) ||
				(oldctx->gl_stencil_back_ref[0]  != newctx->gl_stencil_back_ref[0])  ||
				(oldctx->gl_stencil_back_value_mask[0] !=
				 newctx->gl_stencil_back_value_mask[0])) {
			CHECK_GL_ERROR(_orig_fastpath_glStencilFuncSeparate(GL_BACK,
						   newctx->gl_stencil_back_func[0],
						   newctx->gl_stencil_back_ref[0],
						   newctx->gl_stencil_back_value_mask[0]))
		}
		if ((oldctx->gl_stencil_back_fail[0] != newctx->gl_stencil_back_fail[0]) ||
				(oldctx->gl_stencil_back_pass_depth_fail[0] !=
				 newctx->gl_stencil_back_pass_depth_fail[0]) ||
				(oldctx->gl_stencil_back_pass_depth_pass[0] !=
				 newctx->gl_stencil_back_pass_depth_pass[0])) {
			CHECK_GL_ERROR(_orig_fastpath_glStencilOpSeparate(GL_BACK,
						   newctx->gl_stencil_back_fail[0],
						   newctx->gl_stencil_back_pass_depth_fail[0],
						   newctx->gl_stencil_back_pass_depth_pass[0]))
		}

		if STATE_COMPARE(gl_stencil_back_writemask[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glStencilMaskSeparate(GL_BACK,
						   newctx->gl_stencil_back_writemask[0]))
		}
		if STATE_COMPARE(gl_stencil_clear_value[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glClearStencil(newctx->gl_stencil_clear_value[0]))
		}
	}

	//------------------//
	// _pixel_flag1
	flag = oldctx->_pixel_flag1 | newctx->_pixel_flag1;
	if (flag) {
		if STATE_COMPARE(gl_pack_row_length[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glPixelStorei(GL_PACK_ROW_LENGTH,
						   newctx->gl_pack_row_length[0]))
		}
		if STATE_COMPARE(gl_pack_skip_rows[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glPixelStorei(GL_PACK_SKIP_ROWS,
						   newctx->gl_pack_skip_rows[0]))
		}
		if STATE_COMPARE(gl_pack_skip_pixels[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glPixelStorei(GL_PACK_SKIP_PIXELS,
						   newctx->gl_pack_skip_pixels[0]))
		}
		if STATE_COMPARE(gl_pack_alignment[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glPixelStorei(GL_PACK_ALIGNMENT,
						   newctx->gl_pack_alignment[0]))
		}
	}

	// _pixel_flag2
	flag = oldctx->_pixel_flag2 | newctx->_pixel_flag2;
	if (flag) {
		if STATE_COMPARE(gl_unpack_row_length[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glPixelStorei(GL_UNPACK_ROW_LENGTH,
						   newctx->gl_unpack_row_length[0]))
		}
		if STATE_COMPARE(gl_unpack_skip_rows[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glPixelStorei(GL_UNPACK_SKIP_ROWS,
						   newctx->gl_unpack_skip_rows[0]))
		}
		if STATE_COMPARE(gl_unpack_skip_pixels[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glPixelStorei(GL_UNPACK_SKIP_PIXELS,
						   newctx->gl_unpack_skip_pixels[0]))
		}
		if STATE_COMPARE(gl_unpack_alignment[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glPixelStorei(GL_UNPACK_ALIGNMENT,
						   newctx->gl_unpack_alignment[0]))
		}
		if STATE_COMPARE(gl_unpack_image_height[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glPixelStorei(GL_UNPACK_IMAGE_HEIGHT,
						   newctx->gl_unpack_image_height[0]))
		}
		if STATE_COMPARE(gl_unpack_skip_images[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glPixelStorei(GL_UNPACK_SKIP_IMAGES,
						   newctx->gl_unpack_skip_images[0]))
		}
	}

	//------------------//
	// _misc_flag1
	flag = oldctx->_misc_flag1 | newctx->_misc_flag1;
	if (flag) {
		if STATE_COMPARE(gl_front_face[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glFrontFace(newctx->gl_front_face[0]))
		}
		if STATE_COMPARE(gl_line_width[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glLineWidth(newctx->gl_line_width[0]))
		}
		if ((oldctx->gl_polygon_offset_factor[0] != newctx->gl_polygon_offset_factor[0])
				||
				(oldctx->gl_polygon_offset_units[0]  != newctx->gl_polygon_offset_units[0])) {
			CHECK_GL_ERROR(_orig_fastpath_glPolygonOffset(
							   newctx->gl_polygon_offset_factor[0],
							   newctx->gl_polygon_offset_units[0]))
		}
		if ((oldctx->gl_sample_coverage_value[0]  !=
				newctx->gl_sample_coverage_value[0]) ||
				(oldctx->gl_sample_coverage_invert[0] !=
				 newctx->gl_sample_coverage_invert[0])) {
			CHECK_GL_ERROR(_orig_fastpath_glSampleCoverage(
							   newctx->gl_sample_coverage_value[0],
							   newctx->gl_sample_coverage_invert[0]))
		}
		if STATE_COMPARE(gl_fragment_shader_derivative_hint[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT_OES,
												 newctx->gl_fragment_shader_derivative_hint[0]))
		}
	}

	// _misc_flag2
	flag = oldctx->_misc_flag2 | newctx->_misc_flag2;
	if (flag) {
		if STATES_COMPARE(gl_scissor_box, 4 * sizeof(GLint)) {
			CHECK_GL_ERROR(_orig_fastpath_glScissor(newctx->gl_scissor_box[0],
													newctx->gl_scissor_box[1],
													newctx->gl_scissor_box[2],
													newctx->gl_scissor_box[3]))
		}
	}

	// _misc_flag3
	flag = oldctx->_misc_flag3 | newctx->_misc_flag3;
	if (flag) {
		if STATE_COMPARE(gl_read_buffer[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glReadBuffer(newctx->gl_read_buffer[0]))
		}
		if STATES_COMPARE(gl_draw_buffers,
						  oldctx->gl_draw_buffers_num[0] * sizeof(GLenum)) {
			int drawBuffSize = 16;
			/* If the  context has only default framebuffer, then size of glDrawBuffers can only be 1 */
			if (fastpath_ostate_has_object_type(&newctx->ostate,
												GL_OBJECT_TYPE_FRAMEBUFFER) == 0) {
				drawBuffSize = 1;
			}

			CHECK_GL_ERROR(_orig_fastpath_glDrawBuffers(drawBuffSize,
						   newctx->gl_draw_buffers))
		}

		if (oldctx->gl_transform_feedback_active[0] == GL_TRUE &&
				oldctx->gl_transform_feedback_paused[0] == GL_FALSE) {
			CHECK_GL_ERROR(_orig_fastpath_glPauseTransformFeedback())
		}
		if STATE_COMPARE(gl_transform_feedback_binding[0]) {
			CHECK_GL_ERROR(_orig_fastpath_glBindTransformFeedback(GL_TRANSFORM_FEEDBACK,
						   newctx->gl_transform_feedback_binding[0]))
		}
		if (newctx->gl_transform_feedback_active[0] == GL_TRUE &&
				newctx->gl_transform_feedback_paused[0] == GL_FALSE) {
			CHECK_GL_ERROR(_orig_fastpath_glResumeTransformFeedback())
		}
	}

#ifdef COREGL_USE_MODULE_TRACEPATH
	tracepath_api_trace_end("eglMakeCurrent(FP etc.)", trace_hint_etc, 0);
#endif // COREGL_USE_MODULE_TRACEPATH

	ret = 1;
	goto finish;

finish:

#ifdef COREGL_FASTPATH_TRACE_STATE_INFO
	if (unlikely(trace_state_flag == 1))
		fastpath_dump_context_states(newctx, 0);
#endif // COREGL_FASTPATH_TRACE_STATE_INFO
	return ret;
#undef STATE_COMPARE
#undef STATES_COMPARE
}

