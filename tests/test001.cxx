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

#include "lockmgr/config.hxx"

#include <iostream>
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

#include "lockmgr/lockmgr.hxx"

namespace
{

lockmgr::ILockMgr * lm = 0;

}

BOOST_AUTO_TEST_CASE (test_instantiation)
{
  std::cerr << ">> test_instantiation <<\n";

  BOOST_REQUIRE ((lm = lockmgr::get_lock_manager ()));
}


BOOST_AUTO_TEST_CASE (test_interface_getters)
{
  std::cerr << ">> test_interface_getters <<\n";

#if defined (WIN32) || defined (__CYGWIN__)

  BOOST_CHECK (lm->get_mutex_lockmgr_if ());
  BOOST_CHECK (lm->get_critsec_lockmgr_if ());

#endif // defined (WIN32) || defined (__CYGWIN__)

#if defined (LOCKMANAGER_UNIX)

  BOOST_CHECK (lm->get_pthread_mutex_lockmgr_if ());

#endif // defined (LOCKMANAGER_UNIX)

}
