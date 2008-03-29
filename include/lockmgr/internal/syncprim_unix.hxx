#if ! defined (LOCKMANAGER_LOCKMGR_INTERNAL_SYNCPRIM_UNIX_HXX)
#define LOCKMANAGER_LOCKMGR_INTERNAL_SYNCPRIM_UNIX_HXX

#include <cassert>
#include <boost/utility.hpp>
#include <pthread.h>


namespace lockmgr
{

//
template <typename T>
class UnixSyncPrimitive
    : boost::noncopyable
{
public:
    typedef T primitive_type;

    void
    attach (primitive_type * p)
    {
	assert (! m_handle);
	m_handle = p;
    }

    primitive_type * 
    get () const
    {
	return m_handle;
    }

    typedef primitive_type * (UnixSyncPrimitive::* boolean_type) ();
    operator boolean_type () const
    {
	return static_cast<boolean_type>(m_handle);
    }

protected:
    UnixSyncPrimitive ()
	: m_handle (0)
    { }

    ~UnixSyncPrimitive ()
    {
	delete m_handle;
    }
    
    primitive_type * m_handle;
};


//
class PthreadMutex
    : public UnixSyncPrimitive<pthread_mutex_t>
{
public:
    PthreadMutex ();
    ~PthreadMutex ();

    void lock () const;
    void unlock () const;
};


//
template <typename Primitive>
class UnixLockGuard
    : boost::noncopyable
{
public:
    explicit UnixLockGuard (Primitive const & prim)
        : m_prim (&prim), m_locked (false)
    {
	lock ();
    }

    ~UnixLockGuard ()
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

    void lock ()
    {
	assert (m_prim);
	assert (! m_locked);
	m_prim->lock ();
	m_locked = true;
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


typedef UnixLockGuard<PthreadMutex> PthreadMutexGuard;


} // namespace lockmgr

#endif // LOCKMANAGER_LOCKMGR_INTERNAL_SYNCPRIM_UNIX_HXX


// Local Variables:
// mode: C++
// eval: (c-set-style "microsoft")
// End:


