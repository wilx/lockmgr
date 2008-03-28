#if ! defined (LOCKMANAGER_LOCKMGR_INTERNAL_CONFIG_HXX)
#define LOCKMANAGER_LOCKMGR_INTERNAL_CONFIG_HXX

#if defined (__unix) || defined (__unix__)
#  define LOCKMANAGER_UNIX 1
#endif

#if defined (__CYGWIN__) || defined (__CYGWIN32__)
#  if ! defined (WIN32)
#    define WIN32
#  endif
#endif

#endif // LOCKMANAGER_LOCKMGR_INTERNAL_CONFIG_HXX
