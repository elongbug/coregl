// Defult use-case for exporting symbols
#ifndef _COREGL_SYMBOL
#define _COREGL_SYMBOL_NOT_DEFINED
#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST) COREGL_API extern RET_TYPE FUNC_NAME PARAM_LIST;
#endif
#ifndef _COREGL_EXT_SYMBOL
#define _COREGL_EXT_SYMBOL_NOT_DEFINED
#define _COREGL_EXT_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST) _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)
#endif
#ifndef _COREGL_EXT_SYMBOL_ALIAS
#define _COREGL_EXT_SYMBOL_ALIAS_NOT_DEFINED
#define _COREGL_EXT_SYMBOL_ALIAS(FUNC_NAME, ALIAS_NAME)
#endif
#ifndef _COREGL_FASTPATH_SUPPORTED_EXTENSION
#define _COREGL_FASTPATH_SUPPORTED_EXTENSION_NOT_DEFINED
#define _COREGL_FASTPATH_SUPPORTED_EXTENSION(NAME, MINVER, MAXVER)
#endif
#ifndef _COREGL_EXT_SYMBOL_FASTPATH_PASS
#define _COREGL_EXT_SYMBOL_FASTPATH_PASS_NOT_DEFINED
#define _COREGL_EXT_SYMBOL_FASTPATH_PASS(FUNC_NAME)
#endif
#ifndef _COREGL_EXT_SYMBOL_FASTPATH_BLOCK
#define _COREGL_EXT_SYMBOL_FASTPATH_BLOCK_NOT_DEFINED
#define _COREGL_EXT_SYMBOL_FASTPATH_BLOCK(FUNC_NAME)
#endif

#ifndef _COREGL_START_API
#define _COREGL_START_API(VERSION)
#define _COREGL_START_API_DEFINED_INSIDE
#endif

#ifndef _COREGL_END_API
#define _COREGL_END_API(VERSION)
#define _COREGL_END_API_DEFINED_INSIDE
#endif

/* OpenGL ES 2.0 */
_COREGL_START_API(COREGL_GLAPI_2)
_COREGL_SYMBOL(void, glAttachShader, (GLuint program, GLuint shader))
_COREGL_SYMBOL(void, glBindAttribLocation, (GLuint program, GLuint index,
			   const char *name))
_COREGL_SYMBOL(void, glBindFramebuffer, (GLenum target, GLuint framebuffer))
_COREGL_SYMBOL(void, glBindRenderbuffer, (GLenum target, GLuint renderbuffer))

_COREGL_SYMBOL(void, glBlendColor, (GLclampf red, GLclampf green, GLclampf blue,
									GLclampf alpha))
_COREGL_SYMBOL(void, glBlendEquation, ( GLenum mode ))
_COREGL_SYMBOL(void, glBlendEquationSeparate, (GLenum modeRGB,
			   GLenum modeAlpha))
_COREGL_SYMBOL(void, glBlendFuncSeparate, (GLenum srcRGB, GLenum dstRGB,
			   GLenum srcAlpha, GLenum dstAlpha))
_COREGL_SYMBOL(GLenum, glCheckFramebufferStatus, (GLenum target))
_COREGL_SYMBOL(void, glCompileShader, (GLuint shader))
_COREGL_SYMBOL(GLuint, glCreateProgram, (void))
_COREGL_SYMBOL(GLuint, glCreateShader, (GLenum type))
_COREGL_SYMBOL(void, glDeleteFramebuffers, (GLsizei n,
			   const GLuint *framebuffers))
_COREGL_SYMBOL(void, glDeleteProgram, (GLuint program))
_COREGL_SYMBOL(void, glDeleteRenderbuffers, (GLsizei n,
			   const GLuint *renderbuffers))
_COREGL_SYMBOL(void, glDeleteShader, (GLuint shader))
_COREGL_SYMBOL(void, glDetachShader, (GLuint program, GLuint shader))
_COREGL_SYMBOL(void, glDisableVertexAttribArray, (GLuint index))
_COREGL_SYMBOL(void, glEnableVertexAttribArray, (GLuint index))
_COREGL_SYMBOL(void, glFramebufferRenderbuffer, (GLenum target,
			   GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer))
_COREGL_SYMBOL(void, glFramebufferTexture2D, (GLenum target, GLenum attachment,
			   GLenum textarget, GLuint texture, GLint level))
_COREGL_SYMBOL(void, glGenerateMipmap, (GLenum target))
_COREGL_SYMBOL(void, glGenFramebuffers, (GLsizei n, GLuint *framebuffers))
_COREGL_SYMBOL(void, glGenRenderbuffers, (GLsizei n, GLuint *renderbuffers))
_COREGL_SYMBOL(void, glGetActiveAttrib, (GLuint program, GLuint index,
			   GLsizei bufsize, GLsizei *length, GLint *size, GLenum *type, char *name))
_COREGL_SYMBOL(void, glGetActiveUniform, (GLuint program, GLuint index,
			   GLsizei bufsize, GLsizei *length, GLint *size, GLenum *type, char *name))
_COREGL_SYMBOL(void, glGetAttachedShaders, (GLuint program, GLsizei maxcount,
			   GLsizei *count, GLuint *shaders))
_COREGL_SYMBOL(int, glGetAttribLocation, (GLuint program, const char *name))
_COREGL_SYMBOL(void, glGetFramebufferAttachmentParameteriv, (GLenum target,
			   GLenum attachment, GLenum pname, GLint *params))
_COREGL_SYMBOL(void, glGetProgramiv, (GLuint program, GLenum pname,
									  GLint *params))
_COREGL_SYMBOL(void, glGetProgramInfoLog, (GLuint program, GLsizei bufsize,
			   GLsizei *length, char *infolog))
_COREGL_SYMBOL(void, glGetRenderbufferParameteriv, (GLenum target, GLenum pname,
			   GLint *params))
_COREGL_SYMBOL(void, glGetShaderiv, (GLuint shader, GLenum pname,
									 GLint *params))
_COREGL_SYMBOL(void, glGetShaderInfoLog, (GLuint shader, GLsizei bufsize,
			   GLsizei *length, char *infolog))
_COREGL_SYMBOL(void, glGetShaderPrecisionFormat, (GLenum shadertype,
			   GLenum precisiontype, GLint *range, GLint *precision))
_COREGL_SYMBOL(void, glGetShaderSource, (GLuint shader, GLsizei bufsize,
			   GLsizei *length, char *source))
_COREGL_SYMBOL(void, glGetUniformfv, (GLuint program, GLint location,
									  GLfloat *params))
_COREGL_SYMBOL(void, glGetUniformiv, (GLuint program, GLint location,
									  GLint *params))
_COREGL_SYMBOL(int, glGetUniformLocation, (GLuint program, const char *name))
_COREGL_SYMBOL(void, glGetVertexAttribfv, (GLuint index, GLenum pname,
			   GLfloat *params))
_COREGL_SYMBOL(void, glGetVertexAttribiv, (GLuint index, GLenum pname,
			   GLint *params))
_COREGL_SYMBOL(void, glGetVertexAttribPointerv, (GLuint index, GLenum pname,
			   void **pointer))
_COREGL_SYMBOL(GLboolean, glIsFramebuffer, (GLuint framebuffer))
_COREGL_SYMBOL(GLboolean, glIsProgram, (GLuint program))
_COREGL_SYMBOL(GLboolean, glIsRenderbuffer, (GLuint renderbuffer))
_COREGL_SYMBOL(GLboolean, glIsShader, (GLuint shader))
_COREGL_SYMBOL(void, glLinkProgram, (GLuint program))
_COREGL_SYMBOL(void, glReleaseShaderCompiler, (void))
_COREGL_SYMBOL(void, glRenderbufferStorage, (GLenum target,
			   GLenum internalformat, GLsizei width, GLsizei height))
_COREGL_SYMBOL(void, glShaderBinary, (GLsizei n, const GLuint *shaders,
									  GLenum binaryformat, const void *binary, GLsizei length))
_COREGL_SYMBOL(void, glShaderSource, (GLuint shader, GLsizei count,
									  const char **string, const GLint *length))
_COREGL_SYMBOL(void, glStencilFuncSeparate, (GLenum face, GLenum func,
			   GLint ref, GLuint mask))
_COREGL_SYMBOL(void, glStencilMaskSeparate, (GLenum face, GLuint mask))
_COREGL_SYMBOL(void, glStencilOpSeparate, (GLenum face, GLenum fail,
			   GLenum zfail, GLenum zpass))
_COREGL_SYMBOL(void, glUniform1f, (GLint location, GLfloat x))
_COREGL_SYMBOL(void, glUniform1fv, (GLint location, GLsizei count,
									const GLfloat *v))
_COREGL_SYMBOL(void, glUniform1i, (GLint location, GLint x))
_COREGL_SYMBOL(void, glUniform1iv, (GLint location, GLsizei count,
									const GLint *v))
