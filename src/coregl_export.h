#ifndef COREGL_EXPORT_H
#define COREGL_EXPORT_H

#include "coregl_internal.h"

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     extern RET_TYPE (*ovr_##FUNC_NAME) PARAM_LIST;
# include "headers/sym.h"
#undef _COREGL_SYMBOL

extern int export_initialized;
extern int driver_gl_version;
extern int current_gl_api_version;

extern void     init_export(GLboolean init_egl, GLboolean init_gl);
extern void     clean_overrides();

#endif // COREGL_EXPORT_H

