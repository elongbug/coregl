#include "coregl_export.h"

void
coregl_api_glActiveTexture(GLenum texture)
{
	ovr_glActiveTexture(texture);
}

void
coregl_api_glAttachShader(GLuint program, GLuint shader)
{
	ovr_glAttachShader(program, shader);
}

void
coregl_api_glBindAttribLocation(GLuint program, GLuint index, const char* name)
{
	ovr_glBindAttribLocation(program, index, name);
}

void
coregl_api_glBindBuffer(GLenum target, GLuint buffer)
{
	ovr_glBindBuffer(target, buffer);
}

void
coregl_api_glBindFramebuffer(GLenum target, GLuint framebuffer)
{
	ovr_glBindFramebuffer(target, framebuffer);
}

void
coregl_api_glBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
	ovr_glBindRenderbuffer(target, renderbuffer);
}

void
coregl_api_glBindTexture(GLenum target, GLuint texture)
{
	ovr_glBindTexture(target, texture);
}

void
coregl_api_glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	ovr_glBlendColor(red, green, blue, alpha);
}

void
coregl_api_glBlendEquation(GLenum mode)
{
	ovr_glBlendEquation(mode);
}

void
coregl_api_glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
	ovr_glBlendEquationSeparate(modeRGB, modeAlpha);
}

void
coregl_api_glBlendFunc(GLenum sfactor, GLenum dfactor)
{
	ovr_glBlendFunc(sfactor, dfactor);
}

void
coregl_api_glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
	ovr_glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void
coregl_api_glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
{
	ovr_glBufferData(target, size, data, usage);
}

void
coregl_api_glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data)
{
	ovr_glBufferSubData(target, offset, size, data);
}

GLenum
coregl_api_glCheckFramebufferStatus(GLenum target)
{
	return ovr_glCheckFramebufferStatus(target);
}

void
coregl_api_glClear(GLbitfield mask)
{
	ovr_glClear(mask);
}

void
coregl_api_glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	ovr_glClearColor(red, green, blue, alpha);
}

void
coregl_api_glClearDepthf(GLclampf depth)
{
	ovr_glClearDepthf(depth);
}

void
coregl_api_glClearStencil(GLint s)
{
	ovr_glClearStencil(s);
}

void
coregl_api_glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	ovr_glColorMask(red, green, blue, alpha);
}

void
coregl_api_glCompileShader(GLuint shader)
{
	ovr_glCompileShader(shader);
}

void
coregl_api_glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data)
{
	ovr_glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
}

void
coregl_api_glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data)
{
	ovr_glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
}

void
coregl_api_glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
	ovr_glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
}

void
coregl_api_glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
	ovr_glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

GLuint
coregl_api_glCreateProgram(void)
{
	return ovr_glCreateProgram();
}

GLuint
coregl_api_glCreateShader(GLenum type)
{
	return ovr_glCreateShader(type);
}

void
coregl_api_glCullFace(GLenum mode)
{
	ovr_glCullFace(mode);
}

void
coregl_api_glDeleteBuffers(GLsizei n, const GLuint* buffers)
{
	ovr_glDeleteBuffers(n, buffers);
}

void
coregl_api_glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers)
{
	ovr_glDeleteFramebuffers(n, framebuffers);
}

void
coregl_api_glDeleteProgram(GLuint program)
{
	ovr_glDeleteProgram(program);
}

void
coregl_api_glDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers)
{
	ovr_glDeleteRenderbuffers(n, renderbuffers);
}

void
coregl_api_glDeleteShader(GLuint shader)
{
	ovr_glDeleteShader(shader);
}

void
coregl_api_glDeleteTextures(GLsizei n, const GLuint* textures)
{
	ovr_glDeleteTextures(n, textures);
}

void
coregl_api_glDepthFunc(GLenum func)
{
	ovr_glDepthFunc(func);
}

void
coregl_api_glDepthMask(GLboolean flag)
{
	ovr_glDepthMask(flag);
}

