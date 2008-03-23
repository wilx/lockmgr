#if ! defined (LOCKMANAGER_LOCKMGR_ILOCKMGR_HXX)
#define LOCKMANAGER_LOCKMGR_ILOCKMGR_HXX

#include "lockmgr/icritsec.hxx"
#include "lockmgr/imutex.hxx"

namespace lockmgr
{

//! Public interface to LockManager.
class LOCKMGR_INTERFACE ILockMgr
{
public:
  virtual IMutexLock * get_mutex_lockmgr_if () = 0;
  virtual ICritSectionLock * get_critsec_lockmgr_if () = 0;

protected:
  virtual ~ILockMgr () = 0;
};

} // namespace lockmgr


#endif // LOCKMANAGER_LOCKMGR_ILOCKMGR_HXX
