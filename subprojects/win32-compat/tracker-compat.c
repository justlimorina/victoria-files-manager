#include "libtracker-sparql/tracker-sparql.h"

struct _TrackerNotifier { GObject parent; };

G_DEFINE_TYPE (TrackerSparqlConnection, tracker_sparql_connection, G_TYPE_OBJECT)
static void tracker_sparql_connection_init (TrackerSparqlConnection *self) { (void)self; }
static void tracker_sparql_connection_class_init (TrackerSparqlConnectionClass *klass) { (void)klass; }

G_DEFINE_TYPE (TrackerNotifier, tracker_notifier, G_TYPE_OBJECT)
static void tracker_notifier_init (TrackerNotifier *self) { (void)self; }
static void tracker_notifier_class_init (TrackerNotifierClass *klass) { (void)klass; }

G_DEFINE_TYPE (TrackerSparqlCursor, tracker_sparql_cursor, G_TYPE_OBJECT)
static void tracker_sparql_cursor_init (TrackerSparqlCursor *self) { (void)self; }
static void tracker_sparql_cursor_class_init (TrackerSparqlCursorClass *klass) { (void)klass; }

G_DEFINE_TYPE (TrackerSparqlStatement, tracker_sparql_statement, G_TYPE_OBJECT)
static void tracker_sparql_statement_init (TrackerSparqlStatement *self) { (void)self; }
static void tracker_sparql_statement_class_init (TrackerSparqlStatementClass *klass) { (void)klass; }

char *
tracker_sparql_escape_string (const char *literal)
{
    return g_strdup (literal);
}

TrackerSparqlConnection *
tracker_sparql_connection_new (TrackerSparqlConnectionFlags flags,
                               GFile *store,
                               GFile *ontology,
                               GCancellable *cancellable,
                               GError **error)
{
    (void)flags; (void)store; (void)ontology; (void)cancellable;
    if (error) {
        *error = g_error_new_literal (G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED, "Tracker not supported on Windows");
    }
    return NULL;
}

void
tracker_sparql_connection_bus_new_async (const char *service_name,
                                         const char *object_path,
                                         GDBusConnection *bus,
                                         GCancellable *cancellable,
                                         GAsyncReadyCallback callback,
                                         gpointer user_data)
{
    (void)service_name; (void)object_path; (void)bus;
    GTask *task = g_task_new (NULL, cancellable, callback, user_data);
    g_task_return_new_error (task, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED, "Tracker not supported on Windows");
    g_object_unref (task);
}

TrackerSparqlConnection *
tracker_sparql_connection_bus_new_finish (GAsyncResult *result, GError **error)
{
    return g_task_propagate_pointer (G_TASK (result), error);
}

TrackerSparqlConnection *
tracker_sparql_connection_bus_new (const char *service_name,
                                   GDBusConnection *bus,
                                   GCancellable *cancellable,
                                   GError **error)
{
    (void)service_name; (void)bus; (void)cancellable;
    if (error) {
        *error = g_error_new_literal (G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED, "Tracker not supported on Windows");
    }
    return NULL;
}

TrackerSparqlConnection *
tracker_sparql_connection_new_finish (GAsyncResult *result, GError **error)
{
    return g_task_propagate_pointer (G_TASK (result), error);
}

void
tracker_sparql_connection_query_async (TrackerSparqlConnection *connection,
                                       const char *query,
                                       GCancellable *cancellable,
                                       GAsyncReadyCallback callback,
                                       gpointer user_data)
{
    (void)query;
    GTask *task = g_task_new (connection, cancellable, callback, user_data);
    g_task_return_new_error (task, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED, "Tracker not supported on Windows");
    g_object_unref (task);
}

TrackerSparqlCursor *
tracker_sparql_connection_query_finish (TrackerSparqlConnection *connection,
                                        GAsyncResult *result,
                                        GError **error)
{
    (void)connection;
    return g_task_propagate_pointer (G_TASK (result), error);
}

TrackerSparqlStatement *
tracker_sparql_connection_query_statement (TrackerSparqlConnection *connection,
                                           const gchar *query,
                                           GCancellable *cancellable,
                                           GError **error)
{
    (void)connection; (void)query; (void)cancellable;
    if (error) {
        *error = g_error_new_literal (G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED, "Tracker not supported on Windows");
    }
    return NULL;
}

TrackerNotifier *
tracker_sparql_connection_create_notifier (TrackerSparqlConnection *connection)
{
    (void)connection;
    return g_object_new (TRACKER_TYPE_NOTIFIER, NULL);
}