void
coregl_api_glDepthRangef(GLclampf zNear, GLclampf zFar)
{
	ovr_glDepthRangef(zNear, zFar);
}

void
coregl_api_glDetachShader(GLuint program, GLuint shader)
{
	ovr_glDetachShader(program, shader);
}

void
coregl_api_glDisable(GLenum cap)
{
	ovr_glDisable(cap);
}

void
coregl_api_glDisableVertexAttribArray(GLuint index)
{
	ovr_glDisableVertexAttribArray(index);
}

void
coregl_api_glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
	ovr_glDrawArrays(mode, first, count);
}

void
coregl_api_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices)
{
	ovr_glDrawElements(mode, count, type, indices);
}

void
coregl_api_glEnable(GLenum cap)
{
	ovr_glEnable(cap);
}

void
coregl_api_glEnableVertexAttribArray(GLuint index)
{
	ovr_glEnableVertexAttribArray(index);
}

void
coregl_api_glFinish(void)
{
	ovr_glFinish();
}

void
coregl_api_glFlush(void)
{
	ovr_glFlush();
}

void
coregl_api_glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
	ovr_glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
}

void
coregl_api_glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
	ovr_glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

void
coregl_api_glFrontFace(GLenum mode)
{
	ovr_glFrontFace(mode);
}

void
coregl_api_glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat* params)
{
	ovr_glGetVertexAttribfv(index, pname, params);
}

void
coregl_api_glGetVertexAttribiv(GLuint index, GLenum pname, GLint* params)
{
	ovr_glGetVertexAttribiv(index, pname, params);
}

void
coregl_api_glGetVertexAttribPointerv(GLuint index, GLenum pname, void** pointer)
{
	ovr_glGetVertexAttribPointerv(index, pname, pointer);
}

void
coregl_api_glHint(GLenum target, GLenum mode)
{
	ovr_glHint(target, mode);
}

void
coregl_api_glGenBuffers(GLsizei n, GLuint* buffers)
{
	ovr_glGenBuffers(n, buffers);
}

void
coregl_api_glGenerateMipmap(GLenum target)
{
	ovr_glGenerateMipmap(target);
}

void
coregl_api_glGenFramebuffers(GLsizei n, GLuint* framebuffers)
{
	ovr_glGenFramebuffers(n, framebuffers);
}

void
coregl_api_glGenRenderbuffers(GLsizei n, GLuint* renderbuffers)
{
	ovr_glGenRenderbuffers(n, renderbuffers);
}

void
coregl_api_glGenTextures(GLsizei n, GLuint* textures)
{
	ovr_glGenTextures(n, textures);
}

void
coregl_api_glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name)
{
	ovr_glGetActiveAttrib(program, index, bufsize, length, size, type, name);
}

void
coregl_api_glGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name)
{
	ovr_glGetActiveUniform(program, index, bufsize, length, size, type, name);
}

void
coregl_api_glGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders)
{
	ovr_glGetAttachedShaders(program, maxcount, count, shaders);
}

int
coregl_api_glGetAttribLocation(GLuint program, const char* name)
{
	return ovr_glGetAttribLocation(program, name);
}

void
coregl_api_glGetBooleanv(GLenum pname, GLboolean* params)
{
	ovr_glGetBooleanv(pname, params);
}

void
coregl_api_glGetBufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
	ovr_glGetBufferParameteriv(target, pname, params);
}

GLenum
coregl_api_glGetError(void)
{
	return ovr_glGetError();
}

void
coregl_api_glGetFloatv(GLenum pname, GLfloat* params)
{
	ovr_glGetFloatv(pname, params);
}

void
coregl_api_glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint* params)
{
	ovr_glGetFramebufferAttachmentParameteriv(target, attachment, pname, params);
}

void
coregl_api_glGetIntegerv(GLenum pname, GLint* params)
{
	ovr_glGetIntegerv(pname, params);
}

void
coregl_api_glGetProgramiv(GLuint program, GLenum pname, GLint* params)
{
	ovr_glGetProgramiv(program, pname, params);
}

