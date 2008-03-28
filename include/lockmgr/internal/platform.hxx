#if ! defined (LOCKMANAGER_LOCKMGR_INTERNAL_PLATFORM_HXX)
#define LOCKMANAGER_LOCKMGR_INTERNAL_PLATFORM_HXX

#include "lockmgr/config.hxx"

#if defined (LOCKMANAGER_UNIX)
#  include "lockmgr/internal/platform_unix.hxx"
#elif defined (WIN32)
#  include "lockmgr/internal/platform_windows.hxx"
#endif

#endif // LOCKMANAGER_LOCKMGR_INTERNAL_PLATFORM_HXX
