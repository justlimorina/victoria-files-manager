#pragma once

#ifdef _WIN32

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#include <sys/types.h>

#ifndef _UID_T_DEFINED
#define _UID_T_DEFINED
typedef unsigned int uid_t;
#endif

#ifndef _GID_T_DEFINED
#define _GID_T_DEFINED
typedef unsigned int gid_t;
#endif

#ifndef _UINT_DEFINED
#define _UINT_DEFINED
typedef unsigned int uint;
#endif

#ifndef _USHORT_DEFINED
#define _USHORT_DEFINED
typedef unsigned short ushort;
#endif

#ifndef _ULONG_DEFINED
#define _ULONG_DEFINED
typedef unsigned long ulong;
#endif

/* POSIX permission bits missing on Windows */
#ifndef S_ISUID
#define S_ISUID 04000
#endif
#ifndef S_ISGID
#define S_ISGID 02000
#endif
#ifndef S_ISVTX
#define S_ISVTX 01000
#endif

#ifndef S_IRWXU
#define S_IRWXU 00700
#endif
#ifndef S_IRUSR
#define S_IRUSR 00400
#endif
#ifndef S_IWUSR
#define S_IWUSR 00200
#endif
#ifndef S_IXUSR
#define S_IXUSR 00100
#endif

#ifndef S_IRWXG
#define S_IRWXG 00070
#endif
#ifndef S_IRGRP
#define S_IRGRP 00040
#endif
#ifndef S_IWGRP
#define S_IWGRP 00020
#endif
#ifndef S_IXGRP
#define S_IXGRP 00010
#endif

#ifndef S_IRWXO
#define S_IRWXO 00007
#endif
#ifndef S_IROTH
#define S_IROTH 00004
#endif
#ifndef S_IWOTH
#define S_IWOTH 00002
#endif
#ifndef S_IXOTH
#define S_IXOTH 00001
#endif

/* POSIX user/group ID identity functions */
static inline uid_t getuid (void) { return 1000; }
static inline uid_t geteuid (void) { return 1000; }
static inline gid_t getgid (void) { return 1000; }
static inline gid_t getegid (void) { return 1000; }

/* POSIX pathconf */
#ifndef _PC_NAME_MAX
#define _PC_NAME_MAX 3
#endif
#ifndef _PC_PATH_MAX
#define _PC_PATH_MAX 4
#endif

static inline long pathconf (const char *path, int name)
{
    (void) path;
    if (name == _PC_NAME_MAX)
        return 255;
    if (name == _PC_PATH_MAX)
        return 32767;
    return -1;
}

#endif /* _WIN32 */
