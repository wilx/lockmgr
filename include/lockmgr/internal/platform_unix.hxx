#if ! defined (LOCKMANAGER_LOCKMGR_INTERNAL_PLATFORM_UNIX_HXX)
#define LOCKMANAGER_LOCKMGR_INTERNAL_PLATFORM_UNIX_HXX

#include <functional>
#include <pthread.h>
#include "lockmgr/internal/syncprim_unix.hxx"


namespace lockmgr
{


//! Type of generated thread ID.
typedef unsigned thread_id_type;


//! Comparator for generated thread ID.
typedef std::less<thread_id_type> thread_id_lt;


//! Pthread key for storing generated thread ID.
extern ::pthread_key_t thread_id_key;


//! Mutex protecting access to \c thread_id_counter.
extern PthreadMutex thread_id_mutex;


//! Thread ID counter protected by \c thread_id_mutex.
extern thread_id_type thread_id_counter;


//! \brief Enum specifying distinct categories of values of
//! primitives/resources stored in \c generic_sync_prim_type.
enum land_of_resource_type
  {
    eUserland,
    eKernel
  };


//! \brief Typedef for common synchronization primitive type stored in
//! ResourceNode.  It is a wrapped structure because we need to
//! distinguish between values coming from user land and kernel.
struct generic_syncprim_type_unix
{
  //! Default ctor.
  generic_syncprim_type_unix ()
    : val (0)
  { }

  //! Ctor from pointer and category.
  generic_syncprim_type_unix (void * v, land_of_resource_type c)
    : val (v), cat (c)
  { }

  //! Operator less than.
  bool
  operator < (generic_syncprim_type_unix const & other) const
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
typedef generic_syncprim_type_unix generic_syncprim_type;


//! "Less than" functor for generic_syncprim_type.
typedef std::less<generic_syncprim_type> generic_syncprim_lt;


//! Returns thread ID of calling thread.
extern thread_id_type get_this_thread_id ();


} // namespace lockmgr

#endif // LOCKMANAGER_LOCKMGR_INTERNAL_PLATFORM_HXX
