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

_COREGL_START_API(COREGL_GLAPI_1)
_COREGL_SYMBOL(void, glAlphaFunc, (GLenum func, GLclampf ref))
_COREGL_SYMBOL(void, glClipPlanef, (GLenum plane, const GLfloat *equation))
_COREGL_SYMBOL(void, glColor4f, (GLfloat red, GLfloat green, GLfloat blue,
								 GLfloat alpha))
_COREGL_SYMBOL(void, glFogf, (GLenum pname, GLfloat param))
_COREGL_SYMBOL(void, glFogfv, (GLenum pname, const GLfloat *params))
_COREGL_SYMBOL(void, glFrustumf, (GLfloat left, GLfloat right, GLfloat bottom,
								  GLfloat top, GLfloat zNear, GLfloat zFar))
_COREGL_SYMBOL(void, glGetClipPlanef, (GLenum pname, GLfloat eqn[4]))
_COREGL_SYMBOL(void, glGetLightfv, (GLenum light, GLenum pname,
									GLfloat *params))
_COREGL_SYMBOL(void, glGetMaterialfv, (GLenum face, GLenum pname,
									   GLfloat *params))
_COREGL_SYMBOL(void, glGetTexEnvfv, (GLenum env, GLenum pname, GLfloat *params))
_COREGL_SYMBOL(void, glLightModelf, (GLenum pname, GLfloat param))
_COREGL_SYMBOL(void, glLightModelfv, (GLenum pname, const GLfloat *params))
_COREGL_SYMBOL(void, glLightf, (GLenum light, GLenum pname, GLfloat param))
_COREGL_SYMBOL(void, glLightfv, (GLenum light, GLenum pname,
								 const GLfloat *params))
_COREGL_SYMBOL(void, glLoadMatrixf, (const GLfloat *m))
_COREGL_SYMBOL(void, glMaterialf, (GLenum face, GLenum pname, GLfloat param))
_COREGL_SYMBOL(void, glMaterialfv, (GLenum face, GLenum pname,
									const GLfloat *params))
_COREGL_SYMBOL(void, glMultMatrixf, (const GLfloat *m))
_COREGL_SYMBOL(void, glMultiTexCoord4f, (GLenum target, GLfloat s, GLfloat t,
			   GLfloat r, GLfloat q))
_COREGL_SYMBOL(void, glNormal3f, (GLfloat nx, GLfloat ny, GLfloat nz))
_COREGL_SYMBOL(void, glOrthof, (GLfloat left, GLfloat right, GLfloat bottom,
								GLfloat top, GLfloat zNear, GLfloat zFar))
_COREGL_SYMBOL(void, glPointParameterf, (GLenum pname, GLfloat param))
_COREGL_SYMBOL(void, glPointParameterfv, (GLenum pname, const GLfloat *params))
_COREGL_SYMBOL(void, glPointSize, (GLfloat size))
_COREGL_SYMBOL(void, glRotatef, (GLfloat angle, GLfloat x, GLfloat y,
								 GLfloat z))
_COREGL_SYMBOL(void, glScalef, (GLfloat x, GLfloat y, GLfloat z))
_COREGL_SYMBOL(void, glTexEnvf, (GLenum target, GLenum pname, GLfloat param))
_COREGL_SYMBOL(void, glTexEnvfv, (GLenum target, GLenum pname,
								  const GLfloat *params))
_COREGL_SYMBOL(void, glTranslatef, (GLfloat x, GLfloat y, GLfloat z))
_COREGL_SYMBOL(void, glAlphaFuncx, (GLenum func, GLclampx ref))
_COREGL_SYMBOL(void, glClearColorx, (GLclampx red, GLclampx green,
									 GLclampx blue, GLclampx alpha))
_COREGL_SYMBOL(void, glClearDepthx, (GLclampx depth))
_COREGL_SYMBOL(void, glClientActiveTexture, (GLenum texture))
_COREGL_SYMBOL(void, glClipPlanex, (GLenum plane, const GLfixed *equation))
_COREGL_SYMBOL(void, glColor4ub, (GLubyte red, GLubyte green, GLubyte blue,
								  GLubyte alpha))
_COREGL_SYMBOL(void, glColor4x, (GLfixed red, GLfixed green, GLfixed blue,
								 GLfixed alpha))
_COREGL_SYMBOL(void, glColorPointer, (GLint size, GLenum type, GLsizei stride,
									  const GLvoid *pointer))
_COREGL_SYMBOL(void, glDepthRangex, (GLclampx zNear, GLclampx zFar))
_COREGL_SYMBOL(void, glDisableClientState, (GLenum array))
_COREGL_SYMBOL(void, glEnableClientState, (GLenum array))
_COREGL_SYMBOL(void, glFogx, (GLenum pname, GLfixed param))
_COREGL_SYMBOL(void, glFogxv, (GLenum pname, const GLfixed *params))
_COREGL_SYMBOL(void, glFrustumx, (GLfixed left, GLfixed right, GLfixed bottom,
								  GLfixed top, GLfixed zNear, GLfixed zFar))
_COREGL_SYMBOL(void, glGetClipPlanex, (GLenum pname, GLfixed eqn[4]))
_COREGL_SYMBOL(void, glGetFixedv, (GLenum pname, GLfixed *params))
_COREGL_SYMBOL(void, glGetLightxv, (GLenum light, GLenum pname,
									GLfixed *params))
_COREGL_SYMBOL(void, glGetMaterialxv, (GLenum face, GLenum pname,
									   GLfixed *params))
_COREGL_SYMBOL(void, glGetTexEnviv, (GLenum env, GLenum pname, GLint *params))
_COREGL_SYMBOL(void, glGetTexEnvxv, (GLenum env, GLenum pname, GLfixed *params))
_COREGL_SYMBOL(void, glGetTexParameterxv, (GLenum target, GLenum pname,
			   GLfixed *params))
_COREGL_SYMBOL(void, glLightModelx, (GLenum pname, GLfixed param))
_COREGL_SYMBOL(void, glLightModelxv, (GLenum pname, const GLfixed *params))
_COREGL_SYMBOL(void, glLightx, (GLenum light, GLenum pname, GLfixed param))
_COREGL_SYMBOL(void, glLightxv, (GLenum light, GLenum pname,
								 const GLfixed *params))
_COREGL_SYMBOL(void, glLineWidthx, (GLfixed width))
_COREGL_SYMBOL(void, glLoadIdentity, (void))
_COREGL_SYMBOL(void, glLoadMatrixx, (const GLfixed *m))
_COREGL_SYMBOL(void, glLogicOp, (GLenum opcode))
_COREGL_SYMBOL(void, glMaterialx, (GLenum face, GLenum pname, GLfixed param))
_COREGL_SYMBOL(void, glMaterialxv, (GLenum face, GLenum pname,
									const GLfixed *params))
_COREGL_SYMBOL(void, glMatrixMode, (GLenum mode))
_COREGL_SYMBOL(void, glMultMatrixx, (const GLfixed *m))
_COREGL_SYMBOL(void, glMultiTexCoord4x, (GLenum target, GLfixed s, GLfixed t,
			   GLfixed r, GLfixed q))
_COREGL_SYMBOL(void, glNormal3x, (GLfixed nx, GLfixed ny, GLfixed nz))
_COREGL_SYMBOL(void, glNormalPointer, (GLenum type, GLsizei stride,
									   const GLvoid *pointer))
_COREGL_SYMBOL(void, glOrthox, (GLfixed left, GLfixed right, GLfixed bottom,
								GLfixed top, GLfixed zNear, GLfixed zFar))
_COREGL_SYMBOL(void, glPointParameterx, (GLenum pname, GLfixed param))
_COREGL_SYMBOL(void, glPointParameterxv, (GLenum pname, const GLfixed *params))
_COREGL_SYMBOL(void, glPointSizex, (GLfixed size))
_COREGL_SYMBOL(void, glPolygonOffsetx, (GLfixed factor, GLfixed units))
_COREGL_SYMBOL(void, glPopMatrix, (void))
_COREGL_SYMBOL(void, glPushMatrix, (void))
_COREGL_SYMBOL(void, glRotatex, (GLfixed angle, GLfixed x, GLfixed y,
								 GLfixed z) )
_COREGL_SYMBOL(void, glSampleCoveragex, (GLclampx value, GLboolean invert))
_COREGL_SYMBOL(void, glScalex, (GLfixed x, GLfixed y, GLfixed z))
_COREGL_SYMBOL(void, glShadeModel, (GLenum mode))
_COREGL_SYMBOL(void, glTexCoordPointer, (GLint size, GLenum type,
			   GLsizei stride, const GLvoid *pointer))
