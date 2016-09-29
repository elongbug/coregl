#ifndef COREGL_INTERNAL_H
#define COREGL_INTERNAL_H

#include <stdio.h>
#include <stdint.h>

#include "coregl.h"

#include "modules/coregl_module.h"

#define unlikely(x) __builtin_expect(x, 0)

// Symbol definition for real
#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     extern RET_TYPE (*_sym_##FUNC_NAME) PARAM_LIST;
# include "headers/sym.h"
#undef _COREGL_SYMBOL

#define COREGL_DEBUG

///////////////////////////////////////
// Disable dlog for debugging urgent issues //
#ifdef COREGL_DEBUG
//# define LOG_TAG "CoreGL"
//# include <dlog.h>
# define LOGE(...) fprintf(stderr, __VA_ARGS__)
# define LOGW(...) fprintf(stderr, __VA_ARGS__)
# define LOGD(...) fprintf(stderr, __VA_ARGS__)
#endif
///////////////////////////////////////

# define COREGL_ERR(...) \
     LOGE(" "__VA_ARGS__)
# define COREGL_WRN(...) \
     LOGW(" "__VA_ARGS__)
# ifdef COREGL_DEBUG
#  define COREGL_DBG(...) \
     LOGD(" "__VA_ARGS__)
# else
#  define COREGL_DBG(...)
# endif

# define COREGL_LOG(...) \
     LOGD(" "__VA_ARGS__)


# define TRACE(...) \
     if (trace_fp != NULL) \
       fprintf(trace_fp, __VA_ARGS__); \
     else \
       LOGD(" "__VA_ARGS__)

# define TRACE_END() \
     if (trace_fp != NULL) \
       fflush(trace_fp)
#define _COREGL_TRACE_OUTPUT_INTERVAL_SEC 5

static inline GLint GET_INT_FROM_FLOAT(GLfloat value)
{
	return (GLint)value;
}
static inline GLuint GET_UINT_FROM_FLOAT(GLfloat value)
{
	return (GLuint)value;
}


#ifdef COREGL_DEBUG
# define AST(expr) \
     if (!(expr)) { LOGE("\E[40;31;1m%s(%d) error. '"#expr"'\E[0m\n", __func__, __LINE__); }
#else
# define AST(expr) \
     if (expr)
#endif

typedef GLvoid      *GLvoidptr;
typedef GLuint       GLuintmask;

#define COREGL_GLAPI_32	5
#define COREGL_GLAPI_31	4
#define COREGL_GLAPI_3	3
#define COREGL_GLAPI_2	2
#define COREGL_GLAPI_1	1
#define COREGL_GLAPI_1_2_COMMON 1


#define _COREGL_INT_INIT_VALUE -3

#define COREGL_OVERRIDE_API(mangle, func, prefix) \
	mangle##func = prefix##func

typedef EGLSurface     GLSurface;
typedef EGLDisplay     GLDisplay;
typedef EGLContext     GLContext;

#define COREGL_GL_NO_CONTEXT EGL_NO_CONTEXT

typedef struct _GLThreadState {
	int                      thread_id;
	void                    *module_data[COREGL_MAX_MODULES];
} GLThreadState;

extern void                *glue_lib_handle;
extern void                *egl_lib_handle;

#include "coregl_thread_pthread.h"

typedef struct _General_Trace_List {
	void                          *value;
	struct _General_Trace_List    *next;
} General_Trace_List;

extern General_Trace_List  *thread_trace_list;
extern Mutex                general_trace_lists_access_mutex;

extern FILE               *trace_fp;

extern int                 trace_api_flag;
extern int                 trace_api_all_flag;
extern int                 trace_api_frame_flag;
extern int                 trace_mem_flag;
extern int                 trace_mem_all_flag;
extern int                 trace_ctx_flag;
extern int                 trace_ctx_force_flag;
extern int                 trace_state_flag;
extern int                 trace_surface_flag;
extern int                 trace_surface_sequence_sort_flag;
extern int                 trace_surface_filter_period_begin;
extern int                 trace_surface_filter_period_end;
extern int                 trace_surface_filter_type;
extern uintptr_t           trace_surface_filter_handle;
extern int                 trace_surface_filter_size_w;
extern int                 trace_surface_filter_size_h;
extern int                 trace_surface_print_only_flag;

#define USE_TRACEPATH		(trace_api_flag == 1 || trace_ctx_flag == 1 || trace_state_flag == 1 || trace_mem_flag == 1 || trace_surface_flag == 1)

// Environment functions
extern const char         *get_env_setting(const char *name);

// Main utility functions
extern void                cleanup_current_thread_state();
extern int                 init_new_thread_state();

// Thread functions
extern int                 mutex_init(Mutex *mt);
extern int                 mutex_lock(Mutex *mt);
extern int                 mutex_unlock(Mutex *mt);
extern int                 get_current_thread();
extern int                 set_current_thread_state(GLThreadState *tstate);
extern GLThreadState      *get_current_thread_state();


// Override functions
extern void                init_export(GLboolean init_egl, GLboolean init_gl);
extern void                deinit_export();

// Module interfaces
extern void                init_modules();
extern void                deinit_modules();
extern void                reset_modules_override();
extern void                init_modules_tstate(GLThreadState *tstate);
extern void                deinit_modules_tstate(GLThreadState *tstate);


// Debug & Trace functions
extern int                 add_to_general_trace_list(General_Trace_List **gtl,
		void *value);
extern int                 remove_from_general_trace_list(
	General_Trace_List **gtl, void *value);

#endif // COREGL_INTERNAL_H

