#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>

#include "coregl_internal.h"
#include "coregl_export.h"

void               *egl_lib_handle;
void               *gl_lib_handle;

int                 driver_gl_version = 0;
static int          api_gl_version = COREGL_GLAPI_2;

// Symbol definition for real
#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     RET_TYPE (*_sym_##FUNC_NAME) PARAM_LIST;
#include "headers/sym.h"
#undef _COREGL_SYMBOL

const char *
get_env_setting(const char *name)
{
	char *fp_env = NULL;
	static char *fp_default = "\0";
	fp_env = getenv(name);
	if (fp_env == NULL) fp_env = fp_default;
	return fp_env;
}

void
cleanup_current_thread_state()
{
	GLThreadState *tstate = NULL;

	tstate = get_current_thread_state();

	if (tstate != NULL) {
		COREGL_DBG("de-init thread state");
		deinit_modules_tstate(tstate);
		remove_from_general_trace_list(&thread_trace_list, tstate);
		free(tstate);
		tstate = NULL;
	}

	set_current_thread_state(NULL);
}

int
init_new_thread_state()
{
	int ret = 0;
	GLThreadState *tstate = NULL;

	tstate = get_current_thread_state();
	AST(tstate == NULL);

	tstate = (GLThreadState *)calloc(1, sizeof(GLThreadState));
	tstate->thread_id = get_current_thread();
	add_to_general_trace_list(&thread_trace_list, tstate);

	init_modules_tstate(tstate);
	set_current_thread_state(tstate);

	ret = 1;
	goto finish;

finish:
	return ret;
}

static void
_sym_missing()
{
	COREGL_ERR("GL symbol missing! Check client version!");
}

#define FINDGLSYM(libhandle, getproc, dst, sym) \
	if(api_gl_version <= driver_gl_version) { \
      if (!dst || (void *)dst == (void *)_sym_missing) \
		  if (getproc) dst = (__typeof__(dst))getproc(sym); \
      if (!dst || (void *)dst == (void *)_sym_missing) \
		  dst = (__typeof__(dst))dlsym(libhandle, sym); \
	  if (!dst) dst = (__typeof__(dst))_sym_missing; \
   }

#define FINDEGLSYM(libhandle, getproc, dst, sym) { \
      if (!dst || (void *)dst == (void *)_sym_missing) \
		  if (getproc) dst = (__typeof__(dst))getproc(sym); \
      if (!dst || (void *)dst == (void *)_sym_missing) \
		  dst = (__typeof__(dst))dlsym(libhandle, sym); \
	  if (!dst) dst = (__typeof__(dst))_sym_missing;\
   }

