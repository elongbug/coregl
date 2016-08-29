#include "coregl_export.h"

/* Beginning of OpenGL ES 1.1*/
void
coregl_api_glAlphaFunc(GLenum func, GLclampf ref)
{
	ovr_glAlphaFunc(func, ref);
}

void
coregl_api_glClipPlanef(GLenum plane, const GLfloat *equation)
{
	ovr_glClipPlanef(plane, equation);
}

void
coregl_api_glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	ovr_glColor4f(red, green, blue, alpha);
}

void
coregl_api_glFogf(GLenum pname, GLfloat param)
{
	ovr_glFogf(pname, param);
}

void
coregl_api_glFogfv(GLenum pname, const GLfloat *params)
{
	ovr_glFogfv(pname, params);
}

void
coregl_api_glFrustumf(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,
					  GLfloat zNear, GLfloat zFar)
{
	ovr_glFrustumf(left, right, bottom, top, zNear, zFar);
}

void
coregl_api_glGetClipPlanef(GLenum pname, GLfloat eqn[4])
{
	ovr_glGetClipPlanef(pname, eqn);
}

void
coregl_api_glGetLightfv(GLenum light, GLenum pname, GLfloat *params)
{
	ovr_glGetLightfv(light, pname, params);
}

void
coregl_api_glGetMaterialfv(GLenum face, GLenum pname, GLfloat *params)
{
	ovr_glGetMaterialfv(face, pname, params);
}

void
coregl_api_glGetTexEnvfv(GLenum env, GLenum pname, GLfloat *params)
{
	ovr_glGetTexEnvfv(env, pname, params);
}

void
coregl_api_glLightModelf(GLenum pname, GLfloat param)
{
	ovr_glLightModelf(pname, param);
}

void
coregl_api_glLightModelfv(GLenum pname, const GLfloat *params)
{
	ovr_glLightModelfv(pname, params);
}

void
coregl_api_glLightf(GLenum light, GLenum pname, GLfloat param)
{
	ovr_glLightf(light, pname, param);
}

void
coregl_api_glLightfv(GLenum light, GLenum pname, const GLfloat *params)
{
	ovr_glLightfv(light, pname, params);
}

void
coregl_api_glLoadMatrixf(const GLfloat *m)
{
	ovr_glLoadMatrixf(m);
}

void
coregl_api_glMaterialf(GLenum face, GLenum pname, GLfloat param)
{
	ovr_glMaterialf(face, pname, param);
}

void
coregl_api_glMaterialfv(GLenum face, GLenum pname, const GLfloat *params)
{
	ovr_glMaterialfv(face, pname, params);
}

void
coregl_api_glMultMatrixf(const GLfloat *m)
{
	ovr_glMultMatrixf(m);
}

void
coregl_api_glMultiTexCoord4f(GLenum target, GLfloat s, GLfloat t, GLfloat r,
							 GLfloat q)
{
	ovr_glMultiTexCoord4f(target, s, t, r, q);
}

void
coregl_api_glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)
{
	ovr_glNormal3f(nx, ny, nz);
}

void
coregl_api_glOrthof(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,
					GLfloat zNear, GLfloat zFar)
{
	ovr_glOrthof(left, right, bottom, top, zNear, zFar);
}

void
coregl_api_glPointParameterf(GLenum pname, GLfloat param)
{
	ovr_glPointParameterf(pname, param);
}

void
coregl_api_glPointParameterfv(GLenum pname, const GLfloat *params)
{
	ovr_glPointParameterfv(pname, params);
}

void
coregl_api_glPointSize(GLfloat size)
{
	ovr_glPointSize(size);
}

void
coregl_api_glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	ovr_glRotatef(angle, x, y, z);
}

void
coregl_api_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
	ovr_glScalef(x, y, z);
}

void
coregl_api_glTexEnvf(GLenum target, GLenum pname, GLfloat param)
{
	ovr_glTexEnvf(target, pname, param);
}

void
coregl_api_glTexEnvfv(GLenum target, GLenum pname, const GLfloat *params)
{
	ovr_glTexEnvfv(target, pname, params);
}

void
coregl_api_glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
	ovr_glTranslatef(x, y, z);
}

void
coregl_api_glAlphaFuncx(GLenum func, GLclampx ref)
{
	ovr_glAlphaFuncx(func, ref);
}

void
coregl_api_glClearColorx(GLclampx red, GLclampx green, GLclampx blue,
						 GLclampx alpha)
{
	ovr_glClearColorx(red, green, blue, alpha);
}

void
coregl_api_glClearDepthx(GLclampx depth)
{
	ovr_glClearDepthx(depth);
}

void
coregl_api_glClientActiveTexture(GLenum texture)
{
	ovr_glClientActiveTexture(texture);
}

void
coregl_api_glClipPlanex(GLenum plane, const GLfixed *equation)
{
	ovr_glClipPlanex(plane, equation);
}

void
coregl_api_glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
	ovr_glColor4ub(red, green, blue, alpha);
}

void
coregl_api_glColor4x(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)
{
	ovr_glColor4x(red, green, blue, alpha);
}

void
coregl_api_glColorPointer(GLint size, GLenum type, GLsizei stride,
						  const GLvoid *pointer)
{
	ovr_glColorPointer(size, type, stride, pointer);
}

void
coregl_api_glDepthRangex(GLclampx zNear, GLclampx zFar)
{
	ovr_glDepthRangex(zNear, zFar);
}

void
coregl_api_glDisableClientState(GLenum array)
{
	ovr_glDisableClientState(array);
}

void
coregl_api_glEnableClientState(GLenum array)
{
	ovr_glEnableClientState(array);
}

void
coregl_api_glFogx(GLenum pname, GLfixed param)
{
	ovr_glFogx(pname, param);
}

void
coregl_api_glFogxv(GLenum pname, const GLfixed *params)
{
	ovr_glFogxv(pname, params);
}

void
coregl_api_glFrustumx(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top,
					  GLfixed zNear, GLfixed zFar)
{
	ovr_glFrustumx(left, right, bottom, top, zNear, zFar);
}

void
coregl_api_glGetClipPlanex(GLenum pname, GLfixed eqn[4])
{
	ovr_glGetClipPlanex(pname, eqn);
}

void
coregl_api_glGetFixedv(GLenum pname, GLfixed *params)
{
	ovr_glGetFixedv(pname, params);
}

void
coregl_api_glGetLightxv(GLenum light, GLenum pname, GLfixed *params)
{
	ovr_glGetLightxv(light, pname, params);
}

void
coregl_api_glGetMaterialxv(GLenum face, GLenum pname, GLfixed *params)
{
	ovr_glGetMaterialxv(face, pname, params);
}

void
coregl_api_glGetTexEnviv(GLenum env, GLenum pname, GLint *params)
{
	ovr_glGetTexEnviv(env, pname, params);
}

void
coregl_api_glGetTexEnvxv(GLenum env, GLenum pname, GLfixed *params)
{
	ovr_glGetTexEnvxv(env, pname, params);
}

void
coregl_api_glGetTexParameterxv(GLenum target, GLenum pname, GLfixed *params)
{
	ovr_glGetTexParameterxv(target, pname, params);
}

void
coregl_api_glLightModelx(GLenum pname, GLfixed param)
{
	ovr_glLightModelx(pname, param);
}

void
coregl_api_glLightModelxv(GLenum pname, const GLfixed *params)
{
	ovr_glLightModelxv(pname, params);
}

void
coregl_api_glLightx(GLenum light, GLenum pname, GLfixed param)
{
	ovr_glLightx(light, pname, param);
}

void
coregl_api_glLightxv(GLenum light, GLenum pname, const GLfixed *params)
{
	ovr_glLightxv(light, pname, params);
}

void
coregl_api_glLineWidthx(GLfixed width)
{
	ovr_glLineWidthx(width);
}

void
coregl_api_glLoadIdentity(void)
{
	ovr_glLoadIdentity();
}

