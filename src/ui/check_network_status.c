#include "getMyIp.h"
#include "uiCustomData.h"

gboolean check_network_status(gpointer user_data) {

  NetworkStatusData *app_data = (NetworkStatusData *)user_data;

  g_print("Running check network status.\n");
  if (findMyIP().status != -1) {
    gtk_label_set_text(GTK_LABEL(app_data->label),
                       "You are ready to start the SSDP Listener. Please click "
                       "the 'Start' button below.");
    gtk_widget_set_sensitive(app_data->button, TRUE);
  } else {
    gtk_label_set_text(
        GTK_LABEL(app_data->label),
        "Your device needs to be connected to any network before you proceed. "
        "Please check the network connection of your device");
    gtk_widget_set_sensitive(app_data->button, FALSE);
  }
  return G_SOURCE_CONTINUE;
}
