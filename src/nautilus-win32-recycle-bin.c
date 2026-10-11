/* nautilus-win32-recycle-bin.c - Recycle Bin Shell namespace bridge */

#include "config.h"
#include "nautilus-win32-recycle-bin.h"

#ifdef G_OS_WIN32

#ifndef COBJMACROS
#define COBJMACROS
#endif
#include <windows.h>
#include <shlobj.h>
#include <shobjidl.h>

#include <glib/gi18n.h>

typedef struct
{
    GtkWindow *window;
    GtkListBox *list;
    GtkLabel *summary;
    GtkWidget *restore_button;
    GtkWidget *delete_button;
    gboolean com_initialized;
} RecycleBinWindow;

static void
recycle_bin_window_free (RecycleBinWindow *self)
{
    if (self->com_initialized)
    {
        CoUninitialize ();
    }
    g_free (self);
}

static gchar *
utf8_from_wide (const wchar_t *value)
{
    if (value == NULL)
    {
        return g_strdup ("");
    }

    return g_utf16_to_utf8 ((const gunichar2 *) value, -1, NULL, NULL, NULL);
}

static void recycle_bin_refresh (RecycleBinWindow *self);

static void
refresh_cb (GtkButton         *button,
            RecycleBinWindow *self)
{
    recycle_bin_refresh (self);
}

static void
update_selection_actions (RecycleBinWindow *self)
{
    gboolean has_selection = gtk_list_box_get_selected_row (self->list) != NULL;

    gtk_widget_set_sensitive (self->restore_button, has_selection);
    gtk_widget_set_sensitive (self->delete_button, has_selection);
}

static gboolean
invoke_shell_verb (PIDLIST_ABSOLUTE item_pidl,
                   const char       *verb)
{
    IShellFolder *parent = NULL;
    PCUITEMID_CHILD child = NULL;
    IContextMenu *menu = NULL;
    CMINVOKECOMMANDINFO command = { 0 };
    HRESULT hr;
    gboolean result = FALSE;

    hr = SHBindToParent (item_pidl, &IID_IShellFolder, (void **) &parent, &child);
    if (FAILED (hr))
    {
        return FALSE;
    }

    hr = IShellFolder_GetUIObjectOf (parent, NULL, 1, &child,
                                     &IID_IContextMenu, NULL, (void **) &menu);
    if (SUCCEEDED (hr))
    {
        command.cbSize = sizeof (command);
        command.lpVerb = verb;
        command.nShow = SW_SHOWNORMAL;
        result = SUCCEEDED (IContextMenu_InvokeCommand (menu, &command));
        IContextMenu_Release (menu);
    }

    IShellFolder_Release (parent);
    return result;
}

static PIDLIST_ABSOLUTE
selected_item_pidl (RecycleBinWindow *self)
{
    GtkListBoxRow *row = gtk_list_box_get_selected_row (self->list);

    if (row == NULL)
    {
        return NULL;
    }

    return g_object_get_data (G_OBJECT (row), "victoria-recycle-bin-pidl");
}

static void
restore_selected_cb (GtkButton         *button,
                     RecycleBinWindow *self)
{
    PIDLIST_ABSOLUTE item_pidl = selected_item_pidl (self);

    if (item_pidl != NULL && invoke_shell_verb (item_pidl, "undelete"))
    {
        recycle_bin_refresh (self);
    }
}

static void
delete_selected_cb (GtkButton         *button,
                    RecycleBinWindow *self)
{
    PIDLIST_ABSOLUTE item_pidl = selected_item_pidl (self);

    if (item_pidl != NULL && invoke_shell_verb (item_pidl, "delete"))
    {
        recycle_bin_refresh (self);
    }
}

static void
empty_cb (GtkButton         *button,
          RecycleBinWindow *self)
{
    if (SUCCEEDED (SHEmptyRecycleBinW (NULL, NULL, 0)))
    {
        recycle_bin_refresh (self);
    }
}

static void
row_selected_cb (GtkListBox     *list,
                 GtkListBoxRow  *row,
                 RecycleBinWindow *self)
{
    update_selection_actions (self);
}

