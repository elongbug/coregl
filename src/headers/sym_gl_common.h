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

_COREGL_START_API(COREGL_GLAPI_1_2_COMMON)
_COREGL_SYMBOL(void, glActiveTexture, (GLenum texture))
_COREGL_SYMBOL(void, glClearColor, (GLclampf red, GLclampf green, GLclampf blue,
									GLclampf alpha))
_COREGL_SYMBOL(void, glClearDepthf, (GLclampf depth))
_COREGL_SYMBOL(void, glDepthRangef, (GLclampf zNear, GLclampf zFar))
_COREGL_SYMBOL(void, glGetFloatv, (GLenum pname, GLfloat *params))
_COREGL_SYMBOL(void, glGetTexParameterfv, (GLenum target, GLenum pname,
			   GLfloat *params))
_COREGL_SYMBOL(void, glLineWidth, (GLfloat width))
_COREGL_SYMBOL(void, glPolygonOffset, (GLfloat factor, GLfloat units))
_COREGL_SYMBOL(void, glTexParameterf, (GLenum target, GLenum pname,
									   GLfloat param))
_COREGL_SYMBOL(void, glTexParameterfv, (GLenum target, GLenum pname,
										const GLfloat *params))
_COREGL_SYMBOL(void, glBindBuffer, (GLenum target, GLuint buffer))
_COREGL_SYMBOL(void, glBindTexture, (GLenum target, GLuint texture))
_COREGL_SYMBOL(void, glBlendFunc, (GLenum sfactor, GLenum dfactor))
_COREGL_SYMBOL(void, glBufferData, (GLenum target, GLsizeiptr size,
									const void *data, GLenum usage))
_COREGL_SYMBOL(void, glBufferSubData, (GLenum target, GLintptr offset,
									   GLsizeiptr size, const void *data))
_COREGL_SYMBOL(void, glClear, (GLbitfield mask))
_COREGL_SYMBOL(void, glClearStencil, (GLint s))
_COREGL_SYMBOL(void, glColorMask, (GLboolean red, GLboolean green,
								   GLboolean blue, GLboolean alpha))
_COREGL_SYMBOL(void, glCompressedTexImage2D, (GLenum target, GLint level,
			   GLenum internalformat, GLsizei width, GLsizei height, GLint border,
			   GLsizei imageSize, const void *data))
_COREGL_SYMBOL(void, glCompressedTexSubImage2D, (GLenum target, GLint level,
			   GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format,
			   GLsizei imageSize, const void *data))
_COREGL_SYMBOL(void, glCopyTexImage2D, (GLenum target, GLint level,
										GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height,
										GLint border))
_COREGL_SYMBOL(void, glCopyTexSubImage2D, (GLenum target, GLint level,
			   GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height))
_COREGL_SYMBOL(void, glCullFace, (GLenum mode))
_COREGL_SYMBOL(void, glDeleteBuffers, (GLsizei n, const GLuint *buffers))
_COREGL_SYMBOL(void, glDeleteTextures, (GLsizei n, const GLuint *textures))
_COREGL_SYMBOL(void, glDepthFunc, (GLenum func))
_COREGL_SYMBOL(void, glDepthMask, (GLboolean flag))
_COREGL_SYMBOL(void, glDisable, (GLenum cap))
_COREGL_SYMBOL(void, glDrawArrays, (GLenum mode, GLint first, GLsizei count))
_COREGL_SYMBOL(void, glDrawElements, (GLenum mode, GLsizei count, GLenum type,
									  const void *indices))
_COREGL_SYMBOL(void, glEnable, (GLenum cap))
_COREGL_SYMBOL(void, glFinish, (void))
_COREGL_SYMBOL(void, glFlush, (void))
_COREGL_SYMBOL(void, glFrontFace, (GLenum mode))
_COREGL_SYMBOL(void, glGetBooleanv, (GLenum pname, GLboolean *params))
_COREGL_SYMBOL(void, glGetBufferParameteriv, (GLenum target, GLenum pname,
			   GLint *params))
_COREGL_SYMBOL(void, glGenBuffers, (GLsizei n, GLuint *buffers))
_COREGL_SYMBOL(void, glGenTextures, (GLsizei n, GLuint *textures))
_COREGL_SYMBOL(GLenum, glGetError, (void))
_COREGL_SYMBOL(void, glGetIntegerv, (GLenum pname, GLint *params))
_COREGL_SYMBOL(void, glGetPointerv, (GLenum pname, GLvoid **params))
_COREGL_SYMBOL(const GLubyte *, glGetString, (GLenum name))
_COREGL_SYMBOL(void, glGetTexParameteriv, (GLenum target, GLenum pname,
			   GLint *params))
_COREGL_SYMBOL(void, glHint, (GLenum target, GLenum mode))
_COREGL_SYMBOL(GLboolean, glIsBuffer, (GLuint buffer))
_COREGL_SYMBOL(GLboolean, glIsEnabled, (GLenum cap))
_COREGL_SYMBOL(GLboolean, glIsTexture, (GLuint texture))
_COREGL_SYMBOL(void, glPixelStorei, (GLenum pname, GLint param))
_COREGL_SYMBOL(void, glReadPixels, (GLint x, GLint y, GLsizei width,
									GLsizei height, GLenum format, GLenum type, void *pixels))
_COREGL_SYMBOL(void, glSampleCoverage, (GLclampf value, GLboolean invert))
_COREGL_SYMBOL(void, glScissor, (GLint x, GLint y, GLsizei width,
								 GLsizei height))
