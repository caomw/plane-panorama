# - Try to findOPENMESH
# Once done this will define
#  
# OPENMESH_FOUND           - system has OPENMESH
# OPENMESH_INCLUDE_DIR - theOPENMESH include directory
# OPENMESH_LIBRARY         - Link these to use OPENMESH
# OPENMESH_LIBRARY_DIR  - Library DIR of OPENMESH

# ============= added by justin.seeley.cn@gmail.com  2012-7-2 ==============
# OpenMeshLibraries	-libraries debug  & optimzed 
#   

IF (OPENMESH_INCLUDE_DIR)
  # Already in cache, be silent
  SET(OPENMESH_FIND_QUIETLY TRUE)
ENDIF (OPENMESH_INCLUDE_DIR)

FIND_PATH(OPENMESH_INCLUDE_DIR OpenMesh/Core/Mesh/PolyMeshT.hh
	  PATHS /usr/local/include 
                /usr/include 
                /usr/local/OpenMesh-2.0rc4/include
                "${CMAKE_SOURCE_DIR}/OpenMesh/src"
                "${CMAKE_SOURCE_DIR}/libs_required/OpenMesh/src"
                "${CMAKE_SOURCE_DIR}/../OpenMesh/src"
		"${CMAKE_SOURCE_DIR}/3rdparty/OpenMesh/include"
                /ACG/acgdev/gcc-4.0-x86_64/OM2/OpenMesh-2.0/installed/include
                )

IF(OPENMESH_INCLUDE_DIR)
	SET(OPENMESH_FOUND TRUE)
ELSE(OPENMESH_INCLUDE_DIR)
	SET(OPENMESH_FOUND FALSE)
ENDIF(OPENMESH_INCLUDE_DIR)

SET(OPENMESH_LIBRARY_DIR "${OPENMESH_INCLUDE_DIR}/../lib")

FIND_LIBRARY(OpenMeshCoreLibrary OpenMeshCore 
		${OPENMESH_LIBRARY_DIR})
FIND_LIBRARY(OpenMeshToolsLibrary OpenMeshTools
		${OPENMESH_LIBRARY_DIR})

FIND_LIBRARY(OpenMeshCoreLibraryDebug OpenMeshCored
		${OPENMESH_LIBRARY_DIR})

FIND_LIBRARY(OpenMeshToolsLibraryDebug OpenMeshToolsd
		${OPENMESH_LIBRARY_DIR})

SET (OpenMeshLibraries debug ${OpenMeshToolsLibraryDebug} ${OpenMeshCoreLibraryDebug} optimized ${OpenMeshCoreLibrary} ${OpenMeshToolsLibrary})

#MESSAGE(OpenMeshLibs:${OpenMeshLibraries})