static void
clear_rows (GtkListBox *list)
{
    GtkWidget *child;

    while ((child = gtk_widget_get_first_child (GTK_WIDGET (list))) != NULL)
    {
        gtk_list_box_remove (list, child);
    }
}

static void
append_item (RecycleBinWindow *self,
             PIDLIST_ABSOLUTE  item_pidl,
             const wchar_t    *display_name,
             const wchar_t    *type_name)
{
    GtkWidget *row;
    GtkWidget *box;
    GtkWidget *name;
    GtkWidget *type;
    g_autofree gchar *name_utf8 = utf8_from_wide (display_name);
    g_autofree gchar *type_utf8 = utf8_from_wide (type_name);

    row = gtk_list_box_row_new ();
    box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
    gtk_widget_set_margin_start (box, 12);
    gtk_widget_set_margin_end (box, 12);
    gtk_widget_set_margin_top (box, 8);
    gtk_widget_set_margin_bottom (box, 8);

    name = gtk_label_new (name_utf8);
    gtk_label_set_xalign (GTK_LABEL (name), 0.0f);
    gtk_label_set_ellipsize (GTK_LABEL (name), PANGO_ELLIPSIZE_END);
    gtk_box_append (GTK_BOX (box), name);

    type = gtk_label_new (type_utf8);
    gtk_label_set_xalign (GTK_LABEL (type), 0.0f);
    gtk_widget_add_css_class (type, "dim-label");
    gtk_box_append (GTK_BOX (box), type);

    gtk_list_box_row_set_child (GTK_LIST_BOX_ROW (row), box);
    g_object_set_data_full (G_OBJECT (row), "victoria-recycle-bin-pidl", item_pidl,
                            (GDestroyNotify) CoTaskMemFree);
    gtk_list_box_append (self->list, row);
}

static void
recycle_bin_refresh (RecycleBinWindow *self)
{
    PIDLIST_ABSOLUTE recycle_pidl = NULL;
    IShellFolder *desktop = NULL;
    IShellFolder *recycle_folder = NULL;
    IEnumIDList *enumerator = NULL;
    HRESULT hr;
    guint count = 0;

    clear_rows (self->list);

    hr = SHGetKnownFolderIDList (&FOLDERID_RecycleBinFolder, 0, NULL, &recycle_pidl);
    if (SUCCEEDED (hr))
    {
        hr = SHGetDesktopFolder (&desktop);
    }
    if (SUCCEEDED (hr))
    {
        hr = IShellFolder_BindToObject (desktop, recycle_pidl, NULL,
                                        &IID_IShellFolder, (void **) &recycle_folder);
    }
    if (SUCCEEDED (hr))
    {
        hr = IShellFolder_EnumObjects (recycle_folder, NULL,
                                       SHCONTF_FOLDERS | SHCONTF_NONFOLDERS,
                                       &enumerator);
    }

    if (SUCCEEDED (hr))
    {
        PITEMID_CHILD child;
        ULONG fetched;

        while (IEnumIDList_Next (enumerator, 1, &child, &fetched) == S_OK)
        {
            PIDLIST_ABSOLUTE item_pidl = ILCombine (recycle_pidl, child);
            SHFILEINFOW info = { 0 };

            CoTaskMemFree (child);
            if (item_pidl == NULL)
            {
                continue;
            }

            if (SHGetFileInfoW ((LPCWSTR) item_pidl, 0, &info, sizeof (info),
                                SHGFI_PIDL | SHGFI_DISPLAYNAME | SHGFI_TYPENAME) == 0)
            {
                CoTaskMemFree (item_pidl);
                continue;
            }

            append_item (self, item_pidl, info.szDisplayName, info.szTypeName);
            count++;
        }
    }

    if (enumerator != NULL)
        IEnumIDList_Release (enumerator);
    if (recycle_folder != NULL)
        IShellFolder_Release (recycle_folder);
    if (desktop != NULL)
        IShellFolder_Release (desktop);
    if (recycle_pidl != NULL)
        CoTaskMemFree (recycle_pidl);

    if (FAILED (hr))
    {
        gtk_label_set_text (self->summary, _("Unable to read the Windows Recycle Bin."));
    }
    else if (count == 0)
    {
        gtk_label_set_text (self->summary, _("Recycle Bin is empty"));
    }
    else
    {
        g_autofree gchar *text = g_strdup_printf (ngettext ("%u item", "%u items", count), count);
        gtk_label_set_text (self->summary, text);
    }

    update_selection_actions (self);
}

