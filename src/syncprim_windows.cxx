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

#include "lockmgr/internal/syncprim_windows.hxx"


namespace lockmgr
{

//
Handle::Handle (HANDLE h)
    : m_handle (h)
{ }


Handle::~Handle ()
{
    if (m_handle != 0 && m_handle != INVALID_HANDLE_VALUE)
        ::CloseHandle (m_handle);
    m_handle = 0;
}


void
Handle::attach (HANDLE h)
{
    assert (m_handle == 0 || m_handle == INVALID_HANDLE_VALUE);
    m_handle = h;
}


HANDLE
Handle::get () const
{
    return m_handle;
}


Handle::operator Handle::boolean_type () const
{
    if (m_handle != 0 && m_handle != INVALID_HANDLE_VALUE)
        return &Handle::get;
    else
        return 0;
}


//
SyncPrimitive::~SyncPrimitive ()
{ }


SyncPrimitive::SyncPrimitive ()
    : m_handle (INVALID_HANDLE_VALUE)
{ }


void
SyncPrimitive::attach (HANDLE h)
{
    m_handle.attach (h);
}


HANDLE
SyncPrimitive::get () const
{
    return m_handle.get ();
}


SyncPrimitive::operator SyncPrimitive::boolean_type () const
{
    return static_cast<boolean_type>(m_handle);
}


//
Semaphore::Semaphore (unsigned max)
{
    attach (::CreateSemaphore (0, max, max, 0));
    if (! *this)
        throw "::CreateSemaphore() failed";
}


bool
Semaphore::lock (unsigned timeout, unsigned count) const
{
    unsigned tmp_count = count;
    while (tmp_count)
    {
        DWORD ret = ::WaitForSingleObject (m_handle.get (), timeout);
        switch (ret)
        {
        case WAIT_OBJECT_0:
            tmp_count -= 1;
            continue;

        case WAIT_TIMEOUT:
            unlock_internal (count - tmp_count);
            return false;

        case WAIT_FAILED:
            unlock_internal (count - tmp_count);
            throw "::WaitForSingleObject() failed.";

        default:
            unlock_internal (count - tmp_count);
            throw "Unexpected return value from ::WaitForSingleObject()";
        }
    }

    return true;
}


void
Semaphore::unlock (unsigned count) const
{
    if (::ReleaseSemaphore (m_handle.get (), count, 0))
        return;
    else
        throw "::ReleaseSemaphore() failed.";
}


void
Semaphore::unlock_internal (unsigned count) const
{
    if (count == 0)
        return;

    ::ReleaseSemaphore (m_handle.get (), count, 0);
}


//
Mutex::Mutex ()
{
    attach (::CreateMutex (0, FALSE, 0));
    if (! *this)
        throw "CreateMutex() failed.";
}


bool
Mutex::lock (unsigned timeout) const
{
    DWORD ret = ::WaitForSingleObject (m_handle.get (), timeout);
    switch (ret)
    {
    case WAIT_OBJECT_0:
        return true;

    case WAIT_TIMEOUT:
        return false;

    case WAIT_FAILED:
        throw "::WaitForSingleObject() failed.";

    case WAIT_ABANDONED:
        throw "Mutex has been abandoned?!?";

    default:
        throw "Unexpected return value from ::WaitForSingleObject()";
    }
}


void
Mutex::unlock () const
{
    if (::ReleaseMutex (m_handle.get ()))
        return;
    else
        throw "::ReleaseMutex() failed.";
}

} // namespace lockmgr

#endif // defined (WIN32)


// Local Variables:
// mode: C++
// eval: (c-set-style "microsoft")
// End:
