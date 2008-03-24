#if ! defined (LOCKMANAGER_LOCKMGR_ILOCKMGR_HXX)
#define LOCKMANAGER_LOCKMGR_ILOCKMGR_HXX

#include <stdexcept>
#include "lockmgr/icritsec.hxx"
#include "lockmgr/imutex.hxx"

namespace lockmgr
{

//! Public interface to LockManager.
class LOCKMGR_INTERFACE ILockMgr
{
public:
  //! Method returning interface for locking mutexes.
  virtual IMutexLock * get_mutex_lockmgr_if () = 0;

  //! Method returning interface for locking critical sections.
  virtual ICritSectionLock * get_critsec_lockmgr_if () = 0;

protected:
  virtual ~ILockMgr () = 0;
};


//! Exception thrown if DFS finds cycle in Resource Allocation Graph.
struct LOCKMGR_INTERFACE cycle_found_exception
  : public std::runtime_error
{
public:
  cycle_found_exception ();
};

} // namespace lockmgr


#endif // LOCKMANAGER_LOCKMGR_ILOCKMGR_HXX
