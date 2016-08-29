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
# include "../headers/sym_gl2.h"
#undef _COREGL_EXT_SYMBOL
#undef _COREGL_SYMBOL

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     RET_TYPE (*ovr_##FUNC_NAME) PARAM_LIST = NULL;
#define _COREGL_EXT_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)
# include "../headers/sym_gl2.h"
#undef _COREGL_EXT_SYMBOL
#undef _COREGL_SYMBOL

#define INIT_EXPORT()

void *lib_handle = NULL;
void (* set_driver_gl_version)(int version);

__attribute__((constructor))
int
coregl_glwrap_init()
{
	lib_handle = dlopen("libCOREGL.so", RTLD_NOW);
	if (!lib_handle) {
		LOGE(" \E[40;31;1m%s\E[0m\n\n", dlerror());
		LOGE(" \E[40;31;1mInvalid library link! (Check linkage of libEGL -> libCOREGL)\E[0m\n");
		return 0;
	}

	set_driver_gl_version = dlsym(lib_handle, "set_driver_gl_version");
	if (set_driver_gl_version)
		set_driver_gl_version(2);
	else
		LOGE("%s\n", dlerror());

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST) \
   ovr_##FUNC_NAME = (__typeof__(ovr_##FUNC_NAME))dlsym(lib_handle, "coregl_api_"#FUNC_NAME); \
	if (ovr_##FUNC_NAME == NULL) \
	{ \
		LOGE("\E[40;31;1mCan't find a symbol '%s'!\E[0m\n\n", #FUNC_NAME); \
		LOGE("\E[40;31;1mInvalid library link! (Check linkage of libGLESv2 -> libCOREGL)\E[0m\n"); \
	}

#define _COREGL_EXT_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)
#include "../headers/sym_gl2.h"
#undef _COREGL_EXT_SYMBOL
#undef _COREGL_SYMBOL

	return 1;
}

void
glAttachShader(GLuint program, GLuint shader)
{
	ovr_glAttachShader(program, shader);
}

void
glBindAttribLocation(GLuint program, GLuint index, const char *name)
{
	ovr_glBindAttribLocation(program, index, name);
}

void
glBindFramebuffer(GLenum target, GLuint framebuffer)
{
	ovr_glBindFramebuffer(target, framebuffer);
}

void
glBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
	ovr_glBindRenderbuffer(target, renderbuffer);
}

void
glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	ovr_glBlendColor(red, green, blue, alpha);
}

void
glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
	ovr_glBlendEquationSeparate(modeRGB, modeAlpha);
}

void
glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha,
					GLenum dstAlpha)
{
	ovr_glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void
glBlendEquation(GLenum mode)
{
	ovr_glBlendEquation(mode);
}

GLenum
glCheckFramebufferStatus(GLenum target)
{
	return ovr_glCheckFramebufferStatus(target);
}

void
glCompileShader(GLuint shader)
{
	ovr_glCompileShader(shader);
}


GLuint
glCreateProgram(void)
{
	return ovr_glCreateProgram();
}

GLuint
glCreateShader(GLenum type)
{
	return ovr_glCreateShader(type);
}

void
glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
{
	ovr_glDeleteFramebuffers(n, framebuffers);
}

void
glDeleteProgram(GLuint program)
{
	ovr_glDeleteProgram(program);
}

void
glDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
{
	ovr_glDeleteRenderbuffers(n, renderbuffers);
}

void
glDeleteShader(GLuint shader)
{
	ovr_glDeleteShader(shader);
}

void
glDetachShader(GLuint program, GLuint shader)
{
	ovr_glDetachShader(program, shader);
}

void
glDisableVertexAttribArray(GLuint index)
{
	ovr_glDisableVertexAttribArray(index);
}

void
glEnableVertexAttribArray(GLuint index)
{
	ovr_glEnableVertexAttribArray(index);
}

void
glFramebufferRenderbuffer(GLenum target, GLenum attachment,
						  GLenum renderbuffertarget, GLuint renderbuffer)
{
	ovr_glFramebufferRenderbuffer(target, attachment, renderbuffertarget,
								  renderbuffer);
}

void
glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget,
					   GLuint texture, GLint level)
{
	ovr_glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

void
glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params)
{
	ovr_glGetVertexAttribfv(index, pname, params);
}

void
glGetVertexAttribiv(GLuint index, GLenum pname, GLint *params)
{
	ovr_glGetVertexAttribiv(index, pname, params);
}

void
glGetVertexAttribPointerv(GLuint index, GLenum pname, void **pointer)
{
	ovr_glGetVertexAttribPointerv(index, pname, pointer);
}

void
glGenerateMipmap(GLenum target)
{
	ovr_glGenerateMipmap(target);
}

void
glGenFramebuffers(GLsizei n, GLuint *framebuffers)
{
	ovr_glGenFramebuffers(n, framebuffers);
}

void
glGenRenderbuffers(GLsizei n, GLuint *renderbuffers)
{
	ovr_glGenRenderbuffers(n, renderbuffers);
}

void
glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize,
				  GLsizei *length, GLint *size, GLenum *type, char *name)
{
	ovr_glGetActiveAttrib(program, index, bufsize, length, size, type, name);
}

void
glGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize,
				   GLsizei *length, GLint *size, GLenum *type, char *name)
{
	ovr_glGetActiveUniform(program, index, bufsize, length, size, type, name);
}

void
glGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei *count,
					 GLuint *shaders)
{
	ovr_glGetAttachedShaders(program, maxcount, count, shaders);
}

int
glGetAttribLocation(GLuint program, const char *name)
{
	return ovr_glGetAttribLocation(program, name);
}

void
glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment,
									  GLenum pname, GLint *params)
{
	ovr_glGetFramebufferAttachmentParameteriv(target, attachment, pname, params);
}

void
glGetProgramiv(GLuint program, GLenum pname, GLint *params)
{
	ovr_glGetProgramiv(program, pname, params);
}

