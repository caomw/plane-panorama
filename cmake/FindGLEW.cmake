#
# Try to find GLEW library and include path. 
# Once done this will define 
#
# GLEW_FOUND
# GLEW_INCLUDE_PATH 
# GLEW_LIBRARY 
#

IF (WIN32)
	FIND_PATH(GLEW_INCLUDE_PATH GL/glew.h 	
		${PROJECT_SOURCE_DIR}/3rdparty/glew/include 
		DOC "The directory where GL/glew.h resides")

	FIND_LIBRARY( GLEW_LIBRARY 	
		NAMES glew32 glew32s 	
		PATHS 	
		${PROJECT_SOURCE_DIR}/3rdparty/glew/lib 
		DOC "The GLEW library")

	FIND_LIBRARY( GLEW_LIBRARY_DEBUG	
		NAMES glew32d glew32sd	
		PATHS 	
		${PROJECT_SOURCE_DIR}/3rdparty/glew/lib 
		DOC "The GLEW library")
ELSE (WIN32) 
	FIND_PATH(GLEW_INCLUDE_PATH GL/glew.h 	
		/usr/include 	
		/usr/local/include 	
		/sw/include 	
		/opt/local/include 
		DOC "The directory where GL/glew.h resides") 
	FIND_LIBRARY( GLEW_LIBRARY 
		NAMES GLEW glew 
		PATHS 	
		/usr/lib64 
		/usr/lib 
		/usr/local/lib64 
		/usr/local/lib 	
		/sw/lib 	
		/opt/local/lib 	
		DOC "The GLEW library")
ENDIF (WIN32) 

IF (GLEW_INCLUDE_PATH)
	SET( GLEW_FOUND 1 CACHE STRING "Set to 1 if GLEW is found, 0 otherwise")
	SET(GLEW_LIBRARIES debug ${GLEW_LIBRARY_DEBUG} optimized ${GLEW_LIBRARY})
ELSE (GLEW_INCLUDE_PATH) 
	SET( GLEW_FOUND 0 CACHE STRING "Set to 1 if GLEW is found, 0 otherwise")
ENDIF (GLEW_INCLUDE_PATH)

