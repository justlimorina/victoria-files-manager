#pragma once

#include <sys/types.h>

#ifndef uid_t
typedef unsigned int uid_t;
#endif
#ifndef gid_t
typedef unsigned int gid_t;
#endif

#ifndef NGROUPS_MAX
#define NGROUPS_MAX 32
#endif

struct group {
    char *gr_name;
    char *gr_passwd;
    gid_t gr_gid;
    char **gr_mem;
};

static inline struct group *getgrnam (const char *name) { (void)name; return (void *) 0; }
static inline struct group *getgrgid (gid_t gid) { (void)gid; return (void *) 0; }
static inline struct group *getgrent (void) { return (void *) 0; }
static inline void setgrent (void) {}
static inline void endgrent (void) {}
static inline int getgroups (int size, gid_t list[]) { (void)size; (void)list; return 0; }