void
coregl_api_glGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei* length, char* infolog)
{
	ovr_glGetProgramInfoLog(program, bufsize, length, infolog);
}

void
coregl_api_glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
	ovr_glGetRenderbufferParameteriv(target, pname, params);
}

void
coregl_api_glGetShaderiv(GLuint shader, GLenum pname, GLint* params)
{
	ovr_glGetShaderiv(shader, pname, params);
}

void
coregl_api_glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog)
{
	ovr_glGetShaderInfoLog(shader, bufsize, length, infolog);
}

void
coregl_api_glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision)
{
	ovr_glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
}

void
coregl_api_glGetShaderSource(GLuint shader, GLsizei bufsize, GLsizei* length, char* source)
{
	ovr_glGetShaderSource(shader, bufsize, length, source);
}

const GLubyte *
coregl_api_glGetString(GLenum name)
{
	return ovr_glGetString(name);
}

void
coregl_api_glGetTexParameterfv(GLenum target, GLenum pname, GLfloat* params)
{
	ovr_glGetTexParameterfv(target, pname, params);
}

void
coregl_api_glGetTexParameteriv(GLenum target, GLenum pname, GLint* params)
{
	ovr_glGetTexParameteriv(target, pname, params);
}

void
coregl_api_glGetUniformfv(GLuint program, GLint location, GLfloat* params)
{
	ovr_glGetUniformfv(program, location, params);
}

void
coregl_api_glGetUniformiv(GLuint program, GLint location, GLint* params)
{
	ovr_glGetUniformiv(program, location, params);
}

int
coregl_api_glGetUniformLocation(GLuint program, const char* name)
{
	return ovr_glGetUniformLocation(program, name);
}

GLboolean
coregl_api_glIsBuffer(GLuint buffer)
{
	return ovr_glIsBuffer(buffer);
}

GLboolean
coregl_api_glIsEnabled(GLenum cap)
{
	return ovr_glIsEnabled(cap);
}

GLboolean
coregl_api_glIsFramebuffer(GLuint framebuffer)
{
	return ovr_glIsFramebuffer(framebuffer);
}

GLboolean
coregl_api_glIsProgram(GLuint program)
{
	return ovr_glIsProgram(program);
}

GLboolean
coregl_api_glIsRenderbuffer(GLuint renderbuffer)
{
	return ovr_glIsRenderbuffer(renderbuffer);
}

GLboolean
coregl_api_glIsShader(GLuint shader)
{
	return ovr_glIsShader(shader);
}

GLboolean
coregl_api_glIsTexture(GLuint texture)
{
	return ovr_glIsTexture(texture);
}

void
coregl_api_glLineWidth(GLfloat width)
{
	ovr_glLineWidth(width);
}

void
coregl_api_glLinkProgram(GLuint program)
{
	ovr_glLinkProgram(program);
}

void
coregl_api_glPixelStorei(GLenum pname, GLint param)
{
	ovr_glPixelStorei(pname, param);
}

void
coregl_api_glPolygonOffset(GLfloat factor, GLfloat units)
{
	ovr_glPolygonOffset(factor, units);
}

void
coregl_api_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels)
{
	ovr_glReadPixels(x, y, width, height, format, type, pixels);
}

void
coregl_api_glReleaseShaderCompiler(void)
{
	ovr_glReleaseShaderCompiler();
}

void
coregl_api_glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
	ovr_glRenderbufferStorage(target, internalformat, width, height);
}

void
coregl_api_glSampleCoverage(GLclampf value, GLboolean invert)
{
	ovr_glSampleCoverage(value, invert);
}

void
coregl_api_glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
	ovr_glScissor(x, y, width, height);
}

void
coregl_api_glShaderBinary(GLsizei n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLsizei length)
{
	ovr_glShaderBinary(n, shaders, binaryformat, binary, length);
}

void
coregl_api_glShaderSource(GLuint shader, GLsizei count, const char** string, const GLint* length)
{
	ovr_glShaderSource(shader, count, string, length);
}

