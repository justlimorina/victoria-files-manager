#pragma once

#include <gio/gio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GDesktopAppInfo GDesktopAppInfo;

#define G_TYPE_DESKTOP_APP_INFO (g_app_info_get_type ())
#define G_DESKTOP_APP_INFO(o) ((GDesktopAppInfo *) (o))
#define G_IS_DESKTOP_APP_INFO(o) (G_IS_APP_INFO (o))

static inline GDesktopAppInfo *
g_desktop_app_info_new_from_filename (const char *filename)
{
    if (filename == NULL)
        return NULL;
    return (GDesktopAppInfo *) g_app_info_create_from_commandline (filename, NULL, G_APP_INFO_CREATE_NONE, NULL);
}

static inline GDesktopAppInfo *
g_desktop_app_info_new (const char *desktop_id)
{
    if (desktop_id == NULL)
        return NULL;
    return (GDesktopAppInfo *) g_app_info_create_from_commandline (desktop_id, NULL, G_APP_INFO_CREATE_NONE, NULL);
}

#ifdef __cplusplus
}
#endif
