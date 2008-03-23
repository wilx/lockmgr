#if ! defined (LOCKMANAGER_LOCKMGR_ICRITSEC_HXX)
#define LOCKMANAGER_LOCKMGR_ICRITSEC_HXX

#include <windows.h>
#include "lockmgr/dlldef.hxx"

namespace lockmgr
{

//! Interface for using critical section with the LockManager. 
class LOCKMGR_INTERFACE ICritSectionLock
{
public:
  virtual DWORD crit_lock (HANDLE) = 0;
  virtual DWORD crit_unlock (HANDLE) = 0;
  virtual bool crit_forget (HANDLE) = 0;

protected:
  virtual ~ICritSectionLock () = 0;
};

} // namespace lockmgr

#endif // LOCKMANAGER_LOCKMGR_ICRITSEC_HXX
