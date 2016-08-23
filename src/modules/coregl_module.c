#include "../coregl_internal.h"

// TRACEPATH module
#include "tracepath/coregl_tracepath.h"
#undef MY_MODULE_ID
#undef MY_MODULE_TSTATE

// FASTPATH module
#include "fastpath/coregl_fastpath.h"
#undef MY_MODULE_ID
#undef MY_MODULE_TSTATE

// APIOPT module
#include "appopt/coregl_appopt.h"
#undef MY_MODULE_ID
#undef MY_MODULE_TSTATE

void
init_modules()
{
	init_modules_fastpath();
	init_modules_appopt();
	init_modules_tracepath();

	reset_modules_override();
}

void
deinit_modules()
{
	deinit_modules_tracepath();
	deinit_modules_appopt();
	deinit_modules_fastpath();
}

void
reset_modules_override()
{
	// Step 1 : Initialization
	init_export(GL_TRUE, GL_TRUE);

	// Step 2 : User Define Modules : Sequence is important! (Last module's API is called first)
	fastpath_apply_overrides();

	appopt_apply_overrides();

	// Step 3 : Common Wrapping Modules
	tracepath_apply_overrides();
}

void
init_modules_tstate(GLThreadState *tstate)
{
	// Initialization for each thread state : Sequence is important!
	init_modules_tstate_tracepath(tstate);
	init_modules_tstate_fastpath(tstate);
	init_modules_tstate_appopt(tstate);
}

void
deinit_modules_tstate(GLThreadState *tstate)
{
	// De-Initialization for each thread state(rollback) : Sequence is important!
	deinit_modules_tstate_appopt(tstate);
	deinit_modules_tstate_fastpath(tstate);
	deinit_modules_tstate_tracepath(tstate);
}