_COREGL_SYMBOL(void, glUniform2f, (GLint location, GLfloat x, GLfloat y))
_COREGL_SYMBOL(void, glUniform2fv, (GLint location, GLsizei count,
									const GLfloat *v))
_COREGL_SYMBOL(void, glUniform2i, (GLint location, GLint x, GLint y))
_COREGL_SYMBOL(void, glUniform2iv, (GLint location, GLsizei count,
									const GLint *v))
_COREGL_SYMBOL(void, glUniform3f, (GLint location, GLfloat x, GLfloat y,
								   GLfloat z))
_COREGL_SYMBOL(void, glUniform3fv, (GLint location, GLsizei count,
									const GLfloat *v))
_COREGL_SYMBOL(void, glUniform3i, (GLint location, GLint x, GLint y, GLint z))
_COREGL_SYMBOL(void, glUniform3iv, (GLint location, GLsizei count,
									const GLint *v))
_COREGL_SYMBOL(void, glUniform4f, (GLint location, GLfloat x, GLfloat y,
								   GLfloat z, GLfloat w))
_COREGL_SYMBOL(void, glUniform4fv, (GLint location, GLsizei count,
									const GLfloat *v))
_COREGL_SYMBOL(void, glUniform4i, (GLint location, GLint x, GLint y, GLint z,
								   GLint w))
_COREGL_SYMBOL(void, glUniform4iv, (GLint location, GLsizei count,
									const GLint *v))
_COREGL_SYMBOL(void, glUniformMatrix2fv, (GLint location, GLsizei count,
			   GLboolean transpose, const GLfloat *value))
_COREGL_SYMBOL(void, glUniformMatrix3fv, (GLint location, GLsizei count,
			   GLboolean transpose, const GLfloat *value))
_COREGL_SYMBOL(void, glUniformMatrix4fv, (GLint location, GLsizei count,
			   GLboolean transpose, const GLfloat *value))
_COREGL_SYMBOL(void, glUseProgram, (GLuint program))
_COREGL_SYMBOL(void, glValidateProgram, (GLuint program))
_COREGL_SYMBOL(void, glVertexAttrib1f, (GLuint index, GLfloat x))
_COREGL_SYMBOL(void, glVertexAttrib1fv, (GLuint index, const GLfloat *values))
_COREGL_SYMBOL(void, glVertexAttrib2f, (GLuint index, GLfloat x, GLfloat y))
_COREGL_SYMBOL(void, glVertexAttrib2fv, (GLuint index, const GLfloat *values))
_COREGL_SYMBOL(void, glVertexAttrib3f, (GLuint index, GLfloat x, GLfloat y,
										GLfloat z))
_COREGL_SYMBOL(void, glVertexAttrib3fv, (GLuint index, const GLfloat *values))
_COREGL_SYMBOL(void, glVertexAttrib4f, (GLuint index, GLfloat x, GLfloat y,
										GLfloat z, GLfloat w))
_COREGL_SYMBOL(void, glVertexAttrib4fv, (GLuint index, const GLfloat *values))
_COREGL_SYMBOL(void, glVertexAttribPointer, (GLuint index, GLint size,
			   GLenum type, GLboolean normalized, GLsizei stride, const void *pointer))
_COREGL_EXT_SYMBOL(void, glBindFramebufferOES, (GLenum target,
				   GLuint framebuffer))
_COREGL_EXT_SYMBOL(void, glBindRenderbufferOES, (GLenum target,
				   GLuint renderbuffer))
_COREGL_EXT_SYMBOL(void, glDeleteFramebuffersOES, (GLsizei n,
				   const GLuint *framebuffers))
_COREGL_EXT_SYMBOL(void, glDeleteRenderbuffersOES, (GLsizei n,
				   const GLuint *renderbuffers))
_COREGL_EXT_SYMBOL(void, glDepthRangefOES, (GLclampf zNear, GLclampf zFar))
_COREGL_EXT_SYMBOL(void, glDepthRangexOES, (GLclampx zNear, GLclampx zFar))
_COREGL_EXT_SYMBOL(void, glFramebufferRenderbufferOES, (GLenum target,
				   GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer))
_COREGL_EXT_SYMBOL(void, glFramebufferTexture2DOES, (GLenum target,
				   GLenum attachment, GLenum textarget, GLuint texture, GLint level))
_COREGL_EXT_SYMBOL(void, glFramebufferTextureOES, (GLenum target,
				   GLenum attachment, GLuint texture, GLint level))
_COREGL_EXT_SYMBOL(void, glTexBufferRangeOES, (GLenum target,
				   GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size))
_COREGL_EXT_SYMBOL(void, glSamplerParameterIuivOES, (GLuint sampler,
				   GLenum pname, GLuint const *params))
_COREGL_EXT_SYMBOL(void, glSamplerParameterIivOES, (GLuint sampler,
				   GLenum pname, GLint const *params))
_COREGL_EXT_SYMBOL(void, glGetSamplerParameterIuivOES, (GLuint sampler,
				   GLenum pname, GLuint *params))
_COREGL_EXT_SYMBOL(void, glGetSamplerParameterIivOES, (GLuint sampler,
				   GLenum pname, GLint *params))
_COREGL_EXT_SYMBOL(void, glGetTexParameterIuivOES, (GLenum target, GLenum pname,
				   GLuint *params))
_COREGL_EXT_SYMBOL(void, glGetTexParameterIivOES, (GLenum target, GLenum pname,
				   GLint *params))
_COREGL_EXT_SYMBOL(void, glPatchParameteriOES, (GLenum pname, GLint value))
_COREGL_EXT_SYMBOL(void, glTexParameterIivOES, (GLenum target, GLenum pname,
				   GLint const *params))
_COREGL_EXT_SYMBOL(void, glTexParameterIuivOES, (GLenum target, GLenum pname,
				   GLuint const *params))
_COREGL_EXT_SYMBOL(void, glGetSamplerParameterIivEXT, (GLuint sampler,
				   GLenum pname, GLint *params))
_COREGL_EXT_SYMBOL(void, glGetSamplerParameterIuivEXT, (GLuint sampler,
				   GLenum pname, GLuint *params))
_COREGL_EXT_SYMBOL(void, glSamplerParameterIivEXT, (GLuint sampler,
				   GLenum pname, GLint const *params))
_COREGL_EXT_SYMBOL(void, glFramebufferTextureEXT, (GLenum target,
				   GLenum attachment, GLuint texture, GLint level))
_COREGL_EXT_SYMBOL(void, glTexBufferRangeEXT, (GLenum target,
				   GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size))
_COREGL_EXT_SYMBOL(void, glSamplerParameterIuivEXT, (GLuint sampler,
				   GLenum pname, GLuint const *params))
_COREGL_EXT_SYMBOL(void, glGetTexParameterIuivEXT, (GLenum target, GLenum pname,
				   GLuint *params))
_COREGL_EXT_SYMBOL(void, glGetTexParameterIivEXT, (GLenum target, GLenum pname,
				   GLint *params))
_COREGL_EXT_SYMBOL(void, glPatchParameteriEXT, (GLenum pname, GLint value))
_COREGL_EXT_SYMBOL(void, glTexParameterIivEXT, (GLenum target, GLenum pname,
				   GLint const *params))
_COREGL_EXT_SYMBOL(void, glTexParameterIuivEXT, (GLenum target, GLenum pname,
				   GLuint const *params))


_COREGL_EXT_SYMBOL(void, glGenFramebuffersOES, (GLsizei n,
				   GLuint *framebuffers))
_COREGL_EXT_SYMBOL(void, glGenRenderbuffersOES, (GLsizei n,
				   GLuint *renderbuffers))
_COREGL_EXT_SYMBOL(void, glGetFramebufferAttachmentParameterivOES,
				   (GLenum target, GLenum attachment, GLenum pname, GLint *params))
_COREGL_EXT_SYMBOL(void, glFramebufferTexture3DOES, (GLenum target,
				   GLenum attachment, GLenum textarget, GLuint texture, GLint level,
				   GLint zoffset))
_COREGL_EXT_SYMBOL(GLboolean, glIsFramebufferOES, (GLuint framebuffer))
_COREGL_EXT_SYMBOL(GLboolean, glIsRenderbufferOES, (GLuint renderbuffer))
_COREGL_EXT_SYMBOL(void, glAlphaFuncxOES, (GLenum func, GLclampx ref))
_COREGL_EXT_SYMBOL(void, glBlendBarrierKHR, (void))
_COREGL_EXT_SYMBOL(void, glBlendEquationOES, (GLenum mode))
_COREGL_EXT_SYMBOL(void, glBlendEquationSeparateOES, (GLenum modeRGB,
				   GLenum modeAlpha))
_COREGL_EXT_SYMBOL(void, glBlendFuncSeparateOES, (GLenum srcRGB, GLenum dstRGB,
				   GLenum srcAlpha, GLenum dstAlpha))
_COREGL_EXT_SYMBOL(GLenum, glCheckFramebufferStatusOES, (GLenum target))
_COREGL_EXT_SYMBOL(void, glClearColorxOES, (GLclampx red, GLclampx green,
				   GLclampx blue, GLclampx alpha))
