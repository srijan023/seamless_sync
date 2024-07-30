#include "../SSDPListenerConnection.c"
#include "glib.h"
#include "gtk/gtk.h"
#include "sideBar.c"
#include <gio/gio.h>
#include <pthread.h>

// Structure to hold data for updating UI
typedef struct {
  GtkWidget *h_box;
  GtkWidget *v_box;
  struct ssdpMessage result;
} UiUpdateData;

static void update_ui(gpointer user_data) {
  UiUpdateData *data = (UiUpdateData *)user_data;
  g_print("%s", data->result.message);

  // create_sidebar(box, main_content, items, sizeof(items) / sizeof(items[0]));
  create_sidebar(data->h_box, data->v_box, data->result.arr, data->result.size);

  for (int i = 0; i < data->result.size; i++) {
    g_free(data->result.arr[i]);
  }
  g_free(data->result.arr);
  g_free(data);
}

static void run_ssdp_scan(GTask *task, gpointer source_object, gpointer data,
                          GCancellable *cancellable) {
  g_print("here is run ssdp scan\n");
  UiUpdateData *ui_data = (UiUpdateData *)data;

  struct ssdpMessage *revMsg = SSDPListen();
  g_print("run ssdp listen function\n");
  if (g_task_return_error_if_cancelled(task)) {
    return;
  }

  g_task_return_pointer(task, revMsg, g_free);
  g_print("result of ssdp listen is return using g_task_return_pointer\n");
  // ui_data->result = *revMsg;
  // g_idle_add(update_ui, ui_data);
}

static void on_ssdp_scan_completed(GObject *source_object, GAsyncResult *res,
                                   gpointer user_data) {
  GTask *task = G_TASK(res);
  UiUpdateData *ui_data = (UiUpdateData *)user_data;
  struct ssdpMessage *result = g_task_propagate_pointer(task, NULL);
  ui_data->result = *result;
  update_ui(ui_data);
}

void on_start_clicked(GtkWidget *widget, gpointer data) {
  g_print("%s\n", "start clicked");
  GtkWindow *window = GTK_WINDOW(data);

  // Create the new main content area
  GtkWidget *v_box = create_vertical_box(10, 0, 0, 0, 0);
  g_print("%s\n", "v-box created");

  // Create the box containing the sidebar and the main content area
  GtkWidget *h_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  g_print("%s\n", "h-box created");

  GtkWidget *old_child = gtk_window_get_child(window);
  if (old_child) {
    gtk_window_set_child(window, NULL); // Remove old child
    gtk_widget_unparent(old_child);     // Unparent old child
  }
  g_print("%s\n", "old child of window are removed");

  UiUpdateData *ui_data = g_new0(UiUpdateData, 1);
  ui_data->h_box = h_box;
  ui_data->v_box = v_box;
  g_print("h-box and v-box inserted in uiupdate\n");

  GTask *task = g_task_new(NULL, NULL, on_ssdp_scan_completed, ui_data);
  g_print("new task created\n");
  g_task_run_in_thread(task, run_ssdp_scan);
  g_print("run ssdp scan is run in thread\n");
  g_object_unref(task);
  g_print("task is unreffed\n");

  gtk_box_append(GTK_BOX(h_box), v_box);
  g_print("%s\n", "v-box appended to h-box");

  // set the new child widget for the window
  gtk_window_set_child(window, h_box);
  g_print("%s\n", "h-box appended to window");
}
