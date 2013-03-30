# Try to find Levmar library and include path. 
# Once done this will define 
#
# LEVMAR_FOUND 
# LEVMAR_INCLUDE_PATH 
# LEVMAR_LIBRARIES 
#

IF (WIN32)
	FIND_PATH(LEVMAR_INCLUDE_PATH levmar.h 	
        ${PROJECT_SOURCE_DIR}/3rdparty/Levmar/include 
        DOC "The directory where Levmar.h resides")

	FIND_LIBRARY( LEVMAR_LIBRARY 	
		NAMES levmar  	
		PATHS 	
        ${PROJECT_SOURCE_DIR}/3rdparty/Levmar/lib 
		DOC "The LEVMAR library")

	FIND_LIBRARY( LEVMAR_LIBRARY_DEBUG	
		NAMES levmard 	
		PATHS 	
        ${PROJECT_SOURCE_DIR}/3rdparty/Levmar/lib 
		DOC "The LEVMAR library")
ELSE (WIN32) 
ENDIF (WIN32) 

IF (LEVMAR_INCLUDE_PATH)
	SET( LEVMAR_FOUND 1 CACHE STRING "Set to 1 if LEVMAR is found, 0 otherwise")
	SET(LEVMAR_LIBRARIES debug ${LEVMAR_LIBRARY_DEBUG} optimized ${LEVMAR_LIBRARY})
ELSE (LEVMAR_INCLUDE_PATH) 
	SET( LEVMAR_FOUND 0 CACHE STRING "Set to 1 if LEVMAR is found, 0 otherwise")
ENDIF (LEVMAR_INCLUDE_PATH)


