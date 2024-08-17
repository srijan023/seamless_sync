#include "createWrappedLabel.h"
#include "removeOldChild.h"
#include "uiCustomData.h"
#include "whileDeviceClicked.h"
#include <gtk/gtk.h>

static void update_ui_on_scan(gpointer user_data) {
  UiUpdateData *data = (UiUpdateData *)user_data;
  g_print("%d devices are found\n", data->result.size);

  remove_old_child(data->window);

  GtkWidget *horizontal_box_for_sidebar_and_msgbox =
      gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

  GtkWidget *scrolled_window = gtk_scrolled_window_new();
  gtk_widget_set_size_request(scrolled_window, 150, -1);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                 GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_widget_set_vexpand(scrolled_window, TRUE);

  GtkWidget *sidebar = gtk_list_box_new();

  // Remove spinner and its label
  // gtk_spinner_stop(GTK_SPINNER(data->spinner));
  // gtk_widget_unparent(data->spinner);
  // gtk_widget_unparent(data->spin_label);

  // for demo
  const char *items[] = {"Apple", "Ball", "Cat"};

  // for (int i = 0; i < data->result.size; i++) {
  for (int i = 0; i < sizeof(items) / sizeof(items[i]); i++) {
    GtkWidget *row = gtk_list_box_row_new();
    // GtkWidget *label = create_wrapped_label(data->result.arr[i]);
    GtkWidget *label = create_wrapped_label(items[i]);
    // gtk_widget_set_name(GTK_WIDGET(row), data->result.arr[i]);
    gtk_widget_set_margin_top(label, 5);
    gtk_widget_set_margin_bottom(label, 5);
    gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), label);
    gtk_list_box_append(GTK_LIST_BOX(sidebar), row);
  }
  g_signal_connect(sidebar, "row-selected", G_CALLBACK(while_device_clicked),
                   NULL);

  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), sidebar);
  gtk_box_append(GTK_BOX(horizontal_box_for_sidebar_and_msgbox),
                 scrolled_window);
  gtk_window_set_child(data->window, horizontal_box_for_sidebar_and_msgbox);

  for (int i = 0; i < data->result.size; i++) {
    g_free(data->result.arr[i]);
  }
  g_free(data->result.arr);
  g_free(data);
}
