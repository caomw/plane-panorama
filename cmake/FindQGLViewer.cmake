# - Try to findQGLViwer
# Once done this will define
#  
# QGLViwer_FOUND           - system has QGLViwer
# QGLViwer_INCLUDE_DIR - theQGLViwer include directory
# QGLViwer_LIBRARY         - Link these to use QGLViwer
# QGLViwer_LIBRARY_DIR  - Library DIR of QGLViwer

# ============= added by justin.seeley.cn@gmail.com  2012-7-2 ==============
# OpenMeshLibraries	-libraries debug  & optimzed 
#   

IF (QGLViwer_INCLUDE_DIR)
  # Already in cache, be silent
  SET(QGLViwer_FIND_QUIETLY TRUE)
ENDIF (QGLViwer_INCLUDE_DIR)

FIND_PATH(QGLViwer_INCLUDE_DIR QGLViewer/qglviewer.h
	  PATHS /usr/local/include 
                /usr/include 
		"${CMAKE_SOURCE_DIR}/3rdparty/libqglviewer/include"
                )

IF(QGLViwer_INCLUDE_DIR)
	SET(QGLViwer_FOUND TRUE)
ELSE(QGLViwer_INCLUDE_DIR)
	SET(QGLViwer_FOUND FALSE)
ENDIF(QGLViwer_INCLUDE_DIR)

SET(QGLViwer_LIBRARY_DIR "${QGLViwer_INCLUDE_DIR}/../lib")

FIND_LIBRARY(QGLViewerLibrary QGLViewer2 
		${QGLViwer_LIBRARY_DIR})

FIND_LIBRARY(QGLViewerLibraryDebug QGLViewerd2
		${QGLViwer_LIBRARY_DIR})

SET (QGLViewerLibraries debug ${QGLViewerLibraryDebug} optimized ${QGLViewerLibrary})

MESSAGE(QGLViewerLibs:${QGLViewerLibraries})
