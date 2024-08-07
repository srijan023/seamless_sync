#include <gtk/gtk.h>

GtkWidget *create_wrapped_label(const char *text) {
  GtkWidget *label = gtk_label_new(text);
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
  gtk_label_set_wrap(GTK_LABEL(label), TRUE);
  gtk_label_set_wrap_mode(GTK_LABEL(label), PANGO_WRAP_WORD);
  gtk_label_set_max_width_chars(GTK_LABEL(label), 30);
  return label;
}
