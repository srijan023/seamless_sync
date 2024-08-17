#include "createWrappedLabel.h"
#include "gtk/gtk.h"

GtkWidget *create_header_menu() {
  GtkWidget *popover = gtk_popover_new();
  GtkWidget *menu_box = gtk_list_box_new();
  gtk_widget_set_size_request(menu_box, 90, -1);
  gtk_list_box_set_selection_mode(GTK_LIST_BOX(menu_box), GTK_SELECTION_NONE);

  const char *items[] = {"Refresh", "Option 2", "Option 3"};

  for (int i = 0; i < sizeof(items) / sizeof(items[i]); i++) {
    GtkWidget *row = gtk_list_box_row_new();
    GtkWidget *label = create_wrapped_label(items[i]);
    gtk_widget_set_margin_top(label, 5);
    gtk_widget_set_margin_bottom(label, 5);
    gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), label);
    gtk_list_box_append(GTK_LIST_BOX(menu_box), row);
  }

  // Set the box as the child of the popover
  gtk_popover_set_child(GTK_POPOVER(popover), menu_box);

  return popover;
}
