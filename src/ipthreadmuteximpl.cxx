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

#include <cassert>
#include <cerrno>
#include "lockmgr/internal/lockmgr.hxx"
#include "lockmgr/internal/platform.hxx"


namespace lockmgr 
{

int
LockManager::pthread_mutex_lock (pthread_mutex_t * mutex)
{
  vertex_descr_type tv, rv;
  edge_descr_type edge;

  // Check for deadlocks before attempt for actual lock.
  prepare_locking (generic_syncprim_type (mutex, eUserland), &tv, &rv, &edge);

  // Lock the actual resource.
  int ret = ::pthread_mutex_lock (mutex);

  switch (ret)
    {
    case 0:
      // We have acquired the mutex. Finish the locking procedure.
      finish_locking (tv, rv, edge);
      return ret;

    case EINVAL:
      // Completely remove the mutex from resource allocation graph.
      abandon_locking (generic_syncprim_type (mutex, eUserland));
      return ret;

    default:
      // Clean up.
      abandon_locking (edge);
      return ret;
    }
}


int
LockManager::pthread_mutex_unlock (pthread_mutex_t * mutex)
{
  int ret = ::pthread_mutex_unlock (mutex);
  finish_unlocking (generic_syncprim_type (mutex, eUserland));

  return ret;
}


void
LockManager::pthread_mutex_forget (pthread_mutex_t * mutex)
{
  forget_resource (generic_syncprim_type (mutex, eUserland));
}

} // namespace lockmgr


// Implementation of C style interfaces for Pthreads mutex.


//! C style interface to \c LockManager::pthread_mutex_lock.
extern "C" 
int 
lockmgr_pthread_mutex_lock (pthread_mutex_t * mutex)
{
  try
    {
      return lockmgr::lockmgr_instance.pthread_mutex_lock (mutex);
    }
  catch (lockmgr::cycle_found_exception const &)
    {
      return EDEADLK;
    }
}


//! C style interface to \c LockManager::pthread_mutex_unlock.
extern "C" 
int 
lockmgr_pthread_mutex_unlock (pthread_mutex_t * mutex)
{
  return lockmgr::lockmgr_instance.pthread_mutex_unlock (mutex);
}


#endif // defined (LOCKMANAGER_UNIX)