_COREGL_EXT_SYMBOL(void, glClearDepthfOES, (GLclampf depth))
_COREGL_EXT_SYMBOL(void, glClearDepthxOES, (GLclampx depth))
_COREGL_EXT_SYMBOL(void, glClipPlanefOES, (GLenum plane,
				   GLfloat const *equation))
_COREGL_EXT_SYMBOL(void, glClipPlanexOES, (GLenum plane,
				   GLfixed const *equation))
_COREGL_EXT_SYMBOL(void, glColor4xOES, (GLfixed red, GLfixed green,
										GLfixed blue, GLfixed alpha))
_COREGL_EXT_SYMBOL(void, glFogxOES, (GLenum pname, GLfixed param))
_COREGL_EXT_SYMBOL(void, glFogxvOES, (GLenum pname, GLfixed const *params))
_COREGL_EXT_SYMBOL(void, glFrustumxOES, (GLfixed left, GLfixed right,
				   GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar))
_COREGL_EXT_SYMBOL(void, glFrustumfOES, (GLfloat left, GLfloat right,
				   GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar))
_COREGL_EXT_SYMBOL(void, glGenerateMipmapOES, (GLenum target))
_COREGL_EXT_SYMBOL(void, glGetClipPlanefOES, (GLenum pname, GLfloat eqn[4]))
_COREGL_EXT_SYMBOL(void, glGetClipPlanexOES, (GLenum pname, GLfixed eqn[4]))
_COREGL_EXT_SYMBOL(void, glGetPointervKHR, (GLenum pname, GLvoid **params))
_COREGL_EXT_SYMBOL(void, glDebugMessageCallbackKHR, (GLDEBUGPROCKHR callback,
				   void const *userParam))
_COREGL_EXT_SYMBOL(void, glDebugMessageControlKHR, (GLenum source, GLenum type,
				   GLenum severity, GLsizei count, GLuint const *ids, GLboolean enabled))
_COREGL_EXT_SYMBOL(void, glDebugMessageInsertKHR, (GLenum source, GLenum type,
				   GLuint id, GLenum severity, GLsizei length, GLchar const *buf))
_COREGL_EXT_SYMBOL(GLuint, glGetDebugMessageLogKHR, (GLuint count,
				   GLsizei bufsize, GLenum *sources, GLenum *types, GLuint *ids,
				   GLenum *severities, GLsizei *lengths, GLchar *messageLog))
_COREGL_EXT_SYMBOL(void, glGetObjectLabelKHR, (GLenum identifier, GLuint name,
				   GLsizei bufSize, GLsizei *length, GLchar *label))
_COREGL_EXT_SYMBOL(void, glGetObjectPtrLabelKHR, (void const *ptr,
				   GLsizei bufSize, GLsizei *length, GLchar *label))
_COREGL_EXT_SYMBOL(void, glObjectLabelKHR, (GLenum identifier, GLuint name,
				   GLsizei length, const GLchar *label))
_COREGL_EXT_SYMBOL(void, glObjectPtrLabelKHR, (const void *ptr, GLsizei length,
				   const GLchar *label))
_COREGL_EXT_SYMBOL(void, glPopDebugGroupKHR, (void))
_COREGL_EXT_SYMBOL(void, glPushDebugGroupKHR, (GLenum source, GLuint id,
				   GLsizei length, const GLchar *message))
_COREGL_EXT_SYMBOL(void, glGetFixedvOES, (GLenum pname, GLfixed *params))
_COREGL_EXT_SYMBOL(void, glGetLightxvOES, (GLenum light, GLenum pname,
				   GLfixed *params))
_COREGL_EXT_SYMBOL(void, glGetMaterialxvOES, (GLenum face, GLenum pname,
				   GLfixed *params))
_COREGL_EXT_SYMBOL(void, glGetRenderbufferParameterivOES, (GLenum target,
				   GLenum pname, GLint *params))
_COREGL_EXT_SYMBOL(void, glGetTexEnvxvOES, (GLenum env, GLenum pname,
				   GLfixed *params))
_COREGL_EXT_SYMBOL(void, glGetTexParameterxvOES, (GLenum target, GLenum pname,
				   GLfixed *params))
_COREGL_EXT_SYMBOL(void, glLightModelxOES, (GLenum pname, GLfixed param))
_COREGL_EXT_SYMBOL(void, glLightModelxvOES, (GLenum pname,
				   const GLfixed *params))
_COREGL_EXT_SYMBOL(void, glLightxOES, (GLenum light, GLenum pname,
									   GLfixed param))
_COREGL_EXT_SYMBOL(void, glLightxvOES, (GLenum light, GLenum pname,
										const GLfixed *params))
_COREGL_EXT_SYMBOL(void, glLineWidthxOES, (GLfixed width))
_COREGL_EXT_SYMBOL(void, glLoadMatrixxOES, (const GLfixed *m))
_COREGL_EXT_SYMBOL(void, glMaterialxOES, (GLenum face, GLenum pname,
				   GLfixed param))
_COREGL_EXT_SYMBOL(void, glMaterialxvOES, (GLenum face, GLenum pname,
				   const GLfixed *params))
_COREGL_EXT_SYMBOL(void, glMultMatrixxOES, (const GLfixed *m))
_COREGL_EXT_SYMBOL(void, glMultiTexCoord4bOES, (GLenum target, GLbyte s,
				   GLbyte t, GLbyte r, GLbyte q))
_COREGL_EXT_SYMBOL(void, glMultiTexCoord4xOES, (GLenum target, GLfixed s,
				   GLfixed t, GLfixed r, GLfixed q))
_COREGL_EXT_SYMBOL(void, glNormal3xOES, (GLfixed nx, GLfixed ny, GLfixed nz))
_COREGL_EXT_SYMBOL(void, glOrthofOES, (GLfloat left, GLfloat right,
									   GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar))
_COREGL_EXT_SYMBOL(void, glOrthoxOES, (GLfixed left, GLfixed right,
									   GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar))
_COREGL_EXT_SYMBOL(void, glPointParameterxOES, (GLenum pname, GLfixed param))
_COREGL_EXT_SYMBOL(void, glPointParameterxvOES, (GLenum pname,
				   const GLfixed *params))
_COREGL_EXT_SYMBOL(void, glPointSizexOES, (GLfixed size))
_COREGL_EXT_SYMBOL(void, glPolygonOffsetxOES, (GLfixed factor, GLfixed units))
_COREGL_EXT_SYMBOL(void, glQueryCounterEXT, (GLuint id, GLenum target))
_COREGL_EXT_SYMBOL(void, glRenderbufferStorageOES, (GLenum target,
				   GLenum internalformat, GLsizei width, GLsizei height))
_COREGL_EXT_SYMBOL(void, glRotatexOES, (GLfixed angle, GLfixed x, GLfixed y,
										GLfixed z))
_COREGL_EXT_SYMBOL(void, glSampleCoveragexOES, (GLclampx value,
				   GLboolean invert))
_COREGL_EXT_SYMBOL(void, glScalexOES, (GLfixed x, GLfixed y, GLfixed z))
_COREGL_EXT_SYMBOL(void, glTexEnvxOES, (GLenum target, GLenum pname,
										GLfixed param))
_COREGL_EXT_SYMBOL(void, glTexEnvxvOES, (GLenum target, GLenum pname,
				   const GLfixed *params))
_COREGL_EXT_SYMBOL(void, glTexParameterxOES, (GLenum target, GLenum pname,
				   GLfixed param))
_COREGL_EXT_SYMBOL(void, glTexParameterxvOES, (GLenum target, GLenum pname,
				   const GLfixed *params))
_COREGL_EXT_SYMBOL(void, glTexStorage2DEXT, (GLenum target, GLsizei levels,
				   GLenum internalformat, GLsizei width, GLsizei height))
_COREGL_EXT_SYMBOL(void, glTexStorage3DEXT, (GLenum target, GLsizei levels,
				   GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth))
_COREGL_EXT_SYMBOL(void, glTexStorage3DMultisampleOES, (GLenum target,
				   GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height,
				   GLsizei depth, GLboolean fixedsamplelocations))
_COREGL_EXT_SYMBOL(void, glTranslatexOES, (GLfixed x, GLfixed y, GLfixed z))
_COREGL_EXT_SYMBOL(void, glTexBufferOES, (GLenum target, GLenum internalformat,
				   GLuint buffer))
_COREGL_EXT_SYMBOL(void, glTexBufferEXT, (GLenum target, GLenum internalformat,
				   GLuint buffer))
_COREGL_END_API(COREGL_GLAPI_2)

/* OpenGL ES 3.0 */
_COREGL_START_API(COREGL_GLAPI_3)
_COREGL_SYMBOL(void, glReadBuffer, (GLenum mode))
_COREGL_SYMBOL(void, glDrawRangeElements, (GLenum mode, GLuint start,
			   GLuint end, GLsizei count, GLenum type, const GLvoid *indices))