void
glGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei *length,
					char *infolog)
{
	ovr_glGetProgramInfoLog(program, bufsize, length, infolog);
}

void
glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
{
	ovr_glGetRenderbufferParameteriv(target, pname, params);
}

void
glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
	ovr_glGetShaderiv(shader, pname, params);
}

void
glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei *length,
				   char *infolog)
{
	ovr_glGetShaderInfoLog(shader, bufsize, length, infolog);
}

void
glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype,
						   GLint *range, GLint *precision)
{
	ovr_glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
}

void
glGetShaderSource(GLuint shader, GLsizei bufsize, GLsizei *length, char *source)
{
	ovr_glGetShaderSource(shader, bufsize, length, source);
}

void
glGetUniformfv(GLuint program, GLint location, GLfloat *params)
{
	ovr_glGetUniformfv(program, location, params);
}

void
glGetUniformiv(GLuint program, GLint location, GLint *params)
{
	ovr_glGetUniformiv(program, location, params);
}

int
glGetUniformLocation(GLuint program, const char *name)
{
	return ovr_glGetUniformLocation(program, name);
}

GLboolean
glIsFramebuffer(GLuint framebuffer)
{
	return ovr_glIsFramebuffer(framebuffer);
}

GLboolean
glIsProgram(GLuint program)
{
	return ovr_glIsProgram(program);
}

GLboolean
glIsRenderbuffer(GLuint renderbuffer)
{
	return ovr_glIsRenderbuffer(renderbuffer);
}

GLboolean
glIsShader(GLuint shader)
{
	return ovr_glIsShader(shader);
}

void
glLinkProgram(GLuint program)
{
	ovr_glLinkProgram(program);
}

void
glReleaseShaderCompiler(void)
{
	ovr_glReleaseShaderCompiler();
}

void
glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width,
					  GLsizei height)
{
	ovr_glRenderbufferStorage(target, internalformat, width, height);
}

void
glShaderBinary(GLsizei n, const GLuint *shaders, GLenum binaryformat,
			   const void *binary, GLsizei length)
{
	ovr_glShaderBinary(n, shaders, binaryformat, binary, length);
}

void
glShaderSource(GLuint shader, GLsizei count, const char **string,
			   const GLint *length)
{
	ovr_glShaderSource(shader, count, string, length);
}


void
glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
{
	ovr_glStencilFuncSeparate(face, func, ref, mask);
}

void
glStencilMaskSeparate(GLenum face, GLuint mask)
{
	ovr_glStencilMaskSeparate(face, mask);
}

void
glStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
{
	ovr_glStencilOpSeparate(face, fail, zfail, zpass);
}

void
glUniform1f(GLint location, GLfloat x)
{
	ovr_glUniform1f(location, x);
}

void
glUniform1fv(GLint location, GLsizei count, const GLfloat *v)
{
	ovr_glUniform1fv(location, count, v);
}

void
glUniform1i(GLint location, GLint x)
{
	ovr_glUniform1i(location, x);
}

void
glUniform1iv(GLint location, GLsizei count, const GLint *v)
{
	ovr_glUniform1iv(location, count, v);
}

void
glUniform2f(GLint location, GLfloat x, GLfloat y)
{
	ovr_glUniform2f(location, x, y);
}

void
glUniform2fv(GLint location, GLsizei count, const GLfloat *v)
{
	ovr_glUniform2fv(location, count, v);
}

void
glUniform2i(GLint location, GLint x, GLint y)
{
	ovr_glUniform2i(location, x, y);
}

void
glUniform2iv(GLint location, GLsizei count, const GLint *v)
{
	ovr_glUniform2iv(location, count, v);
}

void
glUniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z)
{
	ovr_glUniform3f(location, x, y, z);
}

void
glUniform3fv(GLint location, GLsizei count, const GLfloat *v)
{
	ovr_glUniform3fv(location, count, v);
}

void
glUniform3i(GLint location, GLint x, GLint y, GLint z)
{
	ovr_glUniform3i(location, x, y, z);
}

void
glUniform3iv(GLint location, GLsizei count, const GLint *v)
{
	ovr_glUniform3iv(location, count, v);
}

void
glUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	ovr_glUniform4f(location, x, y, z, w);
}

void
glUniform4fv(GLint location, GLsizei count, const GLfloat *v)
{
	ovr_glUniform4fv(location, count, v);
}

void
glUniform4i(GLint location, GLint x, GLint y, GLint z, GLint w)
{
	ovr_glUniform4i(location, x, y, z, w);
}

void
glUniform4iv(GLint location, GLsizei count, const GLint *v)
{
	ovr_glUniform4iv(location, count, v);
}

void
glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose,
				   const GLfloat *value)
{
	ovr_glUniformMatrix2fv(location, count, transpose, value);
}

void
glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose,
				   const GLfloat *value)
{
	ovr_glUniformMatrix3fv(location, count, transpose, value);
}

void
glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose,
				   const GLfloat *value)
{
	ovr_glUniformMatrix4fv(location, count, transpose, value);
}

void
glActiveShaderProgram(GLuint pipeline, GLuint program)
{
	ovr_glActiveShaderProgram(pipeline, program);
}

void
glUseProgram(GLuint program)
{
	ovr_glUseProgram(program);
}

void
glValidateProgram(GLuint program)
{
	ovr_glValidateProgram(program);
}

void
glVertexAttrib1f(GLuint index, GLfloat x)
{
	ovr_glVertexAttrib1f(index, x);
}

void
glVertexAttrib1fv(GLuint index, const GLfloat *values)
{
	ovr_glVertexAttrib1fv(index, values);
}

void
glVertexAttrib2f(GLuint index, GLfloat x, GLfloat y)
{
	ovr_glVertexAttrib2f(index, x, y);
}

