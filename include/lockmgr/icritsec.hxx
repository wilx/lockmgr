#if ! defined (LOCKMANAGER_LOCKMGR_ICRITSEC_HXX)
#define LOCKMANAGER_LOCKMGR_ICRITSEC_HXX

#include "lockmgr/internal/windows.h"
#include "lockmgr/dlldef.hxx"

namespace lockmgr
{

//! Interface for using critical section with the LockManager. 
class LOCKMGR_INTERFACE ICritSectionLock
{
public:
  //! \brief Method for locking given critical section.
  virtual DWORD crit_lock (CRITICAL_SECTION *) = 0;

  //! \brief Method for unlocking locked critical section.
  virtual DWORD crit_unlock (CRITICAL_SECTION *) = 0;

  //! \brief Method for removing bookkeeping information for given
  //! critical section.
  virtual bool crit_forget (CRITICAL_SECTION *) = 0;

protected:
  virtual ~ICritSectionLock () = 0;
};

} // namespace lockmgr

#endif // LOCKMANAGER_LOCKMGR_ICRITSEC_HXX
