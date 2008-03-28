#if ! defined (LOCKMANAGER_LOCKMGR_INTERNAL_SYNCPRIM_HXX)
#define LOCKMANAGER_LOCKMGR_INTERNAL_SYNCPRIM_HXX

#include "lockmgr/config.hxx"

#if defined (LOCKMANAGER_UNIX)
#  include "lockmgr/internal/syncprim_unix.hxx"
#endif

#if defined (WIN32)
#  include "lockmgr/internal/syncprim_windows.hxx"
#endif 

#endif // LOCKMANAGER_LOCKMGR_INTERNAL_SYNCPRIM_HXX

// Local Variables:
// mode: C++
// eval: (c-set-style "microsoft")
// End:
