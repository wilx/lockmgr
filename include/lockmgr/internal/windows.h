#if !defined (LOCKMANAGER_LOCKMGR_INTERNAL_WINDOWS_H)
#define LOCKMANAGER_LOCKMGR_INTERNAL_WINDOWS_H

// Windows defines some macros that interfere with regular C++
// source. We use this header to include windows.h and undefine such
// conflicting macros.

#include <windows.h>

#undef min
#undef max

#endif // LOCKMANAGER_LOCKMGR_INTERNAL_WINDOWS_H