void
coregl_api_glLoadMatrixx(const GLfixed *m)
{
	ovr_glLoadMatrixx(m);
}

void
coregl_api_glLogicOp(GLenum opcode)
{
	ovr_glLogicOp(opcode);
}

void
coregl_api_glMaterialx(GLenum face, GLenum pname, GLfixed param)
{
	ovr_glMaterialx(face, pname, param);
}

void
coregl_api_glMaterialxv(GLenum face, GLenum pname, const GLfixed *params)
{
	ovr_glMaterialxv(face, pname, params);
}

void
coregl_api_glMatrixMode(GLenum mode)
{
	ovr_glMatrixMode(mode);
}

void
coregl_api_glMultMatrixx(const GLfixed *m)
{
	ovr_glMultMatrixx(m);
}

void
coregl_api_glMultiTexCoord4x(GLenum target, GLfixed s, GLfixed t, GLfixed r,
							 GLfixed q)
{
	ovr_glMultiTexCoord4x(target, s, t, r, q);
}

void
coregl_api_glNormal3x(GLfixed nx, GLfixed ny, GLfixed nz)
{
	ovr_glNormal3x(nx, ny, nz);
}

void
coregl_api_glNormalPointer(GLenum type, GLsizei stride, const GLvoid *pointer)
{
	ovr_glNormalPointer(type, stride, pointer);
}

void
coregl_api_glOrthox(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top,
					GLfixed zNear, GLfixed zFar)
{
	ovr_glOrthox(left, right, bottom, top, zNear, zFar);
}

void
coregl_api_glPointParameterx(GLenum pname, GLfixed param)
{
	ovr_glPointParameterx(pname, param);
}

void
coregl_api_glPointParameterxv(GLenum pname, const GLfixed *params)
{
	ovr_glPointParameterxv(pname, params);
}

void
coregl_api_glPointSizex(GLfixed size)
{
	ovr_glPointSizex(size);
}

void
coregl_api_glPolygonOffsetx(GLfixed factor, GLfixed units)
{
	ovr_glPolygonOffsetx(factor, units);
}

void
coregl_api_glPopMatrix(void)
{
	ovr_glPopMatrix();
}

void
coregl_api_glPushMatrix(void)
{
	ovr_glPushMatrix();
}

void
coregl_api_glRotatex(GLfixed angle, GLfixed x, GLfixed y, GLfixed z)
{
	ovr_glRotatex (angle, x, y, z);
}

void
coregl_api_glSampleCoveragex(GLclampx value, GLboolean invert)
{
	ovr_glSampleCoveragex(value, invert);
}

void
coregl_api_glScalex(GLfixed x, GLfixed y, GLfixed z)
{
	ovr_glScalex (x, y, z);
}

void
coregl_api_glShadeModel(GLenum mode)
{
	ovr_glShadeModel(mode);
}

void
coregl_api_glTexCoordPointer(GLint size, GLenum type, GLsizei stride,
							 const GLvoid *pointer)
{
	ovr_glTexCoordPointer(size, type, stride, pointer);
}

void
coregl_api_glTexEnvi(GLenum target, GLenum pname, GLint param)
{
	ovr_glTexEnvi(target, pname, param);
}

void
coregl_api_glTexEnvx(GLenum target, GLenum pname, GLfixed param)
{
	ovr_glTexEnvx(target, pname, param);
}

void
coregl_api_glTexEnviv(GLenum target, GLenum pname, const GLint *params)
{
	ovr_glTexEnviv(target, pname, params);
}

void
coregl_api_glTexEnvxv(GLenum target, GLenum pname, const GLfixed *params)
{
	ovr_glTexEnvxv(target, pname, params);
}

void
coregl_api_glTexParameterx(GLenum target, GLenum pname, GLfixed param)
{
	ovr_glTexParameterx(target, pname, param);
}

void
coregl_api_glTexParameterxv(GLenum target, GLenum pname, const GLfixed *params)
{
	ovr_glTexParameterxv(target, pname, params);
}

void
coregl_api_glTranslatex(GLfixed x, GLfixed y, GLfixed z)
{
	ovr_glTranslatex(x, y, z);
}

void
coregl_api_glVertexPointer(GLint size, GLenum type, GLsizei stride,
						   const GLvoid *pointer)
{
	ovr_glVertexPointer(size, type, stride, pointer);
}

/* Khronos GLES 1.1 CTS requires glPointSizePointerOES as shared library symbol.*/
void
coregl_api_glPointSizePointerOES(GLenum type, GLsizei stride,
								 const GLvoid *pointer)
{
	ovr_glPointSizePointerOES(type, stride, pointer);
}
/* Ending of OpenGL ES 1.1*/

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
coregl_api_glBindAttribLocation(GLuint program, GLuint index, const char *name)
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
coregl_api_glBlendColor(GLclampf red, GLclampf green, GLclampf blue,
						GLclampf alpha)
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
coregl_api_glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha,
							   GLenum dstAlpha)
{
	ovr_glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void
coregl_api_glBufferData(GLenum target, GLsizeiptr size, const void *data,
						GLenum usage)
{
	ovr_glBufferData(target, size, data, usage);
}

void
coregl_api_glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
						   const void *data)
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
coregl_api_glClearColor(GLclampf red, GLclampf green, GLclampf blue,
						GLclampf alpha)
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
coregl_api_glColorMask(GLboolean red, GLboolean green, GLboolean blue,
					   GLboolean alpha)
{
	ovr_glColorMask(red, green, blue, alpha);
}

void
coregl_api_glCompileShader(GLuint shader)
{
	ovr_glCompileShader(shader);
}

void
coregl_api_glCompressedTexImage2D(GLenum target, GLint level,
								  GLenum internalformat, GLsizei width, GLsizei height, GLint border,
								  GLsizei imageSize, const void *data)
{
	ovr_glCompressedTexImage2D(target, level, internalformat, width, height, border,
							   imageSize, data);
}

void
coregl_api_glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset,
									 GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize,
									 const void *data)
{
	ovr_glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height,
								  format, imageSize, data);
}

void
coregl_api_glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat,
							GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
	ovr_glCopyTexImage2D(target, level, internalformat, x, y, width, height,
						 border);
}

void
coregl_api_glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset,
							   GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
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
coregl_api_glDeleteBuffers(GLsizei n, const GLuint *buffers)
{
	ovr_glDeleteBuffers(n, buffers);
}

void
coregl_api_glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
{
	ovr_glDeleteFramebuffers(n, framebuffers);
}

void
coregl_api_glDeleteProgram(GLuint program)
{
	ovr_glDeleteProgram(program);
}

void
coregl_api_glDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
{
	ovr_glDeleteRenderbuffers(n, renderbuffers);
}

void
coregl_api_glDeleteShader(GLuint shader)
{
	ovr_glDeleteShader(shader);
}

void
coregl_api_glDeleteTextures(GLsizei n, const GLuint *textures)
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
coregl_api_glDrawElements(GLenum mode, GLsizei count, GLenum type,
						  const void *indices)
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
coregl_api_glFramebufferRenderbuffer(GLenum target, GLenum attachment,
									 GLenum renderbuffertarget, GLuint renderbuffer)
{
	ovr_glFramebufferRenderbuffer(target, attachment, renderbuffertarget,
								  renderbuffer);
}

void
coregl_api_glFramebufferTexture2D(GLenum target, GLenum attachment,
								  GLenum textarget, GLuint texture, GLint level)
{
	ovr_glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

void
coregl_api_glFrontFace(GLenum mode)
{
	ovr_glFrontFace(mode);
}

void
coregl_api_glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params)
{
	ovr_glGetVertexAttribfv(index, pname, params);
}

void
coregl_api_glGetVertexAttribiv(GLuint index, GLenum pname, GLint *params)
{
	ovr_glGetVertexAttribiv(index, pname, params);
}

