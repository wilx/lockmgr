#if ! defined (LOCKMANAGER_LOCKMGR_IMUTEX_HXX)
#define LOCKMANAGER_LOCKMGR_IMUTEX_HXX

#include <windows.h>
#include "lockmgr/dlldef.hxx"

namespace lockmgr
{

//! Interface for mutex with lock manager.
class LOCKMGR_INTERFACE IMutexLock
{
public:
  //! Method for locking given mutex.
  virtual DWORD mutex_lock (HANDLE) = 0;

  //! Method for unlocking locked mutex.
  virtual DWORD mutex_unlock (HANDLE) = 0;

  //! Method for removing bookkeeping information for given mutex.
  virtual bool mutex_forget (HANDLE) = 0;

protected:
  virtual ~IMutexLock () = 0;
};

} // namespace lockmgr

#endif // LOCKMANAGER_LOCKMGR_IMUTEX_HXX
