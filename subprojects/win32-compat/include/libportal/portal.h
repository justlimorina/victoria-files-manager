#pragma once
#include <glib-object.h>
#include <gtk/gtk.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define XDP_TYPE_PORTAL (xdp_portal_get_type ())
G_DECLARE_FINAL_TYPE (XdpPortal, xdp_portal, XDP, PORTAL, GObject)

typedef struct _XdpParent XdpParent;

typedef enum {
    XDP_OPEN_URI_FLAG_NONE = 0,
    XDP_OPEN_URI_FLAG_ASK = 1 << 0
} XdpOpenUriFlags;

typedef enum {
    XDP_EMAIL_FLAG_NONE = 0
} XdpEmailFlags;

typedef enum {
    XDP_WALLPAPER_FLAG_NONE = 0,
    XDP_WALLPAPER_FLAG_PREVIEW = 1 << 0,
    XDP_WALLPAPER_FLAG_BACKGROUND = 1 << 1
} XdpWallpaperFlags;

XdpPortal *xdp_portal_new (void);
XdpPortal *xdp_portal_initable_new (GError **error);
gboolean xdp_portal_running_under_sandbox (void);

XdpParent *xdp_parent_new_gtk (GtkWindow *window);
void xdp_parent_free (XdpParent *parent);

void xdp_portal_open_uri (XdpPortal *portal,
                          XdpParent *parent,
                          const char *uri,
                          XdpOpenUriFlags flags,
                          GCancellable *cancellable,
                          GAsyncReadyCallback callback,
                          gpointer user_data);
gboolean xdp_portal_open_uri_finish (XdpPortal *portal, GAsyncResult *result, GError **error);

void xdp_portal_compose_email (XdpPortal *portal,
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
                               gpointer user_data);
gboolean xdp_portal_compose_email_finish (XdpPortal *portal, GAsyncResult *result, GError **error);

void xdp_portal_set_wallpaper (XdpPortal *portal,
                               XdpParent *parent,
                               const char *uri,
                               XdpWallpaperFlags flags,
                               GCancellable *cancellable,
                               GAsyncReadyCallback callback,
                               gpointer user_data);
gboolean xdp_portal_set_wallpaper_finish (XdpPortal *portal, GAsyncResult *result, GError **error);

G_END_DECLS
