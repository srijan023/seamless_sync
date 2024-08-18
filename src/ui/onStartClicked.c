#include "createVerticalBox.h"
#include "joinSSDP.h"
#include "onSsdpScanCompleted.h"
#include "runSsdpScan.h"

void on_start_clicked(GtkButton *button, gpointer data) {
  if (data != NULL) {
    StartData *user_data = (StartData *)data;
    if (user_data->timeout_id > 0) {
      g_source_remove(user_data->timeout_id);
      user_data->timeout_id = 0;
      g_print("Periodic(5s) network status check stopped\n");
    }
    g_free(user_data->free_data);
    g_print("Network status data is freed from the memory.\n");
  }

  GtkWindow *window =
      GTK_WINDOW(gtk_widget_get_ancestor(GTK_WIDGET(button), GTK_TYPE_WINDOW));

  UiUpdateData *ui_data = g_new0(UiUpdateData, 1);
  JoinSSDPThread *thread_data = g_new0(JoinSSDPThread, 1);

  // if (user_data->timeout_id > 0) {
  // g_source_remove(user_data->timeout_id);
  // user_data->timeout_id = 0;
  // g_print("Periodic(5s) network status check stopped\n");
  // }
  // g_free(user_data->free_data);
  // g_print("Network status data is freed from the memory.\n");

  // GtkWindow *window = GTK_WINDOW(user_data->window);
  // remove_old_child(window);
  // g_print("Old child of windows: label image and button are removed\n");

  GtkWidget *vertical_box_for_spinner = create_vertical_box(0, 0, 0, 0, 0);

  GtkWidget *spinner = gtk_spinner_new();
  gtk_widget_set_size_request(GTK_WIDGET(spinner), 50, 50);
  gtk_spinner_start(GTK_SPINNER(spinner));

  GtkWidget *spin_label = create_wrapped_label("Scanning for devices...");
  gtk_widget_set_margin_top(spin_label, 20);

  GTask *task = g_task_new(NULL, NULL, on_ssdp_scan_completed, ui_data);
  g_task_run_in_thread(task, run_ssdp_scan);
  g_object_unref(task);

  strcpy(thread_data->myIp, findMyIP().message);
  thread_data->timer = 30.0;
  GThread *join_ssdp_thread = g_thread_new("join_ssdp", joinSSDP, thread_data);
  g_thread_unref(join_ssdp_thread);

  gtk_box_append(GTK_BOX(vertical_box_for_spinner), spinner);
  gtk_box_append(GTK_BOX(vertical_box_for_spinner), spin_label);

  gtk_window_set_child(window, vertical_box_for_spinner);

  ui_data->spinner = spinner;
  ui_data->spin_label = spin_label;
  ui_data->window = window;
}
