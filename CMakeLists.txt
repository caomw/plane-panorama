cmake_minimum_required(VERSION 2.6)
project(PPano)


SET(MY_OWN_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install CACHE PATH "Prefix prepended to
install directories")
SET(CMAKE_INSTALL_PREFIX "${MY_OWN_INSTALL_PREFIX}" CACHE INTERNAL "Prefix
prepended to install directories" FORCE)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(CAPGCMakeConfig)


set(CONFIG_FILE "conf.xml" CACHE FILEPATH "Debug Configuration File")
set(CONFIG_FILE ${CMAKE_SOURCE_DIR}/src/${CONFIG_FILE})
option(QUICK_DEBUG ON)

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/src/config/config.h.cmake ${CMAKE_BINARY_DIR}/src/config/config.h)

find_package(Qt4 COMPONENTS QtCOre QtGui QtOpenGL QtXml)
set(QT_USE_OPENGL 1)
INCLUDE(${QT_USE_FILE})
SET(LIBS ${LIBS} ${QT_LIBRARIES})


FIND_PACKAGE(OpenCV REQUIRED)
set(LIBS ${LIBS} ${OpenCV_LIBS})
MESSAGE(CVLib ${OpenCV_LIBS})

set(GTEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/gtest)
find_package(gtest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
set(LIBS ${LIBS} ${GTEST_BOTH_LIBRARIES})

#SET(MKL_INCDIR "D:/Program Files/Intel/Composer XE/mkl/include" CACHE PATH "Path to header files for the MKL library")
#SET(MKL_LIBDIR "D:/Program Files/Intel/Composer XE/mkl/lib/ia32" CACHE PATH "Path to library files for the MKL library")
#SET(HAVE_MKL 1)
#LINK_DIRECTORIES(${MKL_LIBDIR})
#SET(MKL_LIBS "mkl_intel_c_dll;mkl_intel_thread_dll;mkl_core_dll" CACHE STRING "Names for the MKL libraries")
#SET(LAPACKBLAS_LIB_NAMES "mkl_lapack95;mkl_blas95" CACHE STRING "The name of the lapack & blas libraries")
#ADD_DEFINITIONS(-DHAVE_MKL)
#include_directories(${MKL_INCDIR})
#SET(LIBS ${LIBS} ${MKL_LIBS})
#SET(LIBS ${LIBS} ${LAPACKBLAS_LIB_NAMES})


# set output place for executable and lib and static lib

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(ARCHIVE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/staticlib)

#add source directory 
add_subdirectory(src)
MESSAGE(all_3rdparty_libs ${LIBS})
