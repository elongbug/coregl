CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(coregl C)

SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(EXEC_PREFIX "\${prefix}")
SET(INCLUDEDIR "\${prefix}/include_KHR")
SET(COREGL_VERSION_MAJOR 4)
SET(COREGL_VERSION_MINOR 0)
SET(EGL_VERSION_MAJOR 1)
SET(EGL_VERSION_MINOR 4)
SET(GLES1_VERSION_MAJOR 1)
SET(GLES1_VERSION_MINOR 1)
SET(GLES2_VERSION_MAJOR 2)
SET(GLES2_VERSION_MINOR 0)
SET(COREGL_VERSION "${COREGL_VERSION_MAJOR}.${COREGL_VERSION_MINOR}")
SET(EGL_VERSION "${EGL_VERSION_MAJOR}.${EGL_VERSION_MINOR}")
SET(GLES1_VERSION "${GLES1_VERSION_MAJOR}.${GLES1_VERSION_MINOR}")
SET(GLES2_VERSION "${GLES2_VERSION_MAJOR}.${GLES2_VERSION_MINOR}")
SET(BUILD_ARCH "$ENV{RPM_ARCH}")

IF(${BUILD_ARCH} MATCHES "aarch64")
SET(LIB "lib64")
SET(LIBDIR "/usr/lib64")
ELSEIF(${BUILD_ARCH} MATCHES "x86_64")
SET(LIB "lib64")
SET(LIBDIR "/usr/lib64")
ELSE(${BUILD_ARCH} MATCHES "aarch64")
SET(LIB "lib")
SET(LIBDIR "/usr/lib")
ENDIF(${BUILD_ARCH} MATCHES "aarch64")

#################################################################
# Build Package Config File
# ------------------------------
CONFIGURE_FILE(pkgconfig/egl.pc.in pkgconfig/egl.pc @ONLY)
CONFIGURE_FILE(pkgconfig/gles11.pc.in pkgconfig/gles11.pc @ONLY)
CONFIGURE_FILE(pkgconfig/gles20.pc.in pkgconfig/gles20.pc @ONLY)
CONFIGURE_FILE(pkgconfig/glesv1_cm.pc.in pkgconfig/glesv1_cm.pc @ONLY)
CONFIGURE_FILE(pkgconfig/glesv1.pc.in pkgconfig/glesv1.pc @ONLY)
CONFIGURE_FILE(pkgconfig/glesv2.pc.in pkgconfig/glesv2.pc @ONLY)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include_KHR)

ADD_DEFINITIONS("-DPREFIX=\"${PREFIX}\"")

EXEC_PROGRAM(date OUTPUT_VARIABLE COMPILE_DATE)
ADD_DEFINITIONS("-D_COREGL_COMPILE_DATE=\"${COMPILE_DATE}\"")

IF(USE_ADRENO)
ADD_DEFINITIONS("-D_COREGL_VENDOR_EGL_LIB_PATH=\"/usr/lib/egl/libEGL.so\"")
ADD_DEFINITIONS("-D_COREGL_VENDOR_GL_LIB_PATH=\"/usr/lib/egl/libGLESv2.so\"")
ELSE(USE_ADRENO)
ADD_DEFINITIONS("-D_COREGL_VENDOR_EGL_LIB_PATH=\"${LIBDIR}/driver/libEGL.so\"")
ADD_DEFINITIONS("-D_COREGL_VENDOR_GL_LIB_PATH=\"${LIBDIR}/driver/libGLESv2.so\"")
ENDIF(USE_ADRENO)

SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} -fvisibility=hidden")
SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} -g -O2 -fPIC -Wall -std=c99 -D_COREGL_EMBEDDED_GL")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS}")

SET(CMAKE_SKIP_BUILD_RPATH TRUE)

#################################################################
# Build coregl Library
# ------------------------------
SET(COREGL "COREGL")
SET(SRCS_common
		src/coregl.c
		src/coregl_thread_pthread.c
		src/coregl_trace.c
		src/coregl_export.c
		src/coregl_export_egl.c
		src/coregl_export_gl.c
		src/modules/coregl_module.c
		src/modules/tracepath/coregl_tracepath.c
		src/modules/tracepath/coregl_tracepath_egl.c
		src/modules/tracepath/coregl_tracepath_gl.c

		src/modules/fastpath/coregl_fastpath.c
		src/modules/fastpath/coregl_fastpath_egl.c
		src/modules/fastpath/coregl_fastpath_gl.c

		src/modules/appopt/coregl_appopt.c
		src/modules/appopt/coregl_appopt_egl.c
		src/modules/appopt/coregl_appopt_gl.c
)
#################################################################
# Build egl Library
# ------------------------------
SET(EGL "EGL")
SET(EGL_SRCS_common
		src/wraps/coregl_egl.c
)

