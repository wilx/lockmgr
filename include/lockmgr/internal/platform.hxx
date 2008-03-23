#if ! defined (LOCKMANAGER_LOCKMGR_INTERNAL_PLATFORM_HXX)
#define LOCKMANAGER_LOCKMGR_INTERNAL_PLATFORM_HXX

#include <windows.h>

namespace lockmgr
{

//! \brief Typedef for thread ID stored in ThreadNode.
typedef DWORD thread_id_type;

//! \brief Typedef for common synchronization primitive type stored in
//! ResourceNode.
typedef HANDLE generic_syncprim_type;

} // namespace lockmgr

#endif // LOCKMANAGER_LOCKMGR_INTERNAL_PLATFORM_HXX
