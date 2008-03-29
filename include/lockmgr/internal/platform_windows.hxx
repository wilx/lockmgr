// Copyright (c) 2008, Václav Haisman
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