void
glVertexAttrib2fv(GLuint index, const GLfloat *values)
{
	ovr_glVertexAttrib2fv(index, values);
}

void
glVertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z)
{
	ovr_glVertexAttrib3f(index, x, y, z);
}

void
glVertexAttrib3fv(GLuint index, const GLfloat *values)
{
	ovr_glVertexAttrib3fv(index, values);
}

void
glVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	ovr_glVertexAttrib4f(index, x, y, z, w);
}

void
glVertexAttrib4fv(GLuint index, const GLfloat *values)
{
	ovr_glVertexAttrib4fv(index, values);
}

void
glVertexAttribPointer(GLuint index, GLint size, GLenum type,
					  GLboolean normalized, GLsizei stride, const void *pointer)
{
	ovr_glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

/* OpenGL ES 3.0 */
void
glReadBuffer(GLenum mode)
{
	ovr_glReadBuffer(mode);
}

void
glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count,
					GLenum type, const GLvoid *indices)
{
	ovr_glDrawRangeElements(mode, start, end, count, type, indices);
}

void
glTexImage3D(GLenum target, GLint level, GLint GLinternalFormat, GLsizei width,
			 GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type,
			 const GLvoid *pixels)
{
	ovr_glTexImage3D(target, level, GLinternalFormat, width, height, depth, border,
					 format, type, pixels);
}

void
glTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset,
				GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format,
				GLenum type, const GLvoid *pixels)
{
	ovr_glTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height,
						depth, format, type, pixels);
}

void
glCopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset,
					GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
	ovr_glCopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width,
							height);
}

void
glCompressedTexImage3D(GLenum target, GLint level, GLenum GLinternalformat,
					   GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize,
					   const GLvoid *data)
{
	ovr_glCompressedTexImage3D(target, level, GLinternalformat, width, height,
							   depth, border, imageSize, data);
}

void
glCompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset,
						  GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,
						  GLenum format, GLsizei imageSize, const GLvoid *data)
{
	ovr_glCompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width,
								  height, depth, format, imageSize, data);
}

void
glGenQueries(GLsizei n, GLuint *ids)
{
	ovr_glGenQueries(n, ids);
}

void
glDeleteQueries(GLsizei n, const GLuint *ids)
{
	ovr_glDeleteQueries(n, ids);
}

GLboolean
glIsQuery(GLuint id)
{
	return ovr_glIsQuery(id);
}

void
glBeginQuery(GLenum target, GLuint id)
{
	ovr_glBeginQuery(target, id);
}

void
glEndQuery(GLenum target)
{
	ovr_glEndQuery(target);
}

void
glGetQueryiv(GLenum target, GLenum pname, GLint *params)
{
	ovr_glGetQueryiv(target, pname, params);
}

void
glGetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params)
{
	ovr_glGetQueryObjectuiv(id, pname, params);
}

GLboolean
glUnmapBuffer(GLenum target)
{
	return ovr_glUnmapBuffer(target);
}

void
glGetBufferPointerv(GLenum target, GLenum pname, GLvoid **params)
{
	ovr_glGetBufferPointerv(target, pname, params);
}

void
glDrawBuffers(GLsizei n, const GLenum *bufs)
{
	ovr_glDrawBuffers(n, bufs);
}

void
glUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose,
					 const GLfloat *value)
{
	ovr_glUniformMatrix2x3fv(location, count, transpose, value);
}

void
glUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose,
					 const GLfloat *value)
{
	ovr_glUniformMatrix3x2fv(location, count, transpose, value);
}

void
glUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose,
					 const GLfloat *value)
{
	ovr_glUniformMatrix2x4fv(location, count, transpose, value);
}

void
glUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose,
					 const GLfloat *value)
{
	ovr_glUniformMatrix4x2fv(location, count, transpose, value);
}

void
glUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose,
					 const GLfloat *value)
{
	ovr_glUniformMatrix3x4fv(location, count, transpose, value);
}

void
glUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose,
					 const GLfloat *value)
{
	ovr_glUniformMatrix4x3fv(location, count, transpose, value);
}

void
glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
				  GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask,
				  GLenum filter)
{
	ovr_glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1,
						  mask, filter);
}

void
glRenderbufferStorageMultisample(GLenum target, GLsizei samples,
								 GLenum internalformat, GLsizei width, GLsizei height)
{
	ovr_glRenderbufferStorageMultisample(target, samples, internalformat, width,
										 height);
}

void
glFramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture,
						  GLint level, GLint layer)
{
	ovr_glFramebufferTextureLayer(target, attachment, texture, level, layer);
}

GLvoid *
glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length,
				 GLbitfield access)
{
	return ovr_glMapBufferRange(target, offset, length, access);
}

void
glFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length)
{
	ovr_glFlushMappedBufferRange(target, offset, length);
}

void
glBindVertexArray(GLuint array)
{
	ovr_glBindVertexArray(array);
}

void
glDeleteVertexArrays(GLsizei n, const GLuint *arrays)
{
	ovr_glDeleteVertexArrays(n, arrays);
}

void
glGenVertexArrays(GLsizei n, GLuint *arrays)
{
	ovr_glGenVertexArrays(n, arrays);
}

GLboolean
glIsVertexArray(GLuint array)
{
	return ovr_glIsVertexArray(array);
}

void
glGetIntegeri_v(GLenum target, GLuint index, GLint *data)
{
	ovr_glGetIntegeri_v(target, index, data);
}

void
glBeginTransformFeedback(GLenum primitiveMode)
{
	ovr_glBeginTransformFeedback(primitiveMode);
}

void
glEndTransformFeedback()
{
	ovr_glEndTransformFeedback();
}

void
glBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset,
				  GLsizeiptr size)
{
	ovr_glBindBufferRange(target, index, buffer, offset, size);
}

void
glBindBufferBase(GLenum target, GLuint index, GLuint buffer)
{
	ovr_glBindBufferBase(target, index, buffer);
}

