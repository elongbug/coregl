#include "coregl_fastpath.h"

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <execinfo.h>

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>
#include "../../../include_KHR/EGL/eglext.h"

#ifdef COREGL_FASTPATH_TRACE_CONTEXT_INFO

General_Trace_List *glue_ctx_trace_list = NULL;
General_Trace_List *context_state_trace_list = NULL;
int current_gl_api_version = 0;

static void
_dump_context_info(const char *ment, int force_output)
{
	MY_MODULE_TSTATE *tstate = NULL;
	static struct timeval tv_last = { 0, 0 };

	if (trace_ctx_flag != 1) return;

	AST(mutex_lock(&ctx_list_access_mutex) == 1);
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

			TRACE(" %c Thread  [0x%12x] : Surf <D=[%12p] R=[%12p]>",
				  (tstate == cur_tstate_tm) ? '*' : ' ',
				  cur_tstate->thread_id,
				  cur_tstate_tm->rsurf_draw,
				  cur_tstate_tm->rsurf_read);

			if (cur_tstate_tm->cstate != NULL) {
				TRACE(" GlueCTX=[%12p] RealCTX=[%12p]\E[0m\n",
					  cur_tstate_tm->cstate->data,
					  cur_tstate_tm->cstate);
			} else {
				TRACE(" (NOT BINDED TO THREAD)\E[0m\n");
			}

			// Binded Context State List
			{
				General_Trace_List *current = NULL;
				current = context_state_trace_list;

				while (current != NULL) {
					GLContextState *cur_cstate = (GLContextState *)current->value;
					MY_MODULE_TSTATE *cur_tstate_tm = NULL;

					GET_MY_TSTATE(cur_tstate_tm, cur_tstate);
					AST(cur_tstate_tm != NULL);

					if (cur_tstate_tm->cstate == cur_cstate) {
						TRACE("   -> RealCTX [%12p] : EGLDPY=[%12p] EGLCTX=[%12p] <[%2d] GlueCTXs>\E[0m\n",
							  cur_cstate,
							  cur_cstate->rdpy,
							  cur_cstate->rctx,
							  cur_cstate->ref_count);

						// Binded Glue Context List
						{
							General_Trace_List *current = NULL;
							current = glue_ctx_trace_list;

							while (current != NULL) {
								GLGlueContext *cur_gctx = (GLGlueContext *)current->value;

								if (cur_gctx->cstate == cur_cstate) {
									TRACE("    -%c GlueCTX [%12p] : EGLDPY=[%12p] TID=[%12d] <MC count [%10d]>",
										  (cur_cstate->data == cur_gctx) ? '>' : '-',
										  cur_gctx,
										  cur_gctx->rdpy,
										  cur_gctx->thread_id,
										  cur_gctx->used_count);

									TRACE(" <Ref [%2d]>\E[0m\n", cur_gctx->ref_count);
								}

								current = current->next;
							}
						}
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
		General_Trace_List *current = NULL;
		current = context_state_trace_list;

		while (current != NULL) {
			GLContextState *cur_cstate = (GLContextState *)current->value;

			int isbinded = 0;

			if (cur_cstate->data != NULL) {
				General_Trace_List *current_t = NULL;
				current_t = thread_trace_list;

				while (current_t != NULL) {
					GLThreadState *cur_tstate = (GLThreadState *)current_t->value;
					MY_MODULE_TSTATE *cur_tstate_tm = NULL;

					GET_MY_TSTATE(cur_tstate_tm, cur_tstate);
					AST(cur_tstate_tm != NULL);

					if (cur_tstate->thread_id == ((GLGlueContext *)cur_cstate->data)->thread_id) {
						if (cur_tstate_tm->cstate == cur_cstate)
							isbinded = 1;
						break;
					}
					current_t = current_t->next;
				}
			}

			if (isbinded == 0) {
				TRACE("   RealCTX   [%12p] : EGLDPY=[%12p] EGLCTX=[%12p] <[%2d] GlueCTXs>\E[0m\n",
					  cur_cstate,
					  cur_cstate->rdpy,
					  cur_cstate->rctx,
					  cur_cstate->ref_count);

				// Binded Glue Context List
				{
					General_Trace_List *current = NULL;
					current = glue_ctx_trace_list;

					while (current != NULL) {
						GLGlueContext *cur_gctx = (GLGlueContext *)current->value;

						if (cur_gctx->cstate == cur_cstate) {
							TRACE("    -%c GlueCTX [%12p] : EGLDPY=[%12p] TID=[%12d] <MC count [%10d]>",
								  (cur_cstate->data == cur_gctx) ? '>' : '-',
								  cur_gctx,
								  cur_gctx->rdpy,
								  cur_gctx->thread_id,
								  cur_gctx->used_count);

							TRACE(" <Ref [%2d]>\E[0m\n", cur_gctx->ref_count);
						}

						current = current->next;
					}
				}
			}

			current = current->next;
		}

	}

	TRACE("\E[40;33m........................................................................................................................\E[0m\n");

	// Not-binded Glue Context List
	{
		General_Trace_List *current = NULL;
		current = glue_ctx_trace_list;

		while (current != NULL) {
			GLGlueContext *cur_gctx = (GLGlueContext *)current->value;

			if (cur_gctx->cstate == NULL) {
				TRACE("   GlueCTX [%12p]   : EGLDPY=[%12p] TID=[%12d] <MC count [%10d]>",
					  cur_gctx,
					  cur_gctx->rdpy,
					  cur_gctx->thread_id,
					  cur_gctx->used_count);

				TRACE(" <Ref [%2d]>\E[0m\n", cur_gctx->ref_count);
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
	AST(mutex_unlock(&ctx_list_access_mutex) == 1);

}

#endif // COREGL_FASTPATH_TRACE_CONTEXT_INFO


typedef struct {
	EGLint                        context_major_version;
	EGLint                        context_minor_version;
	EGLint                        context_flags;
	EGLint                        context_opengl_profile_mask;
	EGLint                        opengl_reset_notification_strategy_khr;
	EGLint                        opengl_reset_notification_strategy_ext;
} EGL_packed_attrib_list;

typedef struct {
	EGLDisplay                    dpy;
	EGLConfig                     cfg;
	EGL_packed_attrib_list        attrib_list;
	EGLint								force_unique;
} EGL_packed_option;

typedef struct {
	EGLContext                    share_context;
} EGL_packed_sharable_option;


static int
_pack_egl_context_option(EGL_packed_option *pack_data, EGLDisplay dpy,
						 EGLConfig cfg, EGLint force_unique, const EGLint *attrib_list)
{
	static int force_unique_free_id = 0;
	int ret = 0;

	pack_data->dpy = dpy;
	pack_data->cfg = cfg;
	if (force_unique != 0)
		pack_data->force_unique = force_unique_free_id++;

	// Default context attributes
	pack_data->attrib_list.context_major_version = 1;
	pack_data->attrib_list.context_minor_version = 0;
	pack_data->attrib_list.context_flags = EGL_DONT_CARE;
	pack_data->attrib_list.context_opengl_profile_mask = EGL_DONT_CARE;
	pack_data->attrib_list.opengl_reset_notification_strategy_khr = EGL_DONT_CARE;
	pack_data->attrib_list.opengl_reset_notification_strategy_ext = EGL_DONT_CARE;

	// Apply specified attributes
	EGLint *attrib = (EGLint *)attrib_list;
	while ((attrib != NULL) && (attrib[0] != EGL_NONE)) {
		switch (attrib[0]) {
		case EGL_CONTEXT_MAJOR_VERSION_KHR: // EGL_CONTEXT_CLIENT_VERSION
			pack_data->attrib_list.context_major_version = attrib[1];
			break;
		case EGL_CONTEXT_MINOR_VERSION_KHR:
			pack_data->attrib_list.context_minor_version = attrib[1];
			break;
		case EGL_CONTEXT_FLAGS_KHR:
			pack_data->attrib_list.context_flags = attrib[1];
			break;
		case EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR:
			pack_data->attrib_list.context_opengl_profile_mask = attrib[1];
			break;
		case EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_KHR:
			pack_data->attrib_list.opengl_reset_notification_strategy_khr = attrib[1];
			break;
		case EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_EXT:
			pack_data->attrib_list.opengl_reset_notification_strategy_ext = attrib[1];
			break;
		default:
			COREGL_WARN("Invalid context attribute.");
			goto finish;
		}
		attrib += 2;
	}

	// Eject condition for context version
	if (pack_data->attrib_list.context_major_version != 2) {
		pack_data->force_unique = force_unique_free_id;
	}

	ret = 1;
	goto finish;

finish:
	return ret;
}

static int
_unpack_egl_context_option(EGL_packed_option *pack_data, EGLDisplay *dpy,
						   EGLConfig *cfg, EGLint *force_unique, EGLint *attrib_list,
						   const int attrib_list_size)
{
	int ret = 0;

	if (dpy != NULL) *dpy = pack_data->dpy;
	if (cfg != NULL) *cfg = pack_data->cfg;
	if (force_unique != NULL) *force_unique = pack_data->force_unique;

	if (attrib_list != NULL && attrib_list_size > 0) {
		int attrib_list_index = 0;

		memset(attrib_list, 0x00, sizeof(int) * attrib_list_size);

		if (pack_data->attrib_list.context_major_version != EGL_DONT_CARE) {
			AST(attrib_list_index + 2 < attrib_list_size);
			attrib_list[attrib_list_index] = EGL_CONTEXT_MAJOR_VERSION_KHR;
			attrib_list[attrib_list_index + 1] =
				pack_data->attrib_list.context_major_version;
			attrib_list_index += 2;
		}
		if (pack_data->attrib_list.context_minor_version != EGL_DONT_CARE) {
			AST(attrib_list_index + 2 < attrib_list_size);
			attrib_list[attrib_list_index] = EGL_CONTEXT_MINOR_VERSION_KHR;
			attrib_list[attrib_list_index + 1] =
				pack_data->attrib_list.context_minor_version;
			attrib_list_index += 2;
		}
		if (pack_data->attrib_list.context_flags != EGL_DONT_CARE) {
			AST(attrib_list_index + 2 < attrib_list_size);
			attrib_list[attrib_list_index] = EGL_CONTEXT_FLAGS_KHR;
			attrib_list[attrib_list_index + 1] = pack_data->attrib_list.context_flags;
			attrib_list_index += 2;
		}
		if (pack_data->attrib_list.context_opengl_profile_mask != EGL_DONT_CARE) {
			AST(attrib_list_index + 2 < attrib_list_size);
			attrib_list[attrib_list_index] = EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR;
			attrib_list[attrib_list_index + 1] =
				pack_data->attrib_list.context_opengl_profile_mask;
			attrib_list_index += 2;
		}
		if (pack_data->attrib_list.opengl_reset_notification_strategy_khr !=
				EGL_DONT_CARE) {
			AST(attrib_list_index + 2 < attrib_list_size);
			attrib_list[attrib_list_index] =
				EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_KHR;
			attrib_list[attrib_list_index + 1] =
				pack_data->attrib_list.opengl_reset_notification_strategy_khr;
			attrib_list_index += 2;
		}
		if (pack_data->attrib_list.opengl_reset_notification_strategy_ext !=
				EGL_DONT_CARE) {
			AST(attrib_list_index + 2 < attrib_list_size);
			attrib_list[attrib_list_index] =
				EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_EXT;
			attrib_list[attrib_list_index + 1] =
				pack_data->attrib_list.opengl_reset_notification_strategy_ext;
			attrib_list_index += 2;
		}

		attrib_list[attrib_list_index] = EGL_NONE;
	}
	ret = 1;
	goto finish;

finish:
	return ret;
}

static int
_pack_egl_context_sharable_option(EGL_packed_sharable_option *pack_data,
								  EGLContext share_context)
{
	int ret = 0;

	pack_data->share_context = share_context;

	ret = 1;
	goto finish;

finish:
	return ret;
}

#if 0
static int
_unpack_egl_context_sharable_option(EGL_packed_sharable_option *pack_data,
									EGLContext *share_context)
{
	int ret = 0;

	if (share_context != NULL) *share_context = pack_data->share_context;

	ret = 1;
	goto finish;

finish:
	return ret;
}
#endif

static void
_link_context_state(GLGlueContext *gctx, GLContextState *cstate)
{
	AST(gctx->cstate == NULL);
	cstate->ref_count++;
	gctx->cstate = cstate;
}

static void
_unlink_context_state(GLGlueContext *gctx, Mutex *ctx_list_mtx)
{
	GLContextState *cstate = NULL;

	cstate = gctx->cstate;
	AST(cstate != NULL);

	cstate->ref_count--;
	AST(cstate->ref_count >= 0);

	if (cstate->ref_count == 0) {
		MY_MODULE_TSTATE *tstate = NULL;

		AST(cstate->data == NULL || cstate->data == initial_ctx);

#ifdef COREGL_FASTPATH_TRACE_CONTEXT_INFO
		if (unlikely(trace_ctx_flag == 1))
			remove_from_general_trace_list(&context_state_trace_list, cstate);
#endif // COREGL_FASTPATH_TRACE_CONTEXT_INFO

		AST(fastpath_remove_context_states_from_list(cstate, ctx_list_mtx) == 1);

		GET_MY_TSTATE(tstate, get_current_thread_state());
		if (tstate != NULL) {
			if (tstate->cstate == cstate) {
				_orig_fastpath_eglMakeCurrent(cstate->rdpy, tstate->rsurf_draw,
											  tstate->rsurf_read, EGL_NO_CONTEXT);
				tstate->cstate = NULL;
			}
		}
		_orig_fastpath_eglDestroyContext(cstate->rdpy, cstate->rctx);
		free(cstate);
		cstate = NULL;
	}
}

static void
_add_shared_obj_state_ref(GLGlueContext *gctx, GL_Shared_Object_State *sostate)
{
	AST(sostate->ref_count >= 0);
	sostate->ref_count++;
	add_to_general_trace_list(&sostate->using_gctxs, gctx);
}

static void
_remove_shared_obj_state_ref(GLGlueContext *gctx,
							 GL_Shared_Object_State *sostate)
{
	remove_from_general_trace_list(&sostate->using_gctxs, gctx);

	// Restore attached states
	fastpath_release_gl_context(gctx);

	AST(sostate->ref_count > 0);
	sostate->ref_count--;
	if (sostate->ref_count == 0) {
		fastpath_sostate_deinit(sostate);
		free(sostate);
	}
}

static void
_add_context_ref(GLGlueContext *gctx)
{
	if (gctx == initial_ctx) return;

	AST(gctx->ref_count >= 0);
	gctx->ref_count++;
}

static void
_remove_context_ref(GLGlueContext *gctx, Mutex *ctx_list_mtx)
{
	if (gctx == initial_ctx) return;

	AST(gctx->ref_count > 0);
	gctx->ref_count--;
	if (gctx->ref_count == 0) {
#ifdef COREGL_FASTPATH_TRACE_CONTEXT_INFO
		if (unlikely(trace_ctx_flag == 1))
			remove_from_general_trace_list(&glue_ctx_trace_list, gctx);
#endif // COREGL_FASTPATH_TRACE_CONTEXT_INFO

		AST(gctx->cstate != NULL);
		if (gctx->cstate->data == gctx)
			gctx->cstate->data = NULL;

		_unlink_context_state(gctx, ctx_list_mtx);

		AST(gctx->ostate.shared != NULL);
		_remove_shared_obj_state_ref(gctx, gctx->ostate.shared);
		gctx->ostate.shared = NULL;

		fastpath_ostate_deinit(&gctx->ostate);

		if (gctx->real_ctx_option != NULL) {
			free(gctx->real_ctx_option);
			gctx->real_ctx_option = NULL;
		}
		if (gctx->real_ctx_sharable_option != NULL) {
			free(gctx->real_ctx_sharable_option);
			gctx->real_ctx_sharable_option = NULL;
		}

# define GLUE_STATE(TYPE, NAME, SIZE, ARRAY_SIZE, DEFAULT_STMT, GET_STMT)  \
	if (gctx->NAME) {free(gctx->NAME); gctx->NAME = NULL;}	\
	if (gctx->NAME##_updated){free(gctx->NAME##_updated); gctx->NAME##_updated = NULL;}
#  include "coregl_fastpath_state.h"
#undef GLUE_STATE
		free(gctx);

		{
			GLGlueContext_List *current = NULL;

			AST(mutex_lock(&ctx_list_access_mutex) == 1);

			current = gctx_list;

			while (current != NULL) {
				if (current->gctx == gctx) {
					if (current->next != NULL)
						current->next->prev = current->prev;

					if (current->prev != NULL)
						current->prev->next = current->next;
					else
						gctx_list = current->next;

					free(current);

					break;
				}
				current = current->next;
			}

			AST(mutex_unlock(&ctx_list_access_mutex) == 1);
		}
	}
}

static int
_bind_context_state(GLGlueContext *gctx, GLContextState *cstate,
					Mutex *ctx_list_mtx)
{
	if (gctx != NULL) {
		AST(gctx->cstate == cstate);
	}

	if (cstate->data != gctx) {
		GLGlueContext *curctx = (GLGlueContext *)cstate->data;
		GLGlueContext *newctx = gctx;

		if (newctx == NULL)
			newctx = initial_ctx;

		if (curctx == NULL)
			curctx = initial_ctx;

		AST(newctx != NULL);
		AST(curctx != NULL);

		if (!fastpath_make_context_current(curctx, newctx))
			return 0;

		cstate->data = (void *)newctx;
		_remove_context_ref(curctx, ctx_list_mtx);
		_add_context_ref(newctx);
	}

	return 1;

}

GLContextState *
_egl_create_context(EGL_packed_option *real_ctx_option,
					GLContextState **cstate_new, EGLContext *ctx,
					EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list)
{
	GLContextState *cstate = NULL;

	AST(real_ctx_option != NULL);
	AST(cstate_new != NULL);
	AST(ctx != NULL);

	// Pack context option
	AST(_pack_egl_context_option(real_ctx_option, dpy, config, 0,
								 attrib_list) == 1);

	if (debug_nofp == 1) {
		AST(_pack_egl_context_option(real_ctx_option, dpy, config, 1,
									 attrib_list) == 1);
	}

	// Find context state
	if (real_ctx_option->force_unique == 0) {
		cstate = fastpath_get_context_state_from_list(real_ctx_option,
				 sizeof(EGL_packed_option), &ctx_list_access_mutex);
	}

	// Create a real context if it hasn't been created
	if (cstate == NULL) {
		EGLContext *real_share_context = EGL_NO_CONTEXT;

		AST(mutex_lock(&ctx_list_access_mutex) == 1);
		{
			GLContext_List *current = NULL;

			current = glctx_list;
			while (current != NULL) {
				EGLDisplay cur_dpy = EGL_NO_DISPLAY;
				EGLint attribs[11];
				_unpack_egl_context_option(current->option, &cur_dpy, NULL, NULL, attribs, 11);
				if (cur_dpy == dpy) {
					if (attribs[0] == EGL_CONTEXT_MAJOR_VERSION_KHR &&
							attribs[1] != real_ctx_option->attrib_list.context_major_version) {
						current = current->next;
						continue;
					}

					AST(current->cstate != NULL);
					real_share_context = current->cstate->rctx;
					break;
				}
				current = current->next;
			}
		}
		AST(mutex_unlock(&ctx_list_access_mutex) == 1);

		*ctx = _orig_fastpath_eglCreateContext(dpy, config, real_share_context,
											   attrib_list);

		if (*ctx == EGL_NO_CONTEXT) {
			COREGL_WARN("Failed creating a egl real context for Fastpath. (Invalid config?)");
			goto finish;
		}

		*cstate_new = (GLContextState *)calloc(1, sizeof(GLContextState));
		if (*cstate_new == NULL) {
			COREGL_ERR("Error creating a new context state. (Memory full)");
			goto finish;
		}
		(*cstate_new)->rctx = *ctx;
		(*cstate_new)->rdpy = dpy;
		(*cstate_new)->data = NULL;

		AST(fastpath_add_context_state_to_list(real_ctx_option,
											   sizeof(EGL_packed_option), *cstate_new, &ctx_list_access_mutex) == 1);

#ifdef COREGL_FASTPATH_TRACE_CONTEXT_INFO
		if (unlikely(trace_ctx_flag == 1))
			add_to_general_trace_list(&context_state_trace_list, *cstate_new);
#endif // COREGL_FASTPATH_TRACE_CONTEXT_INFO

		cstate = *cstate_new;
	}

	goto finish;

finish:
	return cstate;
}


extern EGLBoolean (*ovr_eglBindAPI)(EGLenum api);
extern EGLenum (*ovr_eglQueryAPI)(void);

EGLBoolean
fastpath_eglBindAPI(EGLenum api)
{
	EGLBoolean ret = EGL_FALSE;
	MY_MODULE_TSTATE *tstate = NULL;

	_COREGL_FASTPATH_FUNC_BEGIN();
	if (fp_opt == FP_UNKNOWN_PATH) {
		COREGL_ERR("Invalid library link! (CoreGL path option is invalid)");
		goto finish;
	}

	ret = _orig_fastpath_eglBindAPI(api);

	GET_MY_TSTATE(tstate, get_current_thread_state());
	if (tstate == NULL) {
		AST(init_new_thread_state() == 1);

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
	}

	{
		EGLenum newapi = _orig_fastpath_eglQueryAPI();
		if (tstate && (tstate->binded_api != EGL_OPENGL_ES_API)) {
			tstate->binded_api = newapi;
		}
	}

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return ret;
}

EGLenum
fastpath_eglQueryAPI(void)
{
	EGLenum ret = 0;
	MY_MODULE_TSTATE *tstate = NULL;

	_COREGL_FASTPATH_FUNC_BEGIN();
	if (fp_opt == FP_UNKNOWN_PATH) {
		COREGL_ERR("Invalid library link! (CoreGL path option is invalid)");
		goto finish;
	}

	ret = _orig_fastpath_eglQueryAPI();

	GET_MY_TSTATE(tstate, get_current_thread_state());
	if (tstate != NULL) {
		AST(tstate->binded_api == ret);
	}

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return ret;
}

EGLContext
fastpath_eglCreateContext(EGLDisplay dpy, EGLConfig config,
						  EGLContext share_context, const EGLint *attrib_list)
{
	GLGlueContext *gctx = NULL, *newgctx = NULL;
	GLGlueContext_List *gctx_list_new = NULL;
	MY_MODULE_TSTATE *tstate = NULL;
	GLContextState *cstate = NULL;
	GLContextState *cstate_new = NULL;
	GL_Shared_Object_State *sostate_new = NULL;
	GLContext ctx = NULL;

	EGL_packed_option *real_ctx_option = NULL;
	EGL_packed_sharable_option *real_ctx_sharable_option = NULL;

	// Special eject condition for binding API
	GET_MY_TSTATE(tstate, get_current_thread_state());
	if (tstate != NULL && tstate->binded_api != EGL_OPENGL_ES_API) {
		return _orig_fastpath_eglCreateContext(dpy, config, share_context, attrib_list);
	}

	_COREGL_FASTPATH_FUNC_BEGIN();

	real_ctx_option = (EGL_packed_option *)calloc(1, sizeof(EGL_packed_option));
	if (real_ctx_option == NULL) {
		COREGL_ERR("Error creating a new GLGlueContext(Memory full 1)");
		goto finish;
	}
	cstate = _egl_create_context(real_ctx_option, &cstate_new, &ctx, dpy, config,
								 attrib_list);
	if (cstate == NULL)
		goto finish;

	// Pack shared context options
	real_ctx_sharable_option = (EGL_packed_sharable_option *)calloc(1,
							   sizeof(EGL_packed_sharable_option));
	if (real_ctx_sharable_option == NULL) {
		COREGL_ERR("Error creating a new GLGlueContext(Memory full 2)");
		goto finish;
	}
	AST(_pack_egl_context_sharable_option(real_ctx_sharable_option,
										  share_context) == 1);

	// Allocate a new context
	newgctx = (GLGlueContext *)calloc(1, sizeof(GLGlueContext));
	if (newgctx == NULL) {
		COREGL_ERR("Error creating a new GLGlueContext(Memory full 3)");
		goto finish;
	}

	newgctx->magic = MAGIC_GLFAST;
	newgctx->initialized = 0;
	newgctx->rdpy = dpy;
	newgctx->thread_id = get_current_thread();

	fastpath_ostate_init(&newgctx->ostate);

	if (share_context != EGL_NO_CONTEXT) {
		GLGlueContext *shared_gctx = (GLGlueContext *)share_context;
		AST(shared_gctx->magic == MAGIC_GLFAST);
		AST(shared_gctx->ostate.shared != NULL);
		newgctx->ostate.shared = shared_gctx->ostate.shared;
	} else {
		sostate_new = (GL_Shared_Object_State *)calloc(1,
					  sizeof(GL_Shared_Object_State));
		if (sostate_new == NULL) {
			COREGL_ERR("Error creating a new GLGlueContext(Memory full 4)");
			goto finish;
		}
		fastpath_sostate_init(sostate_new);
		newgctx->ostate.shared = sostate_new;
	}
	_add_shared_obj_state_ref(newgctx, newgctx->ostate.shared);
	newgctx->real_ctx_option = real_ctx_option;
	newgctx->real_ctx_option_len = sizeof(EGL_packed_option);
	newgctx->real_ctx_sharable_option = real_ctx_sharable_option;
	newgctx->real_ctx_sharable_option_len = sizeof(EGL_packed_sharable_option);

	_link_context_state(newgctx, cstate);
	_add_context_ref(newgctx);

	newgctx->cstate = cstate;

	{
		// Add glue context to list
		gctx_list_new = (GLGlueContext_List *)calloc(1, sizeof(GLGlueContext_List));
		if (gctx_list_new == NULL) {
			COREGL_ERR("Error creating a new GlGlueContext(Memory full 5)");
			goto finish;
		}

		AST(mutex_lock(&ctx_list_access_mutex) == 1);

		gctx_list_new->gctx = newgctx;

		gctx_list_new->prev = NULL;
		gctx_list_new->next = gctx_list;
		if (gctx_list != NULL)
			gctx_list->prev = gctx_list_new;

		gctx_list = gctx_list_new;

		AST(mutex_unlock(&ctx_list_access_mutex) == 1);
	}

	gctx = newgctx;

#ifdef COREGL_FASTPATH_TRACE_CONTEXT_INFO
	if (unlikely(trace_ctx_flag == 1)) {
		char ment[256];
		add_to_general_trace_list(&glue_ctx_trace_list, newgctx);

		snprintf(ment, sizeof(ment), "eglCreateContext completed (GlueCTX=[%12p])",
				 newgctx);
		_dump_context_info(ment, 1);
	}
#endif // COREGL_FASTPATH_TRACE_CONTEXT_INFO

	goto finish;

finish:
	if (gctx == NULL) {
		if (ctx != NULL) {
			_orig_fastpath_eglDestroyContext(dpy, ctx);
			ctx = NULL;
		}
		if (sostate_new != NULL) {
			free(sostate_new);
			sostate_new = NULL;
		}
		if (real_ctx_option != NULL) {
			free(real_ctx_option);
			real_ctx_option = NULL;
		}
		if (real_ctx_sharable_option != NULL) {
			free(real_ctx_sharable_option);
			real_ctx_sharable_option = NULL;
		}
		if (cstate_new != NULL) {
#ifdef COREGL_FASTPATH_TRACE_CONTEXT_INFO
			if (unlikely(trace_ctx_flag == 1))
				remove_from_general_trace_list(&context_state_trace_list, cstate_new);
#endif // COREGL_FASTPATH_TRACE_CONTEXT_INFO

			fastpath_remove_context_states_from_list(cstate_new, &ctx_list_access_mutex);
			free(cstate_new);
			cstate_new = NULL;
		}
		if (gctx_list_new != NULL) {
			AST(mutex_lock(&ctx_list_access_mutex) == 1);

			if (gctx_list_new->next != NULL)
				gctx_list_new->next->prev = gctx_list_new->prev;

			if (gctx_list_new->prev != NULL)
				gctx_list_new->prev->next = gctx_list_new->next;
			else
				gctx_list = gctx_list_new->next;

			AST(mutex_unlock(&ctx_list_access_mutex) == 1);

			free(gctx_list_new);
		}
		if (newgctx != NULL) {
			_remove_context_ref(newgctx, &ctx_list_access_mutex);
			free(newgctx);
			newgctx = NULL;
		}
	}

	_COREGL_FASTPATH_FUNC_END();

	return (EGLContext)gctx;
}

EGLBoolean
fastpath_eglDestroyContext(EGLDisplay dpy, EGLContext ctx)
{
	EGLBoolean ret = EGL_FALSE;
	GLGlueContext *gctx = NULL;

	_COREGL_FASTPATH_FUNC_BEGIN();

	gctx = (GLGlueContext *)ctx;

	if (gctx != NULL && gctx != EGL_NO_CONTEXT) {
		GLContextState *cstate = NULL;

		if (gctx->magic != MAGIC_GLFAST) {
			ret = _orig_fastpath_eglDestroyContext(dpy, ctx);
			goto finish;
		}

		cstate = gctx->cstate;
		AST(cstate != NULL);

		if (gctx->is_destroyed != 1) {
			gctx->is_destroyed = 1;
			_remove_context_ref(gctx, &ctx_list_access_mutex);
		}
	} else {
		COREGL_WARN("Invalid destroying context. (no exists)");
		ret = EGL_FALSE;
		goto finish;
	}

	ret = EGL_TRUE;
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();

#ifdef COREGL_FASTPATH_TRACE_CONTEXT_INFO
	if (unlikely(trace_ctx_flag == 1)) {
		char ment[256];
		snprintf(ment, sizeof(ment), "eglDestroyContext completed (GlueCTX=[%12p])",
				 ctx);
		_dump_context_info(ment, 1);
	}
#endif // COREGL_FASTPATH_TRACE_CONTEXT_INFO

	return ret;
}

EGLBoolean
fastpath_eglQueryContext(EGLDisplay dpy, EGLContext ctx, EGLint attribute,
						 EGLint *value)
{
	EGLBoolean ret = EGL_FALSE;
	EGLContext real_ctx = EGL_NO_CONTEXT;

	_COREGL_FASTPATH_FUNC_BEGIN();

	if (ctx != EGL_NO_CONTEXT) {
		GLGlueContext *gctx = NULL;
		gctx = (GLGlueContext *)ctx;

		if (gctx->magic != MAGIC_GLFAST) {
			real_ctx = ctx;
		} else {
			AST(gctx->cstate != NULL);
			real_ctx = gctx->cstate->rctx;
		}
	}

	ret = _orig_fastpath_eglQueryContext(dpy, real_ctx, attribute, value);
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return ret;
}



EGLBoolean
fastpath_eglReleaseThread(void)
{
	EGLBoolean ret = EGL_FALSE;
	EGLDisplay dpy = EGL_NO_DISPLAY;
	MY_MODULE_TSTATE *tstate = NULL;

	GET_MY_TSTATE(tstate, get_current_thread_state());

	// Special eject condition for binding API
	if (tstate != NULL && tstate->binded_api != EGL_OPENGL_ES_API) {
		return _orig_fastpath_eglReleaseThread();
	}

	_COREGL_FASTPATH_FUNC_BEGIN();

	dpy = _orig_fastpath_eglGetCurrentDisplay();

	/* according to spec,
	 * Even if EGL is not initialized on any EGLDisplay, eglReleaseThread should succeed
	 */
	if (dpy != EGL_NO_DISPLAY)
		fastpath_eglMakeCurrent(dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	ret = _orig_fastpath_eglReleaseThread();
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return ret;
}

EGLDisplay
fastpath_eglGetCurrentDisplay(void)
{
	MY_MODULE_TSTATE *tstate = NULL;
	EGLDisplay dpy = EGL_NO_DISPLAY;

	GET_MY_TSTATE(tstate, get_current_thread_state());

	if (tstate != NULL) {
		// Special eject condition for binding API
		if (tstate->binded_api != EGL_OPENGL_ES_API) {
			return _orig_fastpath_eglGetCurrentDisplay();
		}

		_COREGL_FASTPATH_FUNC_BEGIN();

		if (tstate->cstate != NULL) {
			dpy = tstate->cstate->rdpy;

		}

		_COREGL_FASTPATH_FUNC_END();

	}

	goto finish;

finish:

	return dpy;
}

EGLBoolean
fastpath_eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read,
						EGLContext ctx)
{
	EGLBoolean ret = EGL_FALSE;
	EGLBoolean need_mc = EGL_FALSE;
	GLGlueContext *gctx = NULL;
	int api_version;

	MY_MODULE_TSTATE *tstate = NULL;

	GET_MY_TSTATE(tstate, get_current_thread_state());

	// Special eject condition for binding API
	if (tstate != NULL && tstate->binded_api != EGL_OPENGL_ES_API) {
		return _orig_fastpath_eglMakeCurrent(dpy, draw, read, ctx);
	}

	_COREGL_FASTPATH_FUNC_BEGIN();

	gctx = (GLGlueContext *)ctx;

	if (tstate == NULL) {
		AST(init_new_thread_state() == 1);

		GET_MY_TSTATE(tstate, get_current_thread_state());
		AST(tstate != NULL);
	}

	// Special path for context deletion
	if (ctx == EGL_NO_CONTEXT) {
		AST(gctx == NULL);

		if (tstate) {

			if (tstate->cstate != NULL) {
				if (_bind_context_state(NULL, tstate->cstate, &ctx_list_access_mutex) != 1) {
					COREGL_WARN("Error soft-makecurrent for context deletion");
				}
				tstate->cstate = NULL;
			}
			if (_orig_fastpath_eglMakeCurrent(dpy, draw, read, ctx) != EGL_TRUE) {
				COREGL_WARN("Error making context [%p] current. (invalid EGL display [%p] or EGL surface [D:%p/R:%p])",
							ctx, dpy, draw, read);
				ret = EGL_FALSE;
				goto finish;
			}

			tstate->rsurf_draw = draw;
			tstate->rsurf_read = read;

			ret = EGL_TRUE;
			goto finish;
		}
	}

	AST(gctx != NULL);
	AST(gctx && (gctx->cstate != NULL));

	if (gctx && (gctx->rdpy != dpy)) {
		COREGL_WARN("Invalid context (or invalid EGL display)");
		ret = EGL_FALSE;
		goto finish;
	}

	AST(gctx && (gctx->real_ctx_option != NULL));
	AST(gctx && (gctx->real_ctx_sharable_option != NULL));

	// Handle cross threading of context (when used by two or more gctx)
	if (gctx && gctx->thread_id != get_current_thread() &&
			gctx->cstate->ref_count > 1) {
#define ATTRIB_LIST_BUFFER_SIZE 8

		GLContextState *cstate_new = NULL;
		EGLContext new_real_ctx = EGL_NO_CONTEXT;

		EGLDisplay dpy = EGL_NO_DISPLAY;
		EGLConfig config = NULL;
		int attrib_list[ATTRIB_LIST_BUFFER_SIZE];

		AST(_unpack_egl_context_option(gctx->real_ctx_option, &dpy, &config, NULL,
									   attrib_list, ATTRIB_LIST_BUFFER_SIZE) == 1);
		AST(dpy == gctx->rdpy);

		_egl_create_context(gctx->real_ctx_option, &cstate_new, &new_real_ctx, dpy,
							config, attrib_list);

		AST(cstate_new != NULL);

		if (_bind_context_state(gctx, cstate_new, &ctx_list_access_mutex) != 1) {
			COREGL_WARN("Error soft-makecurrent in Cross-thread usage!");
			free(cstate_new);
			_orig_fastpath_eglDestroyContext(dpy, new_real_ctx);
			ret = EGL_FALSE;
			goto finish;
		}

		// TODO : Setup context state for new real ctx
		COREGL_WARN("Cross-thread usage(makecurrent) can cause a state-broken situation!");

		_unlink_context_state(gctx, &ctx_list_access_mutex);
		_link_context_state(gctx, cstate_new);

		// Apply to new thread
		gctx->thread_id = get_current_thread();

	}

	// Check if the object is correct
	if (gctx && (gctx->magic != MAGIC_GLFAST)) {
		COREGL_ERR("Glue-CTX Magic Check Failed!!! (Memory broken?)");
		ret = EGL_FALSE;
		goto finish;

	}

	// If drawable changed, do a make current
	if (tstate &&
			((tstate->rsurf_draw != draw) || (tstate->rsurf_read != read)))
		need_mc = EGL_TRUE;

	AST(gctx && (gctx->cstate != NULL));

	// If binded real context changed, do a make current
	if (tstate && gctx && ((tstate->cstate == NULL) ||
						   (tstate->cstate != gctx->cstate)))
		need_mc = EGL_TRUE;

	if (tstate && gctx && gctx->cstate && (need_mc == EGL_TRUE)) {
		AST(dpy == gctx->cstate->rdpy);

		// BB : full makecurrent
		if (_orig_fastpath_eglMakeCurrent(dpy, draw, read,
										  gctx->cstate->rctx) != EGL_TRUE) {
			COREGL_WARN("Error making context current with the drawable. (Bad match?)");
			ret = EGL_FALSE;
			goto finish;
		}

		api_version = ((EGL_packed_option *)
					   gctx->real_ctx_option)->attrib_list.context_major_version;

		// Change overriding for modules according to new API version
		if (USE_TRACEPATH || (fp_opt == FP_FAST_PATH)) {
			if (!current_gl_api_version) {
				current_gl_api_version = api_version;
				if (current_gl_api_version == COREGL_GLAPI_1) {
					init_export(GL_FALSE, GL_TRUE);
					COREGL_DBG("Default API path reseted...");
				}
			} else {
				if (current_gl_api_version != api_version) {
					// API version becomes 1.x from higher
					if (api_version == COREGL_GLAPI_1) {
						init_export(GL_FALSE, GL_TRUE);
						COREGL_DBG("Default API path reseted...");
					}
					// API version becomes higher from 1.x
					if (current_gl_api_version == COREGL_GLAPI_1)
						init_modules();
					current_gl_api_version = api_version;
				}
			}
		}

		// Update references only when the contexts are different
		if (tstate->cstate != gctx->cstate)  {
			if (tstate->cstate != NULL && tstate->cstate->data != NULL)
				_remove_context_ref((GLGlueContext *)tstate->cstate->data,
									&ctx_list_access_mutex);

			tstate->cstate = gctx->cstate;

			if (tstate->cstate->data != NULL)
				_add_context_ref((GLGlueContext *)tstate->cstate->data);
		}

		tstate->rsurf_draw = draw;
		tstate->rsurf_read = read;
	}

	// Initialize context states
	if (gctx && (gctx->initialized == 0)) {
		if (fastpath_init_context_states(gctx) != 1) {
			COREGL_ERR("Error intializing context. (Check driver specification)");
			goto finish;
		}
	}

	// Setup initial Viewport & Scissor
	if (gctx && gctx->surface_attached == 0 && draw != EGL_NO_SURFACE) {
		EGLint box_buffer[4];
		EGLint width = 0;
		EGLint height = 0;

		_orig_fastpath_glGetIntegerv(GL_VIEWPORT, box_buffer);
		width = box_buffer[2];
		height = box_buffer[3];

		gctx->_clear_flag1 |= _CLEAR_FLAG1_BIT_gl_viewport;
		if (&gctx->gl_viewport[0]) gctx->gl_viewport[0] = 0;
		if (&gctx->gl_viewport[1]) gctx->gl_viewport[1] = 0;
		if (&gctx->gl_viewport[2]) gctx->gl_viewport[2] = width;
		if (&gctx->gl_viewport[3]) gctx->gl_viewport[3] = height;

		_orig_fastpath_glGetIntegerv(GL_SCISSOR_BOX, box_buffer);
		width = box_buffer[2];
		height = box_buffer[3];

		gctx->_misc_flag2 |= _MISC_FLAG2_BIT_gl_scissor_box;
		if (&gctx->gl_scissor_box[0]) gctx->gl_scissor_box[0] = 0;
		if (&gctx->gl_scissor_box[1]) gctx->gl_scissor_box[1] = 0;
		if (&gctx->gl_scissor_box[2]) gctx->gl_scissor_box[2] = width;
		if (&gctx->gl_scissor_box[3]) gctx->gl_scissor_box[3] = height;

		gctx->surface_attached = 1;
	}

	if (tstate &&
			(_bind_context_state(gctx, tstate->cstate, &ctx_list_access_mutex) != 1)) {
		ret = EGL_FALSE;
		goto finish;
	}
	if (gctx) gctx->used_count++;

	ret = EGL_TRUE;
	goto finish;

finish:

	_COREGL_FASTPATH_FUNC_END();

#ifdef COREGL_FASTPATH_TRACE_CONTEXT_INFO
	if (unlikely(trace_ctx_flag == 1)) {
		char ment[256];
		snprintf(ment, sizeof(ment),
				 "eglMakeCurrent finished (GlueCTX=[%12p] Surf=[D:%12p R:%12p])",
				 ctx, draw, read);
		_dump_context_info(ment, 0);
	}
#endif // COREGL_FASTPATH_TRACE_CONTEXT_INFO

	return ret;
}


EGLContext
fastpath_eglGetCurrentContext(void)
{
	GLGlueContext *ret = NULL;
	MY_MODULE_TSTATE *tstate = NULL;

	GET_MY_TSTATE(tstate, get_current_thread_state());

	// Special eject condition for binding API
	if (tstate != NULL && tstate->binded_api != EGL_OPENGL_ES_API) {
		return _orig_fastpath_eglGetCurrentContext();
	}

	_COREGL_FASTPATH_FUNC_BEGIN();

	if (tstate != NULL) {
		if (tstate->cstate != NULL) {
			ret = (GLGlueContext *)tstate->cstate->data;
			AST(ret->magic == MAGIC_GLFAST);
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return (EGLContext)ret;
}


EGLSurface
fastpath_eglGetCurrentSurface(EGLint readdraw)
{
	EGLSurface ret = EGL_NO_SURFACE;
	MY_MODULE_TSTATE *tstate = NULL;

	GET_MY_TSTATE(tstate, get_current_thread_state());

	// Special eject condition for binding API
	if (tstate != NULL && tstate->binded_api != EGL_OPENGL_ES_API) {
		return _orig_fastpath_eglGetCurrentSurface(readdraw);
	}

	_COREGL_FASTPATH_FUNC_BEGIN();

	if (tstate != NULL) {
		switch (readdraw) {
		case EGL_DRAW :
			ret = (GLGlueContext *)tstate->rsurf_draw;
			break;
		case EGL_READ :
			ret = (GLGlueContext *)tstate->rsurf_read;
			break;
		}
	}
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return ret;
}

EGLImageKHR
fastpath_eglCreateImageKHR (EGLDisplay dpy, EGLContext ctx, EGLenum target,
							EGLClientBuffer buffer, const EGLint *attrib_list)
{
	void *ret = NULL;
	EGLContext real_ctx = EGL_NO_CONTEXT;
	EGLClientBuffer real_obj = NULL;
	GL_Object_Type type = 0;

	if (current_gl_api_version == COREGL_GLAPI_1) {
		ret = _orig_fastpath_eglCreateImageKHR(dpy, real_ctx, target, buffer,
											   attrib_list);
		goto finish;
	}

	if ((EGLint)buffer & GL_OBJECT_TYPE_TEXTURE)
		type = GL_OBJECT_TYPE_TEXTURE;
	else if ((EGLint)buffer & GL_OBJECT_TYPE_RENDERBUFFER)
		type = GL_OBJECT_TYPE_RENDERBUFFER;
	else {
		ret = _orig_fastpath_eglCreateImageKHR(dpy, real_ctx, target, buffer,
											   attrib_list);
		goto finish;
	}

	_COREGL_FASTPATH_FUNC_BEGIN();

	if (ctx != NULL && ctx != EGL_NO_CONTEXT) {
		GLGlueContext *gctx = (GLGlueContext *)ctx;

		if (gctx->magic != MAGIC_GLFAST) {
			real_ctx = ctx;
		} else {
			AST(gctx->cstate != NULL);
			real_ctx = gctx->cstate->rctx;
		}

		switch (target) {
		case EGL_GL_TEXTURE_2D_KHR:
		case EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_X_KHR:
		case EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_X_KHR:
		case EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Y_KHR:
		case EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_KHR:
		case EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Z_KHR:
		case EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_KHR:
		case EGL_GL_TEXTURE_3D_KHR:
		case EGL_GL_RENDERBUFFER_KHR:
			if (buffer == NULL) {
				COREGL_ERR("fastpath_eglCreateImageKHR buffer object NULL");
				real_obj = NULL;
			} else {
				real_obj = (EGLClientBuffer)(uintptr_t)fastpath_ostate_get_object(&gctx->ostate,
						   type, (GLuint)(uintptr_t)buffer);
			}
			break;
		default:
			COREGL_ERR("Error Not supported target NULL client buffer");
			goto finish;
		}

	} else  {
		real_obj = buffer;
	}

	ret = _orig_fastpath_eglCreateImageKHR(dpy, real_ctx, target, real_obj,
										   attrib_list);


	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return ret;
}

_eng_fn
fastpath_eglGetProcAddress(const char *procname)
{
	_eng_fn ret = NULL;

	_COREGL_FASTPATH_FUNC_BEGIN();

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST) \
	if (strcmp(procname, #FUNC_NAME) == 0) \
	{ \
		_eng_fn ret_orig = NULL; \
		ret_orig = _orig_fastpath_eglGetProcAddress(procname); \
		if (ret_orig != NULL) \
			ret = (_eng_fn)ovr_##FUNC_NAME; \
		goto finish; \
	}

#define _COREGL_EXT_SYMBOL_ALIAS(ALIAS_NAME, FUNC_NAME) \
	if (strcmp(procname, #ALIAS_NAME) == 0) \
	{ \
		_eng_fn ret_orig = NULL; \
		ret_orig = _orig_fastpath_eglGetProcAddress(#ALIAS_NAME); \
		if (ret_orig != NULL) \
			ret = (_eng_fn)ovr_##FUNC_NAME; \
		goto finish; \
	}

#include "../../headers/sym_egl.h"
#include "../../headers/sym_gl1.h"
#include "../../headers/sym_gl2.h"
#include "../../headers/sym_gl_common.h"
#undef _COREGL_SYMBOL
#undef _COREGL_EXT_SYMBOL_ALIAS

	ret = _orig_fastpath_eglGetProcAddress(procname);
	if (ret != NULL) {

#define _COREGL_EXT_SYMBOL_FASTPATH_PASS(FUNC_NAME) \
	if (strcmp(procname, #FUNC_NAME) == 0) \
		goto finish;

#define _COREGL_EXT_SYMBOL_FASTPATH_BLOCK(FUNC_NAME) \
	if (strcmp(procname, #FUNC_NAME) == 0) \
	{ \
		ret = NULL; \
		goto finish; \
	}

#include "../../headers/sym_egl.h"
#include "../../headers/sym_gl1.h"
#include "../../headers/sym_gl2.h"
#include "../../headers/sym_gl_common.h"

#undef _COREGL_EXT_SYMBOL_FASTPATH_PASS
#undef _COREGL_EXT_SYMBOL_FASTPATH_BLOCK

		COREGL_ERR("FASTPATH can't support '%s' (will be terminated with Illegal instruction!)",
				   procname);
		assert(0);
	}

	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return ret;
}

EGLBoolean
fastpath_eglTerminate(EGLDisplay dpy)
{
	EGLBoolean ret = EGL_FALSE;
	GLGlueContext_List *current = NULL;
	GLGlueContext_List *remove_list = NULL;

	_COREGL_FASTPATH_FUNC_BEGIN();

	AST(mutex_lock(&ctx_list_access_mutex) == 1);
	current = gctx_list;
	while (current != NULL) {
		if (current->gctx->cstate->rdpy == dpy) {
			GLGlueContext_List *rm_newitm = NULL;
			rm_newitm = (GLGlueContext_List *)calloc(1, sizeof(GLGlueContext_List));
			rm_newitm->gctx = current->gctx;

			rm_newitm->next = remove_list;
			remove_list = rm_newitm;
		}
		current = current->next;
	}
	AST(mutex_unlock(&ctx_list_access_mutex) == 1);

	current = remove_list;
	while (current != NULL) {
		fastpath_eglDestroyContext(dpy, current->gctx);
		remove_list = current->next;
		free(current);
		current = remove_list;
	}

	ret = _orig_fastpath_eglTerminate(dpy);
	goto finish;

finish:
	_COREGL_FASTPATH_FUNC_END();
	return ret;
}

