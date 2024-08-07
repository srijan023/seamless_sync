#include <gtk/gtk.h>
#ifndef CUSTOMDATATYPE_H
#define CUSTOMDATATYPE_H

struct customAddInfo {
  int status;
  char message[1024];
};

struct ssdpMessage {
  char **arr;
  int size;
  char message[1024];
};

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

#endif
