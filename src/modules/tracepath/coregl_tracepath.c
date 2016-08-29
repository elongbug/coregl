#include "coregl_tracepath.h"
#include <GLES/gl.h>

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <sys/types.h>
#include <unistd.h>

#include <dlfcn.h>
static int         api_gl_version;


#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     RET_TYPE (*_orig_tracepath_##FUNC_NAME) PARAM_LIST = NULL;
# include "../../headers/sym_gl1.h"
# include "../../headers/sym_gl2.h"
# include "../../headers/sym_gl_common.h"
# include "../../headers/sym_egl.h"
#undef _COREGL_SYMBOL

#define TIMEVAL_INIT            { 0, 0 }

struct _Trace_Data {
	char                         name[MAX_TRACE_NAME_LENGTH];
	struct _Trace_Data          *next;
};

struct _Apicall_Data {
	struct _Trace_Data          trace_data;

	int                          call_count;
	int                          last_call_count;
	struct timeval              elapsed_time;
	struct timeval              elapsed_time_max;
	struct timeval              last_elapsed_time;
	struct timeval              total_elapsed_time;
	struct timeval              last_total_elapsed_time;
	struct timeval              last_time;

	int                          traced;
};

struct _Memuse_Data {
	struct _Trace_Data          trace_data;

	int                          memsize;
	int                          alloc_count;
	int                          remove_count;
};

struct _Surface_Data {
	struct _Trace_Data          trace_data;

	GLDisplay                    display;
	GLSurface                    surface;
	GLContext                    context;
	GLint                        fbo;
	GLint                        tex;
	GLint                        rb;
	int                          dump_count;
	GLint                        tex_w;
	GLint                        tex_h;
	GLint                        tex_format;
};

typedef struct _GLGlueFakeContext {
	GLuint gl_draw_buffers_num[1];
	GLuint gl_tex_units_num[1];
	GLuint gl_vertex_attribs_num[1];
	GLuint gl_transform_feedback_buffer_binding_num[1];
	GLuint gl_uniform_buffer_binding_num[1];
	GLuint gl_shader_storage_buffer_binding_num[1];
	GLuint gl_atomic_counter_buffer_binding_num[1];
} GLGlueFakeContext;

GLGlueFakeContext initial_fake_ctx_real;
GLGlueFakeContext *initial_fake_ctx = &initial_fake_ctx_real;

struct timeval     initial_time = TIMEVAL_INIT;
struct timeval     last_initial_time = TIMEVAL_INIT;

struct timeval     last_trace_time = TIMEVAL_INIT;
struct timeval     other_elapsed_time = TIMEVAL_INIT;
struct timeval     traced_other_elapsed_time = TIMEVAL_INIT;

Mutex               mtd_access_mutex = MUTEX_INITIALIZER;
Memuse_Data       **mtd_table;

Mutex               std_access_mutex = MUTEX_INITIALIZER;
Surface_Data      **std_table;

static void
_state_get_texture_states(GLenum pname, GLint *params)
{
	GLuint cur_active_tex = 0;

	AST(initial_fake_ctx != NULL);

	_orig_tracepath_glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint *)&cur_active_tex);
	int i;
	for (i = 0; i < initial_fake_ctx->gl_tex_units_num[0]; i++) {
		_orig_tracepath_glActiveTexture(GL_TEXTURE0 + i);
		_orig_tracepath_glGetIntegerv(pname, (GLint *)&params[i]);
	}
	_orig_tracepath_glActiveTexture(cur_active_tex);
}

static void
_state_get_draw_buffers(GLenum *params)
{
	AST(initial_fake_ctx != NULL);

	int i;
	for (i = 0; i < initial_fake_ctx->gl_draw_buffers_num[0]; i++) {
		_orig_tracepath_glGetIntegerv(GL_DRAW_BUFFER0 + i, (GLint *)&params[i]);
	}
}

static void
_state_get_transform_feedback_buffer_bindings(GLuint *params)
{
	AST(initial_fake_ctx != NULL);

	int i;
	for (i = 0; i < initial_fake_ctx->gl_transform_feedback_buffer_binding_num[0];
			i++) {
		_orig_tracepath_glGetIntegeri_v(GL_TRANSFORM_FEEDBACK_BUFFER_BINDING, i,
										(GLint *)&params[i]);
	}
}

static void
_state_get_transform_feedback_buffer_bindings_offset(GLintptr *params)
{
	AST(initial_fake_ctx != NULL);

	int i;
	for (i = 0; i < initial_fake_ctx->gl_transform_feedback_buffer_binding_num[0];
			i++) {
		_orig_tracepath_glGetIntegeri_v(GL_TRANSFORM_FEEDBACK_BUFFER_START, i,
										(GLint *)&params[i]);
	}
}

static void
_state_get_transform_feedback_buffer_bindings_size(GLsizeiptr *params)
{
	AST(initial_fake_ctx != NULL);

	int i;
	for (i = 0; i < initial_fake_ctx->gl_transform_feedback_buffer_binding_num[0];
			i++) {
		_orig_tracepath_glGetIntegeri_v(GL_TRANSFORM_FEEDBACK_BUFFER_SIZE, i,
										(GLint *)&params[i]);
	}
}

static void
_state_get_uniform_buffer_bindings(GLuint *params)
{
	AST(initial_fake_ctx != NULL);

	int i;
	for (i = 0; i < initial_fake_ctx->gl_uniform_buffer_binding_num[0]; i++) {
/////////////////////////////////////////////////////////////////////////////////
// XXXX : AVOID SEGFAULT in ADRENO
		((GLint *)params)[i] = 0;
//		_orig_tracepath_glGetIntegeri_v(GL_UNIFORM_BUFFER_BINDING, i, (GLint *)&params[i]);
/////////////////////////////////////////////////////////////////////////////////
	}
}

static void
_state_get_uniform_buffer_bindings_offset(GLintptr *params)
{
	AST(initial_fake_ctx != NULL);

	int i;
	for (i = 0; i < initial_fake_ctx->gl_uniform_buffer_binding_num[0]; i++) {
		_orig_tracepath_glGetIntegeri_v(GL_UNIFORM_BUFFER_START, i,
										(GLint *)&params[i]);
	}
}

static void
_state_get_uniform_buffer_bindings_size(GLsizeiptr *params)
{
	AST(initial_fake_ctx != NULL);

	int i;
	for (i = 0; i < initial_fake_ctx->gl_uniform_buffer_binding_num[0]; i++) {
		_orig_tracepath_glGetIntegeri_v(GL_UNIFORM_BUFFER_SIZE, i, (GLint *)&params[i]);
	}
}

/* shader storage buffer */
static void
_state_get_shader_storage_buffer_bindings(GLuint *params)
{
	AST(initial_fake_ctx != NULL);

	int i;
	for (i = 0; i < initial_fake_ctx->gl_shader_storage_buffer_binding_num[0];
			i++) {
		_orig_tracepath_glGetIntegeri_v(GL_SHADER_STORAGE_BUFFER_BINDING, i,
										(GLint *)&params[i]);
	}
}

static void
_state_get_shader_storage_buffer_bindings_offset(GLintptr *params)
{
	AST(initial_fake_ctx != NULL);

	int i;
	for (i = 0; i < initial_fake_ctx->gl_shader_storage_buffer_binding_num[0];
			i++) {
		_orig_tracepath_glGetIntegeri_v(GL_SHADER_STORAGE_BUFFER_START, i,
										(GLint *)&params[i]);
	}
}

