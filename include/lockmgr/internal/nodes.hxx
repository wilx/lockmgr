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
