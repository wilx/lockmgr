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

#if defined (WIN32)

#include <cassert>
#include "lockmgr/internal/windows.h"
#include "lockmgr/internal/lockmgr.hxx"


namespace lockmgr 
{

DWORD
LockManager::mutex_lock (HANDLE mutex)
{
  vertex_descr_type tv, rv;
  edge_descr_type edge;

  // Check for deadlocks before attempt for actual lock.
  prepare_locking (generic_syncprim_type (mutex, eKernel), &tv, &rv, &edge);

  // Lock the actual resource.
  DWORD ret = ::WaitForSingleObject (mutex, INFINITE);

  switch (ret)
    {
    case WAIT_OBJECT_0:
      // We have acquired the mutex. Finish the locking procedure.
      finish_locking (tv, rv, edge);
      return ret;

    case WAIT_ABANDONED:
      // Completely remove the mutex from resource allocation graph.
      abandon_locking (generic_syncprim_type (mutex, eKernel));
      return ret;

    case WAIT_TIMEOUT:
    case WAIT_FAILED:
      // Clean up.
      abandon_locking (edge);
      return ret;

    default:
      assert (0);
      throw "Unknown return value from ::WaitOnSingleObject().";
    }
}


DWORD
LockManager::mutex_unlock (HANDLE mutex)
{
  DWORD ret = ::ReleaseMutex (mutex);

  finish_unlocking (generic_syncprim_type (mutex, eKernel));

  return ret;
}


void
LockManager::mutex_forget (HANDLE mutex)
{
  forget_resource (generic_syncprim_type (mutex, eKernel));
}

} // namespace lockmgr

#endif // defined (WIN32)
