#include "coregl_appopt.h"

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     RET_TYPE (*_orig_appopt_##FUNC_NAME) PARAM_LIST = NULL;
#define _COREGL_EXT_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST) _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)
# include "../../headers/sym.h"
#undef _COREGL_EXT_SYMBOL
#undef _COREGL_SYMBOL

void
init_modules_appopt()
{
}

void
deinit_modules_appopt()
{
}

void
init_modules_tstate_appopt(GLThreadState *tstate)
{
	MY_MODULE_TSTATE *tstate_mt = NULL;

	tstate_mt = (MY_MODULE_TSTATE *)calloc(1, sizeof(MY_MODULE_TSTATE));


	tstate->module_data[MY_MODULE_ID] = tstate_mt;
}

void
deinit_modules_tstate_appopt(GLThreadState *tstate)
{
	if (tstate->module_data[MY_MODULE_ID] != NULL) {
		free(tstate->module_data[MY_MODULE_ID]);
		tstate->module_data[MY_MODULE_ID] = NULL;
	}
}

void
appopt_apply_overrides()
{
	//appopt_apply_overrides_egl(1);
	//appopt_apply_overrides_gl(1);
}

void
appopt_apply_overrides_egl(int enable)
{
#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     COREGL_INIT_ORIGINAL(_orig_appopt_, FUNC_NAME);
#define _COREGL_EXT_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST) _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)
# include "../../headers/sym_egl.h"
#undef _COREGL_EXT_SYMBOL
#undef _COREGL_SYMBOL
}

void
appopt_apply_overrides_gl(int enable)
{
#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     COREGL_INIT_ORIGINAL(_orig_appopt_, FUNC_NAME);
#define _COREGL_EXT_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST) _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)
# include "../../headers/sym_gl1.h"
# include "../../headers/sym_gl2.h"
# include "../../headers/sym_gl_common.h"
#undef _COREGL_EXT_SYMBOL
#undef _COREGL_SYMBOL
}

#undef A_ORD

