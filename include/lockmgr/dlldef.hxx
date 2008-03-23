#if ! defined (LOCKMANAGER_LOCKMGR_DLLDEF_HXX)
#define LOCKMANAGER_LOCKMGR_DLLDEF_HXX

#if defined (lockmgr_EXPORTS)
#  define LOCKMGR_INTERFACE __declspec (dllexport)
#else // lockmgr_EXPORTS
#  define LOCKMGR_INTERFACE __declspec (dllimport)
#endif // lockmgr_EXPORTS

#endif // LOCKMANAGER_LOCKMGR_DLLDEF_HXX
