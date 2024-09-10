#include "SSDPListenerConnection.h"
#include <gtk/gtk.h>

static void run_ssdp_scan(GTask *task, gpointer source_object, gpointer data,
                          GCancellable *cancellable) {
  struct ssdpMessage *revMsg = SSDPListen(8);
  g_print("Run ssdp listen function\n");
  if (g_task_return_error_if_cancelled(task)) {
    return;
  }
  g_task_return_pointer(task, revMsg, g_free);
}
