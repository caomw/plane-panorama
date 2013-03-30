#
# Try to find CAPGMath library and include path. 
# Once done this will define 
#
# CAPGMath_FOUND
# CAPGMath_INCLUDE_PATH 
# CAPGMath_LIBRARIES
#

IF (WIN32)
	FIND_PATH(CAPGMath_INCLUDE_PATH CAPGMath/maths.h 	
		${PROJECT_SOURCE_DIR}/3rdparty/CAPGMath/include 
		DOC "The directory where CAPGMath/maths.h resides") 
	FIND_LIBRARY(CAPGMath_LIBRARY 	
		NAMES CAPGMath 	
		PATHS 	
		${PROJECT_SOURCE_DIR}/3rdparty/CAPGMath/lib 
		DOC "The CAPGMath library")
	FIND_LIBRARY(CAPGMath_LIBRARY_DEBUG
		NAMES CAPGMathd 	
		PATHS 	
		${PROJECT_SOURCE_DIR}/3rdparty/CAPGMath/lib 
		DOC "The CAPGMath Debug library")
ENDIF (WIN32) 

IF (CAPGMath_INCLUDE_PATH)
	SET( CAPGMath_FOUND 1 CACHE STRING "Set to 1 if CAPGMath is found, 0 otherwise")
	set(CAPGMath_LIBRARIES optimized ${CAPGMath_LIBRARY} debug ${CAPGMath_LIBRARY_DEBUG})
ELSE (CAPGMath_INCLUDE_PATH) 
	SET( CAPGMath_FOUND 0 CACHE STRING "Set to 1 if CAPGMath is found, 0 otherwise")
ENDIF (CAPGMath_INCLUDE_PATH)


