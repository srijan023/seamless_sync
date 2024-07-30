#include "../getMyIp.c"
#include "onSidebarItemSelected.c"
#include "verticalBox.c"

void create_sidebar(GtkWidget *h_box, GtkWidget *v_box, char *items[],
                    int n_items) {
  // Create a vertical box to hold the sticky box and the list box
  GtkWidget *sidebar_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  // Create the sticky box with a label and button
  GtkWidget *sticky_box = create_vertical_box(0, 10, 10, 10, 10);
  GtkWidget *sticky_button = gtk_button_new_with_label("Refresh");
  gtk_box_append(GTK_BOX(sticky_box), sticky_button);

  GtkWidget *frame = gtk_frame_new(findMyIP().message);
  gtk_frame_set_label_align(GTK_FRAME(frame), 0.5);
  gtk_frame_set_child(GTK_FRAME(frame), sticky_box);
  gtk_box_append(GTK_BOX(sidebar_container), frame);

  GtkWidget *sidebar = gtk_list_box_new();
  for (int i = 0; i < n_items; i++) {
    GtkWidget *row = gtk_list_box_row_new();
    GtkWidget *label = gtk_label_new(items[i]);
    gtk_widget_set_name(GTK_WIDGET(row), items[i]);
    gtk_widget_set_margin_top(label, 10);
    gtk_widget_set_margin_bottom(label, 10);
    gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), label);
    gtk_list_box_append(GTK_LIST_BOX(sidebar), row);
  }
  g_signal_connect(sidebar, "row-selected",
                   G_CALLBACK(on_sidebar_item_selected), v_box);

  // Make the sidebar scrollable
  GtkWidget *scrolled_window = gtk_scrolled_window_new();
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), sidebar);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                 GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_widget_set_size_request(scrolled_window, 150, -1);
  gtk_widget_set_vexpand(scrolled_window, TRUE);

  gtk_box_append(GTK_BOX(sidebar_container), scrolled_window);
  gtk_box_append(GTK_BOX(h_box), sidebar_container);
}