void
coregl_api_glGetVertexAttribPointerv(GLuint index, GLenum pname, void **pointer)
{
	ovr_glGetVertexAttribPointerv(index, pname, pointer);
}

void
coregl_api_glHint(GLenum target, GLenum mode)
{
	ovr_glHint(target, mode);
}

void
coregl_api_glGenBuffers(GLsizei n, GLuint *buffers)
{
	ovr_glGenBuffers(n, buffers);
}

void
coregl_api_glGenerateMipmap(GLenum target)
{
	ovr_glGenerateMipmap(target);
}

void
coregl_api_glGenFramebuffers(GLsizei n, GLuint *framebuffers)
{
	ovr_glGenFramebuffers(n, framebuffers);
}

void
coregl_api_glGenRenderbuffers(GLsizei n, GLuint *renderbuffers)
{
	ovr_glGenRenderbuffers(n, renderbuffers);
}

void
coregl_api_glGenTextures(GLsizei n, GLuint *textures)
{
	ovr_glGenTextures(n, textures);
}

void
coregl_api_glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize,
							 GLsizei *length, GLint *size, GLenum *type, char *name)
{
	ovr_glGetActiveAttrib(program, index, bufsize, length, size, type, name);
}

void
coregl_api_glGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize,
							  GLsizei *length, GLint *size, GLenum *type, char *name)
{
	ovr_glGetActiveUniform(program, index, bufsize, length, size, type, name);
}

void
coregl_api_glGetAttachedShaders(GLuint program, GLsizei maxcount,
								GLsizei *count, GLuint *shaders)
{
	ovr_glGetAttachedShaders(program, maxcount, count, shaders);
}

int
coregl_api_glGetAttribLocation(GLuint program, const char *name)
{
	return ovr_glGetAttribLocation(program, name);
}

void
coregl_api_glGetBooleanv(GLenum pname, GLboolean *params)
{
	ovr_glGetBooleanv(pname, params);
}

void
coregl_api_glGetBufferParameteriv(GLenum target, GLenum pname, GLint *params)
{
	ovr_glGetBufferParameteriv(target, pname, params);
}

GLenum
coregl_api_glGetError(void)
{
	return ovr_glGetError();
}

void
coregl_api_glGetFloatv(GLenum pname, GLfloat *params)
{
	ovr_glGetFloatv(pname, params);
}

void
coregl_api_glGetFramebufferAttachmentParameteriv(GLenum target,
		GLenum attachment, GLenum pname, GLint *params)
{
	ovr_glGetFramebufferAttachmentParameteriv(target, attachment, pname, params);
}

void
coregl_api_glGetIntegerv(GLenum pname, GLint *params)
{
	ovr_glGetIntegerv(pname, params);
}

void
coregl_api_glGetProgramiv(GLuint program, GLenum pname, GLint *params)
{
	ovr_glGetProgramiv(program, pname, params);
}

void
coregl_api_glGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei *length,
							   char *infolog)
{
	ovr_glGetProgramInfoLog(program, bufsize, length, infolog);
}

void
coregl_api_glGetRenderbufferParameteriv(GLenum target, GLenum pname,
										GLint *params)
{
	ovr_glGetRenderbufferParameteriv(target, pname, params);
}

void
coregl_api_glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
	ovr_glGetShaderiv(shader, pname, params);
}

void
coregl_api_glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei *length,
							  char *infolog)
{
	ovr_glGetShaderInfoLog(shader, bufsize, length, infolog);
}

void
coregl_api_glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype,
									  GLint *range, GLint *precision)
{
	ovr_glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
}

void
coregl_api_glGetShaderSource(GLuint shader, GLsizei bufsize, GLsizei *length,
							 char *source)
{
	ovr_glGetShaderSource(shader, bufsize, length, source);
}

const GLubyte *
coregl_api_glGetString(GLenum name)
{
	return ovr_glGetString(name);
}

void
coregl_api_glGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
{
	ovr_glGetTexParameterfv(target, pname, params);
}

void
coregl_api_glGetTexParameteriv(GLenum target, GLenum pname, GLint *params)
{
	ovr_glGetTexParameteriv(target, pname, params);
}

void
coregl_api_glGetUniformfv(GLuint program, GLint location, GLfloat *params)
{
	ovr_glGetUniformfv(program, location, params);
}

void
coregl_api_glGetUniformiv(GLuint program, GLint location, GLint *params)
{
	ovr_glGetUniformiv(program, location, params);
}

int
coregl_api_glGetUniformLocation(GLuint program, const char *name)
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
coregl_api_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height,
						GLenum format, GLenum type, void *pixels)
{
	ovr_glReadPixels(x, y, width, height, format, type, pixels);
}

void
coregl_api_glReleaseShaderCompiler(void)
{
	ovr_glReleaseShaderCompiler();
}

void
coregl_api_glRenderbufferStorage(GLenum target, GLenum internalformat,
								 GLsizei width, GLsizei height)
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
coregl_api_glShaderBinary(GLsizei n, const GLuint *shaders, GLenum binaryformat,
						  const void *binary, GLsizei length)
{
	ovr_glShaderBinary(n, shaders, binaryformat, binary, length);
}

void
coregl_api_glShaderSource(GLuint shader, GLsizei count, const char **string,
						  const GLint *length)
{
	ovr_glShaderSource(shader, count, string, length);
}

void
coregl_api_glStencilFunc(GLenum func, GLint ref, GLuint mask)
{
	ovr_glStencilFunc(func, ref, mask);
}

void
coregl_api_glStencilFuncSeparate(GLenum face, GLenum func, GLint ref,
								 GLuint mask)
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
coregl_api_glStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail,
							   GLenum zpass)
{
	ovr_glStencilOpSeparate(face, fail, zfail, zpass);
}

void
coregl_api_glTexImage2D(GLenum target, GLint level, GLint internalformat,
						GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type,
						const void *pixels)
{
	ovr_glTexImage2D(target, level, internalformat, width, height, border, format,
					 type, pixels);
}

void
coregl_api_glTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
	ovr_glTexParameterf(target, pname, param);
}

void
coregl_api_glTexParameterfv(GLenum target, GLenum pname, const GLfloat *params)
{
	ovr_glTexParameterfv(target, pname, params);
}

void
coregl_api_glTexParameteri(GLenum target, GLenum pname, GLint param)
{
	ovr_glTexParameteri(target, pname, param);
}

void
coregl_api_glTexParameteriv(GLenum target, GLenum pname, const GLint *params)
{
	ovr_glTexParameteriv(target, pname, params);
}

void
coregl_api_glTexSubImage2D(GLenum target, GLint level, GLint xoffset,
						   GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type,
						   const void *pixels)
{
	ovr_glTexSubImage2D(target, level, xoffset, yoffset, width, height, format,
						type, pixels);
}

void
coregl_api_glUniform1f(GLint location, GLfloat x)
{
	ovr_glUniform1f(location, x);
}

void
coregl_api_glUniform1fv(GLint location, GLsizei count, const GLfloat *v)
{
	ovr_glUniform1fv(location, count, v);
}

void
coregl_api_glUniform1i(GLint location, GLint x)
{
	ovr_glUniform1i(location, x);
}

void
coregl_api_glUniform1iv(GLint location, GLsizei count, const GLint *v)
{
	ovr_glUniform1iv(location, count, v);
}

void
coregl_api_glUniform2f(GLint location, GLfloat x, GLfloat y)
{
	ovr_glUniform2f(location, x, y);
}

void
coregl_api_glUniform2fv(GLint location, GLsizei count, const GLfloat *v)
{
	ovr_glUniform2fv(location, count, v);
}

void
coregl_api_glUniform2i(GLint location, GLint x, GLint y)
{
	ovr_glUniform2i(location, x, y);
}

void
coregl_api_glUniform2iv(GLint location, GLsizei count, const GLint *v)
{
	ovr_glUniform2iv(location, count, v);
}

void
coregl_api_glUniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z)
{
	ovr_glUniform3f(location, x, y, z);
}