_COREGL_SYMBOL(void, glTexImage3D, (GLenum target, GLint level,
									GLint internalformat, GLsizei width, GLsizei height, GLsizei depth,
									GLint border, GLenum format, GLenum type, const GLvoid *pixels))
_COREGL_SYMBOL(void, glTexSubImage3D, (GLenum target, GLint level,
									   GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height,
									   GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels))
_COREGL_SYMBOL(void, glCopyTexSubImage3D, (GLenum target, GLint level,
			   GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width,
			   GLsizei height))
_COREGL_SYMBOL(void, glCompressedTexImage3D, (GLenum target, GLint level,
			   GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth,
			   GLint border, GLsizei imageSize, const GLvoid *data))
_COREGL_SYMBOL(void, glCompressedTexSubImage3D, (GLenum target, GLint level,
			   GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height,
			   GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data))
_COREGL_SYMBOL(void, glGenQueries, (GLsizei n, GLuint *ids))
_COREGL_SYMBOL(void, glDeleteQueries, (GLsizei n, const GLuint *ids))
_COREGL_SYMBOL(GLboolean, glIsQuery, (GLuint id))
_COREGL_SYMBOL(void, glBeginQuery, (GLenum target, GLuint id))
_COREGL_SYMBOL(void, glEndQuery, (GLenum target))
_COREGL_SYMBOL(void, glGetQueryiv, (GLenum target, GLenum pname, GLint *params))
_COREGL_SYMBOL(void, glGetQueryObjectuiv, (GLuint id, GLenum pname,
			   GLuint *params))
_COREGL_SYMBOL(GLboolean, glUnmapBuffer, (GLenum target))
_COREGL_SYMBOL(void, glGetBufferPointerv, (GLenum target, GLenum pname,
			   GLvoid **params))
_COREGL_SYMBOL(void, glDrawBuffers, (GLsizei n, const GLenum *bufs))
_COREGL_SYMBOL(void, glUniformMatrix2x3fv, (GLint location, GLsizei count,
			   GLboolean transpose, const GLfloat *value))
_COREGL_SYMBOL(void, glUniformMatrix3x2fv, (GLint location, GLsizei count,
			   GLboolean transpose, const GLfloat *value))
_COREGL_SYMBOL(void, glUniformMatrix2x4fv, (GLint location, GLsizei count,
			   GLboolean transpose, const GLfloat *value))
_COREGL_SYMBOL(void, glUniformMatrix4x2fv, (GLint location, GLsizei count,
			   GLboolean transpose, const GLfloat *value))
_COREGL_SYMBOL(void, glUniformMatrix3x4fv, (GLint location, GLsizei count,
			   GLboolean transpose, const GLfloat *value))
_COREGL_SYMBOL(void, glUniformMatrix4x3fv, (GLint location, GLsizei count,
			   GLboolean transpose, const GLfloat *value))
_COREGL_SYMBOL(void, glBlitFramebuffer, (GLint srcX0, GLint srcY0, GLint srcX1,
			   GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1,
			   GLbitfield mask, GLenum filter))
_COREGL_SYMBOL(void, glRenderbufferStorageMultisample, (GLenum target,
			   GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height))
_COREGL_SYMBOL(void, glFramebufferTextureLayer, (GLenum target,
			   GLenum attachment, GLuint texture, GLint level, GLint layer))
_COREGL_SYMBOL(GLvoid *, glMapBufferRange, (GLenum target, GLintptr offset,
			   GLsizeiptr length, GLbitfield access))
_COREGL_SYMBOL(void, glFlushMappedBufferRange, (GLenum target, GLintptr offset,
			   GLsizeiptr length))
_COREGL_SYMBOL(void, glBindVertexArray, (GLuint array))
_COREGL_SYMBOL(void, glDeleteVertexArrays, (GLsizei n, const GLuint *arrays))
_COREGL_SYMBOL(void, glGenVertexArrays, (GLsizei n, GLuint *arrays))
_COREGL_SYMBOL(GLboolean, glIsVertexArray, (GLuint array))
_COREGL_SYMBOL(void, glGetIntegeri_v, (GLenum target, GLuint index,
									   GLint *data))
_COREGL_SYMBOL(void, glBeginTransformFeedback, (GLenum primitiveMode))
_COREGL_SYMBOL(void, glEndTransformFeedback, (void))
_COREGL_SYMBOL(void, glBindBufferRange, (GLenum target, GLuint index,
			   GLuint buffer, GLintptr offset, GLsizeiptr size))
_COREGL_SYMBOL(void, glBindBufferBase, (GLenum target, GLuint index,
										GLuint buffer))
_COREGL_SYMBOL(void, glTransformFeedbackVaryings, (GLuint program,
			   GLsizei count, const GLchar *const *varyings, GLenum bufferMode))
_COREGL_SYMBOL(void, glGetTransformFeedbackVarying, (GLuint program,
			   GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type,
			   GLchar *name))
_COREGL_SYMBOL(void, glVertexAttribIPointer, (GLuint index, GLint size,
			   GLenum type, GLsizei stride, const GLvoid *pointer))
_COREGL_SYMBOL(void, glGetVertexAttribIiv, (GLuint index, GLenum pname,
			   GLint *params))
_COREGL_SYMBOL(void, glGetVertexAttribIuiv, (GLuint index, GLenum pname,
			   GLuint *params))
_COREGL_SYMBOL(void, glVertexAttribI4i, (GLuint index, GLint x, GLint y,
			   GLint z, GLint w))
_COREGL_SYMBOL(void, glVertexAttribI4ui, (GLuint index, GLuint x, GLuint y,
			   GLuint z, GLuint w))
_COREGL_SYMBOL(void, glVertexAttribI4iv, (GLuint index, const GLint *v))
_COREGL_SYMBOL(void, glVertexAttribI4uiv, (GLuint index, const GLuint *v))
_COREGL_SYMBOL(void, glGetUniformuiv, (GLuint program, GLint location,
									   GLuint *params))
_COREGL_SYMBOL(GLint, glGetFragDataLocation, (GLuint program,
			   const GLchar *name))
_COREGL_SYMBOL(void, glUniform1ui, (GLint location, GLuint v0))
_COREGL_SYMBOL(void, glUniform2ui, (GLint location, GLuint v0, GLuint v1))
_COREGL_SYMBOL(void, glUniform3ui, (GLint location, GLuint v0, GLuint v1,
									GLuint v2))
_COREGL_SYMBOL(void, glUniform4ui, (GLint location, GLuint v0, GLuint v1,
									GLuint v2, GLuint v3))
_COREGL_SYMBOL(void, glUniform1uiv, (GLint location, GLsizei count,
									 const GLuint *value))
_COREGL_SYMBOL(void, glUniform2uiv, (GLint location, GLsizei count,
									 const GLuint *value))
_COREGL_SYMBOL(void, glUniform3uiv, (GLint location, GLsizei count,
									 const GLuint *value))
_COREGL_SYMBOL(void, glUniform4uiv, (GLint location, GLsizei count,
									 const GLuint *value))
_COREGL_SYMBOL(void, glClearBufferiv, (GLenum buffer, GLint drawbuffer,
									   const GLint *value))
_COREGL_SYMBOL(void, glClearBufferuiv, (GLenum buffer, GLint drawbuffer,
										const GLuint *value))
_COREGL_SYMBOL(void, glClearBufferfv, (GLenum buffer, GLint drawbuffer,
									   const GLfloat *value))
_COREGL_SYMBOL(void, glClearBufferfi, (GLenum buffer, GLint drawbuffer,
									   GLfloat depth, GLint stencil))
_COREGL_SYMBOL(const GLubyte *, glGetStringi, (GLenum name, GLuint index))
_COREGL_SYMBOL(void, glCopyBufferSubData, (GLenum readTarget,
			   GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size))
_COREGL_SYMBOL(void, glGetUniformIndices, (GLuint program, GLsizei uniformCount,
			   const GLchar *const *uniformNames, GLuint *uniformIndices))
_COREGL_SYMBOL(void, glGetActiveUniformsiv, (GLuint program,
			   GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname,
			   GLint *params))
_COREGL_SYMBOL(GLuint, glGetUniformBlockIndex, (GLuint program,
			   const GLchar *uniformBlockName))
_COREGL_SYMBOL(void, glGetActiveUniformBlockiv, (GLuint program,
			   GLuint uniformBlockIndex, GLenum pname, GLint *params))
_COREGL_SYMBOL(void, glGetActiveUniformBlockName, (GLuint program,
			   GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length,
			   GLchar *uniformBlockName))
_COREGL_SYMBOL(void, glUniformBlockBinding, (GLuint program,
			   GLuint uniformBlockIndex, GLuint uniformBlockBinding))
_COREGL_SYMBOL(void, glDrawArraysInstanced, (GLenum mode, GLint first,
			   GLsizei count, GLsizei instanceCount))
_COREGL_SYMBOL(void, glDrawElementsInstanced, (GLenum mode, GLsizei count,
			   GLenum type, const GLvoid *indices, GLsizei instanceCount))
