#include "src/getMyIp.c"
#include <gtk/gtk.h>

static void showIpClicked(GtkButton *button, gpointer user_data) {
  GtkWidget *label;
  customAdd ip_info = findMyIP();
  label = gtk_label_new(ip_info.message);
  gtk_grid_attach(GTK_GRID(user_data), GTK_WIDGET(label), 1, 0, 1, 1);
}

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button;
  GtkCssProvider *provider;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Seamless Sync");

  provider = gtk_css_provider_new();
  gtk_css_provider_load_from_file(provider,
                                  g_file_new_for_path("../css/style.css"));
  gtk_style_context_add_provider_for_display(
      gtk_widget_get_display(GTK_WIDGET(window)), GTK_STYLE_PROVIDER(provider),
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  grid = gtk_grid_new();
  gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(grid));

  button = gtk_button_new_with_label("Show IP");
  g_signal_connect(button, "clicked", G_CALLBACK(showIpClicked), grid);
  gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(button), 0, 0, 1, 1);

  gtk_window_present(GTK_WINDOW(window));
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
