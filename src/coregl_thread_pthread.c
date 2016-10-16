#include "coregl_internal.h"

//////////////////////////////////////////////////////////////////////////
// Need implement this
int mutex_init(Mutex *mt);
int mutex_lock(Mutex *mt);
int mutex_unlock(Mutex *mt);
int get_current_thread();
int set_current_thread_state(GLThreadState *tstate);
GLThreadState *get_current_thread_state();
//////////////////////////////////////////////////////////////////////////

static Mutex            thread_key_mutex = MUTEX_INITIALIZER;
static int              thread_key_inited = 0;
static pthread_key_t    thread_key = 0;

int
mutex_init(Mutex *mt)
{
	int ret = 0;

	if (pthread_mutex_init(mt, NULL) == 0)
		ret = 1;
	else
		ret = 0;

	return ret;
}

int
mutex_lock(Mutex *mt)
{
	int ret = 0;

	if (pthread_mutex_lock(mt) == 0)
		ret = 1;
	else
		ret = 0;

	return ret;
}

int
mutex_unlock(Mutex *mt)
{
	int ret = 0;

	if (pthread_mutex_unlock(mt) == 0)
		ret = 1;
	else
		ret = 0;

	return ret;
}

int
get_current_thread()
{
	return pthread_self();
}

int
set_current_thread_state(GLThreadState *tstate)
{
	int ret = 0;

	AST(mutex_lock(&thread_key_mutex) == 1);

	if (thread_key_inited == 0) {
		if (pthread_key_create(&thread_key, NULL) != 0) {
			COREGL_ERR("Failed to create thread key.");
			ret = 0;
			goto finish;
		}
		thread_key_inited = 1;
	}

	if (pthread_setspecific(thread_key, (void *)tstate) != 0) {
		COREGL_ERR("Failed to set thread data.");
		ret = 0;
		goto finish;
	}

	ret = 1;
	goto finish;

finish:
	AST(mutex_unlock(&thread_key_mutex) == 1);

	return ret;
}

GLThreadState *
get_current_thread_state()
{
	GLThreadState *ret = NULL;

	if (thread_key_inited) {
		ret = (GLThreadState *)pthread_getspecific(thread_key);
	}
	goto finish;

finish:
	return ret;
}