void
coregl_api_glStencilFunc(GLenum func, GLint ref, GLuint mask)
{
	ovr_glStencilFunc(func, ref, mask);
}

void
coregl_api_glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
{
	ovr_glStencilFuncSeparate(face, func, ref, mask);
}

void
coregl_api_glStencilMask(GLuint mask)
{
	ovr_glStencilMask(mask);
}

void
coregl_api_glStencilMaskSeparate(GLenum face, GLuint mask)
{
	ovr_glStencilMaskSeparate(face, mask);
}

void
coregl_api_glStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
	ovr_glStencilOp(fail, zfail, zpass);
}

void
coregl_api_glStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
{
	ovr_glStencilOpSeparate(face, fail, zfail, zpass);
}

void
coregl_api_glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels)
{
	ovr_glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
}

void
coregl_api_glTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
	ovr_glTexParameterf(target, pname, param);
}

void
coregl_api_glTexParameterfv(GLenum target, GLenum pname, const GLfloat* params)
{
	ovr_glTexParameterfv(target, pname, params);
}

void
coregl_api_glTexParameteri(GLenum target, GLenum pname, GLint param)
{
	ovr_glTexParameteri(target, pname, param);
}

void
coregl_api_glTexParameteriv(GLenum target, GLenum pname, const GLint* params)
{
	ovr_glTexParameteriv(target, pname, params);
}

void
coregl_api_glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels)
{
	ovr_glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
}

void
coregl_api_glUniform1f(GLint location, GLfloat x)
{
	ovr_glUniform1f(location, x);
}

void
coregl_api_glUniform1fv(GLint location, GLsizei count, const GLfloat* v)
{
	ovr_glUniform1fv(location, count, v);
}

void
coregl_api_glUniform1i(GLint location, GLint x)
{
	ovr_glUniform1i(location, x);
}

void
coregl_api_glUniform1iv(GLint location, GLsizei count, const GLint* v)
{
	ovr_glUniform1iv(location, count, v);
}

void
coregl_api_glUniform2f(GLint location, GLfloat x, GLfloat y)
{
	ovr_glUniform2f(location, x, y);
}

void
coregl_api_glUniform2fv(GLint location, GLsizei count, const GLfloat* v)
{
	ovr_glUniform2fv(location, count, v);
}

void
coregl_api_glUniform2i(GLint location, GLint x, GLint y)
{
	ovr_glUniform2i(location, x, y);
}

void
coregl_api_glUniform2iv(GLint location, GLsizei count, const GLint* v)
{
	ovr_glUniform2iv(location, count, v);
}

void
coregl_api_glUniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z)
{
	ovr_glUniform3f(location, x, y, z);
}

void
coregl_api_glUniform3fv(GLint location, GLsizei count, const GLfloat* v)
{
	ovr_glUniform3fv(location, count, v);
}

void
coregl_api_glUniform3i(GLint location, GLint x, GLint y, GLint z)
{
	ovr_glUniform3i(location, x, y, z);
}

void
coregl_api_glUniform3iv(GLint location, GLsizei count, const GLint* v)
{
	ovr_glUniform3iv(location, count, v);
}

void
coregl_api_glUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	ovr_glUniform4f(location, x, y, z, w);
}

void
coregl_api_glUniform4fv(GLint location, GLsizei count, const GLfloat* v)
{
	ovr_glUniform4fv(location, count, v);
}

void
coregl_api_glUniform4i(GLint location, GLint x, GLint y, GLint z, GLint w)
{
	ovr_glUniform4i(location, x, y, z, w);
}

void
coregl_api_glUniform4iv(GLint location, GLsizei count, const GLint* v)
{
	ovr_glUniform4iv(location, count, v);
}

void
coregl_api_glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	ovr_glUniformMatrix2fv(location, count, transpose, value);
}

void
coregl_api_glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	ovr_glUniformMatrix3fv(location, count, transpose, value);
}

