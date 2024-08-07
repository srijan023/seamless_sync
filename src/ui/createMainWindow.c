#include "button.c"
#include "gtk/gtk.h"
#include "headerBar.c"
#include "image.c"
#include "onStartClicked.c"
#include "wrappedLabel.c"

gboolean check_network_status(gpointer user_data) {
  NetworkStatusData *app_data = (NetworkStatusData *)user_data;

  g_print("Running check network status.\n");
  if (findMyIP().status != -1) {
    gtk_label_set_text(GTK_LABEL(app_data->label),
                       "You are ready to start the SSDP Listener. Please click "
                       "the  'Start' button below.");
    gtk_widget_set_sensitive(app_data->button, TRUE);
  } else {
    gtk_label_set_text(
        GTK_LABEL(app_data->label),
        "Your device needs to be connected to any network before you proceed. "
        "Please check the network connection of your device");
    gtk_widget_set_sensitive(app_data->button, FALSE);
  }
  return G_SOURCE_CONTINUE;
}

// Create a main window i.e. first window to show when application launches
GtkWidget *create_main_window(GtkApplication *app) {
  StartData *start_data = g_new0(StartData, 1);
  NetworkStatusData *app_data = g_new0(NetworkStatusData, 1);

  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Seamless Sync");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 600);
  g_print("Window of 400x600 created\n");

  GtkWidget *header = create_header_bar();
  gtk_window_set_titlebar(GTK_WINDOW(window), header);

  GtkWidget *box = create_vertical_box(10, 20, 20, 20, 20);
  GtkWidget *image = create_image("../include/image.png", 200, 200);

  start_data->window = window;
  start_data->timeout_id =
      g_timeout_add_seconds(4, check_network_status, app_data);

  GtkWidget *button =
      create_button("Start", G_CALLBACK(on_start_clicked), start_data);

  GtkWidget *label;

  if (findMyIP().status != -1) {
    g_print("Device is connected to the network.\n");
    label = create_wrapped_label(
        "You are ready to start the SSDP Listener. Please click "
        "the  'Start' button below.");
    gtk_label_set_text(GTK_LABEL(label),
                       "You are ready to start the SSDP Listener. Please click "
                       "the 'Start' button below.");
    gtk_widget_set_sensitive(button, TRUE);
  } else {
    g_print("Device is not connected to the network.\n");
    label = create_wrapped_label(
        "Your device needs to be connected to any network before you proceed. "
        "Please check the network connection of your device");
    gtk_widget_set_sensitive(button, FALSE);
  }

  gtk_box_append(GTK_BOX(box), label);
  gtk_box_append(GTK_BOX(box), image);
  gtk_box_append(GTK_BOX(box), button);
  gtk_window_set_child(GTK_WINDOW(window), box);

  app_data->label = label;
  app_data->button = button;
  start_data->free_data = app_data;

  return window;
}
