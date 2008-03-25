#if ! defined (LOCKMANAGER_LOCKMGR_INTERNAL_LOCKMGR_HXX)
#define LOCKMANAGER_LOCKMGR_INTERNAL_LOCKMGR_HXX

#include <map>
#include "lockmgr/ilockmgr.hxx"
#include "lockmgr/internal/graph.hxx"
#include "lockmgr/internal/syncprim.hxx"


namespace lockmgr
{

//! \todo The value of HANLDE (for mutex) comes from the kernel
//! address space, and the value of CRITICAL_SECTION * comes from user
//! address space. Theoretically, there could be a HANDLE with value
//! equal to CRITICAL_SECTION *, how do we deal with it?  Lock
//! manager.
class LockManager
  : public virtual ILockMgr
  , public virtual ICritSectionLock
  , public virtual IMutexLock
{
public:
  LockManager ();
  virtual ~LockManager ();

  //!@{
  virtual IMutexLock * get_mutex_lockmgr_if ();
  virtual ICritSectionLock * get_critsec_lockmgr_if ();
  virtual void forget_this_thread ();
  //!@}

  //!@{
  virtual void crit_enter (CRITICAL_SECTION *);
  virtual void crit_leave (CRITICAL_SECTION *);
  virtual void crit_forget (CRITICAL_SECTION *);
  //!@}

  //!@{
  virtual DWORD mutex_lock (HANDLE);
  virtual DWORD mutex_unlock (HANDLE);
  virtual void mutex_forget (HANDLE);
  //!@}

protected:
  //! Type of LockManager's internal synchronization primitive.
  typedef Mutex lock_type;

  //! Type of guard for LockManager's internal lock.
  typedef MutexGuard lock_guard;

  //! Type of map mapping nodes to vertex descriptors.
  typedef std::map<RAGNode, vertex_descr_type> node_to_vertex_map_type;


  //! \Returns Returns RAGNode representing the calling thread.
  static RAGNode this_thread_node ();

  //! \Returns Returns RAGNode representing given thread.
  static RAGNode thread_node (thread_id_type);

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

  //!@{
  //! \brief Call this method in your foo_lock() method if your
  //! resource acquisition fails. It will clean up LockManager's
  //! internal bookkeeping.
  void abandon_locking (edge_descr_type edge);

  //! \brief Call this method in your foo_lock() method if your
  //! resource acquisition fails. It will clean up LockManager's
  //! internal bookkeeping.
  void abandon_locking (generic_syncprim_type prim);
  //!@}

  //! \brief Call this method in your foo_unlock () method after your
  //! resource has been released.
  void finish_unlocking (generic_syncprim_type prim);

  //! \brief Clean up for a resource that is going to be removed from the
  //! system.
  void forget_resource (generic_syncprim_type prim);

  //! \brief Clean up for thread that is going to end.
  void forget_thread (thread_id_type tid);


  //! \brief LockManager's internal lock. It protects all access from public
  //! interface.
  lock_type lockmgr_lock;

  //! Resource Allocation Graph.
  RAG rag;

  //! Mapping from nodes to vertex descriptors.
  node_to_vertex_map_type node_to_vertex;

private:
  LockManager (LockManager const &);
  LockManager & operator = (LockManager const &);
};

} // namespace lockmgr

#endif // LOCKMANAGER_LOCKMGR_INTERNAL_LOCKMGR_HXX
