#if ! defined (LOCKMANAGER_LOCKMGR_DLLDEF_HXX)
#define LOCKMANAGER_LOCKMGR_DLLDEF_HXX

// \todo __declspec will work only with recent GCC and MSVC compilers.

#if defined (lockmgr_EXPORTS)

#  if defined (WIN32) || defined (__CYGWIN__)
#    define LOCKMGR_INTERFACE __declspec (dllexport)
#  else
#    define LOCKMGR_INTERFACE
#  endif

#else // lockmgr_EXPORTS

#  if defined (WIN32) || defined (__CYGWIN__)
#    define LOCKMGR_INTERFACE __declspec (dllimport)
#  else
#    define LOCKMGR_INTERFACE
#  endif

#endif // lockmgr_EXPORTS

#endif // LOCKMANAGER_LOCKMGR_DLLDEF_HXX
