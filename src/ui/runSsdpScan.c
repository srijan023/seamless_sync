#include "SSDPListenerConnection.h"
#include <gtk/gtk.h>
// #include "uiCustomData.h"

static void run_ssdp_scan(GTask *task, gpointer source_object, gpointer data,
                          GCancellable *cancellable) {
  // UiUpdateData *ui_data = (UiUpdateData *)data;

  struct ssdpMessage *revMsg = SSDPListen(5);
  g_print("Run ssdp listen function\n");
  if (g_task_return_error_if_cancelled(task)) {
    return;
  }
  g_task_return_pointer(task, revMsg, g_free);
}