static void
_state_get_shader_storage_buffer_bindings_size(GLsizeiptr *params)
{
	AST(initial_fake_ctx != NULL);

	int i;
	for (i = 0; i < initial_fake_ctx->gl_shader_storage_buffer_binding_num[0];
			i++) {
		_orig_tracepath_glGetIntegeri_v(GL_SHADER_STORAGE_BUFFER_SIZE, i,
										(GLint *)&params[i]);
	}
}

/* atomic counter buffer */
static void
_state_get_atomic_counter_buffer_bindings(GLuint *params)
{
	AST(initial_fake_ctx != NULL);

	int i;
	for (i = 0; i < initial_fake_ctx->gl_atomic_counter_buffer_binding_num[0];
			i++) {
		_orig_tracepath_glGetIntegeri_v(GL_ATOMIC_COUNTER_BUFFER_BINDING, i,
										(GLint *)&params[i]);
	}
}

static void
_state_get_atomic_counter_buffer_bindings_offset(GLintptr *params)
{
	AST(initial_fake_ctx != NULL);

	int i;
	for (i = 0; i < initial_fake_ctx->gl_atomic_counter_buffer_binding_num[0];
			i++) {
		_orig_tracepath_glGetIntegeri_v(GL_ATOMIC_COUNTER_BUFFER_START, i,
										(GLint *)&params[i]);
	}
}

static void
_state_get_atomic_counter_buffer_bindings_size(GLsizeiptr *params)
{
	AST(initial_fake_ctx != NULL);

	int i;
	for (i = 0; i < initial_fake_ctx->gl_atomic_counter_buffer_binding_num[0];
			i++) {
		_orig_tracepath_glGetIntegeri_v(GL_ATOMIC_COUNTER_BUFFER_SIZE, i,
										(GLint *)&params[i]);
	}
}

void
init_modules_tracepath()
{
	{
		const char *output_file = NULL;
		output_file = get_env_setting("COREGL_LOG_FILE");
		if (strlen(output_file) > 0) {
			trace_fp = fopen(output_file, "w");
		}
	}

#ifdef COREGL_TRACEPATH_TRACE_APICALL_INFO
	trace_api_flag = atoi(get_env_setting("COREGL_TRACE_API"));
	trace_api_all_flag = atoi(get_env_setting("COREGL_TRACE_API_ALL"));
	trace_api_frame_flag = atoi(get_env_setting("COREGL_TRACE_API_FRAME"));
#endif
#ifdef COREGL_TRACEPATH_TRACE_MEMUSE_INFO
	trace_mem_flag = atoi(get_env_setting("COREGL_TRACE_MEM"));
	trace_mem_all_flag = atoi(get_env_setting("COREGL_TRACE_MEM_ALL"));
#endif
#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	trace_surface_flag = atoi(get_env_setting("COREGL_TRACE_SURFACE"));
	trace_surface_sequence_sort_flag = atoi(
										   get_env_setting("COREGL_TRACE_SURFACE_SEQUENCE_SORT"));
	trace_surface_print_only_flag = atoi(
										get_env_setting("COREGL_TRACE_SURFACE_PRINT_ONLY"));

	{
		// COREGL_TRACE_SURFACE_FILTER_PERIOD=40~60
		char tmp[64 + 1] = { 0 }, *tmpp = NULL;
		strncpy(tmp, get_env_setting("COREGL_TRACE_SURFACE_FILTER_PERIOD"), 64);
		for (tmpp = &tmp[0]; ; tmpp++) {
			if (*tmpp == 0x00) break;
			if (*tmpp == '~') {
				*tmpp = 0x00;
				trace_surface_filter_period_begin = atoi(tmp);
				trace_surface_filter_period_end = atoi(tmpp + 1);
				break;
			}
		}
	}

	{
		// COREGL_TRACE_SURFACE_FILTER_TYPE=EGL|FBO
		char tmp[64 + 1] = { 0 };
		strncpy(tmp, get_env_setting("COREGL_TRACE_SURFACE_FILTER_TYPE"), 64);
		if (strcmp(tmp, "EGL") == 0) trace_surface_filter_type = 1;
		if (strcmp(tmp, "FBO") == 0) trace_surface_filter_type = 2;
	}

	{
		// COREGL_TRACE_SURFACE_FILTER_HANDLE=0x3234
		char tmp[64 + 1] = { 0 }, *tmpp = NULL;
		strncpy(tmp, get_env_setting("COREGL_TRACE_SURFACE_FILTER_HANDLE"), 64);
		if (tmp[0] == '0' && tmp[1] == 'x') {
			for (tmpp = &tmp[2]; ; tmpp++) {
				if (*tmpp == 0x00) break;
				trace_surface_filter_handle *= 16;
				switch (*tmpp) {
				case '1' :
					trace_surface_filter_handle += 1;
					break;
				case '2' :
					trace_surface_filter_handle += 2;
					break;
				case '3' :
					trace_surface_filter_handle += 3;
					break;
				case '4' :
					trace_surface_filter_handle += 4;
					break;
				case '5' :
					trace_surface_filter_handle += 5;
					break;
				case '6' :
					trace_surface_filter_handle += 6;
					break;
				case '7' :
					trace_surface_filter_handle += 7;
					break;
				case '8' :
					trace_surface_filter_handle += 8;
					break;
				case '9' :
					trace_surface_filter_handle += 9;
					break;
				case 'A' :
				case 'a' :
					trace_surface_filter_handle += 10;
					break;
				case 'B' :
				case 'b' :
					trace_surface_filter_handle += 11;
					break;
				case 'C' :
				case 'c' :
					trace_surface_filter_handle += 12;
					break;
				case 'D' :
				case 'd' :
					trace_surface_filter_handle += 13;
					break;
				case 'E' :
				case 'e' :
					trace_surface_filter_handle += 14;
					break;
				case 'F' :
				case 'f' :
					trace_surface_filter_handle += 15;
					break;
				}
			}
		} else {
			trace_surface_filter_handle = (uintptr_t)atol(tmp);
		}
	}

	{
		// COREGL_TRACE_SURFACE_FILTER_SIZE=640x480
		char tmp[64 + 1] = { 0 }, *tmpp = NULL;
		strncpy(tmp, get_env_setting("COREGL_TRACE_SURFACE_FILTER_SIZE"), 64);
		for (tmpp = &tmp[0]; ; tmpp++) {
			if (*tmpp == 0x00) break;
			if (*tmpp == 'x') {
				*tmpp = 0x00;
				trace_surface_filter_size_w = atoi(tmp);
				trace_surface_filter_size_h = atoi(tmpp + 1);
				break;
			}
		}
	}

#endif
#ifdef COREGL_TRACEPATH_TRACE_CONTEXT_INFO
	trace_ctx_flag = atoi(get_env_setting("COREGL_TRACE_CTX"));
	trace_ctx_force_flag = atoi(get_env_setting("COREGL_TRACE_CTX_FORCE"));
#endif
#ifdef COREGL_TRACEPATH_TRACE_STATE_INFO
	trace_state_flag = atoi(get_env_setting("COREGL_TRACE_STATE"));
#endif

	if (USE_TRACEPATH) {
		COREGL_LOG("[CoreGL] \E[40;32;1m<Trace> \E[40;37;1m: ");

		if (trace_api_flag == 1) {
			COREGL_LOG("\E[40;31;1m(API)\E[0m ");
			if (trace_api_all_flag == 1) COREGL_LOG("\E[40;31;1m(API-ALL)\E[0m ");
			if (trace_api_frame_flag == 1) COREGL_LOG("\E[40;31;1m(API-FRAME)\E[0m ");
		}
		if (trace_ctx_flag == 1) {
			COREGL_LOG("\E[40;33;1m(CONTEXT)\E[0m ");
			if (trace_ctx_force_flag == 1) COREGL_LOG("\E[40;33;1m(CONTEXT-FORCE)\E[0m ");
		}
		if (trace_state_flag == 1) COREGL_LOG("\E[40;36;1m(STATE)\E[0m ");
		if (trace_mem_flag == 1) {
			COREGL_LOG("\E[40;35;1m(MEM)\E[0m ");
			if (trace_mem_all_flag == 1) COREGL_LOG("\E[40;35;1m(MEM-ALL)\E[0m ");
		}
		if (trace_surface_flag == 1) {
			COREGL_LOG("\E[40;36;1m(SURFACE)\E[0m ");
			if (trace_surface_sequence_sort_flag == 1)
				COREGL_LOG("\E[40;36;1m(SURFACE-SEQUENCE SORT)\E[0m ");
			if (trace_surface_print_only_flag == 1)
				COREGL_LOG("\E[40;36;1m(PRINT ONLY)\E[0m ");
			if (trace_surface_filter_period_begin != 0 ||
					trace_surface_filter_period_end != 0)
				COREGL_LOG("\E[40;36;1m(SURFACE-PERIOD:%d~%d)\E[0m ",
						   trace_surface_filter_period_begin, trace_surface_filter_period_end);
			if (trace_surface_filter_type == 1)
				COREGL_LOG("\E[40;36;1m(SURFACE-TYPE:EGL)\E[0m ");
			if (trace_surface_filter_type == 2)
				COREGL_LOG("\E[40;36;1m(SURFACE-TYPE:FBO)\E[0m ");
			if (trace_surface_filter_handle != 0)
				COREGL_LOG("\E[40;36;1m(SURFACE-HANDLE:%p(%ld))\E[0m ",
						   (void *)trace_surface_filter_handle, (long int)trace_surface_filter_handle);
			if (trace_surface_filter_size_w > 0 && trace_surface_filter_size_h > 0)
				COREGL_LOG("\E[40;36;1m(SURFACE-SIZE:%dx%d)\E[0m ", trace_surface_filter_size_w,
						   trace_surface_filter_size_h);
		}

		COREGL_LOG("\E[40;37;1menabled\E[0m\n");
	}
}

