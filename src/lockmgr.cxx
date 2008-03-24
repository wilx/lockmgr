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
  virtual DWORD crit_lock (CRITICAL_SECTION *);
  virtual DWORD crit_unlock (CRITICAL_SECTION *);
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

  //! \Returns Returns existing or inserts new vertex given RAGNode.
  vertex_descr_type find_or_insert_vertex (RAGNode const &);

  //!
  void prepare_locking (generic_syncprim_type prim, vertex_descr_type * tv, 
			vertex_descr_type * rv, edge_descr_type * edge);

  //!
  void finish_locking (vertex_descr_type tv, vertex_descr_type rv,
		       edge_descr_type edge);


  //! LockManager's internal lock. It protects all access from public
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


DWORD
LockManager::crit_lock (CRITICAL_SECTION * cs)
{
  vertex_descr_type tv, rv;
  edge_descr_type edge;

  // Check for deadlocks before attempt for actual lock.
  prepare_locking (cs, &tv, &rv, &edge);

  // Lock the actual resource.
  ::EnterCriticalSection (cs);

  // Record the newly acquired resource.
  finish_locking (tv, rv, edge);

  return 0;
}


DWORD
LockManager::crit_unlock (CRITICAL_SECTION *)
{
  return 0;
}


bool
LockManager::crit_forget (CRITICAL_SECTION *)
{
  return 0;
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


//! \brief Call this method in your foo_lock() method before any
//! attempt to acquire your resource.
void 
LockManager::prepare_locking (generic_syncprim_type prim,
			      vertex_descr_type * tv,
			      vertex_descr_type * rv,
			      edge_descr_type * edge)
{
  RAGNode rag_thread_node (this_thread_node ());
  ResourceNode res_node;
  init_resource_node (res_node, prim);
  RAGNode rag_res_node (res_node);

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


//! \brief Call this method in your foo_lock() method after you have
//! locked/acquired your resource.
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


//! Ctor.
cycle_found_exception::cycle_found_exception ()
  : std::runtime_error ("cycle_found_exception")
{ }

} // namespace lockmgr
