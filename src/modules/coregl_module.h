#ifndef COREGL_MODULE_H
#define COREGL_MODULE_H

#define COREGL_USE_MODULE_TRACEPATH
#define COREGL_USE_MODULE_FASTPATH
#define COREGL_USE_MODULE_APPOPT


typedef enum {
	COREGL_MODULE_TRACEPATH = 0,
	COREGL_MODULE_FASTPATH = 1,
	COREGL_MODULE_APPOPT = 2
} CoreGL_Modules;

#define COREGL_MAX_MODULES		5

#define GET_MY_TSTATE(val, stmt) \
	if (stmt != NULL) \
		val = ((MY_MODULE_TSTATE *)(stmt)->module_data[MY_MODULE_ID]);

#define COREGL_INIT_ORIGINAL(orig_prefix, f) \
{ \
	if (enable == 1) \
	{ \
		if (ovr_##f != NULL) COREGL_OVERRIDE_API(orig_prefix, f, ovr_); \
	} \
	else \
	{ \
		AST(ovr_##f != NULL); \
		COREGL_OVERRIDE_API(ovr_, f, orig_prefix); \
		orig_prefix##f = NULL; \
	} \
}

#define COREGL_OVERRIDE(prefix, f) \
	if (enable == 1) \
	{ \
		COREGL_OVERRIDE_API(ovr_, f, prefix); \
	}

#endif // COREGL_MODULE_H

