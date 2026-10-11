#pragma once

#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define TRACKER_TYPE_SPARQL_CONNECTION (tracker_sparql_connection_get_type ())
G_DECLARE_DERIVABLE_TYPE (TrackerSparqlConnection, tracker_sparql_connection, TRACKER, SPARQL_CONNECTION, GObject)

struct _TrackerSparqlConnectionClass {
    GObjectClass parent_class;
};

#define TRACKER_TYPE_NOTIFIER (tracker_notifier_get_type ())
G_DECLARE_FINAL_TYPE (TrackerNotifier, tracker_notifier, TRACKER, NOTIFIER, GObject)

#define TRACKER_TYPE_SPARQL_CURSOR (tracker_sparql_cursor_get_type ())
G_DECLARE_DERIVABLE_TYPE (TrackerSparqlCursor, tracker_sparql_cursor, TRACKER, SPARQL_CURSOR, GObject)

struct _TrackerSparqlCursorClass {
    GObjectClass parent_class;
};

#define TRACKER_TYPE_SPARQL_STATEMENT (tracker_sparql_statement_get_type ())
G_DECLARE_DERIVABLE_TYPE (TrackerSparqlStatement, tracker_sparql_statement, TRACKER, SPARQL_STATEMENT, GObject)

struct _TrackerSparqlStatementClass {
    GObjectClass parent_class;
};

typedef enum {
    TRACKER_SPARQL_CONNECTION_FLAGS_NONE = 0,
    TRACKER_SPARQL_CONNECTION_FLAGS_READONLY = 1 << 0,
    TRACKER_SPARQL_CONNECTION_FLAGS_ANONYMOUS = 1 << 1
} TrackerSparqlConnectionFlags;

typedef enum {
    TRACKER_NOTIFIER_EVENT_CREATE,
    TRACKER_NOTIFIER_EVENT_DELETE,
    TRACKER_NOTIFIER_EVENT_UPDATE
} TrackerNotifierEventType;

typedef struct _TrackerNotifierEvent TrackerNotifierEvent;

char *tracker_sparql_escape_string (const char *literal);

TrackerSparqlConnection *tracker_sparql_connection_new (TrackerSparqlConnectionFlags flags,
                                                        GFile *store,
                                                        GFile *ontology,
                                                        GCancellable *cancellable,
                                                        GError **error);

void tracker_sparql_connection_bus_new_async (const char *service_name,
                                              const char *object_path,
                                              GDBusConnection *bus,
                                              GCancellable *cancellable,
                                              GAsyncReadyCallback callback,
                                              gpointer user_data);
TrackerSparqlConnection *tracker_sparql_connection_bus_new_finish (GAsyncResult *result, GError **error);
TrackerSparqlConnection *tracker_sparql_connection_bus_new (const char *service_name,
                                                            GDBusConnection *bus,
                                                            GCancellable *cancellable,
                                                            GError **error);
TrackerSparqlConnection *tracker_sparql_connection_new_finish (GAsyncResult *result, GError **error);

void tracker_sparql_connection_query_async (TrackerSparqlConnection *connection,
                                            const char *query,
                                            GCancellable *cancellable,
                                            GAsyncReadyCallback callback,
                                            gpointer user_data);
TrackerSparqlCursor *tracker_sparql_connection_query_finish (TrackerSparqlConnection *connection,
                                                             GAsyncResult *result,
                                                             GError **error);

TrackerSparqlStatement *tracker_sparql_connection_query_statement (TrackerSparqlConnection *connection,
                                                                   const gchar *query,
                                                                   GCancellable *cancellable,
                                                                   GError **error);

TrackerNotifier *tracker_sparql_connection_create_notifier (TrackerSparqlConnection *connection);

void tracker_sparql_connection_update_async (TrackerSparqlConnection *connection,
                                             const char *query,
                                             GCancellable *cancellable,
                                             GAsyncReadyCallback callback,
                                             gpointer user_data);
gboolean tracker_sparql_connection_update_finish (TrackerSparqlConnection *connection,
                                                  GAsyncResult *result,
                                                  GError **error);

void tracker_sparql_statement_bind_string (TrackerSparqlStatement *stmt,
                                           const gchar *name,
                                           const gchar *value);
TrackerSparqlCursor *tracker_sparql_statement_execute (TrackerSparqlStatement *stmt,
                                                       GCancellable *cancellable,
                                                       GError **error);
void tracker_sparql_statement_execute_async (TrackerSparqlStatement *stmt,
                                             GCancellable *cancellable,
                                             GAsyncReadyCallback callback,
                                             gpointer user_data);
TrackerSparqlCursor *tracker_sparql_statement_execute_finish (TrackerSparqlStatement *stmt,
                                                              GAsyncResult *result,
                                                              GError **error);

gboolean tracker_sparql_cursor_next (TrackerSparqlCursor *cursor,
                                     GCancellable *cancellable,
                                     GError **error);
void tracker_sparql_cursor_next_async (TrackerSparqlCursor *cursor,
                                       GCancellable *cancellable,
                                       GAsyncReadyCallback callback,
                                       gpointer user_data);
gboolean tracker_sparql_cursor_next_finish (TrackerSparqlCursor *cursor,
                                             GAsyncResult *result,
                                             GError **error);

const char *tracker_sparql_cursor_get_string (TrackerSparqlCursor *cursor, guint column, gint64 *length);
gboolean tracker_sparql_cursor_get_boolean (TrackerSparqlCursor *cursor, guint column);
gdouble tracker_sparql_cursor_get_double (TrackerSparqlCursor *cursor, guint column);
gint64 tracker_sparql_cursor_get_integer (TrackerSparqlCursor *cursor, guint column);

void tracker_sparql_cursor_close (TrackerSparqlCursor *cursor);
void tracker_sparql_cursor_close_async (TrackerSparqlCursor *cursor,
                                        GCancellable *cancellable,
                                        GAsyncReadyCallback callback,
                                        gpointer user_data);
gboolean tracker_sparql_cursor_close_finish (TrackerSparqlCursor *cursor,
                                             GAsyncResult *result,
                                             GError **error);

TrackerNotifierEventType tracker_notifier_event_get_event_type (TrackerNotifierEvent *event);
const char *tracker_notifier_event_get_urn (TrackerNotifierEvent *event);

G_END_DECLS
