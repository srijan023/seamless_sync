#include "activate.h"
#include "gio/gio.h"

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  g_print("Application is launched\n");

  app = gtk_application_new("org.seamlessSync", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