_COREGL_SYMBOL(GLsync, glFenceSync, (GLenum condition, GLbitfield flags))
_COREGL_SYMBOL(GLboolean, glIsSync, (GLsync sync))
_COREGL_SYMBOL(void, glDeleteSync, (GLsync sync))
_COREGL_SYMBOL(GLenum, glClientWaitSync, (GLsync sync, GLbitfield flags,
			   GLuint64 timeout))
_COREGL_SYMBOL(void, glWaitSync, (GLsync sync, GLbitfield flags,
								  GLuint64 timeout))
_COREGL_SYMBOL(void, glGetInteger64v, (GLenum pname, GLint64 *params))
_COREGL_SYMBOL(void, glGetSynciv, (GLsync sync, GLenum pname, GLsizei bufSize,
								   GLsizei *length, GLint *values))
_COREGL_SYMBOL(void, glGetInteger64i_v, (GLenum target, GLuint index,
			   GLint64 *data))
_COREGL_SYMBOL(void, glGetBufferParameteri64v, (GLenum target, GLenum pname,
			   GLint64 *params))
_COREGL_SYMBOL(void, glGenSamplers, (GLsizei n, GLuint *samplers))
_COREGL_SYMBOL(void, glDeleteSamplers, (GLsizei n, const GLuint *samplers))
_COREGL_SYMBOL(GLboolean, glIsSampler, (GLuint sampler))
_COREGL_SYMBOL(void, glBindSampler, (GLuint unit, GLuint sampler))
_COREGL_SYMBOL(void, glSamplerParameteri, (GLuint sampler, GLenum pname,
			   GLint param))
_COREGL_SYMBOL(void, glSamplerParameteriv, (GLuint sampler, GLenum pname,
			   const GLint *param))
_COREGL_SYMBOL(void, glSamplerParameterf, (GLuint sampler, GLenum pname,
			   GLfloat param))
_COREGL_SYMBOL(void, glSamplerParameterfv, (GLuint sampler, GLenum pname,
			   const GLfloat *param))
_COREGL_SYMBOL(void, glGetSamplerParameteriv, (GLuint sampler, GLenum pname,
			   GLint *params))
_COREGL_SYMBOL(void, glGetSamplerParameterfv, (GLuint sampler, GLenum pname,
			   GLfloat *params))
_COREGL_SYMBOL(void, glVertexAttribDivisor, (GLuint index, GLuint divisor))
_COREGL_SYMBOL(void, glBindTransformFeedback, (GLenum target, GLuint id))
_COREGL_SYMBOL(void, glDeleteTransformFeedbacks, (GLsizei n, const GLuint *ids))
_COREGL_SYMBOL(void, glGenTransformFeedbacks, (GLsizei n, GLuint *ids))
_COREGL_SYMBOL(GLboolean, glIsTransformFeedback, (GLuint id))
_COREGL_SYMBOL(void, glPauseTransformFeedback, (void))
_COREGL_SYMBOL(void, glResumeTransformFeedback, (void))
_COREGL_SYMBOL(void, glGetProgramBinary, (GLuint program, GLsizei bufSize,
			   GLsizei *length, GLenum *binaryFormat, GLvoid *binary))
_COREGL_SYMBOL(void, glProgramBinary, (GLuint program, GLenum binaryFormat,
									   const GLvoid *binary, GLsizei length))
_COREGL_SYMBOL(void, glProgramParameteri, (GLuint program, GLenum pname,
			   GLint value))
_COREGL_SYMBOL(void, glInvalidateFramebuffer, (GLenum target,
			   GLsizei numAttachments, const GLenum *attachments))
_COREGL_SYMBOL(void, glInvalidateSubFramebuffer, (GLenum target,
			   GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y,
			   GLsizei width, GLsizei height))
_COREGL_SYMBOL(void, glTexStorage2D, (GLenum target, GLsizei levels,
									  GLenum internalformat, GLsizei width, GLsizei height))
_COREGL_SYMBOL(void, glTexStorage3D, (GLenum target, GLsizei levels,
									  GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth))
_COREGL_SYMBOL(void, glGetInternalformativ, (GLenum target,
			   GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params))
_COREGL_END_API(COREGL_GLAPI_3)


/* OpenGL ES 3.1 */
_COREGL_START_API(COREGL_GLAPI_31)
_COREGL_SYMBOL(GLuint, glCreateShaderProgramv, (GLenum type,  GLsizei count,
			   const GLchar *const *strings))
_COREGL_SYMBOL(void, glGenProgramPipelines, (GLsizei n,	GLuint *pipelines))
_COREGL_SYMBOL(void, glBindProgramPipeline, (GLuint pipeline))
_COREGL_SYMBOL(void, glGetProgramPipelineiv, (GLuint pipeline, GLenum pname,
			   GLint *params))
_COREGL_SYMBOL(void, glDeleteProgramPipelines, (GLsizei n,
			   GLuint const *pipelines))
_COREGL_SYMBOL(GLboolean, glIsProgramPipeline, (GLuint pipeline))
_COREGL_SYMBOL(void, glValidateProgramPipeline, (GLuint pipeline))
_COREGL_SYMBOL(void, glGetProgramPipelineInfoLog, (GLuint pipeline,
			   GLsizei bufSize, GLsizei *length, GLchar *infoLog))
_COREGL_SYMBOL(void, glDispatchCompute, (GLuint num_groups_x,
			   GLuint num_groups_y, GLuint num_groups_z))
_COREGL_SYMBOL(void, glDispatchComputeIndirect, (GLintptr indirect))
_COREGL_SYMBOL(void, glDrawArraysIndirect, (GLenum mode,
			   GLvoid const *indirect))
_COREGL_SYMBOL(void, glDrawElementsIndirect, (GLenum mode, GLenum type,
			   GLvoid const *indirect))
_COREGL_SYMBOL(void, glFramebufferParameteri, (GLenum target, GLenum pname,
			   GLint param))
_COREGL_SYMBOL(void, glGetFramebufferParameteriv, ( GLenum target,
			   GLenum pname,  GLint *params))
_COREGL_SYMBOL(void, glGetProgramInterfaceiv, (GLuint program,
			   GLenum programInterface,  GLenum pname,  GLint *params))
_COREGL_SYMBOL(GLuint, glGetProgramResourceIndex, (GLuint program,
			   GLenum programInterface,  const char *name))
_COREGL_SYMBOL(void, glGetProgramResourceName, (GLuint program,
			   GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length,
			   GLchar *name))
_COREGL_SYMBOL(void, glGetProgramResourceiv, (GLuint program,
			   GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props,
			   GLsizei bufSize,  GLsizei *length,  GLint *params))
_COREGL_SYMBOL(GLint, glGetProgramResourceLocation, (GLuint program,
			   GLenum programInterface, GLchar const *name))
_COREGL_SYMBOL(void, glUseProgramStages, (GLuint pipeline, GLbitfield stages,
			   GLuint program))
_COREGL_SYMBOL(void, glActiveShaderProgram, (GLuint pipeline, GLuint program))
_COREGL_SYMBOL(void, glProgramUniform1iv, (GLuint program, GLint location,
			   GLsizei count, const GLint *value))
_COREGL_SYMBOL(void, glProgramUniform2iv, (GLuint program, GLint location,
			   GLsizei count, const GLint *value))
_COREGL_SYMBOL(void, glProgramUniform3iv, (GLuint program, GLint location,
			   GLsizei count, const GLint *value))
_COREGL_SYMBOL(void, glProgramUniform4iv, (GLuint program, GLint location,
			   GLsizei count, const GLint *value))
_COREGL_SYMBOL(void, glProgramUniform1fv, (GLuint program, GLint location,
			   GLsizei count, const GLfloat *value))
_COREGL_SYMBOL(void, glProgramUniform2fv, (GLuint program, GLint location,
			   GLsizei count, const GLfloat *value))
_COREGL_SYMBOL(void, glProgramUniform3fv, (GLuint program, GLint location,
			   GLsizei count, const GLfloat *value))
_COREGL_SYMBOL(void, glProgramUniform4fv, (GLuint program, GLint location,
			   GLsizei count, const GLfloat *value))
_COREGL_SYMBOL(void, glProgramUniformMatrix2fv, (GLuint program, GLint location,
			   GLsizei count, GLboolean transpose, const GLfloat *value))
_COREGL_SYMBOL(void, glProgramUniformMatrix3fv, (GLuint program, GLint location,
			   GLsizei count, GLboolean transpose, const GLfloat *value))
_COREGL_SYMBOL(void, glProgramUniformMatrix4fv, (GLuint program, GLint location,
			   GLsizei count, GLboolean transpose, const GLfloat *value))
_COREGL_SYMBOL(void, glProgramUniform1i, (GLuint program, GLint location,
			   GLint x))
_COREGL_SYMBOL(void, glProgramUniform2i, (GLuint program, GLint location,
			   GLint x, GLint y))
_COREGL_SYMBOL(void, glProgramUniform3i, (GLuint program, GLint location,
			   GLint x, GLint y, GLint z))
