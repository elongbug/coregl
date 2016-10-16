#include <stdio.h>
#include <dlfcn.h>

# include <EGL/eglplatform.h>
# include "../headers/egl.h"
# include "../coregl_internal.h"

typedef void (*_eng_fn) (void);

#define COREGL_API           __attribute__((visibility("default")))

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     COREGL_API extern RET_TYPE FUNC_NAME PARAM_LIST;
#define _COREGL_EXT_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)
# include "../headers/sym_egl.h"
#undef _COREGL_EXT_SYMBOL
#undef _COREGL_SYMBOL

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     RET_TYPE (*ovr_##FUNC_NAME) PARAM_LIST = NULL;
#define _COREGL_EXT_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)
# include "../headers/sym_egl.h"
#undef _COREGL_EXT_SYMBOL
#undef _COREGL_SYMBOL

#define INIT_EXPORT()

void *lib_handle = NULL;

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

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST) \
   ovr_##FUNC_NAME = (__typeof__(ovr_##FUNC_NAME))dlsym(lib_handle, "coregl_api_"#FUNC_NAME); \
	if (ovr_##FUNC_NAME == NULL) \
	{ \
		COREGL_ERR("Can't find a symbol '%s'!", #FUNC_NAME); \
		COREGL_ERR("Invalid library link! (Check linkage of libEGL -> libCOREGL)"); \
	}

#define _COREGL_EXT_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)
#include "../headers/sym_egl.h"
#undef _COREGL_EXT_SYMBOL
#undef _COREGL_SYMBOL

	return 1;
}


EGLint
eglGetError(void)
{
	INIT_EXPORT();
	return ovr_eglGetError();
}

EGLDisplay
eglGetDisplay(EGLNativeDisplayType display_id)
{
	INIT_EXPORT();
	return ovr_eglGetDisplay(display_id);
}

EGLBoolean
eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor)
{
	INIT_EXPORT();
	return ovr_eglInitialize(dpy, major, minor);
}

EGLBoolean
eglTerminate(EGLDisplay dpy)
{
	INIT_EXPORT();
	return ovr_eglTerminate(dpy);
}

EGLBoolean
eglGetConfigs(EGLDisplay dpy, EGLConfig *configs, EGLint config_size,
			  EGLint *num_config)
{
	INIT_EXPORT();
	return ovr_eglGetConfigs(dpy, configs, config_size, num_config);
}

EGLBoolean
eglChooseConfig(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs,
				EGLint config_size, EGLint *num_config)
{
	INIT_EXPORT();
	return ovr_eglChooseConfig(dpy, attrib_list, configs, config_size, num_config);
}

EGLBoolean
eglGetConfigAttrib(EGLDisplay dpy, EGLConfig config, EGLint attribute,
				   EGLint *value)
{
	INIT_EXPORT();
	return ovr_eglGetConfigAttrib(dpy, config, attribute, value);
}


EGLSurface
eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config,
					   EGLNativeWindowType win, const EGLint *attrib_list)
{
	INIT_EXPORT();
	return ovr_eglCreateWindowSurface(dpy, config, win, attrib_list);
}

EGLSurface
eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config,
						const EGLint *attrib_list)
{
	INIT_EXPORT();
	return ovr_eglCreatePbufferSurface(dpy, config, attrib_list);
}

EGLSurface
eglCreatePixmapSurface(EGLDisplay dpy, EGLConfig config,
					   EGLNativePixmapType pixmap, const EGLint *attrib_list)
{
	INIT_EXPORT();
	return ovr_eglCreatePixmapSurface(dpy, config, pixmap, attrib_list);
}

EGLBoolean
eglDestroySurface(EGLDisplay dpy, EGLSurface surface)
{
	INIT_EXPORT();
	return ovr_eglDestroySurface(dpy, surface);
}

EGLBoolean
eglQuerySurface(EGLDisplay dpy, EGLSurface surface, EGLint attribute,
				EGLint *value)
{
	INIT_EXPORT();
	return ovr_eglQuerySurface(dpy, surface, attribute, value);
}

EGLBoolean
eglBindAPI(EGLenum api)
{
	INIT_EXPORT();
	return ovr_eglBindAPI(api);
}

EGLenum
eglQueryAPI(void)
{
	INIT_EXPORT();
	return ovr_eglQueryAPI();
}

EGLBoolean
eglWaitClient(void)
{
	INIT_EXPORT();
	return ovr_eglWaitClient();
}

EGLBoolean
eglReleaseThread(void)
{
	INIT_EXPORT();
	return ovr_eglReleaseThread();
}

EGLSurface
eglCreatePbufferFromClientBuffer(EGLDisplay dpy, EGLenum buftype,
								 EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list)
{
	INIT_EXPORT();
	return ovr_eglCreatePbufferFromClientBuffer(dpy, buftype, buffer, config,
			attrib_list);
}

EGLBoolean
eglSurfaceAttrib(EGLDisplay dpy, EGLSurface surface, EGLint attribute,
				 EGLint value)
{
	INIT_EXPORT();
	return ovr_eglSurfaceAttrib(dpy, surface, attribute, value);
}

EGLBoolean
eglBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
	INIT_EXPORT();
	return ovr_eglBindTexImage(dpy, surface, buffer);
}

EGLBoolean
eglReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
	INIT_EXPORT();
	return ovr_eglReleaseTexImage(dpy, surface, buffer);
}

EGLBoolean
eglSwapInterval(EGLDisplay dpy, EGLint interval)
{
	INIT_EXPORT();
	return ovr_eglSwapInterval(dpy, interval);
}

EGLContext
eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context,
				 const EGLint *attrib_list)
{
	INIT_EXPORT();
	return ovr_eglCreateContext(dpy, config, share_context, attrib_list);
}

EGLBoolean
eglDestroyContext(EGLDisplay dpy, EGLContext ctx)
{
	INIT_EXPORT();
	return ovr_eglDestroyContext(dpy, ctx);
}

EGLBoolean
eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx)
{
	INIT_EXPORT();
	return ovr_eglMakeCurrent(dpy, draw, read, ctx);
}

EGLContext
eglGetCurrentContext(void)
{
	INIT_EXPORT();
	return ovr_eglGetCurrentContext();
}

EGLSurface
eglGetCurrentSurface(EGLint readdraw)
{
	INIT_EXPORT();
	return ovr_eglGetCurrentSurface(readdraw);
}

EGLDisplay
eglGetCurrentDisplay(void)
{
	INIT_EXPORT();
	return ovr_eglGetCurrentDisplay();
}

EGLBoolean
eglQueryContext(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value)
{
	INIT_EXPORT();
	return ovr_eglQueryContext(dpy, ctx, attribute, value);
}

EGLBoolean
eglWaitGL(void)
{
	INIT_EXPORT();
	return ovr_eglWaitGL();
}

EGLBoolean
eglWaitNative(EGLint engine)
{
	INIT_EXPORT();
	return ovr_eglWaitNative(engine);
}

EGLBoolean
eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
	INIT_EXPORT();
	return ovr_eglSwapBuffers(dpy, surface);
}

EGLBoolean
eglCopyBuffers(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target)
{
	INIT_EXPORT();
	return ovr_eglCopyBuffers(dpy, surface, target);
}

_eng_fn
eglGetProcAddress(const char *procname)
{
	INIT_EXPORT();
	return ovr_eglGetProcAddress(procname);
}

const char *
eglQueryString(EGLDisplay dpy, EGLint name)
{
	INIT_EXPORT();
	return ovr_eglQueryString(dpy, name);
}

