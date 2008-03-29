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

#include "lockmgr/internal/lockmgr.hxx"
#include "lockmgr/internal/windows.h"


namespace lockmgr
{

void
LockManager::crit_enter (CRITICAL_SECTION * cs)
{
  vertex_descr_type tv, rv;
  edge_descr_type edge;

  // Check for deadlocks before attempt for actual lock.
  prepare_locking (generic_syncprim_type (cs, eUserland), &tv, &rv, &edge);

  // Lock the actual resource.
  ::EnterCriticalSection (cs);

  // Record the newly acquired resource.
  finish_locking (tv, rv, edge);
}


void
LockManager::crit_leave (CRITICAL_SECTION * cs)
{
  // Remove the resource -> thread edge as the resource will be released.
  finish_unlocking (generic_syncprim_type (cs, eUserland));

  ::LeaveCriticalSection (cs);
}


void
LockManager::crit_forget (CRITICAL_SECTION * cs)
{
  forget_resource (generic_syncprim_type (cs, eUserland));
}

} // namespace lockmgr

#endif // defined (WIN32)
