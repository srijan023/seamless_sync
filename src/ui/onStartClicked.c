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

static gboolean update_ui(gpointer user_data) {
  UiUpdateData *data = (UiUpdateData *)user_data;
  g_print("%s", data->result.message);

  // Create the new main content area
  // GtkWidget *main_content = create_vertical_box(10, 20, 20, 20, 20);

  // Create the box containing the sidebar and the main content area
  // GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

  // create_sidebar(box, main_content, items, sizeof(items) / sizeof(items[0]));
  create_sidebar(data->h_box, data->v_box, data->result.arr, data->result.size);

  // gtk_box_append(GTK_BOX(box), main_content);

  // Remove the old child widget from the window
  // GtkWidget *old_child = gtk_window_get_child(GTK_WINDOW(data->window));
  // if (old_child) {
  // gtk_window_set_child(GTK_WINDOW(data->window), NULL); // Remove old child
  // gtk_widget_unparent(old_child);                       // Unparent old child
  // }

  // set the new child widget for the window
  // gtk_window_set_child(GTK_WINDOW(data->window), box);

  // gtk_label_set_text(GTK_LABEL(data->window), data->result.message);
  g_free(data);
  return FALSE;
}

static void run_ssdp_scan(GTask *task, gpointer source_object,
                          gpointer user_data, GCancellable *cancellable) {
  UiUpdateData *ui_data = (UiUpdateData *)user_data;

  // SSDP scan logic
  pthread_t SSDPThread;
  int doLooping = 1;
  pthread_create(&SSDPThread, NULL, SSDPListen, &doLooping);
  g_usleep(50000000);
  doLooping = 0;
  struct ssdpMessage *revMsg;
  pthread_join(SSDPThread, (void **)&revMsg);

  if (g_task_return_error_if_cancelled(task)) {
    return;
  }

  g_task_return_pointer(task, g_memdup2(&revMsg, sizeof(struct ssdpMessage)),
                        g_free);

  // UiUpdateData *data = g_new0(UiUpdateData, 1);
  // data->window = window;
  // ui_data->result = *revMsg;

  // g_idle_add_full(G_PRIORITY_DEFAULT_IDLE, (GSourceFunc)update_ui, ui_data,
  // NULL);

  // g_free(revMsg);
  // return NULL;
}

static void on_ssdp_scan_completed(GObject *source_object, GAsyncResult *res,
                                   gpointer user_data) {
  UiUpdateData *ui_data = (UiUpdateData *)user_data;
  GTask *task = G_TASK(res);
  struct ssdpMessage *result = g_task_propagate_pointer(task, NULL);
  if (result) {
    ui_data->result = *result;
    g_print("%s", result->message);
    g_idle_add(update_ui, ui_data);
    g_free(result);
  }
}

void on_start_clicked(GtkWidget *widget, gpointer data) {
  g_print("%s", "start clicked");
  GtkWindow *window = GTK_WINDOW(data);

  // Create the new main content area
  GtkWidget *v_box = create_vertical_box(10, 0, 0, 0, 0);

  // Create the box containing the sidebar and the main content area
  GtkWidget *h_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

  GtkWidget *old_child = gtk_window_get_child(window);
  if (old_child) {
    gtk_window_set_child(window, NULL); // Remove old child
    gtk_widget_unparent(old_child);     // Unparent old child
  }

  UiUpdateData *ui_data = g_new0(UiUpdateData, 1);
  ui_data->h_box = h_box;
  ui_data->v_box = v_box;

  GTask *task = g_task_new(NULL, NULL, on_ssdp_scan_completed, ui_data);
  g_task_run_in_thread(task, run_ssdp_scan);

  // g_thread_new("SSDP Scan", run_ssdp_scan, ui_data);

  gtk_box_append(GTK_BOX(h_box), v_box);

  // set the new child widget for the window
  gtk_window_set_child(window, h_box);
}
