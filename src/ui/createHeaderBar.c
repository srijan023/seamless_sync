#include "menu.c"
#include <gtk/gtk.h>

GtkWidget *create_header_bar() {
  GtkWidget *header = gtk_header_bar_new();
  GtkWidget *menu_button = gtk_menu_button_new();
  GtkWidget *popover = create_menu();
  gtk_menu_button_set_popover(GTK_MENU_BUTTON(menu_button), popover);
  gtk_widget_set_tooltip_text(menu_button, "Menu");
  gtk_header_bar_pack_end(GTK_HEADER_BAR(header), menu_button);
  return header;
}
