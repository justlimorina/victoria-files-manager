/* Native Windows Recycle Bin window. */
#pragma once

#include <gtk/gtk.h>

G_BEGIN_DECLS

/* Opens the Windows Recycle Bin in a Victoria-owned GTK window. */
void nautilus_win32_recycle_bin_show (GtkWindow *parent);

G_END_DECLS
