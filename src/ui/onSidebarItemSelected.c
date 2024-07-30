#include <gtk/gtk.h>

void on_sidebar_item_selected(GtkListBox *listbox, GtkListBoxRow *row,
                              gpointer user_data) {
  // Ensure row is a valid widget
  if (!GTK_IS_WIDGET(row))
    return;
  GtkWidget *main_content = GTK_WIDGET(user_data);
  GtkWidget *label = gtk_widget_get_first_child(GTK_WIDGET(row));

  if (GTK_IS_WIDGET(label)) {
    const char *item_text = gtk_label_get_text(GTK_LABEL(label));
    // Remove the old child widget from the main content
    GtkWidget *old_child = gtk_widget_get_first_child(main_content);
    if (old_child) {
      gtk_box_remove(GTK_BOX(main_content), old_child);
    }
    // Create a new label with the item text and add it to the main content
    GtkWidget *new_label = gtk_label_new(item_text);
    gtk_box_append(GTK_BOX(main_content), new_label);
  }
}
