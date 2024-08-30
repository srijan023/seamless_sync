#include "../../archive/getClientSocket.c"
#include "../../archive/getServerSocket.c"
#include "../../archive/sendFile.c"
#include "createButton.h"
#include "createVerticalBox.h"
#include "createWrappedLabel.h"
#include "gio/gio.h"
#include "glib-object.h"
#include "glib.h"
#include "gtk/gtkshortcut.h"
#include "pango/pango-layout.h"
#include <gtk/gtk.h>
#include <string.h>
#include <time.h>

int *connSocket = NULL; // global socket file descriptor
GtkWidget *vbox;

void add_message(const gchar *message_text, gboolean is_user_message) {
  GtkWidget *msg = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
  gtk_widget_set_hexpand(msg, FALSE);

  GtkWidget *message_label = gtk_label_new(message_text);
  gtk_label_set_justify(GTK_LABEL(message_label), GTK_JUSTIFY_CENTER);
  gtk_label_set_max_width_chars(GTK_LABEL(message_label), 25);
  gtk_label_set_wrap(GTK_LABEL(message_label), TRUE);
  gtk_label_set_wrap_mode(GTK_LABEL(message_label), PANGO_WRAP_WORD_CHAR);
  gtk_box_append(GTK_BOX(msg), message_label);

  if (is_user_message) {
    gtk_widget_set_halign(msg, GTK_ALIGN_END);
    gtk_widget_set_margin_end(msg, 10);
  } else {
    gtk_widget_set_halign(msg, GTK_ALIGN_START);
    gtk_widget_set_margin_start(msg, 10);
  }
  gtk_box_append(GTK_BOX(vbox), msg);
}

static void on_send_button_clicked(GtkButton *button, gpointer user_data) {
  GtkEntry *entry = GTK_ENTRY(user_data);
  const char *message_text = gtk_editable_get_text(GTK_EDITABLE(entry));
  if (g_strcmp0(message_text, "") != 0 && connSocket &&
      connSocket >= 0) { // Ensure message is not empty
    add_message(message_text, TRUE);
    send(*connSocket, message_text, strlen(message_text), 0);
    gtk_editable_set_text(GTK_EDITABLE(entry),
                          ""); // Clear the entry after sending
  }
}

static void enter_clicked_on_entry(GtkEntry *entry, gpointer user_data) {
  const gchar *message_text = gtk_editable_get_text(GTK_EDITABLE(entry));
  if (g_strcmp0(message_text, "") != 0 && connSocket && connSocket >= 0) {
    add_message(message_text, TRUE);
    send(*connSocket, message_text, strlen(message_text), 0);
    gtk_editable_set_text(GTK_EDITABLE(entry), "");
  }
}

gboolean update_ui_with_message(gpointer data) {
  add_message((gchar *)data, FALSE);
  g_free(data);
  return FALSE;
}

void *receive_messages(gpointer data) {
  char buffer[256];

  while (1) {
    memset(buffer, '\0', sizeof(buffer));
    if (recv(*connSocket, buffer, sizeof(buffer), 0) <= 0) {
      printf("[-] Connection closed or error occurred\n");
      break;
    }
    if (g_strcmp0(buffer, "/file") == 0) {
      g_print("receiving file\n");
    }
    g_idle_add(update_ui_with_message, g_strdup(buffer));
  }
  return NULL;
}

void *client_thread_func(gpointer data) {
  connSocket = getClientSocket(data);
  if (connSocket && connSocket >= 0) {
    g_thread_new("receive-messages", receive_messages, NULL);
  }
  return NULL;
}

void *server_thread_func(gpointer data) {
  printf("Here");
  connSocket = getServerSocket(data);

  printf("%d", *connSocket);

  printf("Receiving message");
  if (connSocket && connSocket >= 0) {
    g_thread_new("receive-messages", receive_messages, NULL);
  }
  return NULL;
}

