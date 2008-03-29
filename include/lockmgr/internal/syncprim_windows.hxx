#if ! defined (LOCKMANAGER_LOCKMGR_INTERNAL_SYNCPRIM_WINDOWS_HXX)
#define LOCKMANAGER_LOCKMGR_INTERNAL_SYNCPRIM_WINDOWS_HXX

#include "lockmgr/config.hxx"

#include <cassert>
#include <boost/utility.hpp>
#include "lockmgr/internal/windows.h"


namespace lockmgr
{

//
class Handle
    : boost::noncopyable
{
public:
    explicit Handle (HANDLE = INVALID_HANDLE_VALUE);
    ~Handle ();

    void attach (HANDLE);
    HANDLE get () const;

    typedef HANDLE (Handle:: * boolean_type) () const;
    operator boolean_type () const;

private:
    HANDLE m_handle;
};


//
class SyncPrimitive
    : boost::noncopyable
{
public:
    ~SyncPrimitive ();

    void attach (HANDLE);
    HANDLE get () const;

    typedef Handle::boolean_type boolean_type;
    operator boolean_type () const;

protected:
    SyncPrimitive ();

    Handle m_handle;
};


//
class Semaphore
    : public SyncPrimitive
{
public:
    explicit Semaphore (unsigned);

    bool lock (unsigned timeout = INFINITE, unsigned count = 1) const;
    void unlock (unsigned count = 1) const;

private:
    void unlock_internal (unsigned) const;
};


//
class Mutex
    : public SyncPrimitive
{
public:
    Mutex ();

    bool lock (unsigned = INFINITE) const;
    void unlock () const;
};


//
template <typename Primitive>
class LockGuard
    : boost::noncopyable
{
public:
    explicit LockGuard (Primitive const & prim, unsigned timeout = INFINITE)
        : m_prim (&prim), m_locked (false)
    {
        m_locked = m_prim->lock (timeout);
    }

    ~LockGuard ()
    {
	try 
	{
	    
	    if (m_prim && m_locked)
		m_prim->unlock ();
	}
	catch (...)
	{
	    assert (0);
	    // We can't do much about it here.
	}
    }

    void detach ()
    {
        m_prim = 0;
	m_locked = false;
    }

    bool lock (unsigned timeout = INFINITE)
    {
	assert (m_prim);
	assert (! m_locked);
	return m_locked = m_prim->lock (timeout);
    }

    void unlock ()
    {
	assert (m_prim);
	assert (m_locked);
	m_prim->unlock ();
	m_locked = false;
    }
    
private:
    const Primitive * m_prim;
    bool m_locked;
};


typedef LockGuard<Semaphore> SemaphoreGuard;
typedef LockGuard<Mutex> MutexGuard;


} // namespace lockmgr

#endif // LOCKMANAGER_LOCKMGR_INTERNAL_SYNCPRIM_WINDOWS_HXX


// Local Variables:
// mode: C++
// eval: (c-set-style "microsoft")
// End:


