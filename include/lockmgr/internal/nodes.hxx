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

#if ! defined (LOCKMANAGER_LOCKMGR_INTERNAL_NODES_HXX)
#define LOCKMANAGER_LOCKMGR_INTERNAL_NODES_HXX

#include <boost/variant.hpp>
#include "lockmgr/internal/platform.hxx"


namespace lockmgr
{

//! Base/header for both node types.
struct NodeBase
{
  //! For DFS colours.
  unsigned char color;
};


//! ResourceNode represents resource in resource allocation graph (RAG).
struct ResourceNode
  : public NodeBase
{
  ResourceNode ()
  { }

  //! Constructs ResourceNode from primitive.
  ResourceNode (generic_syncprim_type p)
    : prim (p)
  { }

  //! Less than operator.
  bool
  operator < (ResourceNode const & other) const
  {
    return prim < other.prim;
  }

  //! Handle/sync primitive.
  generic_syncprim_type prim;
};


//! ThreadNode represents thread in resource allocation graph (RAG).
struct ThreadNode
  : public NodeBase
{
  ThreadNode ()
  { }

  //! Creates ThreadNode from thread ID.
  ThreadNode (thread_id_type tid)
    : thread_id (tid)
  { }
  
  //! Less than operator.
  bool
  operator < (ThreadNode const & other) const
  {
    return thread_id < other.thread_id;
  }

  //! Thread ID.
  thread_id_type thread_id;
};


//! Resource Allocation Graph Node.
typedef boost::variant<ResourceNode, ThreadNode> RAGNode;


} // lockmgr

#endif // LOCKMANAGER_LOCKMGR_INTERNAL_NODES_HXX
