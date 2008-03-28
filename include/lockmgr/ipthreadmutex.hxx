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
