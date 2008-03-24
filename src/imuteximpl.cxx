#include "lockmgr/internal/lockmgr.hxx"


namespace lockmgr 
{

DWORD
LockManager::mutex_lock (HANDLE)
{
  return 0;
}


DWORD
LockManager::mutex_unlock (HANDLE)
{
  return 0;
}


bool
LockManager::mutex_forget (HANDLE)
{
  return 0;
}

} // namespace lockmgr