void
coregl_api_glUniform3fv(GLint location, GLsizei count, const GLfloat *v)
{
	ovr_glUniform3fv(location, count, v);
}

void
coregl_api_glUniform3i(GLint location, GLint x, GLint y, GLint z)
{
	ovr_glUniform3i(location, x, y, z);
}

void
coregl_api_glUniform3iv(GLint location, GLsizei count, const GLint *v)
{
	ovr_glUniform3iv(location, count, v);
}

void
coregl_api_glUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z,
					   GLfloat w)
{
	ovr_glUniform4f(location, x, y, z, w);
}

void
coregl_api_glUniform4fv(GLint location, GLsizei count, const GLfloat *v)
{
	ovr_glUniform4fv(location, count, v);
}

void
coregl_api_glUniform4i(GLint location, GLint x, GLint y, GLint z, GLint w)
{
	ovr_glUniform4i(location, x, y, z, w);
}

void
coregl_api_glUniform4iv(GLint location, GLsizei count, const GLint *v)
{
	ovr_glUniform4iv(location, count, v);
}

void
coregl_api_glUniformMatrix2fv(GLint location, GLsizei count,
							  GLboolean transpose, const GLfloat *value)
{
	ovr_glUniformMatrix2fv(location, count, transpose, value);
}

void
coregl_api_glUniformMatrix3fv(GLint location, GLsizei count,
							  GLboolean transpose, const GLfloat *value)
{
	ovr_glUniformMatrix3fv(location, count, transpose, value);
}

void
coregl_api_glUniformMatrix4fv(GLint location, GLsizei count,
							  GLboolean transpose, const GLfloat *value)
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
coregl_api_glVertexAttrib1fv(GLuint index, const GLfloat *values)
{
	ovr_glVertexAttrib1fv(index, values);
}

void
coregl_api_glVertexAttrib2f(GLuint index, GLfloat x, GLfloat y)
{
	ovr_glVertexAttrib2f(index, x, y);
}

void
coregl_api_glVertexAttrib2fv(GLuint index, const GLfloat *values)
{
	ovr_glVertexAttrib2fv(index, values);
}

void
coregl_api_glVertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z)
{
	ovr_glVertexAttrib3f(index, x, y, z);
}

void
coregl_api_glVertexAttrib3fv(GLuint index, const GLfloat *values)
{
	ovr_glVertexAttrib3fv(index, values);
}

void
coregl_api_glVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z,
							GLfloat w)
{
	ovr_glVertexAttrib4f(index, x, y, z, w);
}

void
coregl_api_glVertexAttrib4fv(GLuint index, const GLfloat *values)
{
	ovr_glVertexAttrib4fv(index, values);
}

void
coregl_api_glVertexAttribPointer(GLuint index, GLint size, GLenum type,
								 GLboolean normalized, GLsizei stride, const void *pointer)
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
coregl_api_glDrawRangeElements(GLenum mode, GLuint start, GLuint end,
							   GLsizei count, GLenum type, const GLvoid *indices)
{
	ovr_glDrawRangeElements(mode, start, end, count, type, indices);
}

void
coregl_api_glTexImage3D(GLenum target, GLint level, GLint GLinternalFormat,
						GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format,
						GLenum type, const GLvoid *pixels)
{
	ovr_glTexImage3D(target, level, GLinternalFormat, width, height, depth, border,
					 format, type, pixels);
}

void
coregl_api_glTexSubImage3D(GLenum target, GLint level, GLint xoffset,
						   GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,
						   GLenum format, GLenum type, const GLvoid *pixels)
{
	ovr_glTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height,
						depth, format, type, pixels);
}

void
coregl_api_glCopyTexSubImage3D(GLenum target, GLint level, GLint xoffset,
							   GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
	ovr_glCopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width,
							height);
}

void
coregl_api_glCompressedTexImage3D(GLenum target, GLint level,
								  GLenum GLinternalformat, GLsizei width, GLsizei height, GLsizei depth,
								  GLint border, GLsizei imageSize, const GLvoid *data)
{
	ovr_glCompressedTexImage3D(target, level, GLinternalformat, width, height,
							   depth, border, imageSize, data);
}

void
coregl_api_glCompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset,
									 GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,
									 GLenum format, GLsizei imageSize, const GLvoid *data)
{
	ovr_glCompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width,
								  height, depth, format, imageSize, data);
}

void
coregl_api_glGenQueries(GLsizei n, GLuint *ids)
{
	ovr_glGenQueries(n, ids);
}

void
coregl_api_glDeleteQueries(GLsizei n, const GLuint *ids)
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
coregl_api_glGetQueryiv(GLenum target, GLenum pname, GLint *params)
{
	ovr_glGetQueryiv(target, pname, params);
}

void
coregl_api_glGetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params)
{
	ovr_glGetQueryObjectuiv(id, pname, params);
}

GLboolean
coregl_api_glUnmapBuffer(GLenum target)
{
	return ovr_glUnmapBuffer(target);
}

void
coregl_api_glGetBufferPointerv(GLenum target, GLenum pname, GLvoid **params)
{
	ovr_glGetBufferPointerv(target, pname, params);
}

void
coregl_api_glDrawBuffers(GLsizei n, const GLenum *bufs)
{
	ovr_glDrawBuffers(n, bufs);
}

void
coregl_api_glUniformMatrix2x3fv(GLint location, GLsizei count,
								GLboolean transpose, const GLfloat *value)
{
	ovr_glUniformMatrix2x3fv(location, count, transpose, value);
}

void
coregl_api_glUniformMatrix3x2fv(GLint location, GLsizei count,
								GLboolean transpose, const GLfloat *value)
{
	ovr_glUniformMatrix3x2fv(location, count, transpose, value);
}

void
coregl_api_glUniformMatrix2x4fv(GLint location, GLsizei count,
								GLboolean transpose, const GLfloat *value)
{
	ovr_glUniformMatrix2x4fv(location, count, transpose, value);
}

void
coregl_api_glUniformMatrix4x2fv(GLint location, GLsizei count,
								GLboolean transpose, const GLfloat *value)
{
	ovr_glUniformMatrix4x2fv(location, count, transpose, value);
}

void
coregl_api_glUniformMatrix3x4fv(GLint location, GLsizei count,
								GLboolean transpose, const GLfloat *value)
{
	ovr_glUniformMatrix3x4fv(location, count, transpose, value);
}

void
coregl_api_glUniformMatrix4x3fv(GLint location, GLsizei count,
								GLboolean transpose, const GLfloat *value)
{
	ovr_glUniformMatrix4x3fv(location, count, transpose, value);
}

void
coregl_api_glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
							 GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask,
							 GLenum filter)
{
	ovr_glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1,
						  mask, filter);
}

void
coregl_api_glRenderbufferStorageMultisample(GLenum target, GLsizei samples,
		GLenum internalformat, GLsizei width, GLsizei height)
{
	ovr_glRenderbufferStorageMultisample(target, samples, internalformat, width,
										 height);
}

void
coregl_api_glFramebufferTextureLayer(GLenum target, GLenum attachment,
									 GLuint texture, GLint level, GLint layer)
{
	ovr_glFramebufferTextureLayer(target, attachment, texture, level, layer);
}

GLvoid *
coregl_api_glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length,
							GLbitfield access)
{
	return ovr_glMapBufferRange(target, offset, length, access);
}

void
coregl_api_glFlushMappedBufferRange(GLenum target, GLintptr offset,
									GLsizeiptr length)
{
	ovr_glFlushMappedBufferRange(target, offset, length);
}

void
coregl_api_glBindVertexArray(GLuint array)
{
	ovr_glBindVertexArray(array);
}

void
coregl_api_glDeleteVertexArrays(GLsizei n, const GLuint *arrays)
{
	ovr_glDeleteVertexArrays(n, arrays);
}

void
coregl_api_glGenVertexArrays(GLsizei n, GLuint *arrays)
{
	ovr_glGenVertexArrays(n, arrays);
}

