#include "lockmgr/config.hxx"

#if defined (WIN32)

#include "lockmgr/imutex.hxx"


namespace lockmgr
{

IMutexLock::~IMutexLock ()
{ }

} // namespace lockmgr


#endif // defined (WIN32)
