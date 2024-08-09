#include <customDataTypes.h>
#include <gtk/gtk.h>

typedef struct {
  GtkWidget *label;
  GtkWidget *button;
} NetworkStatusData;

typedef struct {
  GtkWidget *h_box;
  GtkWidget *v_box;
  struct ssdpMessage result;
} UiUpdateData;

typedef struct {
  GtkWidget *window;
  guint timeout_id;
  NetworkStatusData *free_data;
} StartData;
