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

#include "lockmgr/internal/syncprim_unix.hxx"


namespace lockmgr
{

//
PthreadMutex::PthreadMutex ()
{
    attach (new pthread_mutex_t);
    int ret = ::pthread_mutex_init (m_handle, 0);
    if (ret != 0)
	throw "Creating mutex failed.";
}


PthreadMutex::~PthreadMutex ()
{
    int ret = ::pthread_mutex_destroy (m_handle);
    // We cannot (should not) throw from dtor.
    assert (ret == 0);
}


void
PthreadMutex::lock () const
{
    int ret = ::pthread_mutex_lock (m_handle);
    if (ret != 0)
	throw "Fatal error when locking mutex.";
}


void
PthreadMutex::unlock () const
{
    int ret = ::pthread_mutex_unlock (m_handle);
    if (ret != 0)
        throw "Fatal error when unlocking mutex.";
}

} // namespace lockmgr

#endif // defined (LOCKMANAGER_UNIX)

// Local Variables:
// mode: C++
// eval: (c-set-style "microsoft")
// End:
