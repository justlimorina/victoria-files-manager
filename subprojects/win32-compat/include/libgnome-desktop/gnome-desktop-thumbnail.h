#pragma once

#include <glib-object.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gio/gio.h>

G_BEGIN_DECLS

typedef enum {
    GNOME_DESKTOP_THUMBNAIL_SIZE_NORMAL,
    GNOME_DESKTOP_THUMBNAIL_SIZE_LARGE,
    GNOME_DESKTOP_THUMBNAIL_SIZE_XLARGE,
    GNOME_DESKTOP_THUMBNAIL_SIZE_XXLARGE
} GnomeDesktopThumbnailSize;

#define GNOME_DESKTOP_TYPE_THUMBNAIL_FACTORY (gnome_desktop_thumbnail_factory_get_type ())
G_DECLARE_FINAL_TYPE (GnomeDesktopThumbnailFactory, gnome_desktop_thumbnail_factory, GNOME_DESKTOP, THUMBNAIL_FACTORY, GObject)

GnomeDesktopThumbnailFactory *gnome_desktop_thumbnail_factory_new (GnomeDesktopThumbnailSize size);
char *gnome_desktop_thumbnail_path_for_uri (const char *uri, GnomeDesktopThumbnailSize size);
gboolean gnome_desktop_thumbnail_factory_can_thumbnail (GnomeDesktopThumbnailFactory *factory,
                                                        const char *uri,
                                                        const char *mime_type,
                                                        time_t mtime);

void gnome_desktop_thumbnail_factory_generate_thumbnail_async (GnomeDesktopThumbnailFactory *factory,
                                                               const char *uri,
                                                               const char *mime_type,
                                                               GCancellable *cancellable,
                                                               GAsyncReadyCallback callback,
                                                               gpointer user_data);
GdkPixbuf *gnome_desktop_thumbnail_factory_generate_thumbnail_finish (GnomeDesktopThumbnailFactory *factory,
                                                                      GAsyncResult *result,
                                                                      GError **error);

void gnome_desktop_thumbnail_factory_save_thumbnail_async (GnomeDesktopThumbnailFactory *factory,
                                                           GdkPixbuf *thumbnail,
                                                           const char *uri,
                                                           time_t original_mtime,
                                                           GCancellable *cancellable,
                                                           GAsyncReadyCallback callback,
                                                           gpointer user_data);
gboolean gnome_desktop_thumbnail_factory_save_thumbnail_finish (GnomeDesktopThumbnailFactory *factory,
                                                                GAsyncResult *result,
                                                                GError **error);

void gnome_desktop_thumbnail_factory_create_failed_thumbnail_async (GnomeDesktopThumbnailFactory *factory,
                                                                    const char *uri,
                                                                    time_t mtime,
                                                                    GCancellable *cancellable,
                                                                    GAsyncReadyCallback callback,
                                                                    gpointer user_data);
gboolean gnome_desktop_thumbnail_factory_create_failed_thumbnail_finish (GnomeDesktopThumbnailFactory *factory,
                                                                         GAsyncResult *result,
                                                                         GError **error);

G_END_DECLS
