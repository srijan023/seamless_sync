#include "createVerticalBox.h"
#include "createWrappedLabel.h"
#include "joinSSDP.h"
#include "onSsdpScanCompleted.h"
#include "removeOldChild.h"
#include "runSsdpScan.h"
#include "uiCustomData.h"

void on_start_clicked(GtkButton *button, gpointer data) {
  StartData *user_data = (StartData *)data;
  UiUpdateData *ui_data = g_new0(UiUpdateData, 1);
  JoinSSDPThread *thread_data = g_new0(JoinSSDPThread, 1);

  if (user_data->timeout_id > 0) {
    g_source_remove(user_data->timeout_id);
    user_data->timeout_id = 0;
    g_print("Periodic(5s) network status check stopped\n");
  }
  g_free(user_data->free_data);
  g_print("Network status data is freed from the memory.\n");

  GtkWindow *window = GTK_WINDOW(user_data->window);
  remove_old_child(window);
  g_print("Old child of windows: label image and button are removed\n");

  GtkWidget *main_vertical_box = create_vertical_box(10, 5, 0, 5, 0);
  // gtk_widget_set_halign(main_vertical_box, GTK_ALIGN_START);
  // gtk_widget_set_valign(main_vertical_box, GTK_ALIGN_FILL);
  GtkWidget *label_horizontal_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_margin_bottom(label_horizontal_box, 100);

  GtkWidget *list_vertical_box = create_vertical_box(10, 5, 0, 5, 0);

  // GtkWidget *sticky_box = create_vertical_box(0, 10, 10, 10, 10);
  // GtkWidget *sticky_button = gtk_button_new_with_label("Refresh");
  // gtk_box_append(GTK_BOX(sticky_box), sticky_button);

  // GtkWidget *frame = gtk_frame_new(findMyIP().message);
  // gtk_frame_set_label_align(GTK_FRAME(frame), 0.5);
  // gtk_frame_set_child(GTK_FRAME(frame), sticky_box);

  GtkWidget *ip_label = create_wrapped_label("My IP: ");
  gtk_box_append(GTK_BOX(label_horizontal_box), ip_label);

  GtkWidget *ip_address = create_wrapped_label(findMyIP().message);
  gtk_box_append(GTK_BOX(label_horizontal_box), ip_address);

  GtkWidget *spin_label = create_wrapped_label("Scanning for devices...");

  GtkWidget *spinner = gtk_spinner_new();
  gtk_widget_set_size_request(GTK_WIDGET(spinner), 50, 50);
  gtk_spinner_start(GTK_SPINNER(spinner));

  GTask *task = g_task_new(NULL, NULL, on_ssdp_scan_completed, ui_data);
  g_task_run_in_thread(task, run_ssdp_scan);
  g_object_unref(task);

  strcpy(thread_data->myIp, findMyIP().message);
  thread_data->timer = 5.0;
  GThread *join_ssdp_thread = g_thread_new("join_ssdp", joinSSDP, thread_data);
  g_thread_unref(join_ssdp_thread);

  gtk_box_append(GTK_BOX(main_vertical_box), label_horizontal_box);
  gtk_box_append(GTK_BOX(main_vertical_box), list_vertical_box);
  gtk_box_append(GTK_BOX(list_vertical_box), spinner);
  gtk_box_append(GTK_BOX(list_vertical_box), spin_label);

  gtk_window_set_child(window, main_vertical_box);

  ui_data->label_horizontal_box = label_horizontal_box;
  ui_data->v_box = main_vertical_box;
  ui_data->spin_label = spin_label;
  ui_data->spinner = spinner;
}