void
glTransformFeedbackVaryings(GLuint program, GLsizei count,
							const GLchar *const *varyings, GLenum bufferMode)
{
	ovr_glTransformFeedbackVaryings(program, count, varyings, bufferMode);
}

void
glGetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize,
							  GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)
{
	ovr_glGetTransformFeedbackVarying(program, index, bufSize, length, size, type,
									  name);
}

void
glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride,
					   const GLvoid *pointer)
{
	ovr_glVertexAttribIPointer(index, size, type, stride, pointer);
}

void
glGetVertexAttribIiv(GLuint index, GLenum pname, GLint *params)
{
	ovr_glGetVertexAttribIiv(index, pname, params);
}

void
glGetVertexAttribIuiv(GLuint index, GLenum pname, GLuint *params)
{
	ovr_glGetVertexAttribIuiv(index, pname, params);
}

void
glVertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w)
{
	ovr_glVertexAttribI4i(index, x, y, z, w);
}

void
glVertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w)
{
	ovr_glVertexAttribI4ui(index, x, y, z, w);
}

void
glVertexAttribI4iv(GLuint index, const GLint *v)
{
	ovr_glVertexAttribI4iv(index, v);
}

void
glVertexAttribI4uiv(GLuint index, const GLuint *v)
{
	ovr_glVertexAttribI4uiv(index, v);
}

void
glGetUniformuiv(GLuint program, GLint location, GLuint *params)
{
	ovr_glGetUniformuiv(program, location, params);
}

GLint
glGetFragDataLocation(GLuint program, const GLchar *name)
{
	return ovr_glGetFragDataLocation(program, name);
}

void
glUniform1ui(GLint location, GLuint v0)
{
	ovr_glUniform1ui(location, v0);
}

void
glUniform2ui(GLint location, GLuint v0, GLuint v1)
{
	ovr_glUniform2ui(location, v0, v1);
}

void
glUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
{
	ovr_glUniform3ui(location, v0, v1, v2);
}

void
glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
{
	ovr_glUniform4ui(location, v0, v1, v2, v3);
}

void
glUniform1uiv(GLint location, GLsizei count, const GLuint *value)
{
	ovr_glUniform1uiv(location, count, value);
}

void
glUniform2uiv(GLint location, GLsizei count, const GLuint *value)
{
	ovr_glUniform2uiv(location, count, value);
}

void
glUniform3uiv(GLint location, GLsizei count, const GLuint *value)
{
	ovr_glUniform3uiv(location, count, value);
}

void
glUniform4uiv(GLint location, GLsizei count, const GLuint *value)
{
	ovr_glUniform4uiv(location, count, value);
}

void
glClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint *value)
{
	ovr_glClearBufferiv(buffer, drawbuffer, value);
}

void
glClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint *value)
{
	ovr_glClearBufferuiv(buffer, drawbuffer, value);
}

void
glClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat *value)
{
	ovr_glClearBufferfv(buffer, drawbuffer, value);
}

void
glClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)
{
	ovr_glClearBufferfi(buffer, drawbuffer, depth, stencil);
}

const GLubyte *
glGetStringi(GLenum name, GLuint index)
{
	return ovr_glGetStringi(name, index);
}

void
glCopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset,
					GLintptr writeOffset, GLsizeiptr size)
{
	ovr_glCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
}

void
glGetUniformIndices(GLuint program, GLsizei uniformCount,
					const GLchar *const *uniformNames, GLuint *uniformIndices)
{
	ovr_glGetUniformIndices(program, uniformCount, uniformNames, uniformIndices);
}

void
glGetActiveUniformsiv(GLuint program, GLsizei uniformCount,
					  const GLuint *uniformIndices, GLenum pname, GLint *params)
{
	ovr_glGetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params);
}

GLuint
glGetUniformBlockIndex(GLuint program, const GLchar *uniformBlockName)
{
	return ovr_glGetUniformBlockIndex(program, uniformBlockName);
}

void
glGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex,
						  GLenum pname, GLint *params)
{
	ovr_glGetActiveUniformBlockiv(program, uniformBlockIndex, pname, params);
}

void
glGetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex,
							GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName)
{
	ovr_glGetActiveUniformBlockName(program, uniformBlockIndex, bufSize, length,
									uniformBlockName);
}

void
glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex,
					  GLuint uniformBlockBinding)
{
	ovr_glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
}

void
glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count,
					  GLsizei instanceCount)
{
	ovr_glDrawArraysInstanced(mode, first, count, instanceCount);
}

void
glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type,
						const GLvoid *indices, GLsizei instanceCount)
{
	ovr_glDrawElementsInstanced(mode, count, type, indices, instanceCount);
}

GLsync
glFenceSync(GLenum condition, GLbitfield flags)
{
	return ovr_glFenceSync(condition, flags);
}

GLboolean
glIsSync(GLsync sync)
{
	return ovr_glIsSync(sync);
}

void
glDeleteSync(GLsync sync)
{
	ovr_glDeleteSync(sync);
}

GLenum
glClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
	return ovr_glClientWaitSync(sync, flags, timeout);
}

void
glWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
	ovr_glWaitSync(sync, flags, timeout);
}

void
glGetInteger64v(GLenum pname, GLint64 *params)
{
	ovr_glGetInteger64v(pname, params);
}

void
glGetSynciv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length,
			GLint *values)
{
	ovr_glGetSynciv(sync, pname, bufSize, length, values);
}

void
glGetInteger64i_v(GLenum target, GLuint index, GLint64 *data)
{
	ovr_glGetInteger64i_v(target, index, data);
}

void
glGetBufferParameteri64v(GLenum target, GLenum pname, GLint64 *params)
{
	ovr_glGetBufferParameteri64v(target, pname, params);
}

void
glGenSamplers(GLsizei n, GLuint *samplers)
{
	ovr_glGenSamplers(n, samplers);
}

