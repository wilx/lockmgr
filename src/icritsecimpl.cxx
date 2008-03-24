#include "lockmgr/internal/lockmgr.hxx"
#include "lockmgr/internal/windows.h"


namespace lockmgr
{

void
LockManager::crit_enter (CRITICAL_SECTION * cs)
{
  vertex_descr_type tv, rv;
  edge_descr_type edge;

  // Check for deadlocks before attempt for actual lock.
  prepare_locking (cs, &tv, &rv, &edge);

  // Lock the actual resource.
  ::EnterCriticalSection (cs);

  // Record the newly acquired resource.
  finish_locking (tv, rv, edge);
}


void
LockManager::crit_leave (CRITICAL_SECTION * cs)
{
  // Remove the resource -> thread edge as the resource will be released.
  finish_unlocking (cs);

  ::LeaveCriticalSection (cs);
}


void
LockManager::crit_forget (CRITICAL_SECTION * cs)
{
  forget_resource (cs);
}

} // namespace lockmgr
