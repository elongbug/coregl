#include "coregl_internal.h"
#include "coregl_export.h"

#include <stdlib.h>

#define COREGL_OVERRIDE_API(mangle, func, prefix) \
   mangle##func = prefix##func

void
init_overrides()
{
#define C_ORD(f) COREGL_OVERRIDE_API(ovr_, f, _sym_)

#define _COREGL_SYMBOL(IS_EXTENSION, RET_TYPE, FUNC_NAME, PARAM_LIST)     C_ORD(FUNC_NAME);
# include "headers/sym_egl.h"
#undef _COREGL_SYMBOL

#define _COREGL_SYMBOL(IS_EXTENSION, RET_TYPE, FUNC_NAME, PARAM_LIST)     C_ORD(FUNC_NAME);
# include "headers/sym_gl.h"
#undef _COREGL_SYMBOL

#undef C_ORD
}