void
glDeleteSamplers(GLsizei n, const GLuint *samplers)
{
	ovr_glDeleteSamplers(n, samplers);
}

GLboolean
glIsSampler(GLuint sampler)
{
	return ovr_glIsSampler(sampler);
}

void
glBindSampler(GLuint unit, GLuint sampler)
{
	ovr_glBindSampler(unit, sampler);
}

void
glSamplerParameteri(GLuint sampler, GLenum pname, GLint param)
{
	ovr_glSamplerParameteri(sampler, pname, param);
}

void
glSamplerParameteriv(GLuint sampler, GLenum pname, const GLint *param)
{
	ovr_glSamplerParameteriv(sampler, pname, param);
}

void
glSamplerParameterf(GLuint sampler, GLenum pname, GLfloat param)
{
	ovr_glSamplerParameterf(sampler, pname, param);
}

void
glSamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat *param)
{
	ovr_glSamplerParameterfv(sampler, pname, param);
}

void
glGetSamplerParameteriv(GLuint sampler, GLenum pname, GLint *params)
{
	ovr_glGetSamplerParameteriv(sampler, pname, params);
}

void
glGetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat *params)
{
	ovr_glGetSamplerParameterfv(sampler, pname, params);
}

void
glVertexAttribDivisor(GLuint index, GLuint divisor)
{
	ovr_glVertexAttribDivisor(index, divisor);
}

void
glBindTransformFeedback(GLenum target, GLuint id)
{
	ovr_glBindTransformFeedback(target, id);
}

void
glDeleteTransformFeedbacks(GLsizei n, const GLuint *ids)
{
	ovr_glDeleteTransformFeedbacks(n, ids);
}

void
glGenTransformFeedbacks(GLsizei n, GLuint *ids)
{
	ovr_glGenTransformFeedbacks(n, ids);
}

GLboolean
glIsTransformFeedback(GLuint id)
{
	return ovr_glIsTransformFeedback(id);
}

void
glPauseTransformFeedback()
{
	ovr_glPauseTransformFeedback();
}

void
glResumeTransformFeedback()
{
	ovr_glResumeTransformFeedback();
}

void
glGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei *length,
				   GLenum *binaryFormat, GLvoid *binary)
{
	ovr_glGetProgramBinary(program, bufSize, length, binaryFormat, binary);
}

void
glProgramBinary(GLuint program, GLenum binaryFormat, const GLvoid *binary,
				GLsizei length)
{
	ovr_glProgramBinary(program, binaryFormat, binary, length);
}

void
glProgramParameteri(GLuint program, GLenum pname, GLint value)
{
	ovr_glProgramParameteri(program, pname, value);
}

void
glInvalidateFramebuffer(GLenum target, GLsizei numAttachments,
						const GLenum *attachments)
{
	ovr_glInvalidateFramebuffer(target, numAttachments, attachments);
}

void
glInvalidateSubFramebuffer(GLenum target, GLsizei numAttachments,
						   const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height)
{
	ovr_glInvalidateSubFramebuffer(target, numAttachments, attachments, x, y, width,
								   height);
}

void
glTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat,
			   GLsizei width, GLsizei height)
{
	ovr_glTexStorage2D(target, levels, internalformat, width, height);
}

void
glTexStorage3D(GLenum target, GLsizei levels, GLenum internalformat,
			   GLsizei width, GLsizei height, GLsizei depth)
{
	ovr_glTexStorage3D(target, levels, internalformat, width, height, depth);
}

void
glGetInternalformativ(GLenum target, GLenum internalformat, GLenum pname,
					  GLsizei bufSize, GLint *params)
{
	ovr_glGetInternalformativ(target, internalformat, pname, bufSize, params);
}

/* GLES3.1 API */
GLuint
glCreateShaderProgramv(GLenum type,  GLsizei count,
					   const GLchar *const *strings)
{
	return ovr_glCreateShaderProgramv(type, count, strings);
}

void
glGenProgramPipelines( GLsizei n,  GLuint *pipelines)
{
	ovr_glGenProgramPipelines(n, pipelines);
}

void
glBindProgramPipeline( GLuint pipeline)
{
	ovr_glBindProgramPipeline(pipeline);
}

void
glGetProgramPipelineiv(GLuint pipeline, GLenum pname, GLint *params)
{
	ovr_glGetProgramPipelineiv(pipeline, pname, params);
}

void
glDeleteProgramPipelines(GLsizei n, GLuint const *pipelines)
{
	ovr_glDeleteProgramPipelines(n, pipelines);
}

GLboolean
glIsProgramPipeline( GLuint pipeline)
{
	return ovr_glIsProgramPipeline(pipeline);
}

void
glValidateProgramPipeline(GLuint pipeline)
{
	ovr_glValidateProgramPipeline(pipeline);
}

void
glGetProgramPipelineInfoLog(GLuint pipeline, GLsizei bufSize, GLsizei *length,
							GLchar *infoLog)
{
	ovr_glGetProgramPipelineInfoLog(pipeline, bufSize, length, infoLog);
}

void
glDispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z)
{
	ovr_glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
}

void
glDispatchComputeIndirect( GLintptr indirect)
{
	ovr_glDispatchComputeIndirect(indirect);
}

void
glDrawArraysIndirect(GLenum mode, GLvoid const *indirect)
{
	ovr_glDrawArraysIndirect(mode, indirect);
}

void
glDrawElementsIndirect(GLenum mode, GLenum type, GLvoid const *indirect)
{
	ovr_glDrawElementsIndirect(mode, type, indirect);
}

void
glFramebufferParameteri(GLenum target, GLenum pname, GLint param)
{
	ovr_glFramebufferParameteri(target, pname, param);
}

void
glGetFramebufferParameteriv( GLenum target,  GLenum pname,  GLint *params)
{
	ovr_glGetFramebufferParameteriv(target,  pname,  params);
}

void
glGetProgramInterfaceiv(GLuint program,  GLenum programInterface,  GLenum pname,
						GLint *params)
{
	ovr_glGetProgramInterfaceiv(program, programInterface, pname, params);
}

GLuint
glGetProgramResourceIndex( GLuint program,  GLenum programInterface,
						   const char *name)
{
	return ovr_glGetProgramResourceIndex(program,  programInterface,  name);
}

void
glGetProgramResourceName(GLuint program, GLenum programInterface, GLuint index,
						 GLsizei bufSize, GLsizei *length, GLchar *name)
{
	ovr_glGetProgramResourceName(program, programInterface, index, bufSize, length,
								 name);
}

void
glGetProgramResourceiv( GLuint program,  GLenum programInterface,  GLuint index,
						GLsizei propCount,  const GLenum *props,  GLsizei bufSize,  GLsizei *length,
						GLint *params)
{
	ovr_glGetProgramResourceiv(program,  programInterface,  index,  propCount,
							   props,  bufSize,  length,  params);
}

GLint
glGetProgramResourceLocation(GLuint program, GLenum programInterface,
							 GLchar const *name)
{
	return ovr_glGetProgramResourceLocation(program, programInterface, name);
}

void
glUseProgramStages(GLuint pipeline, GLbitfield stages, GLuint program)
{
	ovr_glUseProgramStages(pipeline, stages, program);
}

void
glProgramUniform1iv(GLuint program, GLint location, GLsizei count,
					const GLint *value)
{
	ovr_glProgramUniform1iv(program, location, count, value);
}


void
glProgramUniform2iv(GLuint program, GLint location, GLsizei count,
					const GLint *value)
{
	ovr_glProgramUniform2iv(program, location, count, value);
}


void
glProgramUniform3iv(GLuint program, GLint location, GLsizei count,
					const GLint *value)
{
	ovr_glProgramUniform3iv(program, location, count, value);
}


void
glProgramUniform4iv(GLuint program, GLint location, GLsizei count,
					const GLint *value)
{
	ovr_glProgramUniform4iv(program, location, count, value);
}



void
glProgramUniform1fv(GLuint program, GLint location, GLsizei count,
					const GLfloat *value)
{
	ovr_glProgramUniform1fv(program, location, count, value);
}


void
glProgramUniform2fv(GLuint program, GLint location, GLsizei count,
					const GLfloat *value)
{
	ovr_glProgramUniform2fv(program, location, count, value);
}


void
glProgramUniform3fv(GLuint program, GLint location, GLsizei count,
					const GLfloat *value)
{
	ovr_glProgramUniform3fv(program, location, count, value);
}


void
glProgramUniform4fv(GLuint program, GLint location, GLsizei count,
					const GLfloat *value)
{
	ovr_glProgramUniform4fv(program, location, count, value);
}

void
glProgramUniformMatrix2fv(GLuint program, GLint location, GLsizei count,
						  GLboolean transpose, const GLfloat *value)
{
	ovr_glProgramUniformMatrix2fv(program, location, count, transpose, value);
}


void
glProgramUniformMatrix3fv(GLuint program, GLint location, GLsizei count,
						  GLboolean transpose, const GLfloat *value)
{
	ovr_glProgramUniformMatrix3fv(program, location, count, transpose, value);
}


void
glProgramUniformMatrix4fv(GLuint program, GLint location, GLsizei count,
						  GLboolean transpose, const GLfloat *value)
{
	ovr_glProgramUniformMatrix4fv(program, location, count, transpose, value);
}

void
glProgramUniform1i(GLuint program, GLint location, GLint x)
{
	ovr_glProgramUniform1i(program, location, x);
}


void
glProgramUniform2i(GLuint program, GLint location, GLint x, GLint y)
{
	ovr_glProgramUniform2i(program, location, x, y);
}


void
glProgramUniform3i(GLuint program, GLint location, GLint x, GLint y, GLint z)
{
	ovr_glProgramUniform3i(program, location, x, y, z);
}


void
glProgramUniform4i(GLuint program, GLint location, GLint x, GLint y, GLint z,
				   GLint w)
{
	ovr_glProgramUniform4i(program, location, x, y, z, w);
}


void
glProgramUniform1f(GLuint program, GLint location, GLfloat x)
{
	ovr_glProgramUniform1f(program, location, x);
}


void
glProgramUniform2f(GLuint program, GLint location, GLfloat x, GLfloat y)
{
	ovr_glProgramUniform2f(program, location, x, y);
}


void
glProgramUniform3f(GLuint program, GLint location, GLfloat x, GLfloat y,
				   GLfloat z)
{
	ovr_glProgramUniform3f(program, location, x, y, z);
}


void
glProgramUniform4f(GLuint program, GLint location, GLfloat x, GLfloat y,
				   GLfloat z, GLfloat w)
{
	ovr_glProgramUniform4f(program, location, x, y, z, w);
}

void
glProgramUniform1ui(GLuint program, GLint location, GLuint x)
{
	ovr_glProgramUniform1ui(program, location, x);
}


void
glProgramUniform2ui(GLuint program, GLint location, GLuint x, GLuint y)
{
	ovr_glProgramUniform2ui(program, location, x, y);
}


void
glProgramUniform3ui(GLuint program, GLint location, GLuint x, GLuint y,
					GLuint z)
{
	ovr_glProgramUniform3ui(program, location, x, y, z);
}


void
glProgramUniform4ui(GLuint program, GLint location, GLuint x, GLuint y,
					GLuint z, GLuint w)
{
	ovr_glProgramUniform4ui(program, location, x, y, z, w);
}

void
glProgramUniform1uiv(GLuint program, GLint location, GLsizei count,
					 const GLuint *value)
{
	ovr_glProgramUniform1uiv(program, location, count, value);
}


void
glProgramUniform2uiv(GLuint program, GLint location, GLsizei count,
					 const GLuint *value)
{
	ovr_glProgramUniform2uiv(program, location, count, value);
}

void
glProgramUniform3uiv(GLuint program, GLint location, GLsizei count,
					 const GLuint *value)
{
	ovr_glProgramUniform3uiv(program, location, count, value);
}

void
glProgramUniform4uiv(GLuint program, GLint location, GLsizei count,
					 const GLuint *value)
{
	ovr_glProgramUniform4uiv(program, location, count, value);
}

void
glProgramUniformMatrix2x3fv(GLuint program, GLint location, GLsizei count,
							GLboolean transpose, const GLfloat *value)
{
	ovr_glProgramUniformMatrix2x3fv(program, location, count, transpose, value);
}


void
glProgramUniformMatrix3x2fv(GLuint program, GLint location, GLsizei count,
							GLboolean transpose, const GLfloat *value)
{
	ovr_glProgramUniformMatrix3x2fv(program, location, count, transpose, value);
}


void
glProgramUniformMatrix4x3fv(GLuint program, GLint location, GLsizei count,
							GLboolean transpose, const GLfloat *value)
{
	ovr_glProgramUniformMatrix4x3fv(program, location, count, transpose, value);
}

void
glProgramUniformMatrix3x4fv(GLuint program, GLint location, GLsizei count,
							GLboolean transpose, const GLfloat *value)
{
	ovr_glProgramUniformMatrix3x4fv(program, location, count, transpose, value);
}


void
glProgramUniformMatrix2x4fv(GLuint program, GLint location, GLsizei count,
							GLboolean transpose, const GLfloat *value)
{
	ovr_glProgramUniformMatrix2x4fv(program, location, count, transpose, value);
}


void
glProgramUniformMatrix4x2fv(GLuint program, GLint location, GLsizei count,
							GLboolean transpose, const GLfloat *value)
{
	ovr_glProgramUniformMatrix4x2fv(program, location, count, transpose, value);
}


void
glBindImageTexture (GLuint unit, GLuint texture, GLint level, GLboolean layered,
					GLint layer, GLenum access, GLenum format)
{
	ovr_glBindImageTexture (unit, texture, level, layered, layer, access, format);
}


void
glGetBooleani_v (GLenum target, GLuint index, GLboolean *data)
{
	ovr_glGetBooleani_v (target, index, data);
}


void
glMemoryBarrier (GLbitfield barriers)
{
	ovr_glMemoryBarrier (barriers);
}


void
glMemoryBarrierByRegion (GLbitfield barriers)
{
	ovr_glMemoryBarrierByRegion (barriers);
}


void
glTexStorage2DMultisample (GLenum target, GLsizei samples,
						   GLenum internalformat, GLsizei width, GLsizei height,
						   GLboolean fixedsamplelocations)
{
	ovr_glTexStorage2DMultisample (target, samples, internalformat, width, height,
								   fixedsamplelocations);
}


void
glGetMultisamplefv (GLenum pname, GLuint index, GLfloat *val)
{
	ovr_glGetMultisamplefv (pname, index, val);
}


void
glSampleMaski (GLuint maskNumber, GLbitfield mask)
{
	ovr_glSampleMaski (maskNumber, mask);
}


void
glGetTexLevelParameteriv (GLenum target, GLint level, GLenum pname,
						  GLint *params)
{
	ovr_glGetTexLevelParameteriv (target, level, pname, params);
}


void
glGetTexLevelParameterfv (GLenum target, GLint level, GLenum pname,
						  GLfloat *params)
{
	ovr_glGetTexLevelParameterfv (target, level, pname, params);
}


void
glBindVertexBuffer (GLuint bindingindex, GLuint buffer, GLintptr offset,
					GLsizei stride)
{
	ovr_glBindVertexBuffer (bindingindex, buffer, offset, stride);
}


void
glVertexAttribFormat (GLuint attribindex, GLint size, GLenum type,
					  GLboolean normalized, GLuint relativeoffset)
{
	ovr_glVertexAttribFormat (attribindex, size, type, normalized, relativeoffset);
}


void
glVertexAttribIFormat (GLuint attribindex, GLint size, GLenum type,
					   GLuint relativeoffset)
{
	ovr_glVertexAttribIFormat (attribindex, size, type, relativeoffset);
}


void
glVertexAttribBinding (GLuint attribindex, GLuint bindingindex)
{
	ovr_glVertexAttribBinding (attribindex, bindingindex);
}


void
glVertexBindingDivisor (GLuint bindingindex, GLuint divisor)
{
	ovr_glVertexBindingDivisor (bindingindex, divisor);
}

/* GLES3.2 API */
void
glBlendBarrier(void)
{
	ovr_glBlendBarrier();
}

void
glCopyImageSubData (GLuint srcName, GLenum srcTarget, GLint srcLevel,
					GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget,
					GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth,
					GLsizei srcHeight, GLsizei srcDepth)
{
	ovr_glCopyImageSubData(srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName,
						   dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth);
}

void
glDebugMessageControl (GLenum source, GLenum type, GLenum severity,
					   GLsizei count, const GLuint *ids, GLboolean enabled)
{
	ovr_glDebugMessageControl (source, type, severity, count, ids, enabled);
}

void
glDebugMessageInsert(GLenum source, GLenum type, GLuint id, GLenum severity,
					 GLsizei length, const GLchar *buf)
{
	ovr_glDebugMessageInsert(source, type, id, severity, length, buf);
}

void
glDebugMessageCallback(GLDEBUGPROC callback, const void *userParam)
{
	ovr_glDebugMessageCallback(callback, userParam);
}

GLuint
glGetDebugMessageLog(GLuint count, GLsizei bufSize, GLenum *sources,
					 GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths,
					 GLchar *messageLog)
{
	return ovr_glGetDebugMessageLog(count, bufSize, sources, types, ids, severities,
									lengths, messageLog);
}

void
glPushDebugGroup(GLenum source, GLuint id, GLsizei length,
				 const GLchar *message)
{
	ovr_glPushDebugGroup(source, id, length, message);
}

