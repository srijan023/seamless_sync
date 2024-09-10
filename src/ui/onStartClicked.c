#include "createVerticalBox.h"
#include "joinSSDP.h"
#include "onSsdpScanCompleted.h"
#include "runSsdpScan.h"

void on_start_clicked(GtkButton *button, gpointer data) {
  StartData *user_data = (StartData *)data;
  UiUpdateData *ui_data = g_new0(UiUpdateData, 1);
  JoinSSDPThread *thread_data = g_new0(JoinSSDPThread, 1);

  GtkWindow *window =
      GTK_WINDOW(gtk_widget_get_ancestor(GTK_WIDGET(button), GTK_TYPE_WINDOW));

  if (user_data->timeout_id > 0) {
    g_source_remove(user_data->timeout_id);
    user_data->timeout_id = 0;
    g_print("[.] Periodic(5s) network status check stopped\n");
  }
  g_free(user_data->free_data);

  GtkWidget *vertical_box_for_spinner = create_vertical_box(0, 0, 0, 0, 0);
  gtk_window_set_child(window, vertical_box_for_spinner);

  GtkWidget *spinner = gtk_spinner_new();
  gtk_widget_set_size_request(GTK_WIDGET(spinner), 50, 50);
  gtk_spinner_start(GTK_SPINNER(spinner));
  gtk_box_append(GTK_BOX(vertical_box_for_spinner), spinner);

  GtkWidget *spin_label = create_wrapped_label("Scanning for devices...", 30);
  gtk_widget_set_margin_top(spin_label, 20);
  gtk_box_append(GTK_BOX(vertical_box_for_spinner), spin_label);

  GTask *task = g_task_new(NULL, NULL, on_ssdp_scan_completed, ui_data);
  g_task_run_in_thread(task, run_ssdp_scan);
  g_object_unref(task);

  strcpy(thread_data->myIp, findMyIP().message);
  thread_data->timer = 30.0;
  GThread *join_ssdp_thread = g_thread_new("join_ssdp", joinSSDP, thread_data);
  g_thread_unref(join_ssdp_thread);

  ui_data->window = window;
}
