#include <cassert>
#include "lockmgr/internal/windows.h"
#include "lockmgr/internal/lockmgr.hxx"


namespace lockmgr 
{

DWORD
LockManager::mutex_lock (HANDLE mutex)
{
  vertex_descr_type tv, rv;
  edge_descr_type edge;

  // Check for deadlocks before attempt for actual lock.
  prepare_locking (generic_syncprim_type (mutex, eKernel), &tv, &rv, &edge);

  // Lock the actual resource.
  DWORD ret = ::WaitForSingleObject (mutex, INFINITE);

  switch (ret)
    {
    case WAIT_OBJECT_0:
      // We have acquired the mutex. Finish the locking procedure.
      finish_locking (tv, rv, edge);
      return ret;

    case WAIT_ABANDONED:
      // Completely remove the mutex from resource allocation graph.
      abandon_locking (generic_syncprim_type (mutex, eKernel));
      return ret;

    case WAIT_TIMEOUT:
    case WAIT_FAILED:
      // Clean up.
      abandon_locking (edge);
      return ret;

    default:
      assert (0);
      throw "Unknown return value from ::WaitOnSingleObject().";
    }
}


DWORD
LockManager::mutex_unlock (HANDLE mutex)
{
  DWORD ret = ::ReleaseMutex (mutex);

  finish_unlocking (generic_syncprim_type (mutex, eKernel));

  return ret;
}


void
LockManager::mutex_forget (HANDLE mutex)
{
  forget_resource (generic_syncprim_type (mutex, eKernel));
}

} // namespace lockmgr