static int
_glue_sym_init(void)
{
#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST) \
    FINDEGLSYM(egl_lib_handle, _sym_eglGetProcAddress, _sym_##FUNC_NAME, #FUNC_NAME);
#define _COREGL_EXT_SYMBOL_ALIAS(FUNC_NAME, ALIAS_NAME) \
    FINDEGLSYM(egl_lib_handle, _sym_eglGetProcAddress, _sym_##ALIAS_NAME, #FUNC_NAME);

#include "headers/sym_egl.h"

#undef _COREGL_EXT_SYMBOL_ALIAS
#undef _COREGL_SYMBOL

	return 1;
}

static int
_gl_sym_init(void)
{

#define _COREGL_START_API(version) 		api_gl_version = version;
#define _COREGL_END_API(version)		api_gl_version = COREGL_GLAPI_2;
#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST) \
	FINDGLSYM(gl_lib_handle, _sym_eglGetProcAddress, _sym_##FUNC_NAME, #FUNC_NAME);
#define _COREGL_EXT_SYMBOL_ALIAS(FUNC_NAME, ALIAS_NAME) \
	FINDGLSYM(gl_lib_handle, _sym_eglGetProcAddress, _sym_##ALIAS_NAME, #FUNC_NAME);

#include "headers/sym_gl1.h"
#include "headers/sym_gl2.h"
#include "headers/sym_gl_common.h"

#undef _COREGL_EXT_SYMBOL_ALIAS
#undef _COREGL_SYMBOL
#undef _COREGL_START_API
#undef _COREGL_END_API

	return 1;
}

#undef FINDEGLSYM
#undef FINDGLSYM


COREGL_API void coregl_symbol_exported()
{
	COREGL_ERR("Invalid library link! (Check linkage of libCOREGL)");
}

COREGL_API void set_driver_gl_version(int version)
{
	driver_gl_version = version;
}

static int
_gl_lib_init(void)
{
	//------------------------------------------------//
	// Open EGL Library as EGL is separate
	egl_lib_handle = dlopen(_COREGL_VENDOR_EGL_LIB_PATH, RTLD_LAZY | RTLD_LOCAL);
	if (!egl_lib_handle) {
		COREGL_ERR("%s", dlerror());
		COREGL_ERR("Invalid library link! (Check linkage of libCOREGL -> %s)",
				   _COREGL_VENDOR_EGL_LIB_PATH);
		return 0;
	}

	// test for invalid linking egl
	if (dlsym(egl_lib_handle, "coregl_symbol_exported")) {
		COREGL_ERR("Invalid library link! (Check linkage of libCOREGL -> %s)",
				   _COREGL_VENDOR_EGL_LIB_PATH);
		return 0;
	}

	// use gl_lib handle for GL symbols
	if (driver_gl_version == COREGL_GLAPI_1) {
		gl_lib_handle = dlopen(_COREGL_VENDOR_GLV1_LIB_PATH, RTLD_LAZY | RTLD_LOCAL);
		if (!gl_lib_handle) {
			COREGL_ERR("%s", dlerror());
			COREGL_ERR("Invalid library link! (Check linkage of libCOREGL -> %s)",
					   _COREGL_VENDOR_GLV1_LIB_PATH);
		} else {
			// test for invalid linking gl
			if (dlsym(gl_lib_handle, "coregl_symbol_exported")) {
				COREGL_ERR("Invalid library link! (Check linkage of libCOREGL -> %s)",
						   _COREGL_VENDOR_GLV1_LIB_PATH);
				return 0;
			}
			COREGL_DBG("Driver GL version 1.1");
		}
	} else if (driver_gl_version == COREGL_GLAPI_2) {
		gl_lib_handle = dlopen(_COREGL_VENDOR_GLV2_LIB_PATH, RTLD_LAZY | RTLD_LOCAL);
		if (!gl_lib_handle) {
			COREGL_ERR("%s", dlerror());
			COREGL_ERR("Invalid library link! (Check linkage of libCOREGL -> %s)",
					   _COREGL_VENDOR_GLV2_LIB_PATH);
		} else {
			// test for invalid linking gl
			if (dlsym(gl_lib_handle, "coregl_symbol_exported")) {
				COREGL_ERR("Invalid library link! (Check linkage of libCOREGL -> %s)",
						   _COREGL_VENDOR_GLV2_LIB_PATH);
				return 0;
			}

			// test for a GLES 3.x symbol
			if (dlsym(gl_lib_handle, "glBlendBarrier")) {
				COREGL_DBG("Driver GL version 3.2");
				driver_gl_version = COREGL_GLAPI_32;
			} else if (dlsym(gl_lib_handle, "glBindProgramPipeline")) {
				COREGL_DBG("Driver GL version 3.1");
				driver_gl_version = COREGL_GLAPI_31;
			} else if (dlsym(gl_lib_handle, "glReadBuffer")) {
				COREGL_DBG("Driver GL version 3.0");
				driver_gl_version = COREGL_GLAPI_3;
			} else {
				COREGL_DBG("Driver GL version 2.0");
			}
		}
	}

	if (!_glue_sym_init()) return 0;
	if (!_gl_sym_init()) return 0;

	return 1;
}

static int
_gl_lib_deinit(void)
{
	if (egl_lib_handle) dlclose(egl_lib_handle);
	if (gl_lib_handle) dlclose(gl_lib_handle);

	return 1;
}

int
coregl_initialize()
{
	COREGL_DBG("(%s) Library initializing...", _COREGL_COMPILE_DATE);

	if (!_gl_lib_init()) return 0;

	init_export(GL_TRUE, GL_TRUE);

	COREGL_DBG(" -> Completed");

	init_modules();

	return 1;
}

__attribute__((destructor))
void
coregl_terminate()
{
	if (export_initialized != 0) {
		deinit_modules();

		_gl_lib_deinit();
	}
}

