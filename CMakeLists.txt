PROJECT (lockmgr)
CMAKE_MINIMUM_REQUIRED (VERSION 2.4)
SET (CMAKE_VERBOSE_MAKEFILE on)

FIND_PACKAGE (Boost)
INCLUDE_DIRECTORIES ("include" ${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES (${Boost_LIBRARY_DIRS})

#
# The lockmgr library specification.
#

FILE (GLOB lockmgr_sources src/*.cxx)
MESSAGE (STATUS "Source files: ${lockmgr_sources}")

ADD_LIBRARY (lockmgr SHARED ${lockmgr_sources})
SET_TARGET_PROPERTIES (lockmgr PROPERTIES
  VERSION "1"
  SOVERSION "1")

#
# Test suite specification.
#

IF (CYGWIN)
  SET(boost_test_exec_monitor_lib "boost_test_exec_monitor-gcc-mt")
ELSE (CYGWIN)
  SET(boost_test_exec_monitor_lib "boost_test_exec_monitor")
ENDIF (CYGWIN)

FILE (GLOB test_sources tests/*.cxx)
ADD_EXECUTABLE (test EXCLUDE_FROM_ALL ${test_sources})
TARGET_LINK_LIBRARIES (test lockmgr ${boost_test_exec_monitor_lib})
