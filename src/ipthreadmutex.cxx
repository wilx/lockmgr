#include "lockmgr/config.hxx"

#if defined (LOCKMANAGER_UNIX)

#include "lockmgr/ipthreadmutex.hxx"


namespace lockmgr
{

IPthreadMutexLock::~IPthreadMutexLock ()
{ }

} // namespace lockmgr


#endif // LOCKMANAGER_UNIX
