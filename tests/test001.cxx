#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

#include "lockmgr/internal/lockmgr.hxx"


BOOST_AUTO_TEST_CASE (test_instantiation)
{
  lockmgr::LockManager lockmgr;
}


BOOST_AUTO_TEST_CASE (test_interface_getters)
{
  lockmgr::LockManager lockmgr;

#if defined (WIN32) || defined (__CYGWIN__)

  BOOST_CHECK (lockmgr.get_mutex_lockmgr_if ());
  BOOST_CHECK (lockmgr.get_critsec_lockmgr_if ());

#endif // defined (WIN32) || defined (__CYGWIN__)

#if defined (LOCKMANAGER_UNIX)

  BOOST_CHECK (lockmgr.get_pthread_mutex_lockmgr_if ());

#endif // defined (LOCKMANAGER_UNIX)

}
