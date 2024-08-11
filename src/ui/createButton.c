#include <gtk/gtk.h>

GtkWidget *create_button(const char *label, GCallback on_button_clicked,
                         gpointer data) {
  GtkWidget *button = gtk_button_new_with_label(label);
  g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), data);
  return button;
}