GLboolean
coregl_api_glIsVertexArray(GLuint array)
{
	return ovr_glIsVertexArray(array);
}

void
coregl_api_glGetIntegeri_v(GLenum target, GLuint index, GLint *data)
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
coregl_api_glBindBufferRange(GLenum target, GLuint index, GLuint buffer,
							 GLintptr offset, GLsizeiptr size)
{
	ovr_glBindBufferRange(target, index, buffer, offset, size);
}

void
coregl_api_glBindBufferBase(GLenum target, GLuint index, GLuint buffer)
{
	ovr_glBindBufferBase(target, index, buffer);
}

void
coregl_api_glTransformFeedbackVaryings(GLuint program, GLsizei count,
									   const GLchar *const *varyings, GLenum bufferMode)
{
	ovr_glTransformFeedbackVaryings(program, count, varyings, bufferMode);
}

void
coregl_api_glGetTransformFeedbackVarying(GLuint program, GLuint index,
		GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)
{
	ovr_glGetTransformFeedbackVarying(program, index, bufSize, length, size, type,
									  name);
}

void
coregl_api_glVertexAttribIPointer(GLuint index, GLint size, GLenum type,
								  GLsizei stride, const GLvoid *pointer)
{
	ovr_glVertexAttribIPointer(index, size, type, stride, pointer);
}

void
coregl_api_glGetVertexAttribIiv(GLuint index, GLenum pname, GLint *params)
{
	ovr_glGetVertexAttribIiv(index, pname, params);
}

void
coregl_api_glGetVertexAttribIuiv(GLuint index, GLenum pname, GLuint *params)
{
	ovr_glGetVertexAttribIuiv(index, pname, params);
}

void
coregl_api_glVertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w)
{
	ovr_glVertexAttribI4i(index, x, y, z, w);
}

void
coregl_api_glVertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z,
							  GLuint w)
{
	ovr_glVertexAttribI4ui(index, x, y, z, w);
}

void
coregl_api_glVertexAttribI4iv(GLuint index, const GLint *v)
{
	ovr_glVertexAttribI4iv(index, v);
}

void
coregl_api_glVertexAttribI4uiv(GLuint index, const GLuint *v)
{
	ovr_glVertexAttribI4uiv(index, v);
}

void
coregl_api_glGetUniformuiv(GLuint program, GLint location, GLuint *params)
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
coregl_api_glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2,
						GLuint v3)
{
	ovr_glUniform4ui(location, v0, v1, v2, v3);
}

void
coregl_api_glUniform1uiv(GLint location, GLsizei count, const GLuint *value)
{
	ovr_glUniform1uiv(location, count, value);
}

void
coregl_api_glUniform2uiv(GLint location, GLsizei count, const GLuint *value)
{
	ovr_glUniform2uiv(location, count, value);
}

void
coregl_api_glUniform3uiv(GLint location, GLsizei count, const GLuint *value)
{
	ovr_glUniform3uiv(location, count, value);
}

void
coregl_api_glUniform4uiv(GLint location, GLsizei count, const GLuint *value)
{
	ovr_glUniform4uiv(location, count, value);
}

void
coregl_api_glClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint *value)
{
	ovr_glClearBufferiv(buffer, drawbuffer, value);
}

void
coregl_api_glClearBufferuiv(GLenum buffer, GLint drawbuffer,
							const GLuint *value)
{
	ovr_glClearBufferuiv(buffer, drawbuffer, value);
}

void
coregl_api_glClearBufferfv(GLenum buffer, GLint drawbuffer,
						   const GLfloat *value)
{
	ovr_glClearBufferfv(buffer, drawbuffer, value);
}

void
coregl_api_glClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth,
						   GLint stencil)
{
	ovr_glClearBufferfi(buffer, drawbuffer, depth, stencil);
}

const GLubyte *
coregl_api_glGetStringi(GLenum name, GLuint index)
{
	return ovr_glGetStringi(name, index);
}

void
coregl_api_glCopyBufferSubData(GLenum readTarget, GLenum writeTarget,
							   GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
{
	ovr_glCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
}

void
coregl_api_glGetUniformIndices(GLuint program, GLsizei uniformCount,
							   const GLchar *const *uniformNames, GLuint *uniformIndices)
{
	ovr_glGetUniformIndices(program, uniformCount, uniformNames, uniformIndices);
}

void
coregl_api_glGetActiveUniformsiv(GLuint program, GLsizei uniformCount,
								 const GLuint *uniformIndices, GLenum pname, GLint *params)
{
	ovr_glGetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params);
}

GLuint
coregl_api_glGetUniformBlockIndex(GLuint program,
								  const GLchar *uniformBlockName)
{
	return ovr_glGetUniformBlockIndex(program, uniformBlockName);
}

void
coregl_api_glGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex,
									 GLenum pname, GLint *params)
{
	ovr_glGetActiveUniformBlockiv(program, uniformBlockIndex, pname, params);
}

void
coregl_api_glGetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex,
									   GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName)
{
	ovr_glGetActiveUniformBlockName(program, uniformBlockIndex, bufSize, length,
									uniformBlockName);
}

void
coregl_api_glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex,
								 GLuint uniformBlockBinding)
{
	ovr_glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
}

void
coregl_api_glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count,
								 GLsizei instanceCount)
{
	ovr_glDrawArraysInstanced(mode, first, count, instanceCount);
}

void
coregl_api_glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type,
								   const GLvoid *indices, GLsizei instanceCount)
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
coregl_api_glGetInteger64v(GLenum pname, GLint64 *params)
{
	ovr_glGetInteger64v(pname, params);
}

void
coregl_api_glGetSynciv(GLsync sync, GLenum pname, GLsizei bufSize,
					   GLsizei *length, GLint *values)
{
	ovr_glGetSynciv(sync, pname, bufSize, length, values);
}

void
coregl_api_glGetInteger64i_v(GLenum target, GLuint index, GLint64 *data)
{
	ovr_glGetInteger64i_v(target, index, data);
}

void
coregl_api_glGetBufferParameteri64v(GLenum target, GLenum pname,
									GLint64 *params)
{
	ovr_glGetBufferParameteri64v(target, pname, params);
}

void
coregl_api_glGenSamplers(GLsizei n, GLuint *samplers)
{
	ovr_glGenSamplers(n, samplers);
}

void
coregl_api_glDeleteSamplers(GLsizei n, const GLuint *samplers)
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
coregl_api_glSamplerParameteriv(GLuint sampler, GLenum pname,
								const GLint *param)
{
	ovr_glSamplerParameteriv(sampler, pname, param);
}

void
coregl_api_glSamplerParameterf(GLuint sampler, GLenum pname, GLfloat param)
{
	ovr_glSamplerParameterf(sampler, pname, param);
}

void
coregl_api_glSamplerParameterfv(GLuint sampler, GLenum pname,
								const GLfloat *param)
{
	ovr_glSamplerParameterfv(sampler, pname, param);
}

void
coregl_api_glGetSamplerParameteriv(GLuint sampler, GLenum pname, GLint *params)
{
	ovr_glGetSamplerParameteriv(sampler, pname, params);
}

void
coregl_api_glGetSamplerParameterfv(GLuint sampler, GLenum pname,
								   GLfloat *params)
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
coregl_api_glDeleteTransformFeedbacks(GLsizei n, const GLuint *ids)
{
	ovr_glDeleteTransformFeedbacks(n, ids);
}

void
coregl_api_glGenTransformFeedbacks(GLsizei n, GLuint *ids)
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
coregl_api_glGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei *length,
							  GLenum *binaryFormat, GLvoid *binary)
{
	ovr_glGetProgramBinary(program, bufSize, length, binaryFormat, binary);
}

void
coregl_api_glProgramBinary(GLuint program, GLenum binaryFormat,
						   const GLvoid *binary, GLsizei length)
{
	ovr_glProgramBinary(program, binaryFormat, binary, length);
}

