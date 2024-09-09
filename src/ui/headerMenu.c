#include "createButton.h"
#include "createVerticalBox.h"
#include "onStartClicked.h"

static void on_menu_button_clicked(GtkButton *button, gpointer user_data) {
  const gchar *button_label = gtk_button_get_label(button);

  if (g_strcmp0(button_label, "Refresh") == 0) {
    g_print("Refresh clicked\n");
    on_start_clicked(GTK_BUTTON(button), NULL);
  }

  g_print("Menu clicked\n");
  gtk_popover_popdown(GTK_POPOVER(user_data));
}

GtkWidget *create_header_menu() {
  GtkWidget *popover = gtk_popover_new();
  GtkWidget *menu_box = create_vertical_box(0, 0, 0, 0, 0);
  gtk_widget_set_size_request(menu_box, 90, -1);

  const char *items[] = {"Refresh", "Option 2", "Option 3"};

  GtkWidget *refresh_button =
      create_button("Refresh", G_CALLBACK(on_menu_button_clicked), popover);
  gtk_box_append(GTK_BOX(menu_box), refresh_button);

  GtkWidget *button_2 =
      create_button("Button 2", G_CALLBACK(on_menu_button_clicked), popover);
  gtk_box_append(GTK_BOX(menu_box), button_2);

  GtkWidget *button_3 =
      create_button("Button 3", G_CALLBACK(on_menu_button_clicked), popover);
  gtk_box_append(GTK_BOX(menu_box), button_3);

  gtk_popover_set_child(GTK_POPOVER(popover), menu_box);

  return popover;
}
