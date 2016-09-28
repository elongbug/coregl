/*
* File :
*	coregl_gl_common.c
* Description:
*	APIs in this file are both implemented in GLES1.1 and GLES2.0, this file will be compiled into
*	libGLESv1_CM and libGLESv2
*/

#include <stdio.h>
#include <dlfcn.h>

# include <GLES/glplatform.h>
# include "../headers/gl.h"

///////////////////////////////////////
// Disable dlog for debugging urgent issues //
//#define LOG_TAG "CoreGL_GLES2"
//#include <dlog.h>
#define LOGE(...) fprintf(stderr, __VA_ARGS__)
#define LOGW(...) fprintf(stderr, __VA_ARGS__)
#define LOGD(...) fprintf(stderr, __VA_ARGS__)
///////////////////////////////////////

#define COREGL_API           __attribute__((visibility("default")))

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     COREGL_API extern RET_TYPE FUNC_NAME PARAM_LIST;
#define _COREGL_EXT_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)
# include "../headers/sym_gl_common.h"
#undef _COREGL_EXT_SYMBOL
#undef _COREGL_SYMBOL

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     RET_TYPE (*ovr_##FUNC_NAME) PARAM_LIST = NULL;
#define _COREGL_EXT_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)
# include "../headers/sym_gl_common.h"
#undef _COREGL_EXT_SYMBOL
#undef _COREGL_SYMBOL

#define INIT_EXPORT()

void *lib_handle_common = NULL;

__attribute__((constructor))
int
coregl_glwrap_init_common()
{
	lib_handle_common = dlopen(_COREGL_LIB, RTLD_NOW);
	if (!lib_handle_common) {
		LOGE(" \E[40;31;1m%s\E[0m\n\n", dlerror());
		LOGE(" \E[40;31;1mInvalid library link! (Check linkage of libEGL -> libCOREGL)\E[0m\n");
		return 0;
	}

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST) \
   ovr_##FUNC_NAME = (__typeof__(ovr_##FUNC_NAME))dlsym(lib_handle_common, "coregl_api_"#FUNC_NAME); \
	if (ovr_##FUNC_NAME == NULL) \
	{ \
		LOGE("\E[40;31;1mCan't find a symbol '%s'!\E[0m\n\n", #FUNC_NAME); \
		LOGE("\E[40;31;1mInvalid library link! (Check linkage of libGLESv2 -> libCOREGL)\E[0m\n"); \
	}

#define _COREGL_EXT_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)
#include "../headers/sym_gl_common.h"
#undef _COREGL_EXT_SYMBOL
#undef _COREGL_SYMBOL

	return 1;
}

void
glActiveTexture(GLenum texture)
{
	ovr_glActiveTexture(texture);
}

void
glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	ovr_glClearColor(red, green, blue, alpha);
}

void
glClearDepthf(GLclampf depth)
{
	ovr_glClearDepthf(depth);
}

void
glDepthRangef(GLclampf zNear, GLclampf zFar)
{
	ovr_glDepthRangef(zNear, zFar);
}

void
glGetFloatv(GLenum pname, GLfloat *params)
{
	ovr_glGetFloatv(pname, params);
}

void
glGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
{
	ovr_glGetTexParameterfv(target, pname, params);
}

void
glLineWidth(GLfloat width)
{
	ovr_glLineWidth(width);
}

void
glPolygonOffset(GLfloat factor, GLfloat units)
{
	ovr_glPolygonOffset(factor, units);
}

void
glTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
	ovr_glTexParameterf(target, pname, param);
}

void
glTexParameterfv(GLenum target, GLenum pname, const GLfloat *params)
{
	ovr_glTexParameterfv(target, pname, params);
}

void
glBindBuffer(GLenum target, GLuint buffer)
{
	ovr_glBindBuffer(target, buffer);
}

void
glBindTexture(GLenum target, GLuint texture)
{
	ovr_glBindTexture(target, texture);
}

void
glBlendFunc(GLenum sfactor, GLenum dfactor)
{
	ovr_glBlendFunc(sfactor, dfactor);
}

void
glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage)
{
	ovr_glBufferData(target, size, data, usage);
}

void
glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
				const void *data)
{
	ovr_glBufferSubData(target, offset, size, data);
}

void
glClear(GLbitfield mask)
{
	ovr_glClear(mask);
}

void
glClearStencil(GLint s)
{
	ovr_glClearStencil(s);
}

void
glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	ovr_glColorMask(red, green, blue, alpha);
}

