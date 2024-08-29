// #include "createHeaderBar.h"
#include "createVerticalBox.h"
#include "createWrappedLabel.h"
#include "getMyIp.h"
#include "gtk/gtk.h"
#include "onDeviceListActivated.h"
#include "uiCustomData.h"

static void update_ui_on_scan(gpointer user_data) {
  UiUpdateData *data = (UiUpdateData *)user_data;
  g_print("%d devices are found\n", data->result.size);

  GtkWidget *vertical_box_for_device_list = create_vertical_box(0, 0, 0, 0, 0);

  // GtkWidget *header = create_header_bar();
  // gtk_window_set_titlebar(GTK_WINDOW(data->window), header);

  GtkWidget *scrolled_window = gtk_scrolled_window_new();
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                 GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

  gtk_scrolled_window_set_min_content_height(
      GTK_SCROLLED_WINDOW(scrolled_window), 200);

  GtkWidget *sidebar = gtk_list_box_new();
  gtk_widget_set_size_request(sidebar, 200, -1);

  GtkWidget *frame = gtk_frame_new(NULL);
  GtkWidget *custom_label = create_wrapped_label(findMyIP().message, 30);
  gtk_widget_set_margin_top(custom_label, 5);
  gtk_widget_set_margin_bottom(custom_label, 5);
  gtk_frame_set_label_widget(GTK_FRAME(frame), custom_label);
  gtk_frame_set_label_align(GTK_FRAME(frame), 0.5);
  gtk_frame_set_child(GTK_FRAME(frame), scrolled_window);

  // for demo
  const char *items[] = {"Apple", "Ball", "Cat"};
  // ,    "Dog", "Egg",   "Fish",
  // "Green",  "Hen",  "Ink",    "Jug", "King",  "Lion",
  // "Monkey", "Nest", "Orange", "Pen", "Queen", "Red"};

  /*for (int i = 0; i < data->result.size; i++) {*/
  for (int i = 0; i < sizeof(items) / sizeof(items[i]); i++) {
    GtkWidget *row = gtk_list_box_row_new();
    // GtkWidget *label = create_wrapped_label(data->result.arr[i], 30);
    GtkWidget *label = create_wrapped_label(items[i], 30);
    gtk_widget_set_name(GTK_WIDGET(row), data->result.arr[i]);
    gtk_widget_set_margin_top(label, 5);
    gtk_widget_set_margin_bottom(label, 5);
    gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), label);
    gtk_list_box_append(GTK_LIST_BOX(sidebar), row);
  }
  g_signal_connect(sidebar, "row-activated",
                   G_CALLBACK(on_device_list_activated), NULL);

  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), sidebar);
  gtk_box_append(GTK_BOX(vertical_box_for_device_list), frame);
  gtk_window_set_child(data->window, vertical_box_for_device_list);

  for (int i = 0; i < data->result.size; i++) {
    g_free(data->result.arr[i]);
  }
  g_free(data->result.arr);
  g_free(data);
}