void
deinit_modules_tracepath()
{
}

void
init_modules_tstate_tracepath(GLThreadState *tstate)
{
	MY_MODULE_TSTATE *tstate_mt = NULL;

	tstate_mt = (MY_MODULE_TSTATE *)calloc(1, sizeof(MY_MODULE_TSTATE));


	tstate->module_data[MY_MODULE_ID] = tstate_mt;
}

void
deinit_modules_tstate_tracepath(GLThreadState *tstate)
{
	if (tstate->module_data[MY_MODULE_ID] != NULL) {
		free(tstate->module_data[MY_MODULE_ID]);
		tstate->module_data[MY_MODULE_ID] = NULL;
	}
}

void
tracepath_apply_overrides()
{
	if (USE_TRACEPATH) {
		tracepath_apply_overrides_egl(1);
		tracepath_apply_overrides_gl(1);
	}
}

void
tracepath_apply_overrides_egl(int enable)
{
#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     COREGL_INIT_ORIGINAL(_orig_tracepath_, FUNC_NAME);
# include "../../headers/sym_egl.h"
#undef _COREGL_SYMBOL

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     COREGL_OVERRIDE(tracepath_, FUNC_NAME);
# include "../../headers/sym_egl.h"
#undef _COREGL_SYMBOL
}

void
tracepath_apply_overrides_gl(int enable)
{
#define _COREGL_START_API(version) api_gl_version = version;
#define _COREGL_END_API(version) api_gl_version = COREGL_GLAPI_2;
#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     \
	if(api_gl_version <= driver_gl_version) COREGL_INIT_ORIGINAL(_orig_tracepath_, FUNC_NAME);
# include "../../headers/sym_gl1.h"
# include "../../headers/sym_gl2.h"
# include "../../headers/sym_gl_common.h"
#undef _COREGL_SYMBOL

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST) \
	if(api_gl_version <= driver_gl_version) COREGL_OVERRIDE(tracepath_, FUNC_NAME);
# include "../../headers/sym_gl1.h"
# include "../../headers/sym_gl2.h"
# include "../../headers/sym_gl_common.h"
#undef _COREGL_SYMBOL
#undef _COREGL_START_API
#undef _COREGL_END_API
}

#undef OVERRIDE

void
tracepath_dump_context_states(int force_output)
{
	static struct timeval tv_last = { 0, 0 };

	if (unlikely(trace_state_flag != 1)) return;

	_sym_glGetIntegerv(GL_MAX_DRAW_BUFFERS,
					   (GLint *)initial_fake_ctx->gl_draw_buffers_num);
	_sym_glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
					   (GLint *)initial_fake_ctx->gl_tex_units_num);
	_sym_glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,
					   (GLint *)initial_fake_ctx->gl_vertex_attribs_num);
	_sym_glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS,
					   (GLint *)initial_fake_ctx->gl_transform_feedback_buffer_binding_num);
	_sym_glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS,
					   (GLint *)initial_fake_ctx->gl_uniform_buffer_binding_num);
	_sym_glGetError();

	if (!force_output) {
		struct timeval tv_now = { 0, 0 };
		AST(gettimeofday(&tv_now, NULL) == 0);
		if (tv_now.tv_sec - tv_last.tv_sec < _COREGL_TRACE_OUTPUT_INTERVAL_SEC) {
			goto finish;
		}
		tv_last = tv_now;
	}

	TRACE("\n");
	TRACE("\E[0;40;34m===================================================================================================================\E[0m\n");
	TRACE("\E[40;32;1m  State info \E[1;37;1m: <PID = %d> (CURRENT BINDED CONTEXT)\E[0m\n",
		  getpid());
	TRACE("\E[0;40;34m===================================================================================================================\E[0m\n");

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

#define INITIAL_CTX initial_fake_ctx

#define GLUE_STATE(TYPE, NAME, SIZE, ARRAY_SIZE, DEFAULT_STMT, GET_STMT)  \
   { \
      TYPE valuedata[SIZE]; \
      TYPE *value = NULL; \
      value = valuedata; GET_STMT; value = valuedata; \
      _sym_glGetError();	\
      TRACE("\E[40;37;1m %-30.30s : (\E[0m ", #NAME); \
      for (int i = 0; i < SIZE; i++) \
      { \
         if (i > 0) { \
            if (i % 4 == 0) \
               TRACE("\n %-30.30s     ", "");\
            else \
               TRACE(", "); \
         } \
         TRACE("["PRINTF_CHAR(TYPE)"]", value[i]); \
      } \
      TRACE(" \E[40;37;1m)\E[0m\n"); \
   }
# include "../fastpath/coregl_fastpath_state.h"
#undef GLUE_STATE

	TRACE("\E[0;40;34m===================================================================================================================\E[0m\n");
	TRACE("\n");

	TRACE_END();

finish:
	return;
}


static inline void
_add_timeval_period(struct timeval *tv_dst, struct timeval tv_now,
					struct timeval tv_old)
{
	tv_dst->tv_sec += tv_now.tv_sec - tv_old.tv_sec;
	tv_dst->tv_usec += 1000000 + (tv_now.tv_usec - tv_old.tv_usec);
	tv_dst->tv_sec += (tv_dst->tv_usec / 1000000) - 1;
	tv_dst->tv_usec = tv_dst->tv_usec % 1000000;
}

static inline void
_add_timeval(struct timeval *tv_dst, struct timeval tv_src)
{
	tv_dst->tv_sec += tv_src.tv_sec;
	tv_dst->tv_usec += tv_src.tv_usec;
	if (tv_dst->tv_usec > 1000000) {
		tv_dst->tv_usec -= 1000000;
		tv_dst->tv_sec++;
	} else if (tv_dst->tv_usec < 0) {
		tv_dst->tv_usec += 1000000;
		tv_dst->tv_sec--;
	}
}

static inline double
_get_timeval_period(struct timeval time_now, struct timeval time_old)
{
	return ((time_now.tv_sec - time_old.tv_sec) * 1000) + ((
				time_now.tv_usec - time_old.tv_usec) / 1000.0);
}

static inline double
_get_timeval(struct timeval time)
{
	return (time.tv_sec * 1000) + (time.tv_usec / 1000.0);
}

static unsigned short
_generate_hash_short(const char *string)
{
	unsigned short hash = 0;
	int i;

	for (i = 0; i < strlen(string); i++) {
		hash = 253 * hash + string[i];
	}
	return (hash ^ (hash >> 8)) % MAX_TRACE_TABLE_SIZE;
}

static Trace_Data *
_get_trace_data(Trace_Data **ftd_table, size_t td_size, const char *name)
{
	Trace_Data *ret = NULL;
	Trace_Data *current = NULL;
	Trace_Data *prev = NULL;
	unsigned short hash = 0;

	AST(strlen(name) < MAX_TRACE_NAME_LENGTH);

	hash = _generate_hash_short(name);

	current = ftd_table[hash];

	while (current != NULL && current->next != NULL) {
		if (strcmp(current->name, name) == 0)
			break;
		prev = current;
		current = current->next;
	}

	if (current != NULL) {
		ret = current;
	} else {
		Trace_Data *newitm = NULL;
		newitm = (Trace_Data *)calloc(1, td_size);
		strncpy(newitm->name, name,
				strlen(name) < (sizeof(newitm->name) - 1) ? strlen(name) : (sizeof(
							newitm->name) - 1));
		newitm->next = NULL;

		if (prev != NULL) {
			prev->next = newitm;
		} else {
			ftd_table[hash] = newitm;
		}
		ret = newitm;
	}

	goto finish;

finish:
	return ret;
}

void
tracepath_mem_trace_add(const char *desc, int alloc_size)
{
	Memuse_Data *mtd = NULL;

	if (trace_mem_flag == 1) {
		AST(mutex_lock(&mtd_access_mutex) == 1);

		if (mtd_table == NULL) {
			mtd_table = (Memuse_Data **)calloc(1,
											   sizeof(Memuse_Data *) * MAX_TRACE_TABLE_SIZE);
		}

		mtd = (Memuse_Data *)_get_trace_data((Trace_Data **)mtd_table,
											 sizeof(Memuse_Data), desc);

		AST(mtd != NULL);

		mtd->alloc_count++;

		if (mtd->memsize == 0)
			mtd->memsize = alloc_size;

		AST(mtd->memsize == alloc_size);

		AST(mutex_unlock(&mtd_access_mutex) == 1);

		if (trace_mem_all_flag == 1)
			_COREGL_TRACE_MEM_OUTPUT(1);
	}

}

void
tracepath_mem_trace_remove(const char *desc, int alloc_size)
{
	Memuse_Data *mtd = NULL;

	if (trace_mem_flag == 1) {
		AST(mutex_lock(&mtd_access_mutex) == 1);

		if (mtd_table == NULL) {
			mtd_table = (Memuse_Data **)calloc(1,
											   sizeof(Memuse_Data *) * MAX_TRACE_TABLE_SIZE);
		}

		mtd = (Memuse_Data *)_get_trace_data((Trace_Data **)mtd_table,
											 sizeof(Memuse_Data), desc);

		AST(mtd != NULL);
		AST(mtd->memsize == alloc_size);

		AST(mtd->alloc_count > mtd->remove_count);
		mtd->remove_count++;

		AST(mutex_unlock(&mtd_access_mutex) == 1);

		if (trace_mem_all_flag == 1)
			_COREGL_TRACE_MEM_OUTPUT(1);
	}
}

void *
tracepath_api_trace_begin(const char *funcname, void *hint,
						  int trace_total_time)
{
	Apicall_Data *ftd = NULL;
	struct timeval t = TIMEVAL_INIT;

	if (trace_api_flag == 1) {
		AST(gettimeofday(&t, NULL) == 0);

		ftd = (Apicall_Data *)hint;

		if (ftd == NULL) {
			MY_MODULE_TSTATE *tstate = NULL;
			GET_MY_TSTATE(tstate, get_current_thread_state());

			if (tstate == NULL) {
				init_new_thread_state();

				GET_MY_TSTATE(tstate, get_current_thread_state());
				AST(tstate != NULL);
			}


			if (tstate) {
				if (tstate->ftd_table == NULL) {
					tstate->ftd_table = (Apicall_Data **)calloc(1,
										sizeof(Apicall_Data *) * MAX_TRACE_TABLE_SIZE);
				}

				ftd = (Apicall_Data *)_get_trace_data((Trace_Data **)tstate->ftd_table,
													  sizeof(Apicall_Data), funcname);
			}
		}

		AST(ftd != NULL);

		if (ftd) ftd->call_count++;
		if (ftd) AST(ftd->last_time.tv_sec == 0);

		if (ftd) AST(gettimeofday(&ftd->last_time, NULL) == 0);

		if (ftd && initial_time.tv_sec == 0) {
			initial_time = ftd->last_time;
			last_initial_time = ftd->last_time;
		}

		if (trace_total_time == 1) {
			if (last_trace_time.tv_sec != 0) {
				_add_timeval_period(&other_elapsed_time, t, last_trace_time);
				last_trace_time.tv_sec = 0;
			}
		}

	}

	return ftd;
}