void
nautilus_win32_recycle_bin_show (GtkWindow *parent)
{
    RecycleBinWindow *self;
    GtkWidget *content;
    GtkWidget *toolbar;
    GtkWidget *scroller;
    GtkWidget *refresh_button;
    GtkWidget *empty_button;
    HRESULT com_result;

    com_result = CoInitializeEx (NULL, COINIT_APARTMENTTHREADED);
    if (FAILED (com_result) && com_result != RPC_E_CHANGED_MODE)
    {
        return;
    }

    self = g_new0 (RecycleBinWindow, 1);
    self->com_initialized = SUCCEEDED (com_result);
    self->window = GTK_WINDOW (gtk_window_new ());
    gtk_window_set_title (self->window, _("Recycle Bin"));
    gtk_window_set_default_size (self->window, 760, 520);
    if (parent != NULL)
        gtk_window_set_transient_for (self->window, parent);
    g_object_set_data_full (G_OBJECT (self->window), "victoria-recycle-bin-window", self,
                            (GDestroyNotify) recycle_bin_window_free);

    content = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    toolbar = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_widget_set_margin_start (toolbar, 12);
    gtk_widget_set_margin_end (toolbar, 12);
    gtk_widget_set_margin_top (toolbar, 12);
    gtk_widget_set_margin_bottom (toolbar, 12);

    refresh_button = gtk_button_new_with_label (_("Refresh"));
    g_signal_connect (refresh_button, "clicked", G_CALLBACK (refresh_cb), self);
    gtk_box_append (GTK_BOX (toolbar), refresh_button);

    self->restore_button = gtk_button_new_with_label (_("Restore"));
    g_signal_connect (self->restore_button, "clicked", G_CALLBACK (restore_selected_cb), self);
    gtk_box_append (GTK_BOX (toolbar), self->restore_button);

    self->delete_button = gtk_button_new_with_label (_("Delete Permanently"));
    gtk_widget_add_css_class (self->delete_button, "destructive-action");
    g_signal_connect (self->delete_button, "clicked", G_CALLBACK (delete_selected_cb), self);
    gtk_box_append (GTK_BOX (toolbar), self->delete_button);

    empty_button = gtk_button_new_with_label (_("Empty Recycle Bin"));
    gtk_widget_add_css_class (empty_button, "destructive-action");
    g_signal_connect (empty_button, "clicked", G_CALLBACK (empty_cb), self);
    gtk_box_append (GTK_BOX (toolbar), empty_button);
    gtk_box_append (GTK_BOX (content), toolbar);

    self->summary = GTK_LABEL (gtk_label_new (NULL));
    gtk_label_set_xalign (self->summary, 0.0f);
    gtk_widget_set_margin_start (GTK_WIDGET (self->summary), 12);
    gtk_widget_set_margin_bottom (GTK_WIDGET (self->summary), 6);
    gtk_box_append (GTK_BOX (content), GTK_WIDGET (self->summary));

    self->list = GTK_LIST_BOX (gtk_list_box_new ());
    gtk_list_box_set_selection_mode (self->list, GTK_SELECTION_SINGLE);
    g_signal_connect (self->list, "row-selected", G_CALLBACK (row_selected_cb), self);
    scroller = gtk_scrolled_window_new ();
    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (scroller), GTK_WIDGET (self->list));
    gtk_widget_set_vexpand (scroller, TRUE);
    gtk_box_append (GTK_BOX (content), scroller);

    gtk_window_set_child (self->window, content);
    recycle_bin_refresh (self);
    gtk_window_present (self->window);
}

#else

void
nautilus_win32_recycle_bin_show (GtkWindow *parent)
{
}

#endif
