#include "coregl_tracepath.h"

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <sys/types.h>
#include <unistd.h>

Mutex ctx_access_mutex = MUTEX_INITIALIZER;
Ctx_Data *ctx_data = NULL;

static Sostate_Data *
_get_sostate(GLContext ctx)
{
	Sostate_Data *ret = NULL;

	Ctx_Data *current = ctx_data;
	while (current != NULL) {
		if (current->handle == ctx) {
			current->sostate->ref_count++;
			ret = current->sostate;
			break;
		}
		current = current->next;
	}

	return ret;
}

#ifdef COREGL_TRACEPATH_TRACE_CONTEXT_INFO

static void
_dump_context_info(const char *ment, int force_output)
{
	MY_MODULE_TSTATE *tstate = NULL;
	static struct timeval tv_last = { 0, 0 };

	if (trace_ctx_flag != 1) return;

	AST(mutex_lock(&ctx_access_mutex) == 1);
	AST(mutex_lock(&general_trace_lists_access_mutex) == 1);

	if (!force_output && !trace_ctx_force_flag) {
		struct timeval tv_now = { 0, 0 };
		AST(gettimeofday(&tv_now, NULL) == 0);
		if (tv_now.tv_sec - tv_last.tv_sec < _COREGL_TRACE_OUTPUT_INTERVAL_SEC) {
			goto finish;
		}
		tv_last = tv_now;
	}

	GET_MY_TSTATE(tstate, get_current_thread_state());

	TRACE("\n");
	TRACE("\E[40;34m========================================================================================================================\E[0m\n");
	TRACE("\E[40;32;1m  Context info \E[1;37;1m: <PID = %d> %s\E[0m\n", getpid(),
		  ment);
	TRACE("\E[40;34m========================================================================================================================\E[0m\n");


	// Thread State List
	{
		General_Trace_List *current = NULL;
		current = thread_trace_list;

		while (current != NULL) {
			GLThreadState *cur_tstate = (GLThreadState *)current->value;
			MY_MODULE_TSTATE *cur_tstate_tm = NULL;

			GET_MY_TSTATE(cur_tstate_tm, cur_tstate);
			AST(cur_tstate_tm != NULL);

			if (cur_tstate_tm ! = NULL)
				continue;

			TRACE(" %c Thread  [0x%12x] : Surf <D=[%12p] R=[%12p]>",
				  (tstate == cur_tstate_tm) ? '*' : ' ',
				  cur_tstate->thread_id,
				  cur_tstate_tm->surf_draw,
				  cur_tstate_tm->surf_read);

			if (cur_tstate_tm->ctx != NULL) {
				TRACE(" EGLCTX=[%12p]\E[0m\n",
					  cur_tstate_tm->ctx->handle);
			} else {
				TRACE(" (NOT BINDED TO THREAD)\E[0m\n");
			}

			// Binded Context State List
			{
				Ctx_Data *current = NULL;
				current = ctx_data;

				while (current != NULL) {
					if (cur_tstate_tm->ctx == current) {
						TRACE("   -> EGLCTX [%12p] : EGLDPY=[%12p] <MC count [%10d]> <Ref [%2d]>\E[0m\n",
							  current->handle,
							  current->dpy,
							  current->mc_count,
							  current->ref_count);
					}

					current = current->next;
				}

			}


			current = current->next;
		}
	}

	TRACE("\E[40;33m........................................................................................................................\E[0m\n");

	// Not-binded Context State List
	{
		Ctx_Data *current = NULL;
		current = ctx_data;

		while (current != NULL) {
			int isbinded = 0;

			General_Trace_List *current_t = NULL;
			current_t = thread_trace_list;

			while (current_t != NULL) {
				GLThreadState *cur_tstate = (GLThreadState *)current_t->value;
				MY_MODULE_TSTATE *cur_tstate_tm = NULL;

				GET_MY_TSTATE(cur_tstate_tm, cur_tstate);
				AST(cur_tstate_tm != NULL);
				if (cur_tstate_tm != NULL && cur_tstate_tm->ctx == current) {
					isbinded = 1;
					break;
				}
				current_t = current_t->next;
			}

			if (isbinded == 0) {
				TRACE("   EGLCTX    [%12p] : EGLDPY=[%12p] <MC count [%10d]> <Ref [%2d]>\E[0m\n",
					  current->handle,
					  current->dpy,
					  current->mc_count,
					  current->ref_count);
			}

			current = current->next;
		}

	}

	TRACE("\E[40;34m========================================================================================================================\E[0m\n");
	TRACE("\n");

	TRACE_END();

	goto finish;

finish:

	AST(mutex_unlock(&general_trace_lists_access_mutex) == 1);
	AST(mutex_unlock(&ctx_access_mutex) == 1);

}