#################################################################
# Build gles1 Library
# ------------------------------
SET(GLES1 "GLESv1_CM")
SET(GLES1_SRCS_common
		src/wraps/coregl_gl1.c
		src/wraps/coregl_gl_common.c
)

#################################################################
# Build gles2 Library
# ------------------------------
SET(GLES2 "GLESv2")
SET(GLES2_SRCS_common
		src/wraps/coregl_gl2.c
		src/wraps/coregl_gl_common.c
)

#SET(HEADERS_common src/coregl_export.h src/coregl_fastpath.h src/coregl_fastpath_state.h src/coregl.h src/coregl_internal.h src/coregl_thread_pthread.h src/coregl_wrappath.h)

INCLUDE(FindPkgConfig)
pkg_check_modules(dlog REQUIRED dlog)
pkg_check_modules(libpng REQUIRED libpng)

INCLUDE_DIRECTORIES(${dlog_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${dlog_INCLUDEDIR})

FOREACH(flag ${pkg_common_CFLAGS})
	SET(EXTRA_CFLAGS_common "${EXTRA_CFLAGS_common} ${flag} -lGLESv2_drv -lEGL_drv")
ENDFOREACH(flag)

ADD_LIBRARY(${COREGL} SHARED ${SRCS_common})
SET_TARGET_PROPERTIES(${COREGL} PROPERTIES SOVERSION ${COREGL_VERSION_MAJOR})
SET_TARGET_PROPERTIES(${COREGL} PROPERTIES VERSION ${COREGL_VERSION})
#SET_TARGET_PROPERTIES(${COREGL} PROPERTIES COMPILE_FLAGS ${EXTRA_CFLAGS_common})
TARGET_LINK_LIBRARIES(${COREGL} ${pkg_common_LDFLAGS} ${dlog_LIBRARIES} "-ldl -g -O2 -fvisibility=hidden -Wall -std=c99 -lpthread")

ADD_LIBRARY(${EGL} SHARED ${EGL_SRCS_common})
SET_TARGET_PROPERTIES(${EGL} PROPERTIES SOVERSION ${EGL_VERSION_MAJOR})
SET_TARGET_PROPERTIES(${EGL} PROPERTIES VERSION ${EGL_VERSION})
#SET_TARGET_PROPERTIES(${COREGL} PROPERTIES COMPILE_FLAGS ${EXTRA_CFLAGS_common})
TARGET_LINK_LIBRARIES(${EGL} ${pkg_common_LDFLAGS} ${dlog_LIBRARIES} "-ldl -g -O2 -fvisibility=hidden -Wall -std=c99 -lpthread")

ADD_LIBRARY(${GLES1} SHARED ${GLES1_SRCS_common})
SET_TARGET_PROPERTIES(${GLES1} PROPERTIES SOVERSION ${GLES1_VERSION_MAJOR})
SET_TARGET_PROPERTIES(${GLES1} PROPERTIES VERSION ${GLES1_VERSION})
#SET_TARGET_PROPERTIES(${COREGL} PROPERTIES COMPILE_FLAGS ${EXTRA_CFLAGS_common})
TARGET_LINK_LIBRARIES(${GLES1} ${pkg_common_LDFLAGS} ${dlog_LIBRARIES} "-ldl -g -O2 -fvisibility=hidden -Wall -std=c99 -lpthread")

ADD_LIBRARY(${GLES2} SHARED ${GLES2_SRCS_common})
SET_TARGET_PROPERTIES(${GLES2} PROPERTIES SOVERSION ${GLES2_VERSION_MAJOR})
SET_TARGET_PROPERTIES(${GLES2} PROPERTIES VERSION ${GLES2_VERSION})
#SET_TARGET_PROPERTIES(${COREGL} PROPERTIES COMPILE_FLAGS ${EXTRA_CFLAGS_common})
TARGET_LINK_LIBRARIES(${GLES2} ${pkg_common_LDFLAGS} ${dlog_LIBRARIES} "-ldl -g -O2 -fvisibility=hidden -Wall -std=c99 -lpthread")

INSTALL(TARGETS ${COREGL} DESTINATION lib COMPONENT RuntimeLibraries)
INSTALL(TARGETS ${EGL} DESTINATION lib COMPONENT RuntimeLibraries)
INSTALL(TARGETS ${GLES1} DESTINATION lib COMPONENT RuntimeLibraries)
INSTALL(TARGETS ${GLES2} DESTINATION lib COMPONENT RuntimeLibraries)

#FOREACH(hfile ${HEADERS_common})
#	INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/${hfile} DESTINATION include/coregl)
#ENDFOREACH(hfile)
#INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/coregl.h DESTINATION include)

