#include "updateUiOnScan.h"

static void on_ssdp_scan_completed(GObject *source_object, GAsyncResult *res,
                                   gpointer user_data) {

  GTask *task = G_TASK(res);
  UiUpdateData *ui_data = (UiUpdateData *)user_data;
  struct ssdpMessage *result = g_task_propagate_pointer(task, NULL);
  ui_data->result = *result;
  update_ui_on_scan(ui_data);
}
