#include "mainWindow.c"
#include <gtk/gtk.h>

void activate(GtkApplication *app, gpointer data) {
  GtkWidget *window = create_main_window(app);
  gtk_window_present(GTK_WINDOW(window));
}