#include <gtk/gtk.h>

GtkWidget *create_vertical_box(int spacing, int t_margin, int b_margin,
                               int s_margin, int e_margin) {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);
  gtk_widget_set_margin_top(box, t_margin);
  gtk_widget_set_margin_bottom(box, b_margin);
  gtk_widget_set_margin_start(box, s_margin);
  gtk_widget_set_margin_end(box, e_margin);
  gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
  gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
  return box;
}
