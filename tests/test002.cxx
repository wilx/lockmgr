#if defined (WIN32) || defined (__CYGWIN__)

#include <windows.h>
#include <iostream>
#define BOOST_TEST_MODULE mutex_deadlock
#include <boost/test/auto_unit_test.hpp>

#include "lockmgr/internal/lockmgr.hxx"


namespace 
{

lockmgr::LockManager mgr;
lockmgr::IMutexLock * mi;
 
volatile bool thread1_throwed = false;
volatile bool thread2_throwed = false;
  
HANDLE mtx1, mtx2;
HANDLE thread1, thread2;
  

DWORD WINAPI thread1_proc (LPVOID)
{
  try
    {
      for (size_t i = 0; i < 2 && ! thread2_throwed; ++i)
	{
	  mi->mutex_lock (mtx1);
	  ::Sleep (30);
	  mi->mutex_lock (mtx2);
	  ::Sleep (30);
	  mi->mutex_unlock (mtx2);
	  ::Sleep (30);
	  mi->mutex_unlock (mtx1);
	  ::Sleep (30);
	}
    }
  catch (lockmgr::cycle_found_exception const & e)
    {
      e;
      thread1_throwed = true;
      std::cerr << "Thread 1 throwed.\n";
    }
}


DWORD WINAPI thread2_proc (LPVOID)
{
  try
    {
      for (size_t i = 0; i < 2 && ! thread1_throwed; ++i)
	{
	  mi->mutex_lock (mtx2);
	  ::Sleep (50);
	  mi->mutex_lock (mtx1);
	  ::Sleep (50);
	  mi->mutex_unlock (mtx1);
	  ::Sleep (50);
	  mi->mutex_unlock (mtx2);
	  ::Sleep (50);
	}
    }
  catch (lockmgr::cycle_found_exception const & e)
    {
      e;
      thread2_throwed = true;
      std::cerr << "Thread 2 throwed.\n";
    }
}

} // namespace

BOOST_AUTO_TEST_CASE (test_deadlock)
{
  BOOST_REQUIRE ((mi = mgr.get_mutex_lockmgr_if ()));
  
  BOOST_REQUIRE ((mtx1 = ::CreateMutex (0, false, TEXT ("mtx1"))));
  BOOST_REQUIRE ((mtx2 = ::CreateMutex (0, false, TEXT ("mtx2"))));

  BOOST_REQUIRE ((thread1 = ::CreateThread (0, 0, thread1_proc, 0, 0, 0)));
  BOOST_REQUIRE ((thread2 = ::CreateThread (0, 0, thread2_proc, 0, 0, 0)));
  
  Sleep (300);

  BOOST_CHECK (thread1_throwed || thread2_throwed);
}

#endif // WIN32
