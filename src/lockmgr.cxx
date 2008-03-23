#include <map>
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
  virtual DWORD crit_lock (HANDLE);
  virtual DWORD crit_unlock (HANDLE);
  virtual bool crit_forget (HANDLE);
  //!@}

  //!@{
  virtual DWORD mutex_lock (HANDLE);
  virtual DWORD mutex_unlock (HANDLE);
  virtual bool mutex_forget (HANDLE);
  //!@}

protected:
  virtual ~LockManager ();

  //! Type of LockManager's internal synchronization primitive.
  typedef Mutex lock_type;

  //! Type of guard for LockManager's internal lock.
  typedef MutexGuard lock_guard;

  //! Type of map mapping nodes to vertex descriptors.
  typedef std::map<RAGNode, vertex_descr_type> node_to_vertex_map_type;

  //! LockManager's internal lock.
  lock_type lockmgr_lock;

  //! Resource Allocation Graph.
  RAG rag;

  //! Mapping from nodes to vertex descriptors.
  node_to_vertex_map_type node_to_vertex;

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
LockManager::crit_lock (HANDLE)
{
  lock_guard lg (lockmgr_lock);
  return 0;
}


DWORD
LockManager::crit_unlock (HANDLE)
{
  return 0;
}


bool
LockManager::crit_forget (HANDLE)
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


} // namespace lockmgr