void
coregl_api_glProgramParameteri(GLuint program, GLenum pname, GLint value)
{
	ovr_glProgramParameteri(program, pname, value);
}

void
coregl_api_glInvalidateFramebuffer(GLenum target, GLsizei numAttachments,
								   const GLenum *attachments)
{
	ovr_glInvalidateFramebuffer(target, numAttachments, attachments);
}

void
coregl_api_glInvalidateSubFramebuffer(GLenum target, GLsizei numAttachments,
									  const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height)
{
	ovr_glInvalidateSubFramebuffer(target, numAttachments, attachments, x, y, width,
								   height);
}

void
coregl_api_glTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat,
						  GLsizei width, GLsizei height)
{
	ovr_glTexStorage2D(target, levels, internalformat, width, height);
}

void
coregl_api_glTexStorage3D(GLenum target, GLsizei levels, GLenum internalformat,
						  GLsizei width, GLsizei height, GLsizei depth)
{
	ovr_glTexStorage3D(target, levels, internalformat, width, height, depth);
}

void
coregl_api_glGetInternalformativ(GLenum target, GLenum internalformat,
								 GLenum pname, GLsizei bufSize, GLint *params)
{
	ovr_glGetInternalformativ(target, internalformat, pname, bufSize, params);
}

/* GLES3.1 API */
GLuint
coregl_api_glCreateShaderProgramv(GLenum type,  GLsizei count,
								  const GLchar *const *strings)
{
	return ovr_glCreateShaderProgramv(type, count, strings);
}

void
coregl_api_glGenProgramPipelines( GLsizei n,  GLuint *pipelines)
{
	ovr_glGenProgramPipelines(n, pipelines);
}

void
coregl_api_glBindProgramPipeline( GLuint pipeline)
{
	ovr_glBindProgramPipeline(pipeline);
}

void
coregl_api_glGetProgramPipelineiv(GLuint pipeline, GLenum pname, GLint *params)
{
	ovr_glGetProgramPipelineiv(pipeline, pname, params);
}

void
coregl_api_glDeleteProgramPipelines(GLsizei n, GLuint const *pipelines)
{
	ovr_glDeleteProgramPipelines(n, pipelines);
}

GLboolean
coregl_api_glIsProgramPipeline( GLuint pipeline)
{
	return ovr_glIsProgramPipeline(pipeline);
}

void
coregl_api_glValidateProgramPipeline(GLuint pipeline)
{
	ovr_glValidateProgramPipeline(pipeline);
}

void
coregl_api_glGetProgramPipelineInfoLog(GLuint pipeline, GLsizei bufSize,
									   GLsizei *length, GLchar *infoLog)
{
	ovr_glGetProgramPipelineInfoLog(pipeline, bufSize, length, infoLog);
}

void
coregl_api_glDispatchCompute(GLuint num_groups_x, GLuint num_groups_y,
							 GLuint num_groups_z)
{
	ovr_glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
}

void
coregl_api_glDispatchComputeIndirect( GLintptr indirect)
{
	ovr_glDispatchComputeIndirect(indirect);
}

void
coregl_api_glDrawArraysIndirect(GLenum mode, GLvoid const *indirect)
{
	ovr_glDrawArraysIndirect(mode, indirect);
}

void
coregl_api_glDrawElementsIndirect(GLenum mode, GLenum type,
								  GLvoid const *indirect)
{
	ovr_glDrawElementsIndirect(mode, type, indirect);
}

void
coregl_api_glFramebufferParameteri(GLenum target, GLenum pname, GLint param)
{
	ovr_glFramebufferParameteri(target, pname, param);
}

void
coregl_api_glGetFramebufferParameteriv( GLenum target,  GLenum pname,
										GLint *params)
{
	ovr_glGetFramebufferParameteriv(target,  pname,  params);
}

void
coregl_api_glGetProgramInterfaceiv(GLuint program,  GLenum programInterface,
								   GLenum pname,  GLint *params)
{
	ovr_glGetProgramInterfaceiv(program, programInterface, pname, params);
}

GLuint
coregl_api_glGetProgramResourceIndex( GLuint program,  GLenum programInterface,
									  const char *name)
{
	return ovr_glGetProgramResourceIndex(program,  programInterface,  name);
}

void
coregl_api_glGetProgramResourceName(GLuint program, GLenum programInterface,
									GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name)
{
	ovr_glGetProgramResourceName(program, programInterface, index, bufSize, length,
								 name);
}

void
coregl_api_glGetProgramResourceiv( GLuint program,  GLenum programInterface,
								   GLuint index,  GLsizei propCount,  const GLenum *props,  GLsizei bufSize,
								   GLsizei *length,  GLint *params)
{
	ovr_glGetProgramResourceiv(program,  programInterface,  index,  propCount,
							   props,  bufSize,  length,  params);
}

GLint
coregl_api_glGetProgramResourceLocation(GLuint program, GLenum programInterface,
										GLchar const *name)
{
	return ovr_glGetProgramResourceLocation(program, programInterface, name);
}

void
coregl_api_glUseProgramStages(GLuint pipeline, GLbitfield stages,
							  GLuint program)
{
	ovr_glUseProgramStages(pipeline, stages, program);
}

void
coregl_api_glActiveShaderProgram(GLuint pipeline, GLuint program)
{
	ovr_glActiveShaderProgram(pipeline, program);
}

void
coregl_api_glProgramUniform1iv(GLuint program, GLint location, GLsizei count,
							   const GLint *value)
{
	ovr_glProgramUniform1iv(program, location, count, value);
}


void
coregl_api_glProgramUniform2iv(GLuint program, GLint location, GLsizei count,
							   const GLint *value)
{
	ovr_glProgramUniform2iv(program, location, count, value);
}


void
coregl_api_glProgramUniform3iv(GLuint program, GLint location, GLsizei count,
							   const GLint *value)
{
	ovr_glProgramUniform3iv(program, location, count, value);
}


void
coregl_api_glProgramUniform4iv(GLuint program, GLint location, GLsizei count,
							   const GLint *value)
{
	ovr_glProgramUniform4iv(program, location, count, value);
}

void
coregl_api_glProgramUniform1fv(GLuint program, GLint location, GLsizei count,
							   const GLfloat *value)
{
	ovr_glProgramUniform1fv(program, location, count, value);
}


void
coregl_api_glProgramUniform2fv(GLuint program, GLint location, GLsizei count,
							   const GLfloat *value)
{
	ovr_glProgramUniform2fv(program, location, count, value);
}


void
coregl_api_glProgramUniform3fv(GLuint program, GLint location, GLsizei count,
							   const GLfloat *value)
{
	ovr_glProgramUniform3fv(program, location, count, value);
}


void
coregl_api_glProgramUniform4fv(GLuint program, GLint location, GLsizei count,
							   const GLfloat *value)
{
	ovr_glProgramUniform4fv(program, location, count, value);
}

void
coregl_api_glProgramUniformMatrix2fv(GLuint program, GLint location,
									 GLsizei count, GLboolean transpose, const GLfloat *value)
{
	ovr_glProgramUniformMatrix2fv(program, location, count, transpose, value);
}


void
coregl_api_glProgramUniformMatrix3fv(GLuint program, GLint location,
									 GLsizei count, GLboolean transpose, const GLfloat *value)
{
	ovr_glProgramUniformMatrix3fv(program, location, count, transpose, value);
}


void
coregl_api_glProgramUniformMatrix4fv(GLuint program, GLint location,
									 GLsizei count, GLboolean transpose, const GLfloat *value)
{
	ovr_glProgramUniformMatrix4fv(program, location, count, transpose, value);
}

void
coregl_api_glProgramUniform1i(GLuint program, GLint location, GLint x)
{
	ovr_glProgramUniform1i(program, location, x);
}


void
coregl_api_glProgramUniform2i(GLuint program, GLint location, GLint x, GLint y)
{
	ovr_glProgramUniform2i(program, location, x, y);
}


