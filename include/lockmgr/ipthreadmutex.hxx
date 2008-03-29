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

#if ! defined (LOCKMANAGER_LOCKMGR_IPTHREADMUTEX_HXX)
#define LOCKMANAGER_LOCKMGR_IPTHREADMUTEX_HXX

#include <pthread.h>
#include "lockmgr/dlldef.hxx"


namespace lockmgr
{

//! Interface for mutex with lock manager.
class LOCKMGR_INTERFACE IPthreadMutexLock
{
public:
  //! Method for locking given mutex.
  virtual int pthread_mutex_lock (pthread_mutex_t *) = 0;

  //! Method for unlocking locked mutex.
  virtual int pthread_mutex_unlock (pthread_mutex_t *) = 0;

  //! Method for removing bookkeeping information for given mutex.
  virtual void pthread_mutex_forget (pthread_mutex_t *) = 0;

protected:
  virtual ~IPthreadMutexLock () = 0;
};

} // namespace lockmgr

#endif // LOCKMANAGER_LOCKMGR_IPTHREADMUTEX_HXX
