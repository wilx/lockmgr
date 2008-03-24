#if ! defined (LOCKMANAGER_LOCKMGR_INTERNAL_PLATFORM_HXX)
#define LOCKMANAGER_LOCKMGR_INTERNAL_PLATFORM_HXX

#include "lockmgr/internal/windows.h"
#include <functional>

namespace lockmgr
{

//! \brief Typedef for thread ID stored in ThreadNode.
typedef DWORD thread_id_type;

//! "Less than" functor for thread_id_type.
typedef std::less<thread_id_type> thread_id_lt;

//! \brief Typedef for common synchronization primitive type stored in
//! ResourceNode.
typedef void * generic_syncprim_type;

//! "Less than" functor for generic_syncprim_type.
typedef std::less<generic_syncprim_type> generic_syncprim_lt;

//! Returns thread ID of calling thread.
inline
thread_id_type
get_this_thread_id ()
{
  return ::GetCurrentThreadId ();
}

} // namespace lockmgr

#endif // LOCKMANAGER_LOCKMGR_INTERNAL_PLATFORM_HXX