void *
tracepath_api_trace_end(const char *funcname, void *hint, int trace_total_time)
{
	Apicall_Data *ftd = NULL;
	struct timeval t = TIMEVAL_INIT;

	if (trace_api_flag == 1) {
		MY_MODULE_TSTATE *tstate = NULL;
		struct timeval elapsed_time = TIMEVAL_INIT;

		AST(gettimeofday(&t, NULL) == 0);

		ftd = (Apicall_Data *)hint;

		if (ftd == NULL) {
			GET_MY_TSTATE(tstate, get_current_thread_state());

			if (tstate == NULL) {
				init_new_thread_state();

				GET_MY_TSTATE(tstate, get_current_thread_state());
				AST(tstate != NULL);
			}

			AST(tstate != NULL);
			AST(tstate && (tstate->ftd_table != NULL));

			if (tstate && (tstate->ftd_table)) {
				ftd = (Apicall_Data *)_get_trace_data((Trace_Data **)tstate->ftd_table,
													  sizeof(Apicall_Data), funcname);
			}
		}

		AST(ftd != NULL);

		if (ftd) _add_timeval_period(&elapsed_time, t, ftd->last_time);

		if (ftd) _add_timeval(&ftd->elapsed_time, elapsed_time);

		if (ftd &&
				elapsed_time.tv_sec >= ftd->elapsed_time_max.tv_sec &&
				elapsed_time.tv_usec > ftd->elapsed_time_max.tv_usec) {
			ftd->elapsed_time_max.tv_sec = elapsed_time.tv_sec;
			ftd->elapsed_time_max.tv_usec = elapsed_time.tv_usec;
		}

		if (ftd) ftd->last_time.tv_sec = 0;

		if (trace_total_time == 1) {
			if (ftd) _add_timeval(&ftd->total_elapsed_time, elapsed_time);

			AST(gettimeofday(&last_trace_time, NULL) == 0);

			if (initial_time.tv_sec == 0) {
				initial_time = t;
				last_initial_time = t;
			}
		}

	}

	return ftd;
}

void
tracepath_api_trace_output(int force_output)
{
	static struct timeval tv_last = TIMEVAL_INIT;
	struct timeval total_now = TIMEVAL_INIT;
	GLThreadState *tstate = NULL;
	MY_MODULE_TSTATE *tstate_tm = NULL;
	Apicall_Data **ftd_table = NULL;

	double total_elapsed_time = 0.0;
	double total_elapsed_time_period = 0.0;
	double total_opengl_elapsed_time = 0.0;
	double total_opengl_elapsed_time_period = 0.0;
	double total_other_elapsed_time = 0.0;
	double total_other_elapsed_time_period = 0.0;

	double swaps_per_sec = 0.0;

	int i;

	if (trace_api_flag != 1) {
		goto finish;
	}

	if (!force_output) {
		struct timeval tv_now = TIMEVAL_INIT;
		AST(gettimeofday(&tv_now, NULL) == 0);
		if (tv_now.tv_sec - tv_last.tv_sec < _COREGL_TRACE_OUTPUT_INTERVAL_SEC) {
			goto finish;
		}
		tv_last = tv_now;
	}

	AST(gettimeofday(&total_now, NULL) == 0);

	total_elapsed_time = _get_timeval_period(total_now, initial_time);
	total_elapsed_time_period = _get_timeval_period(total_now, last_initial_time);
	last_initial_time = total_now;

	total_other_elapsed_time = _get_timeval(other_elapsed_time);
	total_other_elapsed_time_period = _get_timeval_period(other_elapsed_time,
									  traced_other_elapsed_time);
	traced_other_elapsed_time = other_elapsed_time;

	tstate = get_current_thread_state();

	if (tstate == NULL) {
		init_new_thread_state();

		tstate = get_current_thread_state();
		AST(tstate != NULL);
	}

	GET_MY_TSTATE(tstate_tm, tstate);
	if (tstate_tm == NULL) goto finish;

	ftd_table = tstate_tm->ftd_table;
	if (ftd_table == NULL) goto finish;

	{
		static Apicall_Data *trace_hint_swap = NULL;
		if (trace_hint_swap == NULL)
			trace_hint_swap = (Apicall_Data *)_get_trace_data((Trace_Data **)ftd_table,
							  sizeof(Apicall_Data), "tracepath_eglSwapBuffers");

		if (trace_hint_swap != NULL && total_elapsed_time_period > 0) {
			swaps_per_sec = (trace_hint_swap->call_count -
							 trace_hint_swap->last_call_count) / (total_elapsed_time_period / 1000);
		}
	}


	TRACE("\n");
	TRACE("\E[40;34m========================================================================================================================\E[0m\n");
	TRACE("\E[40;32;1m  API call info \E[1;37;1m: <PID = %d> Thread ID = 0x%x  [Swaps per Second(P) = %7.2f]\E[0m\n",
		  getpid(), tstate->thread_id, swaps_per_sec);
	TRACE("\E[40;34m========================================================================================================================\E[0m\n");

	// highlighted
	for (i = 0; i < MAX_TRACE_TABLE_SIZE; i++) {
		if (ftd_table[i] != NULL) {
			Apicall_Data *current = ftd_table[i];

			while (current != NULL) {
				double elapsed_time = _get_timeval(current->elapsed_time);
				double elapsed_time_per_call = elapsed_time / current->call_count;

				total_opengl_elapsed_time += _get_timeval(current->total_elapsed_time);
				total_opengl_elapsed_time_period += _get_timeval(current->total_elapsed_time) -
													_get_timeval(current->last_total_elapsed_time);

				current->traced = 0;

				if (current->call_count > current->last_call_count) {
					double elapsed_time_period = _get_timeval_period(current->elapsed_time,
												 current->last_elapsed_time);
					double elapsed_time_max = _get_timeval(current->elapsed_time_max);
					double elapsed_time_per_call_period = elapsed_time_period /
														  (current->call_count - current->last_call_count);
					char *fname = current->trace_data.name;

					if (!strncmp(fname, "tracepath_", 10))
						fname = &current->trace_data.name[10];

					if (elapsed_time_per_call_period >= 0.01 ||
							current->call_count - current->last_call_count > 1000) {
						TRACE("\E[40;37;1m %-39.39s : %10d call(s), %9.3f ms/API, %9.2f ms(MAX), %9.3f ms/API(P)\E[0m\n",
							  fname, current->call_count, elapsed_time_per_call, elapsed_time_max,
							  elapsed_time_per_call_period);
						current->traced = 1;
					}
				}
				current = (Apicall_Data *)current->trace_data.next;
			}
		}
	}

	{
		int fp_env = 0;
		fp_env = atoi(get_env_setting("COREGL_TRACE_API_ALL"));
		if (fp_env == 1) {
			// other
			for (i = 0; i < MAX_TRACE_TABLE_SIZE; i++) {
				if (ftd_table[i] != NULL) {
					Apicall_Data *current = ftd_table[i];

					while (current != NULL) {
						if (current->traced == 0 && current->call_count > 0) {
							double elapsed_time = _get_timeval(current->elapsed_time);
							double elapsed_time_per_call = elapsed_time / current->call_count;
							double elapsed_time_max = _get_timeval(current->elapsed_time_max);
							char *fname = current->trace_data.name;

							if (!strncmp(fname, "tracepath_", 10))
								fname = &current->trace_data.name[10];

							TRACE(" %-39.39s : %10d call(s), %9.3f ms/API, %9.2f ms(MAX)\n",
								  fname, current->call_count, elapsed_time_per_call, elapsed_time_max);
						}
						current = (Apicall_Data *)current->trace_data.next;
					}
				}
			}
		}
	}

	TRACE("\E[40;34m========================================================================================================================\E[0m\n");

	if (trace_api_frame_flag == 0) {
		TRACE("\E[40;36;1m %-39.39s : %13.2f ms[%6.2f%%], %13.2f ms(P)[%6.2f%%]\E[0m\n",
			  "TOTAL elapsed Time",
			  total_elapsed_time,
			  100.0,
			  total_elapsed_time_period,
			  100.0);


		TRACE("\E[40;36;1m %-39.39s : %13.2f ms[%6.2f%%], %13.2f ms(P)[%6.2f%%]\E[0m\n",
			  "OpenGL elapsed Time",
			  total_opengl_elapsed_time,
			  total_opengl_elapsed_time * 100.0 / total_elapsed_time,
			  total_opengl_elapsed_time_period,
			  total_opengl_elapsed_time_period * 100.0 / total_elapsed_time_period);

		TRACE("\E[40;36;1m %-39.39s : %13.2f ms[%6.2f%%], %13.2f ms(P)[%6.2f%%]\E[0m\n",
			  "Out of OpenGL elapsed time",
			  total_other_elapsed_time,
			  total_other_elapsed_time * 100.0 / total_elapsed_time,
			  total_other_elapsed_time_period,
			  total_other_elapsed_time_period * 100.0 / total_elapsed_time_period);

		TRACE("\E[40;36;1m %-39.39s : %13.2f ms[%6.2f%%], %13.2f ms(P)[%6.2f%%]\E[0m\n",
			  "CoreGL API tracing overhead",
			  total_elapsed_time - total_opengl_elapsed_time - total_other_elapsed_time,
			  (total_elapsed_time - total_opengl_elapsed_time - total_other_elapsed_time) *
			  100.0 / total_elapsed_time,
			  total_elapsed_time_period - total_opengl_elapsed_time_period -
			  total_other_elapsed_time_period,
			  (total_elapsed_time_period - total_opengl_elapsed_time_period -
			   total_other_elapsed_time_period) * 100.0 / total_elapsed_time_period);

		TRACE("\E[40;34m========================================================================================================================\E[0m\n");
	}
	TRACE("\n");

	for (i = 0; i < MAX_TRACE_TABLE_SIZE; i++) {
		if (ftd_table[i] != NULL) {
			Apicall_Data *current = ftd_table[i];

			while (current != NULL) {
				current->last_call_count = current->call_count;
				current->last_elapsed_time = current->elapsed_time;
				current->last_total_elapsed_time = current->total_elapsed_time;
				current = (Apicall_Data *)current->trace_data.next;
			}
		}
	}

	TRACE_END();

	goto finish;

finish:
	return;
}

void
tracepath_api_trace_reset_frame()
{
	GLThreadState *tstate = NULL;
	MY_MODULE_TSTATE *tstate_tm = NULL;
	Apicall_Data **ftd_table = NULL;

	int i;

	if (trace_api_flag != 1) {
		goto finish;
	}

	tstate = get_current_thread_state();

	if (tstate == NULL) {
		init_new_thread_state();

		tstate = get_current_thread_state();
		AST(tstate != NULL);
	}

	GET_MY_TSTATE(tstate_tm, tstate);
	if (tstate_tm == NULL) goto finish;

	ftd_table = tstate_tm->ftd_table;
	if (ftd_table == NULL) goto finish;

	for (i = 0; i < MAX_TRACE_TABLE_SIZE; i++) {
		if (ftd_table[i] != NULL) {
			Apicall_Data *current = ftd_table[i];

			while (current != NULL) {
				current->call_count = 0;
				current->last_call_count = 0;
				current->elapsed_time.tv_sec = 0;
				current->elapsed_time.tv_usec = 0;
				current->last_elapsed_time.tv_sec = 0;
				current->last_elapsed_time.tv_usec = 0;
				current->last_total_elapsed_time.tv_sec = 0;
				current->last_total_elapsed_time.tv_usec = 0;
				current->total_elapsed_time.tv_sec = 0;
				current->total_elapsed_time.tv_usec = 0;
				current = (Apicall_Data *)current->trace_data.next;
			}
		}
	}
	AST(gettimeofday(&last_initial_time, NULL) == 0);

finish:
	return;

}

void
tracepath_mem_trace_output(int force_output)
{
	static struct timeval tv_last = TIMEVAL_INIT;

	int i;

	if (trace_mem_flag != 1) {
		goto finish;
	}

	if (!force_output) {
		struct timeval tv_now = TIMEVAL_INIT;
		AST(gettimeofday(&tv_now, NULL) == 0);
		if (tv_now.tv_sec - tv_last.tv_sec < _COREGL_TRACE_OUTPUT_INTERVAL_SEC) {
			goto finish;
		}
		tv_last = tv_now;
	}

	TRACE("\n");
	TRACE("\E[40;34m========================================================================================================================\E[0m\n");
	TRACE("\E[40;32;1m  Memory usage info \E[1;37;1m: <PID = %d>\E[0m\n", getpid());
	TRACE("\E[40;34m========================================================================================================================\E[0m\n");

	if (mtd_table != NULL) {
		for (i = 0; i < MAX_TRACE_TABLE_SIZE; i++) {
			if (mtd_table[i] != NULL) {
				Memuse_Data *current = mtd_table[i];

				while (current != NULL) {
					int obj_count = current->alloc_count - current->remove_count;
					if (obj_count > 0) {
						TRACE("\E[40;37;1m %-46.46s : %12d byte(s)(E), %9d object(s) [%9d+/%9d-]\E[0m\n",
							  current->trace_data.name, current->memsize, obj_count, current->alloc_count,
							  current->remove_count);
					}
					current = (Memuse_Data *)current->trace_data.next;
				}
			}
		}

		for (i = 0; i < MAX_TRACE_TABLE_SIZE; i++) {
			if (mtd_table[i] != NULL) {
				Memuse_Data *current = mtd_table[i];

				while (current != NULL) {
					int obj_count = current->alloc_count - current->remove_count;
					if (obj_count == 0) {
						TRACE(" %-46.46s : %12d byte(s)(E), %9d object(s) [%9d+/%9d-]\n",
							  current->trace_data.name, current->memsize, obj_count, current->alloc_count,
							  current->remove_count);
					}
					current = (Memuse_Data *)current->trace_data.next;
				}
			}
		}
	}

	TRACE("\E[40;34m========================================================================================================================\E[0m\n");
	TRACE("\n");

	TRACE_END();

	goto finish;

finish:
	return;
}

#include <png.h>

void *png_lib_handle = NULL;

png_structp (*dl_png_create_write_struct) (png_const_charp user_png_ver,
		png_voidp error_ptr,
		png_error_ptr error_fn,
		png_error_ptr warn_fn);


void (*dl_png_destroy_write_struct) (png_structpp png_ptr_ptr,
									 png_infopp info_ptr_ptr);


png_infop (*dl_png_create_info_struct) (png_structp png_ptr);

void (*dl_png_init_io) (png_structp png_ptr,
						png_FILE_p fp);


void (*dl_png_set_IHDR) (png_structp png_ptr,
						 png_infop info_ptr,
						 png_uint_32 width,
						 png_uint_32 height,
						 int bit_depth,
						 int color_type,
						 int interlace_method,
						 int compression_method,
						 int filter_method);

void (*dl_png_set_bKGD) (png_structp png_ptr,
						 png_infop info_ptr,
						 png_color_16p background);

void (*dl_png_set_bgr) (png_structp png_ptr);

void (*dl_png_write_info) (png_structp png_ptr,
						   png_infop info_ptr);

void (*dl_png_write_image) (png_structp png_ptr,
							png_bytepp image);

void (*dl_png_write_end) (png_structp png_ptr,
						  png_infop info_ptr);

