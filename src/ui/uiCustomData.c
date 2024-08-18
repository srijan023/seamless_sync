#include <customDataTypes.h>
#include <gtk/gtk.h>

typedef struct {
  GtkWidget *label;
  GtkWidget *button;
} NetworkStatusData;

typedef struct {
  struct ssdpMessage result;
  GtkWindow *window;
  GtkWidget *spinner;
  GtkWidget *spin_label;
} UiUpdateData;

typedef struct {
  guint timeout_id;
  NetworkStatusData *free_data;
} StartData;

typedef struct {
  char myIp[16];
  double timer;
} JoinSSDPThread;
