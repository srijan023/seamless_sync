#include <gtk/gtk.h>

void remove_old_child(GtkWindow *window) {
  // GtkWindow *window = GTK_WINDOW(old_window);
  GtkWidget *old_child = gtk_window_get_child(window);
  if (old_child) {
    gtk_window_set_child(window, NULL); // Remove old child
    gtk_widget_unparent(old_child);     // Unparent old child
  }
}