static void on_open_file_response(GObject *source_object, GAsyncResult *res,
                                  gpointer user_data) {
  g_autoptr(GFile) file = NULL;
  g_autofree char *path = NULL;

  file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(source_object), res, NULL);
  if (file != NULL) {
    path = g_file_get_path(file);
    g_print("Selected file: %s", path);
    send(*connSocket, "/file", strlen("/file"), 0);
    g_print("sending file\n");
    // sendFile(connSocket, path);
  } else {
    g_print("No file selected\n");
  }
}

static void on_file_button_clicked(GtkButton *button, gpointer data) {
  GtkWindow *window =
      GTK_WINDOW(gtk_widget_get_ancestor(GTK_WIDGET(button), GTK_TYPE_WINDOW));
  g_print("file button clicked\n");
  GtkFileDialog *dialog = gtk_file_dialog_new();
  gtk_file_dialog_open(dialog, window, NULL, on_open_file_response, NULL);
}

void demo_callback(GtkButton *button, gpointer data) {
  const gchar *button_label = gtk_button_get_label(button);
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

  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_vexpand(vbox, TRUE);
  gtk_box_append(GTK_BOX(msgArea), vbox);

  GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
  gtk_box_append(GTK_BOX(msgArea), hbox);
  gtk_widget_set_valign(hbox, GTK_ALIGN_END);

  GtkWidget *entry = gtk_entry_new();
  gtk_widget_set_hexpand(entry, TRUE);
  gtk_box_append(GTK_BOX(hbox), entry);

  GtkWidget *file_button = gtk_button_new_with_label("File");
  gtk_box_append(GTK_BOX(hbox), file_button);

  g_signal_connect(file_button, "clicked", G_CALLBACK(on_file_button_clicked),
                   NULL);

  GtkWidget *send_button = gtk_button_new_with_label("Send");
  gtk_box_append(GTK_BOX(hbox), send_button);

  g_signal_connect(send_button, "clicked", G_CALLBACK(on_send_button_clicked),
                   entry);
  g_signal_connect(entry, "activate", G_CALLBACK(enter_clicked_on_entry), NULL);

  if (g_strcmp0(button_label, "Connect as Server") == 0) {
    printf("Click button");
    g_thread_new("server-thread", server_thread_func, data);
    g_print("Connect as server clicked\n");
  } else if (g_strcmp0(button_label, "Connect as Receiver") == 0) {
    g_thread_new("client-socket_thread", client_thread_func, data);
    g_print("Connect as receiver clicked\n");
  }
}

void on_device_list_activated(GtkListBox *listbox, GtkListBoxRow *row,
                              gpointer user_data) {
  // Ensure row is a valid widget
  if (!GTK_IS_WIDGET(row))
    return;

  GtkWindow *window =
      GTK_WINDOW(gtk_widget_get_ancestor(GTK_WIDGET(listbox), GTK_TYPE_WINDOW));
  GtkWidget *vertical_box_for_client_and_server =
      create_vertical_box(20, 0, 0, 0, 0);

  GtkWidget *label = gtk_widget_get_first_child(GTK_WIDGET(row));
  const char *item_text = gtk_label_get_text(GTK_LABEL(label));
  GtkWidget *new_label = create_wrapped_label(item_text, 30);
  gtk_box_append(GTK_BOX(vertical_box_for_client_and_server), new_label);

  GtkWidget *connect_as_server = create_button(
      "Connect as Server", G_CALLBACK(demo_callback), g_strdup(item_text));
  gtk_box_append(GTK_BOX(vertical_box_for_client_and_server),
                 connect_as_server);

  GtkWidget *connect_as_receiver = create_button(
      "Connect as Receiver", G_CALLBACK(demo_callback), g_strdup(item_text));
  gtk_box_append(GTK_BOX(vertical_box_for_client_and_server),
                 connect_as_receiver);

  gtk_window_set_child(window, vertical_box_for_client_and_server);
}
