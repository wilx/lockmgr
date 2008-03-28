#include "lockmgr/config.hxx"

#if defined (WIN32)

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
  prepare_locking (generic_syncprim_type (cs, eUserland), &tv, &rv, &edge);

  // Lock the actual resource.
  ::EnterCriticalSection (cs);

  // Record the newly acquired resource.
  finish_locking (tv, rv, edge);
}


void
LockManager::crit_leave (CRITICAL_SECTION * cs)
{
  // Remove the resource -> thread edge as the resource will be released.
  finish_unlocking (generic_syncprim_type (cs, eUserland));

  ::LeaveCriticalSection (cs);
}


void
LockManager::crit_forget (CRITICAL_SECTION * cs)
{
  forget_resource (generic_syncprim_type (cs, eUserland));
}

} // namespace lockmgr

#endif // defined (WIN32)
