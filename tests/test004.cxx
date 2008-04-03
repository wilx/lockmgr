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

#if defined (LOCKMANAGER_UNIX)

#include <sys/select.h>
#include <iostream>
#include <pthread.h>
#include <errno.h>
#include <boost/test/auto_unit_test.hpp>

#include "lockmgr/clockmgr.h"


namespace 
{

 
volatile bool thread1_throwed = false;
volatile bool thread2_throwed = false;
  

::pthread_mutex_t mtx1, mtx2;
::pthread_t thread1, thread2;


void 
xsleep (unsigned timeout)
{
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = timeout * 1000;
  select (0, 0, 0, 0, &tv);
}


#define CHECK(x)				\
  if ((x) == EDEADLK)				\
    goto deadlock;
  

void * 
thread1_proc (void *)
{
  for (size_t i = 0; i < 2 && ! thread2_throwed; ++i)
    {
      CHECK (::lockmgr_pthread_mutex_lock (&mtx1));
      xsleep (50);
      CHECK (::lockmgr_pthread_mutex_lock (&mtx2));
      xsleep (50);
      ::lockmgr_pthread_mutex_unlock (&mtx2);
      xsleep (50);
      ::lockmgr_pthread_mutex_unlock (&mtx1);
      xsleep (50);
    }

  BOOST_CHECK (thread2_throwed);
  return 0;

 deadlock:
  thread1_throwed = true;
  std::cerr << "Thread 1 throwed.\n";
  return 0;
}


void * 
thread2_proc (void *)
{
  for (size_t i = 0; i < 2 && ! thread1_throwed; ++i)
    {
      CHECK (::lockmgr_pthread_mutex_lock (&mtx2));
      xsleep (50);
      CHECK (::lockmgr_pthread_mutex_lock (&mtx1));
      xsleep (50);
      ::lockmgr_pthread_mutex_unlock (&mtx1);
      xsleep (50);
      ::lockmgr_pthread_mutex_unlock (&mtx2);
      xsleep (50);
    }
  
  BOOST_CHECK (thread1_throwed);
  return 0;

 deadlock:
  thread2_throwed = true;
  std::cerr << "Thread 2 throwed.\n";
  return 0;
}

} // namespace


BOOST_AUTO_TEST_CASE (test_deadlock_pthread_c)
{
  std::cerr << ">> test_deadlock_pthread_c <<\n";

  BOOST_REQUIRE (::pthread_mutex_init (&mtx1, 0) == 0);
  BOOST_REQUIRE (::pthread_mutex_init (&mtx2, 0) == 0);

  BOOST_REQUIRE (::pthread_create (&thread1, 0, thread1_proc, 0) == 0);
  BOOST_REQUIRE (::pthread_create (&thread2, 0, thread2_proc, 0) == 0);
  
  xsleep (300);

  BOOST_CHECK (thread1_throwed || thread2_throwed);
}

#endif // defined (LOCKMANAGER_UNIX)
