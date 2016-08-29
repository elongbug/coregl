// Defult use-case for exporting symbols
#ifndef _COREGL_SYMBOL
#define _COREGL_SYMBOL_NOT_DEFINED
#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     extern RET_TYPE (*_COREGL_NAME_MANGLE(FUNC_NAME)) PARAM_LIST;
#endif
#ifndef _COREGL_EXT_SYMBOL
#define _COREGL_EXT_SYMBOL_NOT_DEFINED
#define _COREGL_EXT_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)  _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)
#endif

_COREGL_SYMBOL(_eng_fn, glXGetProcAddress, (const char *procName))

// Standard GL(glX) functions
_COREGL_SYMBOL(XVisualInfo *, glXChooseVisual, (Display *dpy, int screen,
			   int *attribList))
_COREGL_SYMBOL(GLXContext, glXCreateContext, (Display *dpy, XVisualInfo *vis,
			   GLXContext shareList, Bool direct))
_COREGL_SYMBOL(void, glXDestroyContext, (Display *dpy, GLXContext ctx))
_COREGL_SYMBOL(GLXContext, glXGetCurrentContext, (void))
_COREGL_SYMBOL(GLXDrawable, glXGetCurrentDrawable, (void))
_COREGL_SYMBOL(Bool, glXMakeCurrent, (Display *dpy, GLXDrawable draw,
									  GLXContext ctx))
_COREGL_SYMBOL(void, glXSwapBuffers, (Display *dpy, GLXDrawable draw))
_COREGL_SYMBOL(void, glXWaitX, (void))
_COREGL_SYMBOL(void, glXWaitGL, (void))
_COREGL_SYMBOL(Bool, glXQueryExtension, (Display *dpy, int *errorb, int *event))
_COREGL_SYMBOL(const char *, glXQueryExtensionsString, (Display *dpy,
			   int screen))

_COREGL_SYMBOL(GLXFBConfig *, glXChooseFBConfig, (Display *dpy, int screen,
			   const int *attribList, int *nitems))
_COREGL_SYMBOL(GLXFBConfig *, glXGetFBConfigs, (Display *dpy, int screen,
			   int *nelements))
_COREGL_SYMBOL(int, glXGetFBConfigAttrib, (Display *dpy, GLXFBConfig config,
			   int attribute, int *value))
_COREGL_SYMBOL(XVisualInfo *, glXGetVisualFromFBConfig, (Display *dpy,
			   GLXFBConfig config))
_COREGL_SYMBOL(void, glXDestroyWindow, (Display *dpy, GLXWindow window))
_COREGL_SYMBOL(Bool, glXMakeContextCurrent, (Display *dpy, GLXDrawable draw,
			   GLXDrawable read, GLXContext ctx))

_COREGL_SYMBOL(void, glXBindTexImage, (Display *dpy, GLXDrawable draw,
									   int buffer, int *attribList))
_COREGL_SYMBOL(void, glXReleaseTexImage, (Display *dpy, GLXDrawable draw,
			   int buffer))
_COREGL_SYMBOL(int, glXGetVideoSync, (unsigned int *count))
_COREGL_SYMBOL(int, glXWaitVideoSync, (int divisor, int remainder,
									   unsigned int *count))
_COREGL_SYMBOL(XID, glXCreatePixmap, (Display *dpy, void *config, Pixmap pixmap,
									  const int *attribList))
_COREGL_SYMBOL(void, glXDestroyPixmap, (Display *dpy, XID pixmap))
_COREGL_SYMBOL(void, glXQueryDrawable, (Display *dpy, XID draw, int attribute,
										unsigned int *value))
_COREGL_SYMBOL(int, glXSwapIntervalSGI, (int interval))
_COREGL_SYMBOL(void, glXSwapIntervalEXT, (Display *dpy, GLXDrawable draw,
			   int interval))

#ifdef _COREGL_EXT_SYMBOL_NOT_DEFINED
#undef _COREGL_EXT_SYMBOL_NOT_DEFINED
#undef _COREGL_EXT_SYMBOL
#endif

#ifdef _COREGL_SYMBOL_NOT_DEFINED
#undef _COREGL_SYMBOL_NOT_DEFINED
#undef _COREGL_SYMBOL
#endif

