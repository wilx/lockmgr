#if ! defined (LOCKMANAGER_LOCKMGR_INTERNAL_PLATFORM_WINDOWS_HXX)
#define LOCKMANAGER_LOCKMGR_INTERNAL_PLATFORM_WINDOWS_HXX

#include <functional>
#include "lockmgr/internal/windows.h"


namespace lockmgr
{

//! \brief Typedef for thread ID stored in ThreadNode.
typedef DWORD thread_id_type;


//! \brief "Less than" functor for thread_id_type.
typedef std::less<thread_id_type> thread_id_lt;


//! \brief Enum specifiying distinct categories of values of
//! primitives/resources stored in \c generic_sync_prim_type.
enum land_of_resource_type
  {
    eUserland,
    eKernel
  };

  
//! \brief Typedef for common synchronization primitive type stored in
//! ResourceNode.  It is a wrapped structure because we need to
//! distinguish between values coming from user land and kernel.
struct generic_syncprim_type_windows
{
  //! Default ctor.
  generic_syncprim_type_windows ()
    : val (0)
  { }

  //! Ctor from pointer and category.
  generic_syncprim_type_windows (void * v, land_of_resource_type c)
    : val (v), cat (c)
  { }

  //! Operator less than.
  bool
  operator < (generic_syncprim_type_windows const & other) const
  {
    if (val < other.val)
      return true;
    else if (val == other.val)
      return cat < other.cat;
    else
      return false;
  }

  //! \brief Value of resource/primitive; mutex handle or pointer to
  //! CRITICAL_SECTION.
  void * val;

  //! \brief Category of the value; eUserland or eKernel.
  land_of_resource_type cat;
};


//! \brief Typedef so that we can use generic_syncprim_type but so
//! that we don't violate ODR.
typedef generic_syncprim_type_windows generic_syncprim_type;


//! "Less than" functor for generic_syncprim_type.
typedef std::less<generic_syncprim_type> generic_syncprim_lt;


//! Returns thread ID of calling thread.
namespace
{

inline
thread_id_type
get_this_thread_id ()
{
  return ::GetCurrentThreadId ();
}

} // namespace

} // namespace lockmgr

#endif // LOCKMANAGER_LOCKMGR_INTERNAL_PLATFORM_WINDOWS_HXX