static void
_dump_surface(int force_output, int type, const char *position,
			  Surface_Data *sdata)
{
	static int alldumpcount = 0;
	unsigned char *data = NULL;
	EGLint width = -1, height = -1, channel = -1;
	char name[200];
	FILE *write_fd = NULL;
	png_struct *png = NULL;
	png_info *info = NULL;
	png_byte **rows = NULL;
	GLenum ret_value = _COREGL_INT_INIT_VALUE;

	if (!png_lib_handle) {
		png_lib_handle = dlopen("libpng16.so.16", RTLD_NOW);

		dl_png_create_write_struct = dlsym(png_lib_handle, "png_create_write_struct");
		dl_png_destroy_write_struct = dlsym(png_lib_handle, "png_destroy_write_struct");
		dl_png_create_info_struct = dlsym(png_lib_handle, "png_create_info_struct");

		dl_png_init_io = dlsym(png_lib_handle, "png_init_io");

		dl_png_set_IHDR = dlsym(png_lib_handle, "png_set_IHDR");
		dl_png_set_bKGD = dlsym(png_lib_handle, "png_set_bKGD");
		dl_png_set_bgr = dlsym(png_lib_handle, "png_set_bgr");

		dl_png_write_info = dlsym(png_lib_handle, "png_write_info");
		dl_png_write_image = dlsym(png_lib_handle, "png_write_image");
		dl_png_write_end = dlsym(png_lib_handle, "png_write_end");
	}

	{
		if (!png_lib_handle ||
				dl_png_create_write_struct == NULL ||
				dl_png_destroy_write_struct == NULL ||
				dl_png_create_info_struct == NULL ||
				dl_png_init_io == NULL ||
				dl_png_set_IHDR == NULL ||
				dl_png_set_bKGD == NULL ||
				dl_png_set_bgr == NULL ||
				dl_png_write_info == NULL ||
				dl_png_write_image == NULL ||
				dl_png_write_end == NULL) {
			COREGL_ERR("Can't trace surface : Failed to use libpng (recommend : 1.2.50-3.4)");
			goto finish;
		}

		if (trace_surface_sequence_sort_flag == 1)
			snprintf(name, sizeof(name), "[%d (%06d)%p-%p] %s %04d (%s).png", getpid(),
					 alldumpcount,
					 sdata->display, sdata->context, sdata->trace_data.name, sdata->dump_count,
					 position);
		else
			snprintf(name, sizeof(name), "[%d %p-%p] %s %04d (%s).png", getpid(),
					 sdata->display,
					 sdata->context, sdata->trace_data.name, sdata->dump_count, position);

		if (!strncmp(sdata->trace_data.name, "EGL", 3) && type != 2) {
			// EGL
			if (trace_surface_filter_type != 0 &&
					trace_surface_filter_type != 1) goto finish;;

			if (trace_surface_filter_handle != 0 &&
					trace_surface_filter_handle != (uintptr_t)sdata->surface) goto finish;

			EGLConfig eglconfig;
			GLint asize, rsize, gsize, bsize;
			_orig_tracepath_eglQuerySurface(sdata->display, sdata->surface, EGL_WIDTH,
											&width);
			_orig_tracepath_eglQuerySurface(sdata->display, sdata->surface, EGL_HEIGHT,
											&height);
			_orig_tracepath_eglQuerySurface(sdata->display, sdata->surface, EGL_CONFIG_ID,
											(GLint *)&eglconfig);
			_orig_tracepath_eglGetConfigAttrib(sdata->display, eglconfig, EGL_ALPHA_SIZE,
											   &asize);
			_orig_tracepath_eglGetConfigAttrib(sdata->display, eglconfig, EGL_RED_SIZE,
											   &rsize);
			_orig_tracepath_eglGetConfigAttrib(sdata->display, eglconfig, EGL_GREEN_SIZE,
											   &gsize);
			_orig_tracepath_eglGetConfigAttrib(sdata->display, eglconfig, EGL_BLUE_SIZE,
											   &bsize);
			channel = 4;
			if (asize == 0) channel = 3;
			if (bsize == 0) channel = 2;
			if (gsize == 0) channel = 1;
			if (rsize == 0) channel = 0;

			if (channel == 2) channel = 3;
			if (width <= 0 || height <= 0 || channel <= 0) goto finish;
			if (trace_surface_filter_size_w > 0 && trace_surface_filter_size_h > 0 &&
					(trace_surface_filter_size_w != width || trace_surface_filter_size_h != height))
				goto finish;

			if ((trace_surface_filter_period_begin > 0 ||
					trace_surface_filter_period_end > 0) &&
					(trace_surface_filter_period_begin > alldumpcount ||
					 trace_surface_filter_period_end < alldumpcount)) {
				alldumpcount++;
				sdata->dump_count++;
				goto finish;
			}

			if (channel == 3) channel = 4;

			TRACE("\E[40;31;1m[[TRACE SURFACE]] : '%s' is dumped (%dx%dx%d).\E[0m\n", name,
				  width, height, channel);
			if (trace_surface_print_only_flag == 1 && force_output == 0) {
				alldumpcount++;
				sdata->dump_count++;
				goto finish;
			}

			data = (unsigned char *)calloc(1,
										   width * height * channel * sizeof(unsigned char));
			if (data == NULL) {
				COREGL_ERR("Can't trace surface : Failed to allocate memory");
				goto finish;
			}

			GLint oldfb;
			_orig_tracepath_glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldfb);
			_orig_tracepath_glBindFramebuffer(GL_FRAMEBUFFER, 0);

			switch (channel) {
			case 4:
				_orig_tracepath_glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
											 data);
				break;
				//case 3: _orig_tracepath_glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data); break;
			case 1:
				_orig_tracepath_glReadPixels(0, 0, width, height, GL_ALPHA, GL_UNSIGNED_BYTE,
											 data);
				break;
			}

			_orig_tracepath_glBindFramebuffer(GL_FRAMEBUFFER, oldfb);
		}
		if (!strncmp(sdata->trace_data.name, "FBO", 3) && type != 1) {
			// FBO
			if (sdata->fbo == 0) goto finish;

			if (trace_surface_filter_type != 0 &&
					trace_surface_filter_type != 2) goto finish;

			if (trace_surface_filter_handle != 0 &&
					trace_surface_filter_handle != (uintptr_t)sdata->tex &&
					trace_surface_filter_handle != (uintptr_t)sdata->rb) goto finish;

			GLint oldfb;
			_orig_tracepath_glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldfb);
			_orig_tracepath_glBindFramebuffer(GL_FRAMEBUFFER, sdata->fbo);

			if (driver_gl_version >= 2)
				ret_value = _orig_tracepath_glCheckFramebufferStatus(GL_FRAMEBUFFER);
			else
				ret_value = _orig_tracepath_glCheckFramebufferStatusOES(GL_FRAMEBUFFER);
			if (ret_value == GL_FRAMEBUFFER_COMPLETE) {
				_orig_tracepath_glBindFramebuffer(GL_FRAMEBUFFER, oldfb);
				width = sdata->tex_w;
				height = sdata->tex_h;
				channel = sdata->tex_format;

				if (channel == 2) channel = 3;
				if (width <= 0 || height <= 0 || channel <= 0) goto finish;
				if (trace_surface_filter_size_w > 0 && trace_surface_filter_size_h > 0 &&
						(trace_surface_filter_size_w != width || trace_surface_filter_size_h != height))
					goto finish;

				if ((trace_surface_filter_period_begin > 0 ||
						trace_surface_filter_period_end > 0) &&
						(trace_surface_filter_period_begin > alldumpcount ||
						 trace_surface_filter_period_end < alldumpcount)) {
					alldumpcount++;
					sdata->dump_count++;
					goto finish;
				}

				TRACE("\E[40;31;1m[[TRACE SURFACE]] : '%s' is dumped (%dx%dx%d).\E[0m\n", name,
					  width, height, channel);
				if (trace_surface_print_only_flag == 1 && force_output == 0) {
					alldumpcount++;
					sdata->dump_count++;
					goto finish;
				}

				if (channel == 3) channel = 4;

				data = (unsigned char *)calloc(1,
											   width * height * channel * sizeof(unsigned char));
				if (data == NULL) {
					COREGL_ERR("Can't trace surface : Failed to allocate memory");
					goto finish;
				}

				_orig_tracepath_glBindFramebuffer(GL_FRAMEBUFFER, sdata->fbo);
				int atttype = _COREGL_INT_INIT_VALUE;
				if (driver_gl_version >= 2)
					_orig_tracepath_glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
							GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &atttype);
				else
					_orig_tracepath_glGetFramebufferAttachmentParameterivOES(GL_FRAMEBUFFER,
							GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &atttype);
				AST(atttype != sdata->tex);
				int attname = _COREGL_INT_INIT_VALUE;
				if (driver_gl_version >= 2)
					_orig_tracepath_glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
							GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &attname);
				else
					_orig_tracepath_glGetFramebufferAttachmentParameterivOES(GL_FRAMEBUFFER,
							GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &attname);
				switch (atttype) {
				case GL_TEXTURE:
					AST(attname == sdata->tex);
					break;
				case GL_RENDERBUFFER:
					AST(attname == sdata->rb);
					break;
				}

				switch (channel) {
				case 4:
					_orig_tracepath_glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
												 data);
					break;
					//case 3: _orig_tracepath_glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data); break;
				case 1:
					_orig_tracepath_glReadPixels(0, 0, width, height, GL_ALPHA, GL_UNSIGNED_BYTE,
												 data);
					break;
				}
			}
			_orig_tracepath_glBindFramebuffer(GL_FRAMEBUFFER, oldfb);
		}



		if (data == NULL) goto finish;

		unlink(name);
		write_fd = fopen(name, "wb");

		if (write_fd == NULL) {
			COREGL_ERR("Can't trace surface : Failed to create png file");
			goto finish;
		}

		rows = (png_byte **)malloc(height * sizeof(png_byte *));
		if (rows == NULL) {
			COREGL_ERR("Can't trace surface : Failed to allocate memory");
			goto finish;
		}

		for (int i = 0; i < height; i++) {
			rows[i] = data + (height - i - 1) * (width * channel);
		}

		png = dl_png_create_write_struct(PNG_LIBPNG_VER_STRING,
										 NULL,
										 NULL,
										 NULL);
		if (png == NULL) {
			COREGL_ERR("Can't trace surface : Failed to create write structure of png file");
			goto finish;
		}

		info = dl_png_create_info_struct(png);
		if (info == NULL) {
			COREGL_ERR("Can't trace surface : Failed to create info structure of png file");
			goto finish;
		}

		dl_png_init_io(png, write_fd);

		switch (channel) {
		case 4:
			dl_png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA,
							PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
			break;
			//case 3: dl_png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT); break;
		case 1:
			dl_png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_GRAY,
							PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
			break;
		}

		dl_png_write_info(png, info);

		dl_png_write_image(png, rows);

		dl_png_write_end(png, info);

		dl_png_destroy_write_struct(&png, &info);

		alldumpcount++;
		sdata->dump_count++;
	}

	goto finish;