_COREGL_SYMBOL(void, glProgramUniform4i, (GLuint program, GLint location,
			   GLint x, GLint y, GLint z, GLint w))
_COREGL_SYMBOL(void, glProgramUniform1f, (GLuint program, GLint location,
			   GLfloat x))
_COREGL_SYMBOL(void, glProgramUniform2f, (GLuint program, GLint location,
			   GLfloat x, GLfloat y))
_COREGL_SYMBOL(void, glProgramUniform3f, (GLuint program, GLint location,
			   GLfloat x, GLfloat y, GLfloat z))
_COREGL_SYMBOL(void, glProgramUniform4f, (GLuint program, GLint location,
			   GLfloat x, GLfloat y, GLfloat z, GLfloat w))
_COREGL_SYMBOL(void, glProgramUniform1ui, (GLuint program, GLint location,
			   GLuint x))
_COREGL_SYMBOL(void, glProgramUniform2ui, (GLuint program, GLint location,
			   GLuint x, GLuint y))
_COREGL_SYMBOL(void, glProgramUniform3ui, (GLuint program, GLint location,
			   GLuint x, GLuint y, GLuint z))
_COREGL_SYMBOL(void, glProgramUniform4ui, (GLuint program, GLint location,
			   GLuint x, GLuint y, GLuint z, GLuint w))
_COREGL_SYMBOL(void, glProgramUniform1uiv, (GLuint program, GLint location,
			   GLsizei count, const GLuint *value))
_COREGL_SYMBOL(void, glProgramUniform2uiv, (GLuint program, GLint location,
			   GLsizei count, const GLuint *value))
_COREGL_SYMBOL(void, glProgramUniform3uiv, (GLuint program, GLint location,
			   GLsizei count, const GLuint *value))
_COREGL_SYMBOL(void, glProgramUniform4uiv, (GLuint program, GLint location,
			   GLsizei count, const GLuint *value))
_COREGL_SYMBOL(void, glProgramUniformMatrix2x3fv, (GLuint program,
			   GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))
_COREGL_SYMBOL(void, glProgramUniformMatrix3x2fv, (GLuint program,
			   GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))
_COREGL_SYMBOL(void, glProgramUniformMatrix4x2fv, (GLuint program,
			   GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))
_COREGL_SYMBOL(void, glProgramUniformMatrix2x4fv, (GLuint program,
			   GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))
_COREGL_SYMBOL(void, glProgramUniformMatrix3x4fv, (GLuint program,
			   GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))
_COREGL_SYMBOL(void, glProgramUniformMatrix4x3fv, (GLuint program,
			   GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))
_COREGL_SYMBOL(void, glBindImageTexture, (GLuint unit, GLuint texture,
			   GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format))
_COREGL_SYMBOL(void, glGetBooleani_v, (GLenum target, GLuint index,
									   GLboolean *data))
_COREGL_SYMBOL(void, glMemoryBarrier, (GLbitfield barriers))
_COREGL_SYMBOL(void, glMemoryBarrierByRegion, (GLbitfield barriers))
_COREGL_SYMBOL(void, glTexStorage2DMultisample, (GLenum target, GLsizei samples,
			   GLenum internalformat, GLsizei width, GLsizei height,
			   GLboolean fixedsamplelocations))
_COREGL_SYMBOL(void, glGetMultisamplefv, (GLenum pname, GLuint index,
			   GLfloat *val))
_COREGL_SYMBOL(void, glSampleMaski, (GLuint maskNumber, GLbitfield mask))
_COREGL_SYMBOL(void, glGetTexLevelParameteriv, (GLenum target, GLint level,
			   GLenum pname, GLint *params))
_COREGL_SYMBOL(void, glGetTexLevelParameterfv, (GLenum target, GLint level,
			   GLenum pname, GLfloat *params))
_COREGL_SYMBOL(void, glBindVertexBuffer, (GLuint bindingindex, GLuint buffer,
			   GLintptr offset, GLsizei stride))
_COREGL_SYMBOL(void, glVertexAttribFormat, (GLuint attribindex, GLint size,
			   GLenum type, GLboolean normalized, GLuint relativeoffset))
_COREGL_SYMBOL(void, glVertexAttribIFormat, (GLuint attribindex, GLint size,
			   GLenum type, GLuint relativeoffset))
_COREGL_SYMBOL(void, glVertexAttribBinding, (GLuint attribindex,
			   GLuint bindingindex))
_COREGL_SYMBOL(void, glVertexBindingDivisor, (GLuint bindingindex,
			   GLuint divisor))
_COREGL_END_API(COREGL_GLAPI_31)

/* OpenGL ES 3.2 */
_COREGL_START_API(COREGL_GLAPI_32)
_COREGL_SYMBOL(void, glBlendBarrier, (void))
_COREGL_SYMBOL(void, glCopyImageSubData, (GLuint srcName, GLenum srcTarget,
			   GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName,
			   GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ,
			   GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth))
_COREGL_SYMBOL(void, glDebugMessageControl, (GLenum source, GLenum type,
			   GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled))
_COREGL_SYMBOL(void, glDebugMessageInsert, (GLenum source, GLenum type,
			   GLuint id, GLenum severity, GLsizei length, const GLchar *buf))
_COREGL_SYMBOL(void, glDebugMessageCallback, (GLDEBUGPROC callback,
			   const void *userParam))
_COREGL_SYMBOL(GLuint, glGetDebugMessageLog, (GLuint count, GLsizei bufSize,
			   GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities,
			   GLsizei *lengths, GLchar *messageLog))
_COREGL_SYMBOL(void, glPushDebugGroup, (GLenum source, GLuint id,
										GLsizei length,
										const GLchar *message))
_COREGL_SYMBOL(void, glPopDebugGroup, (void))
_COREGL_SYMBOL(void, glObjectLabel, (GLenum identifier, GLuint name,
									 GLsizei length, const GLchar *label))
_COREGL_SYMBOL(void, glGetObjectLabel, (GLenum identifier, GLuint name,
										GLsizei bufSize, GLsizei *length, GLchar *label))
_COREGL_SYMBOL(void, glObjectPtrLabel, (const void *ptr, GLsizei length,
										const GLchar *label))
_COREGL_SYMBOL(void, glGetObjectPtrLabel, (const void *ptr, GLsizei bufSize,
			   GLsizei *length, GLchar *label))
_COREGL_SYMBOL(void, glEnablei, (GLenum target, GLuint index))
_COREGL_SYMBOL(void, glDisablei, (GLenum target, GLuint index))
_COREGL_SYMBOL(void, glBlendEquationi, (GLuint buf, GLenum mode))
_COREGL_SYMBOL(void, glBlendEquationSeparatei, (GLuint buf, GLenum modeRGB,
			   GLenum modeAlpha))
_COREGL_SYMBOL(void, glBlendFunci, (GLuint buf, GLenum src, GLenum dst))
_COREGL_SYMBOL(void, glBlendFuncSeparatei, (GLuint buf, GLenum srcRGB,
			   GLenum dstRGB,
			   GLenum srcAlpha, GLenum dstAlpha))
_COREGL_SYMBOL(void, glColorMaski, (GLuint index, GLboolean r, GLboolean g,
									GLboolean b,
									GLboolean a))
_COREGL_SYMBOL(GLboolean, glIsEnabledi, (GLenum target, GLuint index))
_COREGL_SYMBOL(void, glDrawElementsBaseVertex, (GLenum mode, GLsizei count,
			   GLenum type,
			   const void *indices, GLint basevertex))
_COREGL_SYMBOL(void, glDrawRangeElementsBaseVertex, (GLenum mode, GLuint start,
			   GLuint end,
			   GLsizei count, GLenum type, const void *indices, GLint basevertex))
_COREGL_SYMBOL(void, glDrawElementsInstancedBaseVertex, (GLenum mode,
			   GLsizei count, GLenum type,
			   const void *indices, GLsizei instancecount, GLint basevertex))
_COREGL_SYMBOL(void, glFramebufferTexture, (GLenum target, GLenum attachment,
			   GLuint texture,
			   GLint level))
_COREGL_SYMBOL(void, glPrimitiveBoundingBox, (GLfloat minX, GLfloat minY,
			   GLfloat minZ,
			   GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW))
_COREGL_SYMBOL(GLenum, glGetGraphicsResetStatus, (void))
_COREGL_SYMBOL(void, glReadnPixels, (GLint x, GLint y, GLsizei width,
									 GLsizei height,
									 GLenum format, GLenum type, GLsizei bufSize, void *data))
_COREGL_SYMBOL(void, glGetnUniformfv, (GLuint program, GLint location,
									   GLsizei bufSize, GLfloat *params))
_COREGL_SYMBOL(void, glGetnUniformiv, (GLuint program, GLint location,
									   GLsizei bufSize, GLint *params))
_COREGL_SYMBOL(void, glGetnUniformuiv, (GLuint program, GLint location,
										GLsizei bufSize, GLuint *params))
