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
  //! Type of node, values from node_resource_type.
  unsigned short type : 1;

  //! For DFS colours.
  unsigned short color : 3;
};


//! ResourceNode represents resource in Resource Allocation Graph (RAG).
struct ResourceNode
{
  //! Base part of the ResourceNode.
  NodeBase base;

  //! Handle/sync primitive.
  generic_syncprim_type prim;
};


//! Helper function for initialization of ResourceNode.
inline
void
init_resource_node (ResourceNode & rn, generic_syncprim_type pr)
{
  rn.base.type = eResourceNode;
  rn.prim = pr;
}


//! ThreadNode represents thread in Resource Allocation Graph (RAG).
struct ThreadNode
{
  //! Base part of the ThreadNode.
  NodeBase base;

  //! Thread ID.
  thread_id_type thread_id;
};


//! Helper function for initialization of ThreadNode.
inline
void
init_thread_node (ThreadNode & tn, thread_id_type tid)
{
  tn.base.type = eThreadNode;
  tn.thread_id = tid;
}


//! Resource Allocation Graph Node is union of ResourceNode and
//! ThreadNode.
union RAGNode
{
  RAGNode ()
  { }

  RAGNode (ResourceNode const & rn)
    : res (rn)
  { }

  RAGNode (ThreadNode const & tn)
    : thread (tn)
  { }

  //! \todo Maybe using operator < where handle would be compared
  //! first instead of type first could be faster than this.
  bool
  operator < (RAGNode const & other) const
  {
    if (base.type < other.base.type)
      return true;
    else if (base.type == other.base.type)
      {
	if (base.type == eResourceNode)
	  return generic_syncprim_lt () (res.prim, other.res.prim);
	else
	  return thread_id_lt () (thread.thread_id, other.thread.thread_id);
      }
    else
      return false;
  }

  //! Common base/header.
  NodeBase base;

  //! Resource.
  ResourceNode res;

  //! Thread.
  ThreadNode thread;
};


//! Accessor for node type of RAGNode.
inline
node_resource_type
get_node_type (RAGNode const & n)
{
  return static_cast<node_resource_type>(n.base.type);
}


} // lockmgr

#endif // LOCKMANAGER_LOCKMGR_INTERNAL_NODES_HXX