void
coregl_api_glProgramUniform3i(GLuint program, GLint location, GLint x, GLint y,
							  GLint z)
{
	ovr_glProgramUniform3i(program, location, x, y, z);
}


void
coregl_api_glProgramUniform4i(GLuint program, GLint location, GLint x, GLint y,
							  GLint z, GLint w)
{
	ovr_glProgramUniform4i(program, location, x, y, z, w);
}


void
coregl_api_glProgramUniform1f(GLuint program, GLint location, GLfloat x)
{
	ovr_glProgramUniform1f(program, location, x);
}


void
coregl_api_glProgramUniform2f(GLuint program, GLint location, GLfloat x,
							  GLfloat y)
{
	ovr_glProgramUniform2f(program, location, x, y);
}


void
coregl_api_glProgramUniform3f(GLuint program, GLint location, GLfloat x,
							  GLfloat y, GLfloat z)
{
	ovr_glProgramUniform3f(program, location, x, y, z);
}


void
coregl_api_glProgramUniform4f(GLuint program, GLint location, GLfloat x,
							  GLfloat y, GLfloat z, GLfloat w)
{
	ovr_glProgramUniform4f(program, location, x, y, z, w);
}

void
coregl_api_glProgramUniform1ui(GLuint program, GLint location, GLuint x)
{
	ovr_glProgramUniform1i(program, location, x);
}

void
coregl_api_glProgramUniform2ui(GLuint program, GLint location, GLuint x,
							   GLuint y)
{
	ovr_glProgramUniform2ui(program, location, x, y);
}

void
coregl_api_glProgramUniform3ui(GLuint program, GLint location, GLuint x,
							   GLuint y, GLuint z)
{
	ovr_glProgramUniform3ui(program, location, x, y, z);
}

void
coregl_api_glProgramUniform4ui(GLuint program, GLint location, GLuint x,
							   GLuint y, GLuint z, GLuint w)
{
	ovr_glProgramUniform4ui(program, location, x, y, z, w);
}

void
coregl_api_glProgramUniform1uiv(GLuint program, GLint location, GLsizei count,
								const GLuint *value)
{
	ovr_glProgramUniform1uiv(program, location, count, value);
}


void
coregl_api_glProgramUniform2uiv(GLuint program, GLint location, GLsizei count,
								const GLuint *value)
{
	ovr_glProgramUniform2uiv(program, location, count, value);
}


void
coregl_api_glProgramUniform3uiv(GLuint program, GLint location, GLsizei count,
								const GLuint *value)
{
	ovr_glProgramUniform3uiv(program, location, count, value);
}


void
coregl_api_glProgramUniform4uiv(GLuint program, GLint location, GLsizei count,
								const GLuint *value)
{
	ovr_glProgramUniform4uiv(program, location, count, value);
}

void
coregl_api_glProgramUniformMatrix2x3fv(GLuint program, GLint location,
									   GLsizei count, GLboolean transpose, const GLfloat *value)
{
	ovr_glProgramUniformMatrix2x3fv(program, location, count, transpose, value);
}

void
coregl_api_glProgramUniformMatrix3x2fv(GLuint program, GLint location,
									   GLsizei count, GLboolean transpose, const GLfloat *value)
{
	ovr_glProgramUniformMatrix3x2fv(program, location, count, transpose, value);
}

void
coregl_api_glProgramUniformMatrix2x4fv(GLuint program, GLint location,
									   GLsizei count, GLboolean transpose, const GLfloat *value)
{
	ovr_glProgramUniformMatrix2x4fv(program, location, count, transpose, value);
}

void
coregl_api_glProgramUniformMatrix4x2fv(GLuint program, GLint location,
									   GLsizei count, GLboolean transpose, const GLfloat *value)
{
	ovr_glProgramUniformMatrix4x2fv(program, location, count, transpose, value);
}

void
coregl_api_glProgramUniformMatrix3x4fv(GLuint program, GLint location,
									   GLsizei count, GLboolean transpose, const GLfloat *value)
{
	ovr_glProgramUniformMatrix3x4fv(program, location, count, transpose, value);
}

void
coregl_api_glProgramUniformMatrix4x3fv(GLuint program, GLint location,
									   GLsizei count, GLboolean transpose, const GLfloat *value)
{
	ovr_glProgramUniformMatrix4x3fv(program, location, count, transpose, value);
}

void
coregl_api_glBindImageTexture (GLuint unit, GLuint texture, GLint level,
							   GLboolean layered, GLint layer, GLenum access, GLenum format)
{
	ovr_glBindImageTexture (unit, texture, level, layered, layer, access, format);
}

void
coregl_api_glGetBooleani_v (GLenum target, GLuint index, GLboolean *data)
{
	ovr_glGetBooleani_v (target, index, data);
}

void
coregl_api_glMemoryBarrier (GLbitfield barriers)
{
	ovr_glMemoryBarrier (barriers);
}

void
coregl_api_glMemoryBarrierByRegion (GLbitfield barriers)
{
	ovr_glMemoryBarrierByRegion (barriers);
}

void
coregl_api_glTexStorage2DMultisample (GLenum target, GLsizei samples,
									  GLenum internalformat, GLsizei width, GLsizei height,
									  GLboolean fixedsamplelocations)
{
	ovr_glTexStorage2DMultisample (target, samples, internalformat, width, height,
								   fixedsamplelocations);
}

void
coregl_api_glGetMultisamplefv (GLenum pname, GLuint index, GLfloat *val)
{
	ovr_glGetMultisamplefv (pname, index, val);
}

void
coregl_api_glSampleMaski (GLuint maskNumber, GLbitfield mask)
{
	ovr_glSampleMaski (maskNumber, mask);
}

void
coregl_api_glGetTexLevelParameteriv (GLenum target, GLint level, GLenum pname,
									 GLint *params)
{
	ovr_glGetTexLevelParameteriv (target, level, pname, params);
}

void
coregl_api_glGetTexLevelParameterfv (GLenum target, GLint level, GLenum pname,
									 GLfloat *params)
{
	ovr_glGetTexLevelParameterfv (target, level, pname, params);
}

void
coregl_api_glBindVertexBuffer (GLuint bindingindex, GLuint buffer,
							   GLintptr offset, GLsizei stride)
{
	ovr_glBindVertexBuffer (bindingindex, buffer, offset, stride);
}

void
coregl_api_glVertexAttribFormat (GLuint attribindex, GLint size, GLenum type,
								 GLboolean normalized, GLuint relativeoffset)
{
	ovr_glVertexAttribFormat (attribindex, size, type, normalized, relativeoffset);
}

void
coregl_api_glVertexAttribIFormat (GLuint attribindex, GLint size, GLenum type,
								  GLuint relativeoffset)
{
	ovr_glVertexAttribIFormat (attribindex, size, type, relativeoffset);
}

void
coregl_api_glVertexAttribBinding (GLuint attribindex, GLuint bindingindex)
{
	ovr_glVertexAttribBinding (attribindex, bindingindex);
}

void
coregl_api_glVertexBindingDivisor (GLuint bindingindex, GLuint divisor)
{
	ovr_glVertexBindingDivisor (bindingindex, divisor);
}

void
coregl_api_glGetPointerv(GLenum pname, GLvoid **params)
{
	ovr_glGetPointerv(pname, params);
}

/* GLES3.2 API */
void
coregl_api_glBlendBarrier(void)
{
	ovr_glBlendBarrier();
}

void
coregl_api_glCopyImageSubData (GLuint srcName, GLenum srcTarget, GLint srcLevel,
							   GLint srcX, GLint srcY,
							   GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX,
							   GLint dstY, GLint dstZ,
							   GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth)
{
	ovr_glCopyImageSubData(srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName,
						   dstTarget, dstLevel,
						   dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth);
}

void
coregl_api_glDebugMessageControl (GLenum source, GLenum type, GLenum severity,
								  GLsizei count,
								  const GLuint *ids, GLboolean enabled)
{
	ovr_glDebugMessageControl (source, type, severity, count, ids, enabled);
}

