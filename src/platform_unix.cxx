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

#include "lockmgr/internal/platform_unix.hxx"


namespace lockmgr
{

// Definition.
::pthread_key_t thread_id_key;


// Definition
PthreadMutex thread_id_mutex;


// Definition.
thread_id_type thread_id_counter;


// Definition.
thread_id_type
get_this_thread_id ()
{
  unsigned * thread_id = reinterpret_cast<thread_id_type *>
    (::pthread_getspecific (thread_id_key));
  if (thread_id)
      return *thread_id;
  else
    {
      thread_id_type new_thread_id;
      {
	PthreadMutexGuard mg (thread_id_mutex);
	new_thread_id = ++thread_id_counter;
      }

      std::auto_ptr<thread_id_type> ptid (new thread_id_type (new_thread_id));
      int ret = ::pthread_setspecific (thread_id_key, ptid.get ());
      if (ret != 0)
	throw "Using pthread_setspecific() has failed.";
      else
	ptid.release ();

      return new_thread_id;
    }
}


namespace
{

::pthread_once_t once_control_lockmgr = PTHREAD_ONCE_INIT;


void deleter (void * val)
{
  delete reinterpret_cast<thread_id_type *>(val);
}


void
init_lockmgr_pthread_key ()  
{
  ::pthread_key_create (&thread_id_key, deleter);
}


struct setup
{
  setup ()
  { 
    ::pthread_once (&once_control_lockmgr, init_lockmgr_pthread_key);
  }
} setup_instance;

} // namespace

} // namespace lockmgr

#endif // defined (LOCKMANAGER_UNIX)
