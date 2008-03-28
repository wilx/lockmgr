#include "lockmgr/internal/lockmgr.hxx"
#include <utility>
#include <boost/graph/depth_first_search.hpp>


namespace lockmgr
{

namespace
{

//! 
struct dfs_visitor
  : public boost::default_dfs_visitor
{ 
  void back_edge (boost::graph_traits<RAG>::edge_descriptor, const RAG &)
  {
    throw cycle_found_exception ();
  }
};

} // namespace

LockManager::LockManager ()
{ }


LockManager::~LockManager ()
{ }


#if defined (WIN32) || defined (__CYGWIN__)

IMutexLock *
LockManager::get_mutex_lockmgr_if ()
{
  return dynamic_cast<IMutexLock *>(this);
}


ICritSectionLock *
LockManager::get_critsec_lockmgr_if ()
{
  return dynamic_cast<ICritSectionLock *>(this);
}

#endif // defined (WIN32) || defined (__CYGWIN__)


#if defined (LOCKMANAGER_UNIX)

IPthreadMutexLock * 
LockManager::get_pthread_mutex_lockmgr_if ()
{
  return dynamic_cast<IPthreadMutexLock *>(this);
}

#endif // defined (LOCKMANAGER_UNIX)

void 
LockManager::forget_this_thread ()
{
  forget_thread (get_this_thread_id ());
}


RAGNode
LockManager::this_thread_node ()
{
  return thread_node (get_this_thread_id ());
}


RAGNode
LockManager::thread_node (thread_id_type tid)
{
  ThreadNode tn (tid);
  RAGNode node (tn);
  return node;
}


RAGNode 
LockManager::resource_node (generic_syncprim_type prim)
{
  ResourceNode res_node (prim);
  RAGNode rag_res_node (res_node);
  return rag_res_node;
}


vertex_descr_type 
LockManager::find_or_insert_vertex (RAGNode const & node)
{
  vertex_descr_type v;
  
  std::pair<node_to_vertex_map_type::iterator, bool> insert_res
    = node_to_vertex.insert 
    (std::make_pair (node, vertex_descr_type ()));
  if (insert_res.second)
    {
      v = add_vertex (rag);
      insert_res.first->second = v;
    }
  else
    v = insert_res.first->second;

  return v;
}


vertex_descr_type 
LockManager::find_vertex (RAGNode const & node) const
{
  node_to_vertex_map_type::const_iterator it = node_to_vertex.find (node);
  if (it == node_to_vertex.end ())
    throw "Unknown node.";
  else
    return it->second;
}


void 
LockManager::prepare_locking (generic_syncprim_type prim,
			      vertex_descr_type * tv,
			      vertex_descr_type * rv,
			      edge_descr_type * edge)
{
  RAGNode const & rag_thread_node (this_thread_node ());
  RAGNode const & rag_res_node (resource_node (prim));

  lock_guard lg (lockmgr_lock);

  // Prepare vertexes.
  *tv = find_or_insert_vertex (rag_thread_node);
  *rv = find_or_insert_vertex (rag_res_node);

  // Insert edge from thread node to resource node.
  std::pair<edge_descr_type, bool> add_edge_res 
    = boost::add_edge (*tv, *rv, rag);
  *edge = add_edge_res.first;

  // Check for cycles starting with thread node using DFS
  // algorithm. The dfs_visitor throws cycle_found_exception.
  boost::depth_first_search (rag, dfs_visitor (), RAGColorMap (rag), *tv);
}


void 
LockManager::finish_locking (vertex_descr_type tv, vertex_descr_type rv,
			     edge_descr_type edge)
{
  lock_guard lg (lockmgr_lock);

  // Remove the thread -> resource edge added few lines above.
  boost::remove_edge (edge, rag);

  // Add new edge resource -> thread for acquired resouce.
  boost::add_edge (rv, tv, rag);
}


void 
LockManager::abandon_locking (edge_descr_type edge)
{
  lock_guard lg (lockmgr_lock);

  // Remove the thread -> resource edge add in prepare_locking().
  boost::remove_edge (edge, rag);
}


void 
LockManager::abandon_locking (generic_syncprim_type prim)
{
  lock_guard lg (lockmgr_lock);

  // Forget the resource completely if it is requested.
  forget_resource (prim);
}



void 
LockManager::finish_unlocking (generic_syncprim_type prim)
{
  vertex_descr_type tv, rv;
  RAGNode const & thread_node (this_thread_node ());
  RAGNode const & res_node (resource_node (prim));

  lock_guard lg (lockmgr_lock);

  tv = find_vertex (thread_node);
  rv = find_vertex (res_node);

  // Remove the resource -> thread edge because the resource has been freed.
  boost::remove_edge (rv, tv, rag);
}

  
void 
LockManager::forget_resource (generic_syncprim_type prim)
{
  vertex_descr_type rv;
  RAGNode const & res_node (resource_node (prim));

  lock_guard lg (lockmgr_lock);
  
  rv = find_vertex (res_node);
  boost::remove_vertex (rv, rag);
}


void 
LockManager::forget_thread (thread_id_type tid)
{
  vertex_descr_type tv;
  RAGNode const & rag_thread_node (thread_node (tid));

  lock_guard lg (lockmgr_lock);
  
  tv = find_vertex (rag_thread_node);
  boost::remove_vertex (tv, rag);
}


//! Ctor.
cycle_found_exception::cycle_found_exception ()
  : std::runtime_error ("cycle_found_exception")
{ }

} // namespace lockmgr
