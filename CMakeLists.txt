# CMakeLists.txt file for lockmgr.
PROJECT (lockmgr)
CMAKE_MINIMUM_REQUIRED (VERSION 2.4)
SET (CMAKE_VERBOSE_MAKEFILE on)

FIND_PACKAGE (Boost)

IF (CYGWIN)
#  INCLUDE_DIRECTORIES
ENDIF (CYGWIN)

INCLUDE_DIRECTORIES ("include" ${Boost_INCLUDE_DIRS})
FILE (GLOB lockmgr_sources src/*.cxx)
MESSAGE (STATUS "Source files: ${lockmgr_sources}")

ADD_LIBRARY (lockmgr SHARED ${lockmgr_sources})
