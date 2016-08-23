#include "coregl_export.h"

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     RET_TYPE (*ovr_##FUNC_NAME) PARAM_LIST = NULL;
#include "headers/sym.h"
#undef _COREGL_SYMBOL

#include "coregl_internal.h"
#include "coregl_export.h"

#include <stdlib.h>

int export_initialized = 0;
static int api_gl_version;

static void
_clean_overrides(GLboolean init_egl, GLboolean init_gl)
{
#define _COREGL_START_API(version) api_gl_version = version;
#define _COREGL_END_API(version) api_gl_version = COREGL_GLAPI_2;

#define OVERRIDE(f) \
		COREGL_OVERRIDE_API(ovr_, f, _sym_)

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     OVERRIDE(FUNC_NAME);
	if(init_egl == GL_TRUE) {
		# include "headers/sym_egl.h"
	}
#undef _COREGL_SYMBOL
#undef OVERRIDE

#define OVERRIDE(f) \
	if(api_gl_version<=driver_gl_version) COREGL_OVERRIDE_API(ovr_, f, _sym_)
#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     OVERRIDE(FUNC_NAME);
	if(init_gl == GL_TRUE) {
		#include "headers/sym_gl1.h"
		#include "headers/sym_gl2.h"
		#include "headers/sym_gl_common.h"
	}
#undef _COREGL_SYMBOL
#undef OVERRIDE

#undef _COREGL_START_API
#undef _COREGL_END_API
}

void
init_export(GLboolean init_egl, GLboolean init_gl)
{
	_clean_overrides(init_egl, init_gl);
}

