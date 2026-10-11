#include "libportal/portal.h"
#include <windows.h>

struct _XdpPortal {
    GObject parent_instance;
};

struct _XdpParent {
    GtkWindow *window;
};

G_DEFINE_TYPE (XdpPortal, xdp_portal, G_TYPE_OBJECT)

static void
xdp_portal_init (XdpPortal *self)
{
}

static void
xdp_portal_class_init (XdpPortalClass *klass)
{
}

XdpPortal *
xdp_portal_new (void)
{
    return g_object_new (XDP_TYPE_PORTAL, NULL);
}

XdpPortal *
xdp_portal_initable_new (GError **error)
{
    return xdp_portal_new ();
}

gboolean
xdp_portal_running_under_sandbox (void)
{
    return FALSE;
}

XdpParent *
xdp_parent_new_gtk (GtkWindow *window)
{
    XdpParent *parent = g_new0 (XdpParent, 1);
    parent->window = window;
    return parent;
}

void
xdp_parent_free (XdpParent *parent)
{
    g_free (parent);
}

void
xdp_portal_open_uri (XdpPortal *portal,
                     XdpParent *parent,
                     const char *uri,
                     XdpOpenUriFlags flags,
                     GCancellable *cancellable,
                     GAsyncReadyCallback callback,
                     gpointer user_data)
{
    GTask *task = g_task_new (portal, cancellable, callback, user_data);
    GError *error = NULL;
    gboolean res = g_app_info_launch_default_for_uri (uri, NULL, &error);
    if (res) {
        g_task_return_boolean (task, TRUE);
    } else {
        g_task_return_error (task, error);
    }
    g_object_unref (task);
}

gboolean
xdp_portal_open_uri_finish (XdpPortal *portal, GAsyncResult *result, GError **error)
{
    return g_task_propagate_boolean (G_TASK (result), error);
}

void
xdp_portal_compose_email (XdpPortal *portal,
                          XdpParent *parent,
                          const char * const *addresses,
                          const char * const *cc,
                          const char * const *bcc,
                          const char *subject,
                          const char *body,
                          const char * const *attachments,
                          XdpEmailFlags flags,
                          GCancellable *cancellable,
                          GAsyncReadyCallback callback,
                          gpointer user_data)
{
    GTask *task = g_task_new (portal, cancellable, callback, user_data);
    g_task_return_boolean (task, TRUE);
    g_object_unref (task);
}

gboolean
xdp_portal_compose_email_finish (XdpPortal *portal, GAsyncResult *result, GError **error)
{
    return g_task_propagate_boolean (G_TASK (result), error);
}

void
xdp_portal_set_wallpaper (XdpPortal *portal,
                          XdpParent *parent,
                          const char *uri,
                          XdpWallpaperFlags flags,
                          GCancellable *cancellable,
                          GAsyncReadyCallback callback,
                          gpointer user_data)
{
    GTask *task = g_task_new (portal, cancellable, callback, user_data);
    g_autoptr (GFile) file = g_file_new_for_uri (uri);
    g_autofree char *path = g_file_get_path (file);
    if (path != NULL) {
        wchar_t *wpath = g_utf8_to_utf16 (path, -1, NULL, NULL, NULL);
        if (wpath != NULL) {
            SystemParametersInfoW (SPI_SETDESKWALLPAPER, 0, (void*)wpath, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
            g_free (wpath);
            g_task_return_boolean (task, TRUE);
            g_object_unref (task);
            return;
        }
    }
    g_task_return_new_error (task, G_IO_ERROR, G_IO_ERROR_FAILED, "Failed to set wallpaper");
    g_object_unref (task);
}

gboolean
xdp_portal_set_wallpaper_finish (XdpPortal *portal, GAsyncResult *result, GError **error)
{
    return g_task_propagate_boolean (G_TASK (result), error);
}