void
glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat,
					   GLsizei width, GLsizei height, GLint border, GLsizei imageSize,
					   const void *data)
{
	ovr_glCompressedTexImage2D(target, level, internalformat, width, height, border,
							   imageSize, data);
}

void
glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset,
						  GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize,
						  const void *data)
{
	ovr_glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height,
								  format, imageSize, data);
}

void
glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x,
				 GLint y, GLsizei width, GLsizei height, GLint border)
{
	ovr_glCopyTexImage2D(target, level, internalformat, x, y, width, height,
						 border);
}

void
glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset,
					GLint x, GLint y, GLsizei width, GLsizei height)
{
	ovr_glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

void
glCullFace(GLenum mode)
{
	ovr_glCullFace(mode);
}

void
glDeleteBuffers(GLsizei n, const GLuint *buffers)
{
	ovr_glDeleteBuffers(n, buffers);
}

void
glDeleteTextures(GLsizei n, const GLuint *textures)
{
	ovr_glDeleteTextures(n, textures);
}

void
glDepthFunc(GLenum func)
{
	ovr_glDepthFunc(func);
}

void
glDepthMask(GLboolean flag)
{
	ovr_glDepthMask(flag);
}

void
glDisable(GLenum cap)
{
	ovr_glDisable(cap);
}

void
glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
	ovr_glDrawArrays(mode, first, count);
}

void
glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices)
{
	ovr_glDrawElements(mode, count, type, indices);
}

void
glEnable(GLenum cap)
{
	ovr_glEnable(cap);
}

void
glFinish(void)
{
	ovr_glFinish();
}

void
glFlush(void)
{
	ovr_glFlush();
}

void
glFrontFace(GLenum mode)
{
	ovr_glFrontFace(mode);
}

void
glGetBooleanv(GLenum pname, GLboolean *params)
{
	ovr_glGetBooleanv(pname, params);
}

void
glGetBufferParameteriv(GLenum target, GLenum pname, GLint *params)
{
	ovr_glGetBufferParameteriv(target, pname, params);
}

void
glGenBuffers(GLsizei n, GLuint *buffers)
{
	ovr_glGenBuffers(n, buffers);
}

void
glGenTextures(GLsizei n, GLuint *textures)
{
	ovr_glGenTextures(n, textures);
}

GLenum
glGetError(void)
{
	return ovr_glGetError();
}

void
glGetIntegerv(GLenum pname, GLint *params)
{
	ovr_glGetIntegerv(pname, params);
}

void
glGetPointerv(GLenum pname, GLvoid **params)
{
	ovr_glGetPointerv(pname, params);
}

const GLubyte *
glGetString(GLenum name)
{
	return ovr_glGetString(name);
}

void
glGetTexParameteriv(GLenum target, GLenum pname, GLint *params)
{
	ovr_glGetTexParameteriv(target, pname, params);
}

void
glHint(GLenum target, GLenum mode)
{
	ovr_glHint(target, mode);
}

GLboolean
glIsBuffer(GLuint buffer)
{
	return ovr_glIsBuffer(buffer);
}

GLboolean
glIsEnabled(GLenum cap)
{
	return ovr_glIsEnabled(cap);
}

GLboolean
glIsTexture(GLuint texture)
{
	return ovr_glIsTexture(texture);
}
void
glPixelStorei(GLenum pname, GLint param)
{
	ovr_glPixelStorei(pname, param);
}

void
glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format,
			 GLenum type, void *pixels)
{
	ovr_glReadPixels(x, y, width, height, format, type, pixels);
}

void
glSampleCoverage(GLclampf value, GLboolean invert)
{
	ovr_glSampleCoverage(value, invert);
}

void
glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
	ovr_glScissor(x, y, width, height);
}


void
glStencilFunc(GLenum func, GLint ref, GLuint mask)
{
	ovr_glStencilFunc(func, ref, mask);
}

void
glStencilMask(GLuint mask)
{
	ovr_glStencilMask(mask);
}

void
glStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
	ovr_glStencilOp(fail, zfail, zpass);
}

void
glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width,
			 GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
	ovr_glTexImage2D(target, level, internalformat, width, height, border, format,
					 type, pixels);
}

void
glTexParameteri(GLenum target, GLenum pname, GLint param)
{
	ovr_glTexParameteri(target, pname, param);
}

void
glTexParameteriv(GLenum target, GLenum pname, const GLint *params)
{
	ovr_glTexParameteriv(target, pname, params);
}

void
glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset,
				GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
{
	ovr_glTexSubImage2D(target, level, xoffset, yoffset, width, height, format,
						type, pixels);
}

void
glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	ovr_glViewport(x, y, width, height);
}
