#include "lockmgr/config.hxx"

#if defined (LOCKMANAGER_UNIX)

#include "lockmgr/internal/platform_unix.hxx"


namespace lockmgr
{

// Definition.
::pthread_key_t thread_id_key;


// Definition
PthreadMutex thread_id_mutex;


// Definition.
thread_id_type thread_id_counter;


// Definition.
thread_id_type
get_this_thread_id ()
{
  unsigned * thread_id = reinterpret_cast<thread_id_type *>
    (::pthread_getspecific (thread_id_key));
  if (thread_id)
      return *thread_id;
  else
    {
      thread_id_type new_thread_id;
      {
	PthreadMutexGuard mg (thread_id_mutex);
	new_thread_id = ++thread_id_counter;
      }

      std::auto_ptr<thread_id_type> ptid (new thread_id_type (new_thread_id));
      int ret = ::pthread_setspecific (thread_id_key, ptid.get ());
      if (ret != 0)
	throw "Using pthread_setspecific() has failed.";
      else
	ptid.release ();

      return new_thread_id;
    }
}


namespace
{

::pthread_once_t once_control_lockmgr = PTHREAD_ONCE_INIT;


void deleter (void * val)
{
  delete reinterpret_cast<thread_id_type *>(val);
}


void
init_lockmgr_pthread_key ()  
{
  ::pthread_key_create (&thread_id_key, deleter);
}


struct setup
{
  setup ()
  { 
    ::pthread_once (&once_control_lockmgr, init_lockmgr_pthread_key);
  }
} setup_instance;

} // namespace

} // namespace lockmgr

#endif // defined (LOCKMANAGER_UNIX)
