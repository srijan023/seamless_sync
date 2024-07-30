#include "gtk/gtk.h"

GtkWidget *create_menu() {
  // Create a new popover menu
  GtkWidget *popover = gtk_popover_new();
  GtkWidget *menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  // Create menu items
  GtkWidget *option1 = gtk_button_new_with_label("Option 1");
  GtkWidget *option2 = gtk_button_new_with_label("Option 2");
  GtkWidget *option3 = gtk_button_new_with_label("Option 3");

  // Add the menu items to the box
  gtk_box_append(GTK_BOX(menu_box), option1);
  gtk_box_append(GTK_BOX(menu_box), option2);
  gtk_box_append(GTK_BOX(menu_box), option3);

  // Set the box as the child of the popover
  gtk_popover_set_child(GTK_POPOVER(popover), menu_box);

  return popover;
}
