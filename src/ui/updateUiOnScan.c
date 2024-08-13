#include "createButton.h"
#include "uiCustomData.h"
#include "whileDeviceClicked.h"
#include <gtk/gtk.h>

static void update_ui_on_scan(gpointer user_data) {
  UiUpdateData *data = (UiUpdateData *)user_data;
  g_print("%d devices are found\n", data->result.size);

  // create_sidebar(box, main_content, items, sizeof(items) / sizeof(items[0]));
  // create_sidebar(data->h_box, data->v_box, data->result.arr,
  // data->result.size);

  gtk_spinner_stop(GTK_SPINNER(data->spinner));
  gtk_widget_unparent(data->spinner);
  gtk_widget_unparent(data->spin_label);

  gtk_widget_set_margin_bottom(data->label_horizontal_box, 0);

  GtkWidget *frame = gtk_frame_new("Found Devices");

  GtkWidget *list_of_device = gtk_list_box_new();
  gtk_frame_set_child(GTK_FRAME(frame), list_of_device);

  // for demo
  const char *items[] = {"Apple", "Ball", "Cat"};

  for (int i = 0; i < data->result.size; i++) {
    GtkWidget *button = create_button(data->result.arr[i],
                                      G_CALLBACK(while_device_clicked), NULL);
    gtk_list_box_append(GTK_LIST_BOX(list_of_device), button);
  }
  gtk_box_append(GTK_BOX(data->v_box), frame);

  for (int i = 0; i < data->result.size; i++) {
    g_free(data->result.arr[i]);
  }
  g_free(data->result.arr);
  g_free(data);
}
