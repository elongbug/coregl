// Defult use-case for exporting symbols
#ifndef _COREGL_SYMBOL
#define _COREGL_SYMBOL_NOT_DEFINED
#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     COREGL_API extern RET_TYPE FUNC_NAME PARAM_LIST;
#endif
#ifndef _COREGL_EXT_SYMBOL
#define _COREGL_EXT_SYMBOL_NOT_DEFINED
#define _COREGL_EXT_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)   _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)
#endif
#ifndef _COREGL_EXT_SYMBOL_ALIAS
#define _COREGL_EXT_SYMBOL_ALIAS_NOT_DEFINED
#define _COREGL_EXT_SYMBOL_ALIAS(FUNC_NAME, ALIAS_NAME)
#endif
#ifndef _COREGL_EXT_SYMBOL_FASTPATH_PASS
#define _COREGL_EXT_SYMBOL_FASTPATH_PASS_NOT_DEFINED
#define _COREGL_EXT_SYMBOL_FASTPATH_PASS(FUNC_NAME)
#endif
#ifndef _COREGL_EXT_SYMBOL_FASTPATH_BLOCK
#define _COREGL_EXT_SYMBOL_FASTPATH_BLOCK_NOT_DEFINED
#define _COREGL_EXT_SYMBOL_FASTPATH_BLOCK(FUNC_NAME)
#endif

// Standard EGL APIs

_COREGL_SYMBOL(EGLint, eglGetError, (void))
_COREGL_SYMBOL(EGLDisplay, eglGetDisplay, (EGLNativeDisplayType display_id))
_COREGL_SYMBOL(EGLBoolean, eglInitialize, (EGLDisplay dpy, EGLint *major,
			   EGLint *minor))
_COREGL_SYMBOL(EGLBoolean, eglTerminate, (EGLDisplay dpy))
_COREGL_SYMBOL(const char *, eglQueryString, (EGLDisplay dpy, EGLint name))

_COREGL_SYMBOL(EGLBoolean, eglGetConfigs, (EGLDisplay dpy, EGLConfig *configs,
			   EGLint config_size, EGLint *num_config))
_COREGL_SYMBOL(EGLBoolean, eglChooseConfig, (EGLDisplay dpy,
			   const EGLint *attrib_list, EGLConfig *configs, EGLint config_size,
			   EGLint *num_config))
_COREGL_SYMBOL(EGLBoolean, eglGetConfigAttrib, (EGLDisplay dpy,
			   EGLConfig config, EGLint attribute, EGLint *value))

_COREGL_SYMBOL(EGLSurface, eglCreateWindowSurface, (EGLDisplay dpy,
			   EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list))
_COREGL_SYMBOL(EGLSurface, eglCreatePbufferSurface, (EGLDisplay dpy,
			   EGLConfig config, const EGLint *attrib_list))
_COREGL_SYMBOL(EGLSurface, eglCreatePixmapSurface, (EGLDisplay dpy,
			   EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list))
_COREGL_SYMBOL(EGLBoolean, eglDestroySurface, (EGLDisplay dpy,
			   EGLSurface surface))
_COREGL_SYMBOL(EGLBoolean, eglQuerySurface, (EGLDisplay dpy, EGLSurface surface,
			   EGLint attribute, EGLint *value))

_COREGL_SYMBOL(EGLBoolean, eglBindAPI, (EGLenum api))
_COREGL_SYMBOL(EGLenum, eglQueryAPI, (void))
_COREGL_SYMBOL(EGLBoolean, eglWaitClient, (void))
_COREGL_SYMBOL(EGLBoolean, eglReleaseThread, (void))
_COREGL_SYMBOL(EGLSurface, eglCreatePbufferFromClientBuffer, (EGLDisplay dpy,
			   EGLenum buftype, EGLClientBuffer buffer, EGLConfig config,
			   const EGLint *attrib_list))

_COREGL_SYMBOL(EGLBoolean, eglSurfaceAttrib, (EGLDisplay dpy,
			   EGLSurface surface, EGLint attribute, EGLint value))
_COREGL_SYMBOL(EGLBoolean, eglBindTexImage, (EGLDisplay dpy, EGLSurface surface,
			   EGLint buffer))
_COREGL_SYMBOL(EGLBoolean, eglReleaseTexImage, (EGLDisplay dpy,
			   EGLSurface surface, EGLint buffer))
_COREGL_SYMBOL(EGLBoolean, eglSwapInterval, (EGLDisplay dpy, EGLint interval))
_COREGL_SYMBOL(EGLContext, eglCreateContext, (EGLDisplay dpy, EGLConfig config,
			   EGLContext share_context, const EGLint *attrib_list))
_COREGL_SYMBOL(EGLBoolean, eglDestroyContext, (EGLDisplay dpy, EGLContext ctx))
_COREGL_SYMBOL(EGLBoolean, eglMakeCurrent, (EGLDisplay dpy, EGLSurface draw,
			   EGLSurface read, EGLContext ctx))
_COREGL_SYMBOL(EGLContext, eglGetCurrentContext, (void))
_COREGL_SYMBOL(EGLSurface, eglGetCurrentSurface, (EGLint readdraw))
_COREGL_SYMBOL(EGLDisplay, eglGetCurrentDisplay, (void))

