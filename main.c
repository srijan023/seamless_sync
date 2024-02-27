#include <gtk/gtk.h>

static void activate(GtkApplication *app, gpointer user_data) {
  GtkBuilder *builder;
  GtkCssProvider *provider;

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "../ui/builder.ui", NULL);

  GObject *window = gtk_builder_get_object(builder, "window");
  gtk_window_set_application(GTK_WINDOW(window), app);

  provider = gtk_css_provider_new();
  gtk_css_provider_load_from_file(provider,
                                  g_file_new_for_path("../css/style.css"));
  gtk_style_context_add_provider_for_display(
      gtk_widget_get_display(GTK_WIDGET(window)), GTK_STYLE_PROVIDER(provider),
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gtk_widget_set_visible(GTK_WIDGET(window), TRUE);
  g_object_unref(builder);
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("org.seamlessSync", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);

  g_object_unref(app);

  return status;
}
