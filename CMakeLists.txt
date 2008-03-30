PROJECT (lockmgr)
CMAKE_MINIMUM_REQUIRED (VERSION 2.4)
SET (CMAKE_VERBOSE_MAKEFILE on)

#
# Globals.
#
SET (lockmgr_version_major 0)
SET (lockmgr_version_minor 1)
SET (lockmgr_version_patch 0)

#
# Threads detection.
# 

FIND_PACKAGE (Threads)
MESSAGE (STATUS "Threads: ${CMAKE_THREAD_LIBS_INIT}")

#
# Boost detection.
#

FIND_PACKAGE (Boost)
INCLUDE_DIRECTORIES ("include" ${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES (${Boost_LIBRARY_DIRS})

#
# The lockmgr library specification.
#

FILE (GLOB_RECURSE lockmgr_headers include/*.hxx)
FILE (GLOB lockmgr_sources src/*.cxx)
MESSAGE (STATUS "Source files: ${lockmgr_sources} ${lockmgr_headers}")

ADD_LIBRARY (lockmgr SHARED ${lockmgr_sources} ${lockmgr_headers})
TARGET_LINK_LIBRARIES (lockmgr ${CMAKE_THREAD_LIBS_INIT})
SET_TARGET_PROPERTIES (lockmgr PROPERTIES
  VERSION "${lockmgr_version_major}.${lockmgr_version_minor}.${lockmgr_version_patch}"
  SOVERSION "${lockmgr_version_major}")

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
TARGET_LINK_LIBRARIES (test lockmgr ${boost_test_exec_monitor_lib} 
		      ${CMAKE_THREAD_LIBS_INIT})

#
# CPack configuration.
#
INCLUDE (InstallRequiredSystemLibraries)
SET (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Lock manager")
SET (CPACK_PACKAGE_VENDOR "wilx")
SET (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENCE")
SET (CPACK_PACKAGE_VERSION_MAJOR "${lockmgr_version_major}")
SET (CPACK_PACKAGE_VERSION_MINOR "${lockmgr_version_minor}")
SET (CPACK_PACKAGE_VERSION_PATCH "${lockmgr_version_patch}")
SET (CPACK_PACKAGE_INSTALL_DIRECTORY "lockmgr-${lockmgr_version_major}")
SET (CPACK_PACKAGE_EXECUTABLES "test")
INCLUDE (CPack)

INSTALL (TARGETS lockmgr 
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