void
coregl_api_glDebugMessageInsert(GLenum source, GLenum type, GLuint id,
								GLenum severity, GLsizei length,
								const GLchar *buf)
{
	ovr_glDebugMessageInsert(source, type, id, severity, length, buf);
}

void
coregl_api_glDebugMessageCallback(GLDEBUGPROC callback, const void *userParam)
{
	ovr_glDebugMessageCallback(callback, userParam);
}

GLuint
coregl_api_glGetDebugMessageLog(GLuint count, GLsizei bufSize, GLenum *sources,
								GLenum *types,
								GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog)
{
	return ovr_glGetDebugMessageLog(count, bufSize, sources, types, ids, severities,
									lengths, messageLog);
}

void
coregl_api_glPushDebugGroup(GLenum source, GLuint id, GLsizei length,
							const GLchar *message)
{
	ovr_glPushDebugGroup(source, id, length, message);
}

void
coregl_api_glPopDebugGroup(void)
{
	ovr_glPopDebugGroup();
}

void
coregl_api_glObjectLabel(GLenum identifier, GLuint name, GLsizei length,
						 const GLchar *label)
{
	ovr_glObjectLabel(identifier, name, length, label);
}

void
coregl_api_glGetObjectLabel(GLenum identifier, GLuint name, GLsizei bufSize,
							GLsizei *length, GLchar *label)
{
	ovr_glGetObjectLabel(identifier, name, bufSize, length, label);
}

void
coregl_api_glObjectPtrLabel(const void *ptr, GLsizei length,
							const GLchar *label)
{
	ovr_glObjectPtrLabel(ptr, length, label);
}

void
coregl_api_glGetObjectPtrLabel(const void *ptr, GLsizei bufSize,
							   GLsizei *length, GLchar *label)
{
	ovr_glGetObjectPtrLabel(ptr, bufSize, length, label);
}

void
coregl_api_glEnablei(GLenum target, GLuint index)
{
	ovr_glEnablei(target, index);
}

void
coregl_api_glDisablei(GLenum target, GLuint index)
{
	ovr_glDisablei(target, index);
}

void
coregl_api_glBlendEquationi(GLuint buf, GLenum mode)
{
	ovr_glBlendEquationi(buf, mode);
}

void
coregl_api_glBlendEquationSeparatei(GLuint buf, GLenum modeRGB,
									GLenum modeAlpha)
{
	ovr_glBlendEquationSeparatei(buf, modeRGB, modeAlpha);
}

void
coregl_api_glBlendFunci(GLuint buf, GLenum src, GLenum dst)
{
	ovr_glBlendFunci(buf, src, dst);
}

void
coregl_api_glBlendFuncSeparatei(GLuint buf, GLenum srcRGB, GLenum dstRGB,
								GLenum srcAlpha, GLenum dstAlpha)
{
	ovr_glBlendFuncSeparatei(buf, srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void
coregl_api_glColorMaski(GLuint index, GLboolean r, GLboolean g, GLboolean b,
						GLboolean a)
{
	ovr_glColorMaski(index, r, g, b, a);
}

GLboolean
coregl_api_glIsEnabledi(GLenum target, GLuint index)
{
	return ovr_glIsEnabledi(target, index);
}

void
coregl_api_glDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type,
									const void *indices,
									GLint basevertex)
{
	ovr_glDrawElementsBaseVertex(mode, count, type, indices, basevertex);
}

void
coregl_api_glDrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end,
		GLsizei count,
		GLenum type, const void *indices, GLint basevertex)
{
	ovr_glDrawRangeElementsBaseVertex(mode, start, end, count, type, indices,
									  basevertex);
}

void
coregl_api_glDrawElementsInstancedBaseVertex(GLenum mode, GLsizei count,
		GLenum type,
		const void *indices, GLsizei instancecount, GLint basevertex)
{
	ovr_glDrawElementsInstancedBaseVertex(mode, count, type, indices, instancecount,
										  basevertex);
}

void
coregl_api_glFramebufferTexture(GLenum target, GLenum attachment,
								GLuint texture, GLint level)
{
	ovr_glFramebufferTexture(target, attachment, texture, level);
}

void
coregl_api_glPrimitiveBoundingBox(GLfloat minX, GLfloat minY, GLfloat minZ,
								  GLfloat minW,
								  GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW)
{
	ovr_glPrimitiveBoundingBox(minX, minY, minZ, minW, maxX, maxY, maxZ, maxW);
}

GLenum
coregl_api_glGetGraphicsResetStatus(void)
{
	return ovr_glGetGraphicsResetStatus();
}

void
coregl_api_glReadnPixels(GLint x, GLint y, GLsizei width, GLsizei height,
						 GLenum format, GLenum type,
						 GLsizei bufSize, void *data)
{
	ovr_glReadnPixels(x, y, width, height, format, type, bufSize, data);
}

void
coregl_api_glGetnUniformfv(GLuint program, GLint location, GLsizei bufSize,
						   GLfloat *params)
{
	ovr_glGetnUniformfv(program, location, bufSize, params);
}

void
coregl_api_glGetnUniformiv(GLuint program, GLint location, GLsizei bufSize,
						   GLint *params)
{
	ovr_glGetnUniformiv(program, location, bufSize, params);
}

void
coregl_api_glGetnUniformuiv(GLuint program, GLint location, GLsizei bufSize,
							GLuint *params)
{
	ovr_glGetnUniformuiv(program, location, bufSize, params);
}

void
coregl_api_glMinSampleShading(GLfloat value)
{
	ovr_glMinSampleShading(value);
}

void
coregl_api_glPatchParameteri(GLenum pname, GLint value)
{
	ovr_glPatchParameteri(pname, value);
}

void
coregl_api_glTexParameterIiv(GLenum target, GLenum pname, const GLint *params)
{
	ovr_glTexParameterIiv(target, pname, params);
}

void
coregl_api_glTexParameterIuiv(GLenum target, GLenum pname, const GLuint *params)
{
	ovr_glTexParameterIuiv(target, pname, params);
}

void
coregl_api_glGetTexParameterIiv(GLenum target, GLenum pname, GLint *params)
{
	ovr_glGetTexParameterIiv(target, pname, params);
}

void
coregl_api_glGetTexParameterIuiv(GLenum target, GLenum pname, GLuint *params)
{
	ovr_glGetTexParameterIuiv(target, pname, params);
}

void
coregl_api_glSamplerParameterIiv(GLuint sampler, GLenum pname,
								 const GLint *param)
{
	ovr_glSamplerParameterIiv(sampler, pname, param);
}

void
coregl_api_glSamplerParameterIuiv(GLuint sampler, GLenum pname,
								  const GLuint *param)
{
	ovr_glSamplerParameterIuiv(sampler, pname, param);
}

void
coregl_api_glGetSamplerParameterIiv(GLuint sampler, GLenum pname, GLint *params)
{
	ovr_glGetSamplerParameterIiv(sampler, pname, params);
}

void
coregl_api_glGetSamplerParameterIuiv(GLuint sampler, GLenum pname,
									 GLuint *params)
{
	ovr_glGetSamplerParameterIuiv(sampler, pname, params);
}

void
coregl_api_glTexBuffer(GLenum target, GLenum internalformat, GLuint buffer)
{
	ovr_glTexBuffer(target, internalformat, buffer);
}

void
coregl_api_glTexBufferRange(GLenum target, GLenum internalformat, GLuint buffer,
							GLintptr offset,
							GLsizeiptr size)
{
	ovr_glTexBufferRange(target, internalformat, buffer, offset, size);
}

void
coregl_api_glTexStorage3DMultisample(GLenum target, GLsizei samples,
									 GLenum internalformat,
									 GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)
{
	ovr_glTexStorage3DMultisample(target, samples, internalformat, width, height,
								  depth, fixedsamplelocations);
}
