#if ! defined (LOCKMANAGER_LOCKMGR_INTERNAL_LOCKMGR_HXX)
#define LOCKMANAGER_LOCKMGR_INTERNAL_LOCKMGR_HXX

#include <map>
#include "lockmgr/ilockmgr.hxx"
#include "lockmgr/internal/graph.hxx"
#include "lockmgr/internal/syncprim.hxx"

#if defined (WIN32)
#  include "lockmgr/icritsec.hxx"
#  include "lockmgr/imutex.hxx"
#endif

#if defined (LOCKMANAGER_UNIX)
#  include "lockmgr/ipthreadmutex.hxx"
#endif


namespace lockmgr
{

//! \brief LockManager implements interfaces for locking/acquiring
//! various resources without getting into deadlock.  Deadlock
//! situation is detected by using resource allocation graph and
//! finding cycles in it.
class LockManager
  : public virtual ILockMgr

#if defined (WIN32)

  , public virtual ICritSectionLock
  , public virtual IMutexLock

#endif

#if defined (LOCKMANAGER_UNIX)

  , public virtual IPthreadMutexLock

#endif

{
public:
  LockManager ();
  virtual ~LockManager ();

  //!@{
#if defined (WIN32)

  virtual IMutexLock * get_mutex_lockmgr_if ();
  virtual ICritSectionLock * get_critsec_lockmgr_if ();

#endif // defined (WIN32)

#if defined (LOCKMANAGER_UNIX)

  virtual IPthreadMutexLock * get_pthread_mutex_lockmgr_if ();

#endif // defined (LOCKMANAGER_UNIX)

  virtual void forget_this_thread ();
  //!@}

#if defined (WIN32)

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

#endif // defined (WIN32)

#if defined (LOCKMANAGER_UNIX)

  //!@{
  virtual int pthread_mutex_lock (pthread_mutex_t *);
  virtual int pthread_mutex_unlock (pthread_mutex_t *);
  virtual void pthread_mutex_forget (pthread_mutex_t *);
  //!@}

#endif // defined (LOCKMANAGER_UNIX)

protected:
#if defined (WIN32)

  //! Type of LockManager's internal synchronization primitive.
  typedef Mutex lock_type;

  //! Type of guard for LockManager's internal lock.
  typedef MutexGuard lock_guard;

#elif defined (LOCKMANAGER_UNIX)

  //! Type of LockManager's internal synchronization primitive.
  typedef PthreadMutex lock_type;

  //! Type of guard for LockManager's internal lock.
  typedef PthreadMutexGuard lock_guard;

#endif

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