void
tracker_sparql_connection_update_async (TrackerSparqlConnection *connection,
                                        const char *query,
                                        GCancellable *cancellable,
                                        GAsyncReadyCallback callback,
                                        gpointer user_data)
{
    (void)query;
    GTask *task = g_task_new (connection, cancellable, callback, user_data);
    g_task_return_new_error (task, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED, "Tracker not supported on Windows");
    g_object_unref (task);
}

gboolean
tracker_sparql_connection_update_finish (TrackerSparqlConnection *connection,
                                         GAsyncResult *result,
                                         GError **error)
{
    (void)connection;
    return g_task_propagate_boolean (G_TASK (result), error);
}

void
tracker_sparql_statement_bind_string (TrackerSparqlStatement *stmt,
                                      const gchar *name,
                                      const gchar *value)
{
    (void)stmt; (void)name; (void)value;
}

TrackerSparqlCursor *
tracker_sparql_statement_execute (TrackerSparqlStatement *stmt,
                                  GCancellable *cancellable,
                                  GError **error)
{
    (void)stmt; (void)cancellable;
    if (error) {
        *error = g_error_new_literal (G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED, "Tracker not supported on Windows");
    }
    return NULL;
}

void
tracker_sparql_statement_execute_async (TrackerSparqlStatement *stmt,
                                        GCancellable *cancellable,
                                        GAsyncReadyCallback callback,
                                        gpointer user_data)
{
    GTask *task = g_task_new (stmt, cancellable, callback, user_data);
    g_task_return_new_error (task, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED, "Tracker not supported on Windows");
    g_object_unref (task);
}

TrackerSparqlCursor *
tracker_sparql_statement_execute_finish (TrackerSparqlStatement *stmt,
                                         GAsyncResult *result,
                                         GError **error)
{
    (void)stmt;
    return g_task_propagate_pointer (G_TASK (result), error);
}

gboolean
tracker_sparql_cursor_next (TrackerSparqlCursor *cursor,
                            GCancellable *cancellable,
                            GError **error)
{
    (void)cursor; (void)cancellable; (void)error;
    return FALSE;
}

void
tracker_sparql_cursor_next_async (TrackerSparqlCursor *cursor,
                                  GCancellable *cancellable,
                                  GAsyncReadyCallback callback,
                                  gpointer user_data)
{
    GTask *task = g_task_new (cursor, cancellable, callback, user_data);
    g_task_return_boolean (task, FALSE);
    g_object_unref (task);
}

gboolean
tracker_sparql_cursor_next_finish (TrackerSparqlCursor *cursor,
                                   GAsyncResult *result,
                                   GError **error)
{
    (void)cursor;
    return g_task_propagate_boolean (G_TASK (result), error);
}

const char *
tracker_sparql_cursor_get_string (TrackerSparqlCursor *cursor, guint column, gint64 *length)
{
    (void)cursor; (void)column;
    if (length) *length = 0;
    return "";
}

gboolean
tracker_sparql_cursor_get_boolean (TrackerSparqlCursor *cursor, guint column)
{
    (void)cursor; (void)column;
    return FALSE;
}

gdouble
tracker_sparql_cursor_get_double (TrackerSparqlCursor *cursor, guint column)
{
    (void)cursor; (void)column;
    return 0.0;
}

gint64
tracker_sparql_cursor_get_integer (TrackerSparqlCursor *cursor, guint column)
{
    (void)cursor; (void)column;
    return 0;
}

void
tracker_sparql_cursor_close (TrackerSparqlCursor *cursor)
{
    (void)cursor;
}

void
tracker_sparql_cursor_close_async (TrackerSparqlCursor *cursor,
                                   GCancellable *cancellable,
                                   GAsyncReadyCallback callback,
                                   gpointer user_data)
{
    GTask *task = g_task_new (cursor, cancellable, callback, user_data);
    g_task_return_boolean (task, TRUE);
    g_object_unref (task);
}

gboolean
tracker_sparql_cursor_close_finish (TrackerSparqlCursor *cursor,
                                    GAsyncResult *result,
                                    GError **error)
{
    (void)cursor;
    return g_task_propagate_boolean (G_TASK (result), error);
}

TrackerNotifierEventType
tracker_notifier_event_get_event_type (TrackerNotifierEvent *event)
{
    (void)event;
    return TRACKER_NOTIFIER_EVENT_UPDATE;
}

const char *
tracker_notifier_event_get_urn (TrackerNotifierEvent *event)
{
    (void)event;
    return "";
}