void
coregl_api_glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	ovr_glUniformMatrix4fv(location, count, transpose, value);
}

void
coregl_api_glUseProgram(GLuint program)
{
	ovr_glUseProgram(program);
}

void
coregl_api_glValidateProgram(GLuint program)
{
	ovr_glValidateProgram(program);
}

void
coregl_api_glVertexAttrib1f(GLuint index, GLfloat x)
{
	ovr_glVertexAttrib1f(index, x);
}

void
coregl_api_glVertexAttrib1fv(GLuint index, const GLfloat* values)
{
	ovr_glVertexAttrib1fv(index, values);
}

void
coregl_api_glVertexAttrib2f(GLuint index, GLfloat x, GLfloat y)
{
	ovr_glVertexAttrib2f(index, x, y);
}

void
coregl_api_glVertexAttrib2fv(GLuint index, const GLfloat* values)
{
	ovr_glVertexAttrib2fv(index, values);
}

void
coregl_api_glVertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z)
{
	ovr_glVertexAttrib3f(index, x, y, z);
}

void
coregl_api_glVertexAttrib3fv(GLuint index, const GLfloat* values)
{
	ovr_glVertexAttrib3fv(index, values);
}

void
coregl_api_glVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	ovr_glVertexAttrib4f(index, x, y, z, w);
}

void
coregl_api_glVertexAttrib4fv(GLuint index, const GLfloat* values)
{
	ovr_glVertexAttrib4fv(index, values);
}

void
coregl_api_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
{
	ovr_glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void
coregl_api_glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	ovr_glViewport(x, y, width, height);
}

/* OpenGL ES 3.0 */
void
coregl_api_glReadBuffer(GLenum mode)
{
	ovr_glReadBuffer(mode);
}

void
coregl_api_glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid* indices)
{
	ovr_glDrawRangeElements(mode, start, end, count, type, indices);
}

void
coregl_api_glTexImage3D(GLenum target, GLint level, GLint GLinternalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
	ovr_glTexImage3D(target, level, GLinternalFormat, width, height, depth, border, format, type, pixels);
}

void
coregl_api_glTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels)
{
	ovr_glTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
}

void
coregl_api_glCopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
	ovr_glCopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width, height);
}

void
coregl_api_glCompressedTexImage3D(GLenum target, GLint level, GLenum GLinternalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data)
{
	ovr_glCompressedTexImage3D(target, level, GLinternalformat, width, height, depth, border, imageSize, data);
}

void
coregl_api_glCompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data)
{
	ovr_glCompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
}

void
coregl_api_glGenQueries(GLsizei n, GLuint* ids)
{
	ovr_glGenQueries(n, ids);
}

void
coregl_api_glDeleteQueries(GLsizei n, const GLuint* ids)
{
	ovr_glDeleteQueries(n, ids);
}

GLboolean
coregl_api_glIsQuery(GLuint id)
{
	return ovr_glIsQuery(id);
}

void
coregl_api_glBeginQuery(GLenum target, GLuint id)
{
	ovr_glBeginQuery(target, id);
}

void
coregl_api_glEndQuery(GLenum target)
{
	ovr_glEndQuery(target);
}

void
coregl_api_glGetQueryiv(GLenum target, GLenum pname, GLint* params)
{
	ovr_glGetQueryiv(target, pname, params);
}

void
coregl_api_glGetQueryObjectuiv(GLuint id, GLenum pname, GLuint* params)
{
	ovr_glGetQueryObjectuiv(id, pname, params);
}

GLboolean
coregl_api_glUnmapBuffer(GLenum target)
{
	return ovr_glUnmapBuffer(target);
}

void
coregl_api_glGetBufferPointerv(GLenum target, GLenum pname, GLvoid** params)
{
	ovr_glGetBufferPointerv(target, pname, params);
}

void
coregl_api_glDrawBuffers(GLsizei n, const GLenum* bufs)
{
	ovr_glDrawBuffers(n, bufs);
}

void
coregl_api_glUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	ovr_glUniformMatrix2x3fv(location, count, transpose, value);
}

