#ifndef COREGL_INTERNAL_H
#define COREGL_INTERNAL_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "coregl.h"

#include "modules/coregl_module.h"

#define unlikely(x) __builtin_expect(x, 0)

// Symbol definition for real
#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     extern RET_TYPE (*_sym_##FUNC_NAME) PARAM_LIST;
# include "headers/sym.h"
#undef _COREGL_SYMBOL

#ifdef _COREGL_DEBUG
#define COREGL_LOG_ERR  3
#define COREGL_LOG_WARN 2
#define COREGL_LOG_DBG  1
#define COREGL_LOG_INIT 0

#define LOG_TAG "COREGL"
#include <dlog.h>

unsigned int coregl_log_lvl;
unsigned int coregl_dlog_enable;
unsigned int coregl_log_initialized;

#define FONT_DEFAULT	"\033[0m"	/* for reset to default color */
#define FONT_RED		"\033[31m"	/* for error logs */
#define FONT_YELLOW		"\033[33m"	/* for warning logs */
#define FONT_GREEN		"\033[32m"	/* for debug logs */

#define coregl_log_e(t, f, x...)											\
	do {																	\
		if(coregl_dlog_enable)												\
			LOGE(FONT_RED t " " f FONT_DEFAULT, ##x);						\
		else																\
			fprintf(stderr, FONT_RED t "[(pid:%d)(%s)] " f FONT_DEFAULT "\n", getpid(), __func__, ##x);\
	} while(0)

#define coregl_log_w(t, f, x...)											\
	do {																	\
		if(coregl_dlog_enable)												\
			LOGW(FONT_YELLOW t " " f FONT_DEFAULT, ##x);					\
		else																\
			fprintf(stderr, FONT_YELLOW t "[(pid:%d)(%s)] " f FONT_DEFAULT "\n", getpid(), __func__, ##x);\
	} while(0)

#define coregl_log_d(t, f, x...)											\
	do {																	\
		if(coregl_dlog_enable)												\
			LOGD(FONT_GREEN t " " f FONT_DEFAULT, ##x); 					\
		else																\
			fprintf(stderr, FONT_GREEN t "[(pid:%d)(%s)] " f FONT_DEFAULT "\n", getpid(), __func__, ##x);\
	} while(0)

#define LOG_INIT()										\
	do {												\
		if (!coregl_log_initialized) {					\
			char *lvl = (char *)getenv("COREGL_LOG_LEVEL");	\
			char *dlog = (char *)getenv("COREGL_TRACE_DLOG");\
			if (lvl)									\
				coregl_log_lvl = atoi(lvl);				\
			else										\
				coregl_log_lvl = _COREGL_DEFAULT_LOG_LEVEL;\
														\
			if (dlog)									\
				coregl_dlog_enable = atoi(dlog);		\
			else										\
				coregl_dlog_enable = _COREGL_DLOG_ENABLE;\
			coregl_log_initialized = 1;					\
		}												\
	}while(0)

#define COREGL_ERR(f, x...)								\
	do {												\
		LOG_INIT();										\
		if(coregl_dlog_enable)							\
			coregl_log_e("[COREGL_ERROR]", f, ##x);		\
		else {											\
			if (coregl_log_lvl > COREGL_LOG_INIT 		\
				&& coregl_log_lvl <= COREGL_LOG_ERR)	\
				coregl_log_e("[COREGL_ERROR]", f, ##x);	\
		}												\
	}while(0)

#define COREGL_WARN(f, x...)							\
	do {												\
		LOG_INIT();										\
		if(coregl_dlog_enable) 							\
			coregl_log_w("[COREGL_WARN]", f, ##x); 		\
		else {											\
			if (coregl_log_lvl > COREGL_LOG_INIT 		\
				&& coregl_log_lvl <= COREGL_LOG_WARN)	\
				coregl_log_w("[COREGL_WARN]", f, ##x); 	\
		}												\
	}while(0)

#define COREGL_DBG(f, x...)								\
	do {												\
		LOG_INIT();										\
		if(coregl_dlog_enable) 							\
			coregl_log_d("[COREGL_DBG]", f, ##x);		\
		else{											\
			if (coregl_log_lvl > COREGL_LOG_INIT 		\
				&& coregl_log_lvl <= COREGL_LOG_DBG)	\
				coregl_log_d("[COREGL_DBG]", f, ##x); 	\
		}												\
	}while(0)
#else
#define COREGL_ERR(f, x...)
#define COREGL_WARN(f, x...)
#define COREGL_DBG(f, x...)
#endif

#define TRACE(...) 							\
	do {									\
		if (trace_fp != NULL) 				\
			fprintf(trace_fp, __VA_ARGS__); \
		else 								\
			fprintf(stderr, __VA_ARGS__);	\
	} while(0)


#define TRACE_END() \
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


#ifdef _COREGL_DEBUG
#define AST(expr) \
     if (!(expr)) { LOGE("\E[40;31;1m%s(%d) error. '"#expr"'\E[0m\n", __func__, __LINE__); }
#else
#define AST(expr) \
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