#endif // COREGL_TRACEPATH_TRACE_CONTEXT_INFO


void
tracepath_add_context(GLContext ctx, GLDisplay dpy, GLContext share_ctx)
{
	Ctx_Data *current = NULL;
	Ctx_Data *data = NULL;

	AST(mutex_lock(&ctx_access_mutex) == 1);

	current = ctx_data;

	while (current != NULL) {
		if (current->handle == ctx) {
			data = current;
			break;
		}
		current = current->next;
	}

	if (data == NULL) {
		data = (Ctx_Data *)calloc(1, sizeof(Ctx_Data));
		data->ref_count = 1;
		data->handle = ctx;
		data->dpy = dpy;

		data->sostate = _get_sostate(share_ctx);
		if (data->sostate == NULL) {
			data->sostate = (Sostate_Data *)calloc(1, sizeof(Sostate_Data));
			data->sostate->ref_count = 1;
		}

		if (ctx_data != NULL)
			data->next = ctx_data;

		ctx_data = data;
	}
	goto finish;

finish:
	AST(mutex_unlock(&ctx_access_mutex) == 1);
	return;
}

Ctx_Data *
tracepath_get_context(GLContext ctx)
{
	Ctx_Data *current = NULL;
	Ctx_Data *data = NULL;

	AST(mutex_lock(&ctx_access_mutex) == 1);

	current = ctx_data;

	while (current != NULL) {
		if (current->handle == ctx) {
			data = current;
			break;
		}
		current = current->next;
	}
	if (data == NULL) {
		COREGL_WARN("Error making context [%p] current. (invalid EGL context)", ctx);
		goto finish;
	}
	data->ref_count++;
	goto finish;

finish:
	AST(mutex_unlock(&ctx_access_mutex) == 1);
	return data;
}

void
tracepath_remove_context(GLContext ctx)
{
	Ctx_Data *current = NULL;
	Ctx_Data *prev = NULL;

	AST(mutex_lock(&ctx_access_mutex) == 1);

	current = ctx_data;

	while (current != NULL) {
		if (current->handle == ctx) {
			if (--current->ref_count <= 0) {
				if (prev != NULL)
					prev->next = current->next;
				else
					ctx_data = current->next;

				if (--current->sostate->ref_count <= 0) {
					tracepath_glbuf_clear(current->sostate->glbuf_rb);
					tracepath_glbuf_clear(current->sostate->glbuf_tex);
					free(current->sostate);
					current->sostate = NULL;
				}

				free(current);
				current = NULL;
			}
			break;
		}
		prev = current;
		current = current->next;
	}
	goto finish;

finish:
	AST(mutex_unlock(&ctx_access_mutex) == 1);
	return;
}

