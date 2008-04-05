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

#if ! defined (LOCKMANAGER_LOCKMGR_CLOCKMGR_H)
#define LOCKMANAGER_LOCKMGR_CLOCKMGR_H

#include "lockmgr/config.hxx"
#include "lockmgr/dlldef.hxx"

#if defined (WIN32) || defined (__CYGWIN__)

#include <windows.h>

#endif // defined (WIN32) || defined (__CYGWIN__)

#if defined (LOCKMANAGER_UNIX)

#include <pthread.h>

#endif // defined (LOCKMANAGER_UNIX)


#if defined (__cplusplus)
extern "C"
{
#endif // defined (__cplusplus)

#if defined (WIN32) || defined (__CYGWIN__)

LOCKMGR_INTERFACE DWORD LockMgrMutexLock (HANDLE mutex);
LOCKMGR_INTERFACE DWORD LockMgrMutexUnlock (HANDLE mutex);

#endif // defined (WIN32) || defined (__CYGWIN__)

#if defined (LOCKMANAGER_UNIX)

LOCKMGR_INTERFACE int lockmgr_pthread_mutex_lock (pthread_mutex_t * mutex);
LOCKMGR_INTERFACE int lockmgr_pthread_mutex_unlock (pthread_mutex_t * mutex);

#endif // defined (LOCKMANAGER_UNIX)

#if defined (__cplusplus)
}
#endif // defined (__cplusplus)


#endif // LOCKMANAGER_LOCKMGR_CLOCKMGR_H