void
coregl_api_glUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	ovr_glUniformMatrix3x2fv(location, count, transpose, value);
}

void
coregl_api_glUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	ovr_glUniformMatrix2x4fv(location, count, transpose, value);
}

void
coregl_api_glUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	ovr_glUniformMatrix4x2fv(location, count, transpose, value);
}

void
coregl_api_glUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	ovr_glUniformMatrix3x4fv(location, count, transpose, value);
}

void
coregl_api_glUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	ovr_glUniformMatrix4x3fv(location, count, transpose, value);
}

void
coregl_api_glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{
	ovr_glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
}

void
coregl_api_glRenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
	ovr_glRenderbufferStorageMultisample(target, samples, internalformat, width, height);
}

void
coregl_api_glFramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
{
	ovr_glFramebufferTextureLayer(target, attachment, texture, level, layer);
}

GLvoid*
coregl_api_glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
	return ovr_glMapBufferRange(target, offset, length, access);
}

void
coregl_api_glFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length)
{
	ovr_glFlushMappedBufferRange(target, offset, length);
}

void
coregl_api_glBindVertexArray(GLuint array)
{
	ovr_glBindVertexArray(array);
}

void
coregl_api_glDeleteVertexArrays(GLsizei n, const GLuint* arrays)
{
	ovr_glDeleteVertexArrays(n, arrays);
}

void
coregl_api_glGenVertexArrays(GLsizei n, GLuint* arrays)
{
	ovr_glGenVertexArrays(n, arrays);
}

GLboolean
coregl_api_glIsVertexArray(GLuint array)
{
	return ovr_glIsVertexArray(array);
}

void
coregl_api_glGetIntegeri_v(GLenum target, GLuint index, GLint* data)
{
	ovr_glGetIntegeri_v(target, index, data);
}

void
coregl_api_glBeginTransformFeedback(GLenum primitiveMode)
{
	ovr_glBeginTransformFeedback(primitiveMode);
}

void
coregl_api_glEndTransformFeedback()
{
	ovr_glEndTransformFeedback();
}

void
coregl_api_glBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
	ovr_glBindBufferRange(target, index, buffer, offset, size);
}

void
coregl_api_glBindBufferBase(GLenum target, GLuint index, GLuint buffer)
{
	ovr_glBindBufferBase(target, index, buffer);
}

void
coregl_api_glTransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar* const* varyings, GLenum bufferMode)
{
	ovr_glTransformFeedbackVaryings(program, count, varyings, bufferMode);
}

void
coregl_api_glGetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, GLchar* name)
{
	ovr_glGetTransformFeedbackVarying(program, index, bufSize, length, size, type, name);
}

void
coregl_api_glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
	ovr_glVertexAttribIPointer(index, size, type, stride, pointer);
}

void
coregl_api_glGetVertexAttribIiv(GLuint index, GLenum pname, GLint* params)
{
	ovr_glGetVertexAttribIiv(index, pname, params);
}

void
coregl_api_glGetVertexAttribIuiv(GLuint index, GLenum pname, GLuint* params)
{
	ovr_glGetVertexAttribIuiv(index, pname, params);
}

void
coregl_api_glVertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w)
{
	ovr_glVertexAttribI4i(index, x, y, z, w);
}

void
coregl_api_glVertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w)
{
	ovr_glVertexAttribI4ui(index, x, y, z, w);
}

void
coregl_api_glVertexAttribI4iv(GLuint index, const GLint* v)
{
	ovr_glVertexAttribI4iv(index, v);
}

void
coregl_api_glVertexAttribI4uiv(GLuint index, const GLuint* v)
{
	ovr_glVertexAttribI4uiv(index, v);
}

void
coregl_api_glGetUniformuiv(GLuint program, GLint location, GLuint* params)
{
	ovr_glGetUniformuiv(program, location, params);
}

GLint
coregl_api_glGetFragDataLocation(GLuint program, const GLchar *name)
{
	return ovr_glGetFragDataLocation(program, name);
}