_COREGL_SYMBOL(void, glTexEnvi, (GLenum target, GLenum pname, GLint param))
_COREGL_SYMBOL(void, glTexEnvx, (GLenum target, GLenum pname, GLfixed param))
_COREGL_SYMBOL(void, glTexEnviv, (GLenum target, GLenum pname,
								  const GLint *params))
_COREGL_SYMBOL(void, glTexEnvxv, (GLenum target, GLenum pname,
								  const GLfixed *params))
_COREGL_SYMBOL(void, glTexParameterx, (GLenum target, GLenum pname,
									   GLfixed param))
_COREGL_SYMBOL(void, glTexParameterxv, (GLenum target, GLenum pname,
										const GLfixed *params))
_COREGL_SYMBOL(void, glTranslatex, (GLfixed x, GLfixed y, GLfixed z))
_COREGL_SYMBOL(void, glVertexPointer, (GLint size, GLenum type, GLsizei stride,
									   const GLvoid *pointer))
/* Khronos GLES 1.1 CTS requires glPointSizePointerOES as shared library symbol.*/
_COREGL_SYMBOL(void, glPointSizePointerOES, (GLenum type, GLsizei stride,
			   const GLvoid *pointer))

//OpenGL ES 1.1 extensions
_COREGL_EXT_SYMBOL(void, glDrawTexsOES, (GLshort x, GLshort y, GLshort z,
				   GLshort width, GLshort height))
_COREGL_EXT_SYMBOL(void, glDrawTexiOES, (GLint x, GLint y, GLint z, GLint width,
				   GLint height))
_COREGL_EXT_SYMBOL(void, glDrawTexxOES, (GLfixed x, GLfixed y, GLfixed z,
				   GLfixed width, GLfixed height))
_COREGL_EXT_SYMBOL(void, glDrawTexsvOES, (const GLshort *coords))
_COREGL_EXT_SYMBOL(void, glDrawTexivOES, (const GLint *coords))
_COREGL_EXT_SYMBOL(void, glDrawTexxvOES, (const GLfixed *coords))
_COREGL_EXT_SYMBOL(void, glDrawTexfOES, (GLfloat x, GLfloat y, GLfloat z,
				   GLfloat width, GLfloat height))
_COREGL_EXT_SYMBOL(void, glDrawTexfvOES, (const GLfloat *coords))
_COREGL_EXT_SYMBOL(void, glCurrentPaletteMatrixOES, (GLuint matrixpaletteindex))
_COREGL_EXT_SYMBOL(void, glLoadPaletteFromModelViewMatrixOES, (void))
_COREGL_EXT_SYMBOL(void, glMatrixIndexPointerOES, (GLint size, GLenum type,
				   GLsizei stride, const GLvoid *pointer))
_COREGL_EXT_SYMBOL(void, glWeightPointerOES, (GLint size, GLenum type,
				   GLsizei stride, const GLvoid *pointer))
_COREGL_EXT_SYMBOL(GLbitfield, glQueryMatrixxOES, (GLfixed mantissa[16],
				   GLint exponent[16]))
_COREGL_EXT_SYMBOL(void, glTexGenfOES, (GLenum coord, GLenum pname,
										GLfloat param))
_COREGL_EXT_SYMBOL(void, glTexGenfvOES, (GLenum coord, GLenum pname,
				   const GLfloat *params))
_COREGL_EXT_SYMBOL(void, glTexGeniOES, (GLenum coord, GLenum pname,
										GLint param))
_COREGL_EXT_SYMBOL(void, glTexGenivOES, (GLenum coord, GLenum pname,
				   const GLint *params))
_COREGL_EXT_SYMBOL(void, glTexGenxOES, (GLenum coord, GLenum pname,
										GLfixed param))
_COREGL_EXT_SYMBOL(void, glTexGenxvOES, (GLenum coord, GLenum pname,
				   const GLfixed *params))
_COREGL_EXT_SYMBOL(void, glGetTexGenfvOES, (GLenum coord, GLenum pname,
				   GLfloat *params))
_COREGL_EXT_SYMBOL(void, glGetTexGenivOES, (GLenum coord, GLenum pname,
				   GLint *params))
_COREGL_EXT_SYMBOL(void, glGetTexGenxvOES, (GLenum coord, GLenum pname,
				   GLfixed *params))
_COREGL_EXT_SYMBOL(void, glClipPlanefIMG, (GLenum, const GLfloat *))
_COREGL_EXT_SYMBOL(void, glClipPlanexIMG, (GLenum, const GLfixed *))
_COREGL_END_API(COREGL_GLAPI_1)

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
