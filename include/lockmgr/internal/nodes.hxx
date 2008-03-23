#if ! defined (LOCKMANAGER_LOCKMGR_INTERNAL_NODES_HXX)
#define LOCKMANAGER_LOCKMGR_INTERNAL_NODES_HXX

#include "lockmgr/internal/platform.hxx"


namespace lockmgr
{

//! Constants for node types.
enum node_resource_type
  {
    eResourceNode,
    eThreadNode
  };


//! Base/header for both node types.
struct NodeBase
{
  unsigned short type : 1;
};


//! ResourceNode represents resource in Resource Allocation Graph (RAG).
struct ResourceNode
{
  //! Base part of the ResourceNode.
  NodeBase base;

  //! Handle/sync primitive.
  generic_syncprim_type prim;
};


//! ThreadNode represents thread in Resource Allocation Graph (RAG).
struct ThreadNode
{
  //! Base part of the ThreadNode.
  NodeBase base;

  //! Thread ID.
  thread_id_type thread_id;
};


//! Resource Allocation Graph Node is union of ResourceNode and
//! ThreadNode.
union RAGNode
{
  //! Common base/header.
  NodeBase base;

  //! Resource.
  ResourceNode res;

  //! Thread.
  ThreadNode thread;
};


inline
node_resource_type
get_node_type (RAGNode const & n)
{
  return static_cast<node_resource_type>(n.base.type);
}


} // lockmgr

#endif // LOCKMANAGER_LOCKMGR_INTERNAL_NODES_HXX