_COREGL_SYMBOL(EGLBoolean, eglQueryContext, (EGLDisplay dpy, EGLContext ctx,
			   EGLint attribute, EGLint *value))
_COREGL_SYMBOL(EGLBoolean, eglWaitGL, (void))
_COREGL_SYMBOL(EGLBoolean, eglWaitNative, (EGLint engine))
_COREGL_SYMBOL(EGLBoolean, eglSwapBuffers, (EGLDisplay dpy, EGLSurface surface))
_COREGL_SYMBOL(EGLBoolean, eglCopyBuffers, (EGLDisplay dpy, EGLSurface surface,
			   EGLNativePixmapType target))

_COREGL_SYMBOL(_eng_fn, eglGetProcAddress, (const char *procname))


// Extensions
_COREGL_EXT_SYMBOL(EGLImageKHR, eglCreateImageKHR, (EGLDisplay dpy,
				   EGLContext ctx, EGLenum target, EGLClientBuffer buffer,
				   const EGLint *attrib_list))
_COREGL_EXT_SYMBOL(EGLBoolean, eglDestroyImageKHR, (EGLDisplay dpy,
				   EGLImageKHR image))

_COREGL_EXT_SYMBOL(void *, eglMapImageSEC, (EGLDisplay dpy, EGLImageKHR image,
				   EGLint device_type, EGLint access_option))
_COREGL_EXT_SYMBOL(EGLBoolean, eglUnmapImageSEC, (EGLDisplay dpy,
				   EGLImageKHR image, EGLint device_type))
_COREGL_EXT_SYMBOL(EGLBoolean, eglGetImageAttribSEC, (EGLDisplay dpy,
				   EGLImageKHR image, EGLint attribute, EGLint *value))

_COREGL_EXT_SYMBOL(EGLBoolean, eglLockSurfaceKHR, (EGLDisplay display,
				   EGLSurface surface, const EGLint *attrib_list))
_COREGL_EXT_SYMBOL(EGLBoolean, eglUnlockSurfaceKHR, (EGLDisplay display,
				   EGLSurface surface))

_COREGL_EXT_SYMBOL(EGLBoolean, eglSwapBuffersWithDamageEXT, (EGLDisplay dpy,
				   EGLSurface surface, EGLint *rects, EGLint n_rects))

_COREGL_EXT_SYMBOL(EGLBoolean, eglSwapBuffersRegionEXT, (EGLDisplay dpy,
				   EGLSurface surface, EGLint numRects, const EGLint *rects))

_COREGL_EXT_SYMBOL(EGLBoolean, eglBindWaylandDisplayWL, (EGLDisplay dpy,
				   void *display))

_COREGL_EXT_SYMBOL(EGLBoolean, eglUnbindWaylandDisplayWL, (EGLDisplay dpy,
				   void *display))

_COREGL_EXT_SYMBOL(EGLBoolean, eglQueryWaylandBufferWL, (EGLDisplay dpy,
				   void *buffer, EGLint attribute, EGLint *value))

_COREGL_EXT_SYMBOL(EGLBoolean, eglSetDamageRegionKHR, (EGLDisplay dpy,
				   EGLSurface surface, EGLint *rects, EGLint n_rects))

_COREGL_EXT_SYMBOL(EGLBoolean, eglSwapBuffersWithDamageKHR, (EGLDisplay dpy,
				   EGLSurface surface, EGLint *rects, EGLint n_rects))

/* Fastpath Verified extensions */
//_COREGL_EXT_SYMBOL_FASTPATH_PASS(eglSwapBuffersRegionSEC)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(eglCreateSyncKHR)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(eglDestroySyncKHR)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(eglClientWaitSyncKHR)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(eglSignalSyncKHR)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(eglGetSyncAttribKHR)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(eglWaitSyncKHR)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(eglPostSubBufferNV)

/* Fastpath Blocked extensions */
//_COREGL_EXT_SYMBOL_FASTPATH_BLOCK()

#ifdef _COREGL_EXT_SYMBOL_NOT_DEFINED
#undef _COREGL_EXT_SYMBOL_NOT_DEFINED
#undef _COREGL_EXT_SYMBOL
#endif

#ifdef _COREGL_SYMBOL_NOT_DEFINED
#undef _COREGL_SYMBOL_NOT_DEFINED
#undef _COREGL_SYMBOL
#endif

#ifdef _COREGL_EXT_SYMBOL_ALIAS_NOT_DEFINED
#undef _COREGL_EXT_SYMBOL_ALIAS_NOT_DEFINED
#undef _COREGL_EXT_SYMBOL_ALIAS
#endif

#ifdef _COREGL_EXT_SYMBOL_FASTPATH_PASS_NOT_DEFINED
#undef _COREGL_EXT_SYMBOL_FASTPATH_PASS_NOT_DEFINED
#undef _COREGL_EXT_SYMBOL_FASTPATH_PASS
#endif

#ifdef _COREGL_EXT_SYMBOL_FASTPATH_BLOCK_NOT_DEFINED
#undef _COREGL_EXT_SYMBOL_FASTPATH_BLOCK_NOT_DEFINED
#undef _COREGL_EXT_SYMBOL_FASTPATH_BLOCK
#endif