_COREGL_SYMBOL(void, glStencilFunc, (GLenum func, GLint ref, GLuint mask))
_COREGL_SYMBOL(void, glStencilMask, (GLuint mask))
_COREGL_SYMBOL(void, glStencilOp, (GLenum fail, GLenum zfail, GLenum zpass))
_COREGL_SYMBOL(void, glTexImage2D, (GLenum target, GLint level,
									GLint internalformat, GLsizei width, GLsizei height, GLint border,
									GLenum format, GLenum type, const void *pixels))
_COREGL_SYMBOL(void, glTexParameteri, (GLenum target, GLenum pname,
									   GLint param))
_COREGL_SYMBOL(void, glTexParameteriv, (GLenum target, GLenum pname,
										const GLint *params))
_COREGL_SYMBOL(void, glTexSubImage2D, (GLenum target, GLint level,
									   GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format,
									   GLenum type, const void *pixels))
_COREGL_SYMBOL(void, glViewport, (GLint x, GLint y, GLsizei width,
								  GLsizei height))

/* extensions */
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_mapbuffer", 1.1, -1)
_COREGL_EXT_SYMBOL(void, glGetBufferPointervOES, (GLenum target, GLenum pname,
				   GLvoid **params))
_COREGL_EXT_SYMBOL(void *, glMapBufferOES, (GLenum target, GLenum access))
_COREGL_EXT_SYMBOL(GLboolean, glUnmapBufferOES, (GLenum target))

_COREGL_EXT_SYMBOL(void, glBindVertexArrayOES, (GLuint array))
_COREGL_EXT_SYMBOL(void, glDeleteVertexArraysOES, (GLsizei n,
				   GLuint const *arrays))
_COREGL_EXT_SYMBOL(void, glGenVertexArraysOES, (GLsizei n, GLuint *arrays))
_COREGL_EXT_SYMBOL(GLboolean, glIsVertexArrayOES, (GLuint array))
_COREGL_EXT_SYMBOL(void, glRenderbufferStorageMultisampleAPPLE, (GLenum,
				   GLsizei, GLenum, GLsizei, GLsizei))
_COREGL_EXT_SYMBOL(void, glResolveMultisampleFramebufferAPPLE, (void))

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_discard_framebuffer", 1.0, -1)
_COREGL_EXT_SYMBOL(void, glDiscardFramebufferEXT, (GLenum target,
				   GLsizei numAttachments, const GLenum *attachments))
_COREGL_EXT_SYMBOL(void, glMultiDrawArraysEXT, (GLenum mode, const GLint *first,
				   const GLsizei *count, GLsizei primcount))
_COREGL_EXT_SYMBOL(void, glMultiDrawElementsEXT, (GLenum mode,
				   const GLsizei *count, GLenum type,
				   const void *const *indices, GLsizei primcount))

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_multisampled_render_to_texture",
									 2.0, -1)
_COREGL_EXT_SYMBOL(void, glRenderbufferStorageMultisampleEXT, (GLenum target,
				   GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height))
_COREGL_EXT_SYMBOL(void, glFramebufferTexture2DMultisampleEXT, (GLenum target,
				   GLenum attachment, GLenum textarget, GLuint texture, GLint level,
				   GLsizei samples))
_COREGL_EXT_SYMBOL(void, glRenderbufferStorageMultisampleIMG, (GLenum, GLsizei,
				   GLenum, GLsizei, GLsizei))
_COREGL_EXT_SYMBOL(void, glFramebufferTexture2DMultisampleIMG, (GLenum, GLenum,
				   GLenum, GLuint, GLint, GLsizei))

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_NV_fence", -1, -1)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glDeleteFencesNV)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGenFencesNV)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glIsFenceNV)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glTestFenceNV)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetFenceivNV)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glFinishFenceNV)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glSetFenceNV)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_QCOM_driver_control", 1.0, -1)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetDriverControlsQCOM)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glGetDriverControlStringQCOM)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glEnableDriverControlQCOM)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glDisableDriverControlQCOM)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_QCOM_tiled_rendering", 1.0, -1)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glStartTilingQCOM)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glEndTilingQCOM)

// QCOM_extended_get
//_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_QCOM_extended_get", 1.0, -1)
_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glExtGetTexturesQCOM)
_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glExtGetBuffersQCOM)
_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glExtGetRenderbuffersQCOM)
_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glExtGetFramebuffersQCOM)
_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glExtGetTexLevelParameterivQCOM)
_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glExtTexObjectStateOverrideiQCOM)
_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glExtGetTexSubImageQCOM)
_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glExtGetBufferPointervQCOM)

// QCOM_extended_get2
//_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_QCOM_extended_get2", 1.0, -1)
_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glExtGetShadersQCOM)
_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glExtGetProgramsQCOM)
_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glExtIsProgramBinaryQCOM)
_COREGL_EXT_SYMBOL_FASTPATH_BLOCK(glExtGetProgramBinarySourceQCOM)
_COREGL_END_API(COREGL_GLAPI_1_2_COMMON)

_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_texture_storage_multisample_2d_array",
									 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_sample_variables", 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_shader_multisample_interpolation",
									 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_KHR_blend_equation_advanced", 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_KHR_blend_equation_advanced_coherent",
									 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_shader_image_atomic", 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_gpu_shader5", 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_gpu_shader5", 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_OES_texture_cube_map_array", 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_texture_cube_map_array", 1.0, -1)
_COREGL_FASTPATH_SUPPORTED_EXTENSION("GL_EXT_draw_elements_base_vertex", 1.0,
									 -1)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glDrawElementsBaseVertexEXT)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glDrawElementsInstancedBaseVertexEXT)
_COREGL_EXT_SYMBOL_FASTPATH_PASS(glDrawRangeElementsBaseVertexEXT)

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
