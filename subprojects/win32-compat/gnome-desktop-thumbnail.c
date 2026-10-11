#include "libgnome-desktop/gnome-desktop-thumbnail.h"

struct _GnomeDesktopThumbnailFactory {
    GObject parent_instance;
    GnomeDesktopThumbnailSize size;
    int target_size;
};

G_DEFINE_TYPE (GnomeDesktopThumbnailFactory, gnome_desktop_thumbnail_factory, G_TYPE_OBJECT)

static void
gnome_desktop_thumbnail_factory_init (GnomeDesktopThumbnailFactory *self)
{
    self->size = GNOME_DESKTOP_THUMBNAIL_SIZE_NORMAL;
    self->target_size = 128;
}

static void
gnome_desktop_thumbnail_factory_class_init (GnomeDesktopThumbnailFactoryClass *klass)
{
}

GnomeDesktopThumbnailFactory *
gnome_desktop_thumbnail_factory_new (GnomeDesktopThumbnailSize size)
{
    GnomeDesktopThumbnailFactory *factory = g_object_new (GNOME_DESKTOP_TYPE_THUMBNAIL_FACTORY, NULL);
    factory->size = size;
    switch (size) {
        case GNOME_DESKTOP_THUMBNAIL_SIZE_NORMAL: factory->target_size = 128; break;
        case GNOME_DESKTOP_THUMBNAIL_SIZE_LARGE: factory->target_size = 256; break;
        case GNOME_DESKTOP_THUMBNAIL_SIZE_XLARGE: factory->target_size = 512; break;
        case GNOME_DESKTOP_THUMBNAIL_SIZE_XXLARGE: factory->target_size = 1024; break;
        default: factory->target_size = 128; break;
    }
    return factory;
}

char *
gnome_desktop_thumbnail_path_for_uri (const char *uri, GnomeDesktopThumbnailSize size)
{
    return NULL;
}

gboolean
gnome_desktop_thumbnail_factory_can_thumbnail (GnomeDesktopThumbnailFactory *factory,
                                               const char *uri,
                                               const char *mime_type,
                                               time_t mtime)
{
    if (mime_type == NULL) return FALSE;
    return g_str_has_prefix (mime_type, "image/");
}

static void
generate_thumbnail_thread (GTask *task, gpointer source_obj, gpointer task_data, GCancellable *cancellable)
{
    const char *uri = (const char *) task_data;
    g_autoptr (GFile) file = g_file_new_for_uri (uri);
    g_autofree char *path = g_file_get_path (file);
    GnomeDesktopThumbnailFactory *factory = GNOME_DESKTOP_THUMBNAIL_FACTORY (source_obj);

    if (path == NULL) {
        g_task_return_new_error (task, G_IO_ERROR, G_IO_ERROR_NOT_FOUND, "No local path for URI");
        return;
    }

    GError *error = NULL;
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale (path, factory->target_size, factory->target_size, TRUE, &error);
    if (pixbuf != NULL) {
        g_task_return_pointer (task, pixbuf, g_object_unref);
    } else {
        g_task_return_error (task, error);
    }
}

void
gnome_desktop_thumbnail_factory_generate_thumbnail_async (GnomeDesktopThumbnailFactory *factory,
                                                          const char *uri,
                                                          const char *mime_type,
                                                          GCancellable *cancellable,
                                                          GAsyncReadyCallback callback,
                                                          gpointer user_data)
{
    GTask *task = g_task_new (factory, cancellable, callback, user_data);
    g_task_set_task_data (task, g_strdup (uri), g_free);
    g_task_run_in_thread (task, generate_thumbnail_thread);
    g_object_unref (task);
}

GdkPixbuf *
gnome_desktop_thumbnail_factory_generate_thumbnail_finish (GnomeDesktopThumbnailFactory *factory,
                                                           GAsyncResult *result,
                                                           GError **error)
{
    return g_task_propagate_pointer (G_TASK (result), error);
}

void
gnome_desktop_thumbnail_factory_save_thumbnail_async (GnomeDesktopThumbnailFactory *factory,
                                                      GdkPixbuf *thumbnail,
                                                      const char *uri,
                                                      time_t original_mtime,
                                                      GCancellable *cancellable,
                                                      GAsyncReadyCallback callback,
                                                      gpointer user_data)
{
    GTask *task = g_task_new (factory, cancellable, callback, user_data);
    g_task_return_boolean (task, TRUE);
    g_object_unref (task);
}

gboolean
gnome_desktop_thumbnail_factory_save_thumbnail_finish (GnomeDesktopThumbnailFactory *factory,
                                                       GAsyncResult *result,
                                                       GError **error)
{
    return g_task_propagate_boolean (G_TASK (result), error);
}

void
gnome_desktop_thumbnail_factory_create_failed_thumbnail_async (GnomeDesktopThumbnailFactory *factory,
                                                              const char *uri,
                                                              time_t mtime,
                                                              GCancellable *cancellable,
                                                              GAsyncReadyCallback callback,
                                                              gpointer user_data)
{
    GTask *task = g_task_new (factory, cancellable, callback, user_data);
    g_task_return_boolean (task, TRUE);
    g_object_unref (task);
}

gboolean
gnome_desktop_thumbnail_factory_create_failed_thumbnail_finish (GnomeDesktopThumbnailFactory *factory,
                                                               GAsyncResult *result,
                                                               GError **error)
{
    return g_task_propagate_boolean (G_TASK (result), error);
}
