#include <gtk/gtk.h>

void while_device_clicked(GtkListBox *listbox, GtkListBoxRow *row,
                          gpointer user_data) {
  g_print("clicked\n");
}
