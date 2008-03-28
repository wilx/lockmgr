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