_COREGL_SYMBOL(void, glMinSampleShading, (GLfloat value))
_COREGL_SYMBOL(void, glPatchParameteri, (GLenum pname, GLint value))
_COREGL_SYMBOL(void, glTexParameterIiv, (GLenum target, GLenum pname,
			   const GLint *params))
_COREGL_SYMBOL(void, glTexParameterIuiv, (GLenum target, GLenum pname,
			   const GLuint *params))
_COREGL_SYMBOL(void, glGetTexParameterIiv, (GLenum target, GLenum pname,
			   GLint *params))
_COREGL_SYMBOL(void, glGetTexParameterIuiv, (GLenum target, GLenum pname,
			   GLuint *params))
_COREGL_SYMBOL(void, glSamplerParameterIiv, (GLuint sampler, GLenum pname,
			   const GLint *param))
_COREGL_SYMBOL(void, glSamplerParameterIuiv, (GLuint sampler, GLenum pname,
			   const GLuint *param))
_COREGL_SYMBOL(void, glGetSamplerParameterIiv, (GLuint sampler, GLenum pname,
			   GLint *params))
_COREGL_SYMBOL(void, glGetSamplerParameterIuiv, (GLuint sampler, GLenum pname,
			   GLuint *params))
_COREGL_SYMBOL(void, glTexBuffer, (GLenum target, GLenum internalformat,
								   GLuint buffer))
_COREGL_SYMBOL(void, glTexBufferRange, (GLenum target, GLenum internalformat,
										GLuint buffer,
										GLintptr offset, GLsizeiptr size))
_COREGL_SYMBOL(void, glTexStorage3DMultisample, (GLenum target, GLsizei samples,
			   GLenum internalformat,
			   GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations))
_COREGL_END_API(COREGL_GLAPI_32)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_texture_type_2_10_10_10_REV", 2.0,
									 -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_required_internalformat", 2.0, -1)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_EGL_image", 1.1, -1)
_COREGL_EXT_SYMBOL(void, glEGLImageTargetTexture2DOES, (GLenum target,
				   GLeglImageOES image))
_COREGL_EXT_SYMBOL(void, glEGLImageTargetRenderbufferStorageOES, (GLenum target,
				   GLeglImageOES image))
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_EGL_image_external", 1.1, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_surfaceless_context", 1.1, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_fixed_point", 1.1, -1)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_get_program_binary", 2.0, -1)
_COREGL_EXT_SYMBOL(void, glGetProgramBinaryOES, (GLuint program,
				   GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary))
_COREGL_EXT_SYMBOL(void, glProgramBinaryOES, (GLuint program,
				   GLenum binaryFormat, const GLvoid *binary, GLsizei length))

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_AMD_program_binary_Z400", 2.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_IMG_program_binary ", 2.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_ARM_mali_program_binary", 2.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_ANGLE_program_binary", 2.0, -1)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_separate_shader_objects", 2.0, -1)
_COREGL_EXT_SYMBOL(void, glUseProgramStagesEXT, (GLuint pipeline,
				   GLbitfield stages, GLuint program))
_COREGL_EXT_SYMBOL(void, glActiveShaderProgramEXT, (GLuint pipeline,
				   GLuint program))
_COREGL_EXT_SYMBOL(GLuint, glCreateShaderProgramvEXT, (GLenum type,
				   GLsizei count, const char **strings))
_COREGL_EXT_SYMBOL(void, glBindProgramPipelineEXT, (GLuint pipeline))
_COREGL_EXT_SYMBOL(void, glDeleteProgramPipelinesEXT, (GLsizei n,
				   const GLuint *pipelines))
_COREGL_EXT_SYMBOL(void, glGenProgramPipelinesEXT, (GLsizei n,
				   GLuint *pipelines))
_COREGL_EXT_SYMBOL(GLboolean, glIsProgramPipelineEXT, (GLuint pipeline))
_COREGL_EXT_SYMBOL(void, glProgramParameteriEXT, (GLuint program, GLenum pname,
				   GLint value))
_COREGL_EXT_SYMBOL(void, glGetProgramPipelineivEXT, (GLuint pipeline,
				   GLenum pname, GLint *params))
_COREGL_EXT_SYMBOL(void, glProgramUniform1iEXT, (GLuint program, GLint location,
				   GLint x))
_COREGL_EXT_SYMBOL(void, glProgramUniform2iEXT, (GLuint program, GLint location,
				   GLint x, GLint y))
_COREGL_EXT_SYMBOL(void, glProgramUniform3iEXT, (GLuint program, GLint location,
				   GLint x, GLint y, GLint z))
_COREGL_EXT_SYMBOL(void, glProgramUniform4iEXT, (GLuint program, GLint location,
				   GLint x, GLint y, GLint z, GLint w))
_COREGL_EXT_SYMBOL(void, glProgramUniform1fEXT, (GLuint program, GLint location,
				   GLfloat x))
_COREGL_EXT_SYMBOL(void, glProgramUniform2fEXT, (GLuint program, GLint location,
				   GLfloat x, GLfloat y))
_COREGL_EXT_SYMBOL(void, glProgramUniform3fEXT, (GLuint program, GLint location,
				   GLfloat x, GLfloat y, GLfloat z))
_COREGL_EXT_SYMBOL(void, glProgramUniform4fEXT, (GLuint program, GLint location,
				   GLfloat x, GLfloat y, GLfloat z, GLfloat w))
_COREGL_EXT_SYMBOL(void, glProgramUniform1ivEXT, (GLuint program,
				   GLint location, GLsizei count, const GLint *value))
_COREGL_EXT_SYMBOL(void, glProgramUniform2ivEXT, (GLuint program,
				   GLint location, GLsizei count, const GLint *value))
_COREGL_EXT_SYMBOL(void, glProgramUniform3ivEXT, (GLuint program,
				   GLint location, GLsizei count, const GLint *value))
_COREGL_EXT_SYMBOL(void, glProgramUniform4ivEXT, (GLuint program,
				   GLint location, GLsizei count, const GLint *value))
_COREGL_EXT_SYMBOL(void, glProgramUniform1fvEXT, (GLuint program,
				   GLint location, GLsizei count, const GLfloat *value))
_COREGL_EXT_SYMBOL(void, glProgramUniform2fvEXT, (GLuint program,
				   GLint location, GLsizei count, const GLfloat *value))
_COREGL_EXT_SYMBOL(void, glProgramUniform3fvEXT, (GLuint program,
				   GLint location, GLsizei count, const GLfloat *value))
_COREGL_EXT_SYMBOL(void, glProgramUniform4fvEXT, (GLuint program,
				   GLint location, GLsizei count, const GLfloat *value))
_COREGL_EXT_SYMBOL(void, glProgramUniformMatrix2fvEXT, (GLuint program,
				   GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))
_COREGL_EXT_SYMBOL(void, glProgramUniformMatrix3fvEXT, (GLuint program,
				   GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))
_COREGL_EXT_SYMBOL(void, glProgramUniformMatrix4fvEXT, (GLuint program,
				   GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))
_COREGL_EXT_SYMBOL(void, glValidateProgramPipelineEXT, (GLuint pipeline))
_COREGL_EXT_SYMBOL(void, glGetProgramPipelineInfoLogEXT, (GLuint pipeline,
				   GLsizei bufSize, GLsizei *length, char *infoLog))

_COREGL_EXT_SYMBOL(void, glGetQueryObjecti64vEXT, (GLuint id, GLenum pname,
				   GLint64 *params))
_COREGL_EXT_SYMBOL(void, glGetQueryObjectivEXT, (GLuint id, GLenum pname,
				   GLint *params))
_COREGL_EXT_SYMBOL(void, glGetQueryObjectui64vEXT, (GLuint id, GLenum pname,
				   GLuint64 *params))
_COREGL_EXT_SYMBOL(void, glGetQueryObjectuivEXT, (GLuint id, GLenum pname,
				   GLuint *params))
_COREGL_EXT_SYMBOL(void, glGetQueryivEXT, (GLenum target, GLenum pname,
				   GLint *params))
_COREGL_EXT_SYMBOL(void, glBeginQueryEXT, (GLenum target, GLuint id))
_COREGL_EXT_SYMBOL(void, glDeleteQueriesEXT, (GLsizei n, const GLuint *ids))
_COREGL_EXT_SYMBOL(void, glGenQueriesEXT, (GLsizei n, GLuint *ids))
_COREGL_EXT_SYMBOL(GLboolean, glIsQueryEXT, (GLuint id))
_COREGL_EXT_SYMBOL(void, glEndQueryEXT, (GLenum target))

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_texture_3D", 2.0, -1)
_COREGL_EXT_SYMBOL(void, glTexImage3DOES, (GLenum target, GLint level,
				   GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth,
				   GLint border, GLenum format, GLenum type, const void *pixels))
_COREGL_EXT_SYMBOL(void, glTexSubImage3DOES, (GLenum target, GLint level,
				   GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height,
				   GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels))
_COREGL_EXT_SYMBOL(void, glCopyTexSubImage3DOES, (GLenum target, GLint level,
				   GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width,
				   GLsizei height))
_COREGL_EXT_SYMBOL(void, glCompressedTexImage3DOES, (GLenum target, GLint level,
				   GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth,
				   GLint border, GLsizei imageSize, const GLvoid *data))
_COREGL_EXT_SYMBOL(void, glCompressedTexSubImage3DOES, (GLenum target,
				   GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width,
				   GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize,
				   const GLvoid *data))

/*
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_ANGLE_framebuffer_blit", 1.1, -1)
_COREGL_EXT_SYMBOL(void, glBlitFramebufferANGLE, (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter))

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_ANGLE_framebuffer_multisample", 1.1, -1)
_COREGL_EXT_SYMBOL(void, glRenderbufferStorageMultisampleANGLE, (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height))
*/


/* Fastpath Verified extensions */
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_IMG_shader_binary", -1, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_ARM_mali_shader_binary", -1, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_VIV_shader_binary", -1, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_DMP_shader_binary", -1, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_FJ_shader_binary_GCCSO", -1, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_ARM_mali_shader_binary", -1, -1)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_compressed_ETC1_RGB8_texture", -1,
									 -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_compressed_paletted_texture", -1,
									 -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_AMD_compressed_3DC_texture", -1, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_AMD_compressed_ATC_texture", -1, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_texture_compression_dxt1", -1, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_IMG_texture_compression_pvrtc", -1, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_NV_texture_compression_s3tc_update",
									 -1, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_ANGLE_texture_compression_dxt3", -1,
									 -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_ANGLE_texture_compression_dxt5", -1,
									 -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_KHR_texture_compression_astc_ldr", -1,
									 -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_NV_texture_compression_s3tc", -1, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_NV_texture_compression_latc", -1, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_IMG_texture_compression_pvrtc2", -1,
									 -1)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_texture_type_2_10_10_10_REV", -1,
									 -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_texture_format_BGRA8888", -1, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_APPLE_texture_format_BGRA8888", 1.0,
									 -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_texture_half_float_linear", 2.0,
									 -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_texture_float_linear", 2.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_texture_half_float", 2.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_texture_float", 2.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_texture_npot", 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_IMG_texture_env_enhanced_fixed_function",
									 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_APPLE_texture_max_level", 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_NV_texture_npot_2D_mipmap", 2.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_ANGLE_texture_usage", 2.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_texture_rg", 2.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_sRGB", 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_texture_sRGB_decode", 2.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_texture_filter_anisotropic", 1.2,
									 -1)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_standard_derivatives", 2.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_depth24", 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_ARM_rgba8", 1.1, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_depth_texture", 2.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_packed_depth_stencil", 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_texture_format_BGRA8888", 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_blend_minmax", -1, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_EGL_sync", -1, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_rgb8_rgba8", 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_shader_texture_lod", 2.0, -1)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_element_index_uint", 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_fbo_render_mipmap", 1.0, -1)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_depth_texture_cube_map", 2.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_vertex_half_float", 2.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_vertex_type_10_10_10_2", 2.0, -1)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_color_buffer_half_float", 2.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_color_buffer_float", 3.0, -1)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_element_index_uint", 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_element_index_uint", 1.0, -1)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_debug_marker", 1.1, -1)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_geometry_shader", 2.0, -1)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glFramebufferTextureEXT)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_sample_shading", 2.0, -1)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glMinSampleShadingOES)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_tessellation_shader", 2.0, -1)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glPatchParameteriEXT)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_texture_border_clamp", 2.0, -1)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glTexParameterIivEXT)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glTexParameterIuivEXT)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetTexParameterIivEXT)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetTexParameterIuivEXT)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glSamplerParameterIivEXT)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glSamplerParameterIuivEXT)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetSamplerParameterIivEXT)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetSamplerParameterIuivEXT)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_texture_buffer", 2.0, -1)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glTexBufferEXT)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glTexBufferRangeEXT)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_geometry_shader", 2.0, -1)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glFramebufferTextureOES)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_tessellation_shader", 2.0, -1)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glPatchParameteriOES)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_texture_border_clamp", 2.0, -1)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glTexParameterIivOES)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glTexParameterIuivOES)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetTexParameterIivOES)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetTexParameterIuivOES)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glSamplerParameterIivOES)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glSamplerParameterIuivOES)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetSamplerParameterIivOES)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetSamplerParameterIuivOES)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_texture_buffer", 2.0, -1)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glTexBufferRangeOES)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_NV_internalformat_sample_query", 2.0,
									 -1)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetInternalformatSampleivNV)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_draw_buffers_indexed", 2.0, -1)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glEnableiOES)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glDisableiOES)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glBlendEquationiOES)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glBlendEquationSeparateiOES)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glBlendFunciOES)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glBlendFuncSeparateiOES)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glColorMaskiOES)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glIsEnablediOES)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_draw_buffers_indexed", 2.0, -1)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glEnableiEXT)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glDisableiEXT)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glBlendEquationiEXT)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glBlendEquationSeparateiEXT)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glBlendFunciEXT)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glBlendFuncSeparateiEXT)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glColorMaskiEXT)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glIsEnablediEXT)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_AMD_performance_monitor", -1, -1)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetPerfMonitorGroupsAMD)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetPerfMonitorCountersAMD)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetPerfMonitorGroupStringAMD)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetPerfMonitorCounterStringAMD)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetPerfMonitorCounterInfoAMD)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGenPerfMonitorsAMD)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glDeletePerfMonitorsAMD)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glSelectPerfMonitorCountersAMD)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glBeginPerfMonitorAMD)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glEndPerfMonitorAMD)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetPerfMonitorCounterDataAMD)

_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetTexLevelParameterfv)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetTexLevelParameteriv)

/* Fastpath Blocked extensions */

//_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_robustness", 1.1, -1)
_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glGetGraphicsResetStatusEXT)
_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glReadnPixelsEXT)
_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glGetnUniformfvEXT)
_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glGetnUniformivEXT)

//_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_debug_label", 1.1, -1)
_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glLabelObjectEXT)
_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glGetObjectLabelEXT)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_disjoint_timer_query", 2.0, -1)
//_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glGenQueriesEXT)
//_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glDeleteQueriesEXT)
//_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glIsQueryEXT)
//_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glBeginQueryEXT)
//_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glEndQueryEXT)
//_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glQueryCounterEXT)
//_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glGetQueryivEXT)
//_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glGetQueryObjectivEXT)
//_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glGetQueryObjectuivEXT)
//_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glGetQueryObjecti64vEXT)
//_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glGetQueryObjectui64vEXT)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_vertex_array_object", 1.1, -1)
//_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glBindVertexArrayOES)
//_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glDeleteVertexArraysOES)
//_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glGenVertexArraysOES)
//_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glIsVertexArrayOES)

//_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_QCOM_alpha_test", 2.0, -1)

//_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_QCOM_binning_control", 1.0, -1)

//_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_QCOM_perfmon_global_mode", -1, -1)

//_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_QCOM_writeonly_rendering", 1.0, -1)

// ES3 Invalid usages
_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glBlitFramebuffer)

#ifdef _COREGL_EXT_SYMBOL_NOT_DEFINED
#undef _COREGL_EXT_SYMBOL_NOT_DEFINED
#undef _COREGL_EXT_SYMBOL
#endif

#ifdef _COREGL_SYMBOL_NOT_DEFINED
#undef _COREGL_SYMBOL_NOT_DEFINED
#undef _COREGL_SYMBOL
#endif

#ifdef _COREGL_EXT_SYMBOL_ALIAS_NOT_DEFINED
#undef _COREGL_EXT_SYMBOL_ALIAS_NOT_DEFINED
#undef _COREGL_EXT_SYMBOL_ALIAS
#endif

#ifdef _COREGL_FASTPATH_SUPPORTED_EXTENSION_NOT_DEFINED
#undef _COREGL_FASTPATH_SUPPORTED_EXTENSION_NOT_DEFINED
#undef _COREGL_FASTPATH_SUPPORTED_EXTENSION
#endif

#ifdef _COREGL_EXT_SYMBOL_FASTPATH_PASS_NOT_DEFINED
#undef _COREGL_EXT_SYMBOL_FASTPATH_PASS_NOT_DEFINED
#undef _COREGL_EXT_SYMBOL_FASTPATH_PASS
#endif

#ifdef _COREGL_EXT_SYMBOL_FASTPATH_BLOCK_NOT_DEFINED
#undef _COREGL_EXT_SYMBOL_FASTPATH_BLOCK_NOT_DEFINED
#undef _COREGL_EXT_SYMBOL_FASTPATH_BLOCK
#endif

#ifdef _COREGL_START_API_DEFINED_INSIDE
#undef _COREGL_START_API
#undef _COREGL_START_API_DEFINED_INSIDE
#endif

#ifdef _COREGL_END_API_DEFINED_INSIDE
#undef _COREGL_END_API
#undef _COREGL_END_API_DEFINED_INSIDE
#endif
