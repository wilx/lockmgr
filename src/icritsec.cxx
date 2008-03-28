#include "lockmgr/config.hxx"

#if defined (WIN32)

#include "lockmgr/icritsec.hxx"


namespace lockmgr
{

ICritSectionLock::~ICritSectionLock ()
{ }

} // namespace lockmgr


#endif // defined (WIN32)
