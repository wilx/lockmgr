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

#if ! defined (LOCKMANAGER_LOCKMGR_ILOCKMGR_HXX)
#define LOCKMANAGER_LOCKMGR_ILOCKMGR_HXX

#include <stdexcept>
#include "lockmgr/config.hxx"

#if defined (WIN32)
#  include "lockmgr/icritsec.hxx"
#  include "lockmgr/imutex.hxx"
#endif

#if defined (LOCKMANAGER_UNIX)
#  include "lockmgr/ipthreadmutex.hxx"
#endif


namespace lockmgr
{

//! Public interface to LockManager.
class LOCKMGR_INTERFACE ILockMgr
{
public:

#if defined (WIN32)

  //! \brief Method returning interface for locking mutexes.
  virtual IMutexLock * get_mutex_lockmgr_if () = 0;

  //! \brief Method returning interface for locking critical sections.
  virtual ICritSectionLock * get_critsec_lockmgr_if () = 0;

#endif // defined (WIN32)

#if defined (LOCKMANAGER_UNIX)

  //! \brief Method returning interface for locking Pthread mutexes.
  virtual IPthreadMutexLock * get_pthread_mutex_lockmgr_if () = 0;

#endif // defined (LOCKMANAGER_UNIX)

  //! \brief Removes calling thread from resource allocation
  //! graph. Call this method only if the thread is going to end.
  virtual void forget_this_thread () = 0;

protected:
  virtual ~ILockMgr () = 0;
};


//! Exception thrown if DFS finds cycle in Resource Allocation Graph.
struct LOCKMGR_INTERFACE cycle_found_exception
  : public std::runtime_error
{
public:
  cycle_found_exception ();
};

} // namespace lockmgr


#endif // LOCKMANAGER_LOCKMGR_ILOCKMGR_HXX