void
coregl_api_glUniform1ui(GLint location, GLuint v0)
{
	ovr_glUniform1ui(location, v0);
}

void
coregl_api_glUniform2ui(GLint location, GLuint v0, GLuint v1)
{
	ovr_glUniform2ui(location, v0, v1);
}

void
coregl_api_glUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
{
	ovr_glUniform3ui(location, v0, v1, v2);
}

void
coregl_api_glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
{
	ovr_glUniform4ui(location, v0, v1, v2, v3);
}

void
coregl_api_glUniform1uiv(GLint location, GLsizei count, const GLuint* value)
{
	ovr_glUniform1uiv(location, count, value);
}

void
coregl_api_glUniform2uiv(GLint location, GLsizei count, const GLuint* value)
{
	ovr_glUniform2uiv(location, count, value);
}

void
coregl_api_glUniform3uiv(GLint location, GLsizei count, const GLuint* value)
{
	ovr_glUniform3uiv(location, count, value);
}

void
coregl_api_glUniform4uiv(GLint location, GLsizei count, const GLuint* value)
{
	ovr_glUniform4uiv(location, count, value);
}

void
coregl_api_glClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint* value)
{
	ovr_glClearBufferiv(buffer, drawbuffer, value);
}

void
coregl_api_glClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint* value)
{
	ovr_glClearBufferuiv(buffer, drawbuffer, value);
}

void
coregl_api_glClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat* value)
{
	ovr_glClearBufferfv(buffer, drawbuffer, value);
}

void
coregl_api_glClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)
{
	ovr_glClearBufferfi(buffer, drawbuffer, depth, stencil);
}

const GLubyte*
coregl_api_glGetStringi(GLenum name, GLuint index)
{
	return ovr_glGetStringi(name, index);
}

void
coregl_api_glCopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
{
	ovr_glCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
}

void
coregl_api_glGetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar* const* uniformNames, GLuint* uniformIndices)
{
	ovr_glGetUniformIndices(program, uniformCount, uniformNames, uniformIndices);
}

void
coregl_api_glGetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params)
{
	ovr_glGetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params);
}

GLuint
coregl_api_glGetUniformBlockIndex(GLuint program, const GLchar* uniformBlockName)
{
	return ovr_glGetUniformBlockIndex(program, uniformBlockName);
}

void
coregl_api_glGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params)
{
	ovr_glGetActiveUniformBlockiv(program, uniformBlockIndex, pname, params);
}

void
coregl_api_glGetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName)
{
	ovr_glGetActiveUniformBlockName(program, uniformBlockIndex, bufSize, length, uniformBlockName);
}

void
coregl_api_glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
{
	ovr_glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
}

void
coregl_api_glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instanceCount)
{
	ovr_glDrawArraysInstanced(mode, first, count, instanceCount);
}

void
coregl_api_glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices, GLsizei instanceCount)
{
	ovr_glDrawElementsInstanced(mode, count, type, indices, instanceCount);
}

GLsync
coregl_api_glFenceSync(GLenum condition, GLbitfield flags)
{
	return ovr_glFenceSync(condition, flags);
}

GLboolean
coregl_api_glIsSync(GLsync sync)
{
	return ovr_glIsSync(sync);
}

void
coregl_api_glDeleteSync(GLsync sync)
{
	ovr_glDeleteSync(sync);
}

GLenum
coregl_api_glClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
	return ovr_glClientWaitSync(sync, flags, timeout);
}

void
coregl_api_glWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
	ovr_glWaitSync(sync, flags, timeout);
}

void
coregl_api_glGetInteger64v(GLenum pname, GLint64* params)
{
	ovr_glGetInteger64v(pname, params);
}

void
coregl_api_glGetSynciv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei* length, GLint* values)
{
	ovr_glGetSynciv(sync, pname, bufSize, length, values);
}

void
coregl_api_glGetInteger64i_v(GLenum target, GLuint index, GLint64* data)
{
	ovr_glGetInteger64i_v(target, index, data);
}

