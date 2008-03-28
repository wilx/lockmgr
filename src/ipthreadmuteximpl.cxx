#include "lockmgr/config.hxx"

#if defined (LOCKMANAGER_UNIX)

#include <cassert>
#include <cerrno>
#include "lockmgr/internal/lockmgr.hxx"
#include "lockmgr/internal/platform.hxx"


namespace lockmgr 
{

int
LockManager::pthread_mutex_lock (pthread_mutex_t * mutex)
{
  vertex_descr_type tv, rv;
  edge_descr_type edge;

  // Check for deadlocks before attempt for actual lock.
  prepare_locking (generic_syncprim_type (mutex, eUserland), &tv, &rv, &edge);

  // Lock the actual resource.
  int ret = ::pthread_mutex_lock (mutex);

  switch (ret)
    {
    case 0:
      // We have acquired the mutex. Finish the locking procedure.
      finish_locking (tv, rv, edge);
      return ret;

    case EINVAL:
      // Completely remove the mutex from resource allocation graph.
      abandon_locking (generic_syncprim_type (mutex, eUserland));
      return ret;

    default:
      // Clean up.
      abandon_locking (edge);
      return ret;
    }
}


int
LockManager::pthread_mutex_unlock (pthread_mutex_t * mutex)
{
  int ret = ::pthread_mutex_unlock (mutex);
  finish_unlocking (generic_syncprim_type (mutex, eUserland));

  return ret;
}


void
LockManager::pthread_mutex_forget (pthread_mutex_t * mutex)
{
  forget_resource (generic_syncprim_type (mutex, eUserland));
}

} // namespace lockmgr

#endif // defined (LOCKMANAGER_UNIX)
