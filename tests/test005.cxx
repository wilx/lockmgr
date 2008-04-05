// Copyright (c) 2008, Václav Haisman
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "lockmgr/config.hxx"

#if defined (WIN32) || defined (__CYGWIN__)

#include <windows.h>
#include <iostream>
#include <boost/test/auto_unit_test.hpp>

#include "lockmgr/clockmgr.h"


namespace 
{

volatile bool thread1_throwed = false;
volatile bool thread2_throwed = false;
  
HANDLE mtx1, mtx2;
HANDLE thread1, thread2;
  

#define CHECK(x)				\
  if ((x) == WAIT_FAILED)			\
    goto deadlock;


DWORD WINAPI thread1_proc (LPVOID)
{
  for (size_t i = 0; i < 2 && ! thread2_throwed; ++i)
    {
      CHECK (::LockMgrMutexLock (mtx1));
      ::Sleep (30);
      CHECK (::LockMgrMutexLock (mtx2));
      ::Sleep (30);
      ::LockMgrMutexUnlock (mtx2);
      ::Sleep (30);
      ::LockMgrMutexUnlock (mtx1);
      ::Sleep (30);
    }

  BOOST_CHECK (thread2_throwed);
  return 0;
  
 deadlock:
  thread1_throwed = true;
  std::cerr << "Thread 1 throwed.\n";
  return 0;
}


DWORD WINAPI thread2_proc (LPVOID)
{
  for (size_t i = 0; i < 2 && ! thread1_throwed; ++i)
    {
      CHECK (::LockMgrMutexLock (mtx2));
      ::Sleep (50);
      CHECK (::LockMgrMutexLock (mtx1));
      ::Sleep (50);
      ::LockMgrMutexUnlock (mtx1);
      ::Sleep (50);
      ::LockMgrMutexUnlock (mtx2);
      ::Sleep (50);
    }

  BOOST_CHECK (thread1_throwed);
  return 0;
  
 deadlock:
  thread2_throwed = true;
  std::cerr << "Thread 2 throwed.\n";
  return 0;
}

} // namespace


BOOST_AUTO_TEST_CASE (test_deadlock_win32_mutex_c)
{
  std::cerr << ">> test_deadlock_win32_mutex_c <<\n";

  BOOST_REQUIRE ((mtx1 = ::CreateMutex (0, false, TEXT ("mtx1"))));
  BOOST_REQUIRE ((mtx2 = ::CreateMutex (0, false, TEXT ("mtx2"))));

  BOOST_REQUIRE ((thread1 = ::CreateThread (0, 0, thread1_proc, 0, 0, 0)));
  BOOST_REQUIRE ((thread2 = ::CreateThread (0, 0, thread2_proc, 0, 0, 0)));
  
  Sleep (300);

  BOOST_CHECK (thread1_throwed || thread2_throwed);
}

#endif // WIN32
