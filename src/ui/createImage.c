#include <gtk/gtk.h>

GtkWidget *create_image(const char *path, int width, int height) {
  GtkWidget *image = gtk_image_new_from_file(path);
  gtk_widget_set_size_request(image, width, height);
  return image;
}