EGLint
tracepath_eglGetError(void)
{
	EGLint ret = _COREGL_INT_INIT_VALUE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglGetError();
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLDisplay
tracepath_eglGetDisplay(EGLNativeDisplayType display_id)
{
	EGLDisplay ret = EGL_NO_DISPLAY;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglGetDisplay(display_id);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor)
{
	EGLBoolean ret = EGL_FALSE;
	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglInitialize(dpy, major, minor);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglTerminate(EGLDisplay dpy)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglTerminate(dpy);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglGetConfigs(EGLDisplay dpy, EGLConfig *configs, EGLint config_size,
						EGLint *num_config)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglGetConfigs(dpy, configs, config_size, num_config);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglChooseConfig(EGLDisplay dpy, const EGLint *attrib_list,
						  EGLConfig *configs, EGLint config_size, EGLint *num_config)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglChooseConfig(dpy, attrib_list, configs, config_size,
										  num_config);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglGetConfigAttrib(EGLDisplay dpy, EGLConfig config, EGLint attribute,
							 EGLint *value)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglGetConfigAttrib(dpy, config, attribute, value);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}


EGLSurface
tracepath_eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config,
								 EGLNativeWindowType win, const EGLint *attrib_list)
{
	EGLSurface ret = EGL_NO_SURFACE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglCreateWindowSurface(dpy, config, win, attrib_list);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLSurface
tracepath_eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config,
								  const EGLint *attrib_list)
{
	EGLSurface ret = EGL_NO_SURFACE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglCreatePbufferSurface(dpy, config, attrib_list);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLSurface
tracepath_eglCreatePixmapSurface(EGLDisplay dpy, EGLConfig config,
								 EGLNativePixmapType pixmap, const EGLint *attrib_list)
{
	EGLSurface ret = EGL_NO_SURFACE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglCreatePixmapSurface(dpy, config, pixmap, attrib_list);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglDestroySurface(EGLDisplay dpy, EGLSurface surface)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglDestroySurface(dpy, surface);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
#ifdef COREGL_TRACEPATH_TRACE_SURFACE_INFO
	{
		char name[256];
		snprintf(name, sizeof(name), "EGLSURFACE_%p", surface);
		tracepath_surface_trace_add(name, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL);
	}
#endif // COREGL_TRACEPATH_TRACE_SURFACE_INFO
	return ret;
}

EGLBoolean
tracepath_eglQuerySurface(EGLDisplay dpy, EGLSurface surface, EGLint attribute,
						  EGLint *value)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglQuerySurface(dpy, surface, attribute, value);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglBindAPI(EGLenum api)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglBindAPI(api);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLenum
tracepath_eglQueryAPI(void)
{
	EGLenum ret = 0;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglQueryAPI();
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglWaitClient(void)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglWaitClient();
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglReleaseThread(void)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglReleaseThread();
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLSurface
tracepath_eglCreatePbufferFromClientBuffer(EGLDisplay dpy, EGLenum buftype,
		EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list)
{
	EGLSurface ret = EGL_NO_SURFACE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglCreatePbufferFromClientBuffer(dpy, buftype, buffer,
			config, attrib_list);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglSurfaceAttrib(EGLDisplay dpy, EGLSurface surface, EGLint attribute,
						   EGLint value)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglSurfaceAttrib(dpy, surface, attribute, value);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglBindTexImage(dpy, surface, buffer);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglReleaseTexImage(dpy, surface, buffer);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglSwapInterval(EGLDisplay dpy, EGLint interval)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglSwapInterval(dpy, interval);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLContext
tracepath_eglCreateContext(EGLDisplay dpy, EGLConfig config,
						   EGLContext share_context, const EGLint *attrib_list)
{
	EGLContext ret = EGL_NO_CONTEXT;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglCreateContext(dpy, config, share_context, attrib_list);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	{
		if (ret != EGL_NO_CONTEXT) {
			tracepath_add_context(ret, dpy, share_context);
		}
	}
#ifdef COREGL_TRACEPATH_TRACE_CONTEXT_INFO
	if (unlikely(trace_ctx_flag == 1)) {
		if (_orig_tracepath_eglCreateContext == _sym_eglCreateContext) {
			char ment[256];
			snprintf(ment, sizeof(ment), "eglCreateContext completed (EGLCTX=[%12p])", ret);
			_dump_context_info(ment, 1);
		}
	}
#endif // COREGL_TRACEPATH_TRACE_CONTEXT_INFO
	return ret;
}

EGLBoolean
tracepath_eglDestroyContext(EGLDisplay dpy, EGLContext ctx)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglDestroyContext(dpy, ctx);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	{
		AST(ctx != EGL_NO_CONTEXT);

		tracepath_remove_context(ctx);
	}
#ifdef COREGL_TRACEPATH_TRACE_CONTEXT_INFO
	if (unlikely(trace_ctx_flag == 1)) {
		if (_orig_tracepath_eglDestroyContext == _sym_eglDestroyContext) {
			char ment[256];
			snprintf(ment, sizeof(ment), "eglDestroyContext completed (EGLCTX=[%12p])",
					 ctx);
			_dump_context_info(ment, 1);
		}
	}
#endif // COREGL_TRACEPATH_TRACE_CONTEXT_INFO
	return ret;
}

EGLBoolean
tracepath_eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read,
						 EGLContext ctx)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglMakeCurrent(dpy, draw, read, ctx);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	{
		MY_MODULE_TSTATE *tstate = NULL;

		GET_MY_TSTATE(tstate, get_current_thread_state());
		if (tstate == NULL) {
			init_new_thread_state();

			GET_MY_TSTATE(tstate, get_current_thread_state());
			AST(tstate != NULL);
		}

		if (tstate) {

			Ctx_Data *oldctx = tstate->ctx;

			if (ctx != EGL_NO_CONTEXT) {
				tstate->ctx = tracepath_get_context(ctx);
				if (tstate->ctx != NULL)
					tstate->ctx->mc_count++;
			} else {
				tstate->ctx = NULL;
			}

			if (oldctx != NULL)
				tracepath_remove_context(oldctx->handle);

			tstate->surf_draw = draw;
			tstate->surf_read = read;
		}
	}
#ifdef COREGL_TRACEPATH_TRACE_STATE_INFO
	if (unlikely(trace_state_flag == 1)) {
		if (_orig_tracepath_eglMakeCurrent == _sym_eglMakeCurrent)
			tracepath_dump_context_states(0);
	}
#endif // COREGL_TRACEPATH_TRACE_STATE_INFO
#ifdef COREGL_TRACEPATH_TRACE_CONTEXT_INFO
	if (unlikely(trace_ctx_flag == 1)) {
		if (_orig_tracepath_eglMakeCurrent == _sym_eglMakeCurrent) {
			char ment[256];
			snprintf(ment, sizeof(ment),
					 "eglMakeCurrent finished (EGLCTX=[%12p] Surf=[D:%12p R:%12p])",
					 ctx, draw, read);
			_dump_context_info(ment, 0);
		}
	}
#endif // COREGL_TRACEPATH_TRACE_CONTEXT_INFO
	return ret;
}

