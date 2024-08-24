#include "createButton.h"
#include "createWrappedLabel.h"
#include "glib-object.h"
#include "gtk/gtkshortcut.h"
#include "pango/pango-layout.h"
#include <gtk/gtk.h>

void add_message(GtkBox *v_box, const char *message_text,
                 gboolean is_user_message) {
  GtkWidget *msg = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
  gtk_widget_set_hexpand(msg, FALSE);

  GtkWidget *message_label = gtk_label_new(message_text);
  gtk_label_set_justify(GTK_LABEL(message_label), GTK_JUSTIFY_CENTER);
  gtk_label_set_max_width_chars(GTK_LABEL(message_label), 25);
  gtk_label_set_wrap(GTK_LABEL(message_label), TRUE);
  gtk_label_set_wrap_mode(GTK_LABEL(message_label), PANGO_WRAP_WORD_CHAR);
  gtk_box_append(GTK_BOX(msg), message_label);

  // Adjust the alignment and style of the message based on the sender
  if (is_user_message) {
    gtk_widget_set_halign(msg, GTK_ALIGN_END);
    // gtk_widget_set_margin_start(row, 50); // Indent for user messages
    gtk_widget_set_margin_end(msg, 10);
  } else {
    gtk_widget_set_halign(msg, GTK_ALIGN_START);
    gtk_widget_set_margin_start(msg, 10);
    // gtk_widget_set_margin_end(row, 50); // Indent for received messages
  }

  // gtk_list_box_insert(v_box, row, -1); // Insert the row into the list box
  // gtk_widget_set_visible(row, TRUE);      // Show the row after adding it
  gtk_box_append(v_box, msg);
}

static void on_send_button_clicked(GtkButton *button, gpointer user_data) {
  GtkEntry *entry = GTK_ENTRY(user_data);
  const char *message_text = gtk_editable_get_text(GTK_EDITABLE(entry));

  if (g_strcmp0(message_text, "") != 0) { // Ensure message is not empty
    GtkBox *v_box = GTK_BOX(g_object_get_data(G_OBJECT(button), "v_box"));
    add_message(v_box, message_text, TRUE);
    add_message(v_box, "From another side", FALSE);
    gtk_editable_set_text(GTK_EDITABLE(entry),
                          ""); // Clear the entry after sending
  }
}

static void enter_clicked_on_entry(GtkEntry *entry, gpointer user_data) {
  const char *message_text = gtk_editable_get_text(GTK_EDITABLE(entry));
  if (g_strcmp0(message_text, "") != 0) {
    GtkBox *v_box = GTK_BOX(user_data);
    add_message(v_box, message_text, TRUE);
    add_message(v_box, "FROM ANOTHER SIDE WITH ENTER", FALSE);
    gtk_editable_set_text(GTK_EDITABLE(entry), "");
  }
}

void demo_callback(GtkButton *button, gpointer data) {
  g_print("clicked");

  GtkWindow *window =
      GTK_WINDOW(gtk_widget_get_ancestor(GTK_WIDGET(button), GTK_TYPE_WINDOW));

  GtkWidget *msgArea = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_widget_set_vexpand(msgArea, TRUE);
  gtk_widget_set_hexpand(msgArea, TRUE);
  gtk_widget_set_margin_end(msgArea, 10);
  gtk_widget_set_margin_start(msgArea, 10);
  gtk_widget_set_margin_top(msgArea, 10);
  gtk_widget_set_margin_bottom(msgArea, 10);
  gtk_window_set_child(window, msgArea);

  // GtkWidget *old_child =
  // gtk_widget_get_first_child(vertical_box_for_client_and_server);
  // while (old_child) {
  // gtk_box_remove(GTK_BOX(vertical_box_for_client_and_server), old_child);
  // old_child = gtk_widget_get_first_child(vertical_box_for_client_and_server);
  // }

  // GtkWidget *listbox = gtk_list_box_new();
  // gtk_widget_set_vexpand(listbox, true);
  // gtk_box_append(GTK_BOX(msgArea), listbox);

  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_vexpand(vbox, TRUE);
  gtk_box_append(GTK_BOX(msgArea), vbox);

  GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
  gtk_box_append(GTK_BOX(msgArea), hbox);
  gtk_widget_set_valign(hbox, GTK_ALIGN_END);

  GtkWidget *entry = gtk_entry_new();
  gtk_widget_set_hexpand(entry, TRUE);
  gtk_box_append(GTK_BOX(hbox), entry);

  GtkWidget *send_button = gtk_button_new_with_label("Send");
  gtk_box_append(GTK_BOX(hbox), send_button);

  g_object_set_data(G_OBJECT(send_button), "v_box", vbox);
  g_signal_connect(send_button, "clicked", G_CALLBACK(on_send_button_clicked),
                   entry);
  // g_object_set_data(G_OBJECT(entry), "v_box", vbox);
  g_signal_connect(entry, "activate", G_CALLBACK(enter_clicked_on_entry), vbox);
}

void on_device_list_activated(GtkListBox *listbox, GtkListBoxRow *row,
                              gpointer user_data) {
  // Ensure row is a valid widget
  if (!GTK_IS_WIDGET(row))
    return;

  GtkWidget *vertical_box_for_client_and_server = GTK_WIDGET(user_data);
  GtkWindow *window =
      GTK_WINDOW(gtk_widget_get_ancestor(GTK_WIDGET(listbox), GTK_TYPE_WINDOW));

  GtkWidget *label = gtk_widget_get_first_child(GTK_WIDGET(row));
  if (GTK_IS_WIDGET(label)) {
    const char *item_text = gtk_label_get_text(GTK_LABEL(label));
    GtkWidget *new_label = create_wrapped_label(item_text, 30);
    gtk_box_append(GTK_BOX(vertical_box_for_client_and_server), new_label);
  }

  GtkWidget *old_child =
      gtk_widget_get_first_child(vertical_box_for_client_and_server);
  if (old_child) {
    gtk_box_remove(GTK_BOX(vertical_box_for_client_and_server), old_child);
    // old_child =
    // gtk_widget_get_first_child(vertical_box_for_client_and_server);
  }

  // GtkWindow *window = GTK_WINDOW(gtk_widget_get_ancestor(
  // GTK_WIDGET(vertical_box_for_client_and_server), GTK_TYPE_WINDOW));

  GtkWidget *connect_as_server =
      create_button("Connect as Server", G_CALLBACK(demo_callback),
                    vertical_box_for_client_and_server);
  gtk_box_append(GTK_BOX(vertical_box_for_client_and_server),
                 connect_as_server);

  GtkWidget *connect_as_receiver =
      create_button("Connect as Receiver", G_CALLBACK(demo_callback),
                    vertical_box_for_client_and_server);
  gtk_box_append(GTK_BOX(vertical_box_for_client_and_server),
                 connect_as_receiver);

  // Remove the old child widget from the main content

  // Create a new label with the item text and add it to the main content
  // gtk_box_append(GTK_BOX(main_content),
  // vertical_box_for_client_and_server);
}
// }
