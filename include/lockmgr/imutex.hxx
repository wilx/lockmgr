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
  virtual DWORD mutex_lock (HANDLE) = 0;
  virtual DWORD mutex_unlock (HANDLE) = 0;
  virtual bool mutex_forget (HANDLE) = 0;

protected:
  virtual ~IMutexLock () = 0;
};

} // namespace lockmgr

#endif // LOCKMANAGER_LOCKMGR_IMUTEX_HXX