void
coregl_api_glGetBufferParameteri64v(GLenum target, GLenum pname, GLint64* params)
{
	ovr_glGetBufferParameteri64v(target, pname, params);
}

void
coregl_api_glGenSamplers(GLsizei n, GLuint* samplers)
{
	ovr_glGenSamplers(n, samplers);
}

void
coregl_api_glDeleteSamplers(GLsizei n, const GLuint* samplers)
{
	ovr_glDeleteSamplers(n, samplers);
}

GLboolean
coregl_api_glIsSampler(GLuint sampler)
{
	return ovr_glIsSampler(sampler);
}

void
coregl_api_glBindSampler(GLuint unit, GLuint sampler)
{
	ovr_glBindSampler(unit, sampler);
}

void
coregl_api_glSamplerParameteri(GLuint sampler, GLenum pname, GLint param)
{
	ovr_glSamplerParameteri(sampler, pname, param);
}

void
coregl_api_glSamplerParameteriv(GLuint sampler, GLenum pname, const GLint* param)
{
	ovr_glSamplerParameteriv(sampler, pname, param);
}

void
coregl_api_glSamplerParameterf(GLuint sampler, GLenum pname, GLfloat param)
{
	ovr_glSamplerParameterf(sampler, pname, param);
}

void
coregl_api_glSamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat* param)
{
	ovr_glSamplerParameterfv(sampler, pname, param);
}

void
coregl_api_glGetSamplerParameteriv(GLuint sampler, GLenum pname, GLint* params)
{
	ovr_glGetSamplerParameteriv(sampler, pname, params);
}

void
coregl_api_glGetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat* params)
{
	ovr_glGetSamplerParameterfv(sampler, pname, params);
}

void
coregl_api_glVertexAttribDivisor(GLuint index, GLuint divisor)
{
	ovr_glVertexAttribDivisor(index, divisor);
}

void
coregl_api_glBindTransformFeedback(GLenum target, GLuint id)
{
	ovr_glBindTransformFeedback(target, id);
}

void
coregl_api_glDeleteTransformFeedbacks(GLsizei n, const GLuint* ids)
{
	ovr_glDeleteTransformFeedbacks(n, ids);
}

void
coregl_api_glGenTransformFeedbacks(GLsizei n, GLuint* ids)
{
	ovr_glGenTransformFeedbacks(n, ids);
}

GLboolean
coregl_api_glIsTransformFeedback(GLuint id)
{
	return ovr_glIsTransformFeedback(id);
}

void
coregl_api_glPauseTransformFeedback()
{
	ovr_glPauseTransformFeedback();
}

void
coregl_api_glResumeTransformFeedback()
{
	ovr_glResumeTransformFeedback();
}

void
coregl_api_glGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, GLvoid* binary)
{
	ovr_glGetProgramBinary(program, bufSize, length, binaryFormat, binary);
}

void
coregl_api_glProgramBinary(GLuint program, GLenum binaryFormat, const GLvoid* binary, GLsizei length)
{
	ovr_glProgramBinary(program, binaryFormat, binary, length);
}

void
coregl_api_glProgramParameteri(GLuint program, GLenum pname, GLint value)
{
	ovr_glProgramParameteri(program, pname, value);
}

void
coregl_api_glInvalidateFramebuffer(GLenum target, GLsizei numAttachments, const GLenum* attachments)
{
	ovr_glInvalidateFramebuffer(target, numAttachments, attachments);
}

void
coregl_api_glInvalidateSubFramebuffer(GLenum target, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height)
{
	ovr_glInvalidateSubFramebuffer(target, numAttachments, attachments, x, y, width, height);
}

void
coregl_api_glTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
	ovr_glTexStorage2D(target, levels, internalformat, width, height);
}

void
coregl_api_glTexStorage3D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
	ovr_glTexStorage3D(target, levels, internalformat, width, height, depth);
}

void
coregl_api_glGetInternalformativ(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint* params)
{
	ovr_glGetInternalformativ(target, internalformat, pname, bufSize, params);
}