void
glPopDebugGroup(void)
{
	ovr_glPopDebugGroup();
}

void
glObjectLabel(GLenum identifier, GLuint name, GLsizei length,
			  const GLchar *label)
{
	ovr_glObjectLabel(identifier, name, length, label);
}

void
glGetObjectLabel(GLenum identifier, GLuint name, GLsizei bufSize,
				 GLsizei *length, GLchar *label)
{
	ovr_glGetObjectLabel(identifier, name, bufSize, length, label);
}

void
glObjectPtrLabel(const void *ptr, GLsizei length, const GLchar *label)
{
	ovr_glObjectPtrLabel(ptr, length, label);
}

void
glGetObjectPtrLabel(const void *ptr, GLsizei bufSize, GLsizei *length,
					GLchar *label)
{
	ovr_glGetObjectPtrLabel(ptr, bufSize, length, label);
}

void
glEnablei(GLenum target, GLuint index)
{
	ovr_glEnablei(target, index);
}

void
glDisablei(GLenum target, GLuint index)
{
	ovr_glDisablei(target, index);
}

void
glBlendEquationi(GLuint buf, GLenum mode)
{
	ovr_glBlendEquationi(buf, mode);
}

void
glBlendEquationSeparatei(GLuint buf, GLenum modeRGB, GLenum modeAlpha)
{
	ovr_glBlendEquationSeparatei(buf, modeRGB, modeAlpha);
}

void
glBlendFunci(GLuint buf, GLenum src, GLenum dst)
{
	ovr_glBlendFunci(buf, src, dst);
}

void
glBlendFuncSeparatei(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha,
					 GLenum dstAlpha)
{
	ovr_glBlendFuncSeparatei(buf, srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void
glColorMaski(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a)
{
	ovr_glColorMaski(index, r, g, b, a);
}

GLboolean
glIsEnabledi(GLenum target, GLuint index)
{
	return ovr_glIsEnabledi(target, index);
}

void
glDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type,
						 const void *indices, GLint basevertex)
{
	ovr_glDrawElementsBaseVertex(mode, count, type, indices, basevertex);
}

void
glDrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end,
							  GLsizei count, GLenum type, const void *indices, GLint basevertex)
{
	ovr_glDrawRangeElementsBaseVertex(mode, start, end, count, type, indices,
									  basevertex);
}

void
glDrawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type,
								  const void *indices, GLsizei instancecount, GLint basevertex)
{
	ovr_glDrawElementsInstancedBaseVertex(mode, count, type, indices, instancecount,
										  basevertex);
}
void
glFramebufferTexture(GLenum target, GLenum attachment, GLuint texture,
					 GLint level)
{
	ovr_glFramebufferTexture(target, attachment, texture, level);
}

void
glPrimitiveBoundingBox(GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW,
					   GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW)
{
	ovr_glPrimitiveBoundingBox(minX, minY, minZ, minW, maxX, maxY, maxZ, maxW);
}

GLenum
glGetGraphicsResetStatus(void)
{
	return ovr_glGetGraphicsResetStatus();
}
void
glReadnPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format,
			  GLenum type, GLsizei bufSize, void *data)
{
	ovr_glReadnPixels(x, y, width, height, format, type, bufSize, data);
}

void
glGetnUniformfv(GLuint program, GLint location, GLsizei bufSize,
				GLfloat *params)
{
	ovr_glGetnUniformfv(program, location, bufSize, params);
}

void
glGetnUniformiv(GLuint program, GLint location, GLsizei bufSize, GLint *params)
{
	ovr_glGetnUniformiv(program, location, bufSize, params);
}

void
glGetnUniformuiv(GLuint program, GLint location, GLsizei bufSize,
				 GLuint *params)
{
	ovr_glGetnUniformuiv(program, location, bufSize, params);
}

void
glMinSampleShading(GLfloat value)
{
	ovr_glMinSampleShading(value);
}

void
glPatchParameteri(GLenum pname, GLint value)
{
	ovr_glPatchParameteri(pname, value);
}

void
glTexParameterIiv(GLenum target, GLenum pname, const GLint *params)
{
	ovr_glTexParameterIiv(target, pname, params);
}

void
glTexParameterIuiv(GLenum target, GLenum pname, const GLuint *params)
{
	ovr_glTexParameterIuiv(target, pname, params);
}

void
glGetTexParameterIiv(GLenum target, GLenum pname, GLint *params)
{
	ovr_glGetTexParameterIiv(target, pname, params);
}

void
glGetTexParameterIuiv(GLenum target, GLenum pname, GLuint *params)
{
	ovr_glGetTexParameterIuiv(target, pname, params);
}

void
glSamplerParameterIiv(GLuint sampler, GLenum pname, const GLint *param)
{
	ovr_glSamplerParameterIiv(sampler, pname, param);
}

void
glSamplerParameterIuiv(GLuint sampler, GLenum pname, const GLuint *param)
{
	ovr_glSamplerParameterIuiv(sampler, pname, param);
}

void
glGetSamplerParameterIiv(GLuint sampler, GLenum pname, GLint *params)
{
	ovr_glGetSamplerParameterIiv(sampler, pname, params);
}

void
glGetSamplerParameterIuiv(GLuint sampler, GLenum pname, GLuint *params)
{
	ovr_glGetSamplerParameterIuiv(sampler, pname, params);
}

void
glTexBuffer(GLenum target, GLenum internalformat, GLuint buffer)
{
	ovr_glTexBuffer(target, internalformat, buffer);
}

void
glTexBufferRange(GLenum target, GLenum internalformat, GLuint buffer,
				 GLintptr offset, GLsizeiptr size)
{
	ovr_glTexBufferRange(target, internalformat, buffer, offset, size);
}

void
glTexStorage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat,
						  GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)
{
	ovr_glTexStorage3DMultisample(target, samples, internalformat, width, height,
								  depth, fixedsamplelocations);
}
