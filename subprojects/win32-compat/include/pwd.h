#pragma once

#include <sys/types.h>

#ifndef uid_t
typedef unsigned int uid_t;
#endif
#ifndef gid_t
typedef unsigned int gid_t;
#endif

struct passwd {
    char *pw_name;
    char *pw_passwd;
    uid_t pw_uid;
    gid_t pw_gid;
    char *pw_gecos;
    char *pw_dir;
    char *pw_shell;
};

static inline struct passwd *getpwnam (const char *name) { (void)name; return (void *) 0; }
static inline struct passwd *getpwuid (uid_t uid) { (void)uid; return (void *) 0; }
static inline struct passwd *getpwent (void) { return (void *) 0; }
static inline void setpwent (void) {}
static inline void endpwent (void) {}