EGLContext
tracepath_eglGetCurrentContext(void)
{
	EGLContext ret = EGL_NO_CONTEXT;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglGetCurrentContext();
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLSurface
tracepath_eglGetCurrentSurface(EGLint readdraw)
{
	EGLSurface ret = EGL_NO_SURFACE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglGetCurrentSurface(readdraw);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLDisplay
tracepath_eglGetCurrentDisplay(void)
{
	EGLDisplay ret = EGL_NO_DISPLAY;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglGetCurrentDisplay();
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglQueryContext(EGLDisplay dpy, EGLContext ctx, EGLint attribute,
						  EGLint *value)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglQueryContext(dpy, ctx, attribute, value);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglWaitGL(void)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglWaitGL();

	_COREGL_TRACE_SURFACE(0, 1, "EGLWAITGL");

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglWaitNative(EGLint engine)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglWaitNative(engine);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACE_SURFACE(0, 1, "SWAPBUFFERS");

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglSwapBuffers(dpy, surface);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	if (unlikely(trace_api_frame_flag == 1)) {
		if (unlikely(trace_api_all_flag == 1)) {
			_COREGL_TRACE_API_OUTPUT(1);
		} else {
			_COREGL_TRACE_API_OUTPUT(0);
		}
		_COREGL_TRACE_API_RESET_FRAME();
	} else {
		_COREGL_TRACE_API_OUTPUT(0);
	}
	_COREGL_TRACE_MEM_OUTPUT(0);
	return ret;
}

EGLBoolean
tracepath_eglSwapBuffersWithDamageEXT(EGLDisplay dpy, EGLSurface surface,
									  EGLint *rects, EGLint n_rects)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACE_SURFACE(0, 1, "SWAPBUFFERS");

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglSwapBuffersWithDamageEXT(dpy, surface, rects, n_rects);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	if (unlikely(trace_api_frame_flag == 1)) {
		if (unlikely(trace_api_all_flag == 1)) {
			_COREGL_TRACE_API_OUTPUT(1);
		} else {
			_COREGL_TRACE_API_OUTPUT(0);
		}
		_COREGL_TRACE_API_RESET_FRAME();
	} else {
		_COREGL_TRACE_API_OUTPUT(0);
	}
	_COREGL_TRACE_MEM_OUTPUT(0);
	return ret;
}

EGLBoolean
tracepath_eglSwapBuffersRegionEXT(EGLDisplay dpy, EGLSurface surface,
								  EGLint numRects, const EGLint *rects)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACE_SURFACE(0, 1, "SWAPBUFFERS");

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglSwapBuffersRegionEXT(dpy, surface, numRects, rects);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	if (unlikely(trace_api_frame_flag == 1)) {
		if (unlikely(trace_api_all_flag == 1)) {
			_COREGL_TRACE_API_OUTPUT(1);
		} else {
			_COREGL_TRACE_API_OUTPUT(0);
		}
		_COREGL_TRACE_API_RESET_FRAME();
	} else {
		_COREGL_TRACE_API_OUTPUT(0);
	}
	_COREGL_TRACE_MEM_OUTPUT(0);
	return ret;
}

