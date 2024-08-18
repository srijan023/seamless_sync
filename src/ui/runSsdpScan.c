#include "SSDPListenerConnection.h"
#include "uiCustomData.h"

static void run_ssdp_scan(GTask *task, gpointer source_object, gpointer data,
                          GCancellable *cancellable) {
  g_print("here is run ssdp scan\n");
  UiUpdateData *ui_data = (UiUpdateData *)data;

  struct ssdpMessage *revMsg = SSDPListen(3);
  g_print("run ssdp listen function\n");
  if (g_task_return_error_if_cancelled(task)) {
    return;
  }

  g_task_return_pointer(task, revMsg, g_free);
  g_print("Result of ssdp listen is returned using g_task_return_pointer\n");
}