finish:
	if (data != NULL) {
		free(data);
		data = NULL;
	}
	if (write_fd != NULL) {
		fclose(write_fd);
		write_fd = NULL;
	}
	if (rows != NULL) {
		free(rows);
		rows = NULL;
	}
	if (png != NULL) {
		if (info != NULL) {
			if (dl_png_destroy_write_struct)
				dl_png_destroy_write_struct(&png, &info);
			info = NULL;
		} else {
			if (dl_png_destroy_write_struct)
				dl_png_destroy_write_struct(&png, NULL);
		}
		png = NULL;
	}
}

void
tracepath_surface_trace_add(const char *desc, GLDisplay dpy, GLContext ctx,
							GLSurface surf, GLint fbo, GLint tex, GLint rb, GLint tex_w, GLint tex_h,
							GLint tex_format, const char *dump)
{
	Surface_Data *std = NULL;

	if (trace_surface_flag == 1) {
		AST(mutex_lock(&std_access_mutex) == 1);

		if (std_table == NULL) {
			std_table = (Surface_Data **)calloc(1,
												sizeof(Surface_Data *) * MAX_TRACE_TABLE_SIZE);
		}

		std = (Surface_Data *)_get_trace_data((Trace_Data **)std_table,
											  sizeof(Surface_Data), desc);

		AST(std != NULL);

		if (dump != NULL) {
			_dump_surface(0, 0, dump, std);
		}

		std->display = dpy;
		std->surface = surf;
		std->context = ctx;
		if (fbo >= 0) std->fbo = fbo;
		std->tex = tex;
		std->rb = rb;
		if (tex_w >= 0) std->tex_w = tex_w;
		if (tex_h >= 0) std->tex_h = tex_h;
		if (tex_format >= 0) std->tex_format = tex_format;

		AST(mutex_unlock(&std_access_mutex) == 1);

	}

}

void
tracepath_surface_trace(int force_output, int type, const char *position)
{
	int i;

	if (trace_surface_flag != 1) {
		goto finish;
	}

	AST(mutex_lock(&std_access_mutex) == 1);

	if (std_table != NULL) {
		EGLDisplay olddpy = _orig_tracepath_eglGetCurrentDisplay();
		EGLContext oldctx = _orig_tracepath_eglGetCurrentContext();
		EGLSurface oldsurf_read = _orig_tracepath_eglGetCurrentSurface(EGL_READ);
		EGLSurface oldsurf_draw = _orig_tracepath_eglGetCurrentSurface(EGL_DRAW);

		for (i = 0; i < MAX_TRACE_TABLE_SIZE; i++) {
			if (std_table[i] != NULL) {
				Surface_Data *current = std_table[i];

				while (current != NULL) {
					if (current->surface != EGL_NO_SURFACE && current->display != EGL_NO_DISPLAY &&
							current->context != EGL_NO_CONTEXT) {
						if (_orig_tracepath_eglMakeCurrent(current->display, current->surface,
														   current->surface, current->context) == EGL_TRUE) {
							_dump_surface(force_output, type, position, current);
						}
					}

					current = (Surface_Data *)current->trace_data.next;
				}
			}
		}
		_orig_tracepath_eglMakeCurrent(olddpy, oldsurf_read, oldsurf_draw, oldctx);

	}

	AST(mutex_unlock(&std_access_mutex) == 1);

	goto finish;

finish:
	return;
}


COREGL_API void
coregl_dump_surface()
{
	_COREGL_TRACE_SURFACE(1, 0, "USER CALL");
}

