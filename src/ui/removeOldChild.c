#include <gtk/gtk.h>

void remove_old_child(GtkWindow *window) {
  GtkWidget *old_child = gtk_window_get_child(window);
  if (old_child && GTK_IS_WIDGET(old_child)) {
    gtk_widget_unparent(old_child);     // Unparent old child
    gtk_window_set_child(window, NULL); // Remove old child
  }
}
