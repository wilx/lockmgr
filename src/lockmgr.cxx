#include <map>
#include <utility>
#include <boost/graph/depth_first_search.hpp>
#include "lockmgr/ilockmgr.hxx"
#include "lockmgr/internal/graph.hxx"
#include "lockmgr/internal/syncprim.hxx"



namespace lockmgr
{

//! Lock manager.
class LockManager
  : public virtual ILockMgr
  , public virtual ICritSectionLock
  , public virtual IMutexLock
{
public:
  LockManager ();

  //!@{
  virtual IMutexLock * get_mutex_lockmgr_if ();
  virtual ICritSectionLock * get_critsec_lockmgr_if ();
  //!@}

  //!@{
  virtual void crit_enter (CRITICAL_SECTION *);
  virtual void crit_leave (CRITICAL_SECTION *);
  virtual bool crit_forget (CRITICAL_SECTION *);
  //!@}

  //!@{
  virtual DWORD mutex_lock (HANDLE);
  virtual DWORD mutex_unlock (HANDLE);
  virtual bool mutex_forget (HANDLE);
  //!@}

protected:
  //! Type of LockManager's internal synchronization primitive.
  typedef Mutex lock_type;

  //! Type of guard for LockManager's internal lock.
  typedef MutexGuard lock_guard;

  //! Type of map mapping nodes to vertex descriptors.
  typedef std::map<RAGNode, vertex_descr_type> node_to_vertex_map_type;

  //! 
  struct dfs_visitor
    : public boost::default_dfs_visitor
  { 
    void back_edge (boost::graph_traits<RAG>::edge_descriptor, const RAG &)
    {
      throw cycle_found_exception ();
    }
  };


  //! \Returns Returns RAGNode representing the calling thread.
  static RAGNode this_thread_node ();

  //! \Returns Returns RAGNode for given resource.
  static RAGNode resource_node (generic_syncprim_type);

  //! \Returns Returns existing or inserts new vertex given RAGNode.
  vertex_descr_type find_or_insert_vertex (RAGNode const &);

  //! \Returns Returns existing vertex given RAGNode.
  vertex_descr_type find_vertex (RAGNode const &) const;

  //! \brief Call this method in your foo_lock() method before any
  //! attempt to acquire your resource.
  void prepare_locking (generic_syncprim_type prim, vertex_descr_type * tv, 
			vertex_descr_type * rv, edge_descr_type * edge);

  //! \brief Call this method in your foo_lock() method after you have
  //! locked/acquired your resource.
  void finish_locking (vertex_descr_type tv, vertex_descr_type rv,
		       edge_descr_type edge);

  //! \brief Call this method in your foo_unlock () method after your
  //! resource has been released.
  void finish_unlocking (generic_syncprim_type prim);

  //! \brief LockManager's internal lock. It protects all access from public
  //! interface.
  lock_type lockmgr_lock;

  //! Resource Allocation Graph.
  RAG rag;

  //! Mapping from nodes to vertex descriptors.
  node_to_vertex_map_type node_to_vertex;


  //! Dtor. We don't want it public, so that LockManager is not too
  //! easily destroyable.
  virtual ~LockManager ();

private:
  LockManager (LockManager const &);
  LockManager & operator = (LockManager const &);
};


LockManager::LockManager ()
{ }


LockManager::~LockManager ()
{ }


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


void
LockManager::crit_enter (CRITICAL_SECTION * cs)
{
  vertex_descr_type tv, rv;
  edge_descr_type edge;

  // Check for deadlocks before attempt for actual lock.
  prepare_locking (cs, &tv, &rv, &edge);

  // Lock the actual resource.
  ::EnterCriticalSection (cs);

  // Record the newly acquired resource.
  finish_locking (tv, rv, edge);
}


void
LockManager::crit_leave (CRITICAL_SECTION * cs)
{
  ::LeaveCriticalSection (cs);

  // Remove the resource -> thread node as the resource has been released.
  finish_unlocking (cs);
}


void
LockManager::crit_forget (CRITICAL_SECTION * cs)
{
  forget_node (cs);
}


DWORD
LockManager::mutex_lock (HANDLE)
{
  return 0;
}


DWORD
LockManager::mutex_unlock (HANDLE)
{
  return 0;
}


bool
LockManager::mutex_forget (HANDLE)
{
  return 0;
}


RAGNode
LockManager::this_thread_node ()
{
  ThreadNode tn;
  init_thread_node (tn, get_this_thread_id ());
  RAGNode node (tn);
  return node;
}


RAGNode 
LockManager::resource_node (generic_syncprim_type prim)
{
  ResourceNode res_node;
  init_resource_node (res_node, prim);
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


//! Ctor.
cycle_found_exception::cycle_found_exception ()
  : std::runtime_error ("cycle_found_exception")
{ }

} // namespace lockmgr