EGLBoolean
tracepath_eglCopyBuffers(EGLDisplay dpy, EGLSurface surface,
						 EGLNativePixmapType target)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglCopyBuffers(dpy, surface, target);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

_eng_fn
tracepath_eglGetProcAddress(const char *procname)
{
	_eng_fn ret = NULL;

	_COREGL_TRACEPATH_FUNC_BEGIN();

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST) \
	if (strcmp(procname, #FUNC_NAME) == 0) \
	{ \
		_eng_fn ret_orig = NULL; \
		ret_orig = _orig_tracepath_eglGetProcAddress(procname); \
		if (ret_orig != NULL) \
			ret = (_eng_fn)ovr_##FUNC_NAME; \
		goto finish; \
	}

#define _COREGL_EXT_SYMBOL_ALIAS(ALIAS_NAME, FUNC_NAME) \
   if (strcmp(procname, #ALIAS_NAME) == 0) \
   { \
		_eng_fn ret_orig = NULL; \
		ret_orig = _orig_tracepath_eglGetProcAddress(#ALIAS_NAME); \
		if (ret_orig != NULL) \
			ret = (_eng_fn)ovr_##FUNC_NAME; \
		goto finish; \
   }

#include "../../headers/sym_egl.h"
#include "../../headers/sym_gl1.h"
#include "../../headers/sym_gl2.h"
#undef _COREGL_SYMBOL
#undef _COREGL_EXT_SYMBOL_ALIAS

	ret = _orig_tracepath_eglGetProcAddress(procname);
	if (ret != NULL) {
		COREGL_WARN("TRACEPATH can't support '%s' (tracing for this function will be ignored)",
					procname);
	}

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

const char *
tracepath_eglQueryString(EGLDisplay dpy, EGLint name)
{
	const char *ret = NULL;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglQueryString(dpy, name);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLImageKHR
tracepath_eglCreateImageKHR (EGLDisplay dpy, EGLContext ctx, EGLenum target,
							 EGLClientBuffer buffer, const EGLint *attrib_list)
{
	void *ret = NULL;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglCreateImageKHR(dpy, ctx, target, buffer, attrib_list);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglDestroyImageKHR(EGLDisplay dpy, EGLImageKHR image)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglDestroyImageKHR(dpy, image);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

void *
tracepath_eglMapImageSEC(EGLDisplay dpy, EGLImageKHR image, EGLint device_type,
						 EGLint access_option)
{
	void *ret = NULL;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglMapImageSEC(dpy, image, device_type, access_option);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglUnmapImageSEC(EGLDisplay dpy, EGLImageKHR image,
						   EGLint device_type)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglUnmapImageSEC(dpy, image, device_type);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglGetImageAttribSEC(EGLDisplay dpy, EGLImageKHR image,
							   EGLint attribute, EGLint *value)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglGetImageAttribSEC(dpy, image, attribute, value);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglLockSurfaceKHR(EGLDisplay display, EGLSurface surface,
							const EGLint *attrib_list)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglLockSurfaceKHR(display, surface, attrib_list);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglUnlockSurfaceKHR(EGLDisplay display, EGLSurface surface)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglUnlockSurfaceKHR(display, surface);
	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglBindWaylandDisplayWL(EGLDisplay dpy, void *display)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglBindWaylandDisplayWL(dpy, display);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglUnbindWaylandDisplayWL(EGLDisplay dpy, void *display)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglUnbindWaylandDisplayWL(dpy, display);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglQueryWaylandBufferWL(EGLDisplay dpy, void *buffer,
								  EGLint attribute, EGLint *value)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglQueryWaylandBufferWL(dpy, buffer, attribute, value);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglSetDamageRegionKHR(EGLDisplay dpy, EGLSurface surface,
								EGLint *rects, EGLint n_rects)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglSetDamageRegionKHR(dpy, surface, rects, n_rects);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}

EGLBoolean
tracepath_eglSwapBuffersWithDamageKHR(EGLDisplay dpy, EGLSurface surface,
									  EGLint *rects, EGLint n_rects)
{
	EGLBoolean ret = EGL_FALSE;

	_COREGL_TRACEPATH_FUNC_BEGIN();
	ret = _orig_tracepath_eglSwapBuffersWithDamageKHR(dpy, surface, rects, n_rects);

	goto finish;

finish:
	_COREGL_TRACEPATH_FUNC_END();
	return ret;
}
