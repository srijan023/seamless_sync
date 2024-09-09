#include "../../archive/getClientSocket.c"
#include "../../archive/getServerSocket.c"
#include "../../archive/receiveFile.c"
#include "../../archive/sendFile.c"
#include "createButton.h"
#include "createVerticalBox.h"
#include "createWrappedLabel.h"

int *connSocket = NULL; // global socket file descriptor
GtkWindow *window;
GtkWidget *vbox;

void add_message(const gchar *message_text, gboolean is_user_message) {
  GtkWidget *msg = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
  gtk_widget_set_hexpand(msg, FALSE);
  gtk_box_append(GTK_BOX(vbox), msg);

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
}

static void on_open_file_response(GObject *source_object, GAsyncResult *res,
                                  gpointer user_data) {
  char fileMsg[256];
  memset(fileMsg, '\0', sizeof(fileMsg));
  strncpy(fileMsg, "/file", 5);
  send(*connSocket, fileMsg, sizeof(fileMsg), 0);
  g_autoptr(GFile) file = NULL;
  g_autofree char *path = NULL;

  file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(source_object), res, NULL);
  if (file != NULL) {
    path = g_file_get_path(file);
    g_print("sending file\n");
    sendFile(connSocket, path);
    add_message(g_strconcat(path, " is sent.", NULL), TRUE);
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

static void on_send_button_clicked(GtkButton *button, gpointer user_data) {
  GtkEntry *entry = GTK_ENTRY(user_data);
  const char *message_text = gtk_editable_get_text(GTK_EDITABLE(entry));
  if (g_strcmp0(message_text, "") != 0 && connSocket && connSocket >= 0) {
    add_message(message_text, TRUE);
    send(*connSocket, message_text, strlen(message_text), 0);
    gtk_editable_set_text(GTK_EDITABLE(entry), "");
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

static void create_message_area(GObject *source_object, GAsyncResult *res,
                                gpointer user_data) {
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
  g_signal_connect(entry, "activate", G_CALLBACK(enter_clicked_on_entry), NULL);

  GtkWidget *file_button =
      create_button("File", G_CALLBACK(on_file_button_clicked), NULL);
  gtk_box_append(GTK_BOX(hbox), file_button);

  GtkWidget *send_button =
      create_button("Send", G_CALLBACK(on_send_button_clicked), entry);
  gtk_box_append(GTK_BOX(hbox), send_button);
};

gboolean update_ui_with_message(gpointer data) {
  add_message((gchar *)data, FALSE);
  g_free(data);
  return FALSE;
}

void *receive_messages(gpointer data) {
  char receiveBuffer[256];
  gboolean expecting_file = FALSE;

  while (1) {
    memset(receiveBuffer, '\0', sizeof(receiveBuffer));
    ssize_t recv_len =
        recv(*connSocket, receiveBuffer, sizeof(receiveBuffer), 0);

    if (recv_len <= 0) {
      if (recv_len == 0) {
        printf("[-] Connection closed by peer\n");
      } else {
        perror("[-] recv error");
      }
      break;
    }

    if (strncmp(receiveBuffer, "/file", 5) == 0) {
      char file_name[100];
      receiveFile(connSocket, file_name);
      g_idle_add(update_ui_with_message,
                 g_strconcat(file_name, " file is received.", NULL));
    } else {
      // g_print("Hello world error");
      g_idle_add(update_ui_with_message, g_strdup(receiveBuffer));
    }
  }
  return NULL;
}

static void client_thread_func(GTask *task, gpointer source_object,
                               gpointer data, GCancellable *cancellable) {
  connSocket = getClientSocket(data);
  if (g_task_return_error_if_cancelled(task)) {
    return;
  }
  if (connSocket && connSocket >= 0) {
    g_thread_new("receive-messages", receive_messages, NULL);
    // receive_messages(NULL);
  }
}

static void server_thread_func(GTask *task, gpointer source_object,
                               gpointer data, GCancellable *cancellable) {
  connSocket = getServerSocket(data);
  if (g_task_return_error_if_cancelled(task)) {
    return;
  }
  if (connSocket && connSocket >= 0) {
    g_thread_new("receive-messages", receive_messages, NULL);
    // receive_messages(NULL);
  }
}

void button_callback(GtkButton *button, gpointer data) {
  const gchar *button_label = gtk_button_get_label(button);

  GTask *task = g_task_new(NULL, NULL, create_message_area, NULL);
  g_task_set_task_data(task, data, g_free);

  if (g_strcmp0(button_label, "Connect as Server") == 0) {
    g_task_run_in_thread(task, server_thread_func);
    g_object_unref(task);
    g_print("Connect as server clicked\n");
  } else if (g_strcmp0(button_label, "Connect as Receiver") == 0) {
    g_task_run_in_thread(task, client_thread_func);
    g_object_unref(task);
    g_print("Connect as receiver clicked\n");
  }

  GtkWidget *vertical_box_for_spinner = create_vertical_box(0, 0, 0, 0, 0);
  gtk_window_set_child(window, vertical_box_for_spinner);

  GtkWidget *spinner = gtk_spinner_new();
  gtk_widget_set_size_request(GTK_WIDGET(spinner), 50, 50);
  gtk_spinner_start(GTK_SPINNER(spinner));
  gtk_box_append(GTK_BOX(vertical_box_for_spinner), spinner);

  GtkWidget *spin_label =
      create_wrapped_label("Waiting for other device to connect...", 30);
  gtk_widget_set_margin_top(spin_label, 20);
  gtk_box_append(GTK_BOX(vertical_box_for_spinner), spin_label);
}

void on_device_list_activated(GtkListBox *listbox, GtkListBoxRow *row,
                              gpointer user_data) {
  if (!GTK_IS_WIDGET(row)) // Ensure row is a valid widget
    return;

  window =
      GTK_WINDOW(gtk_widget_get_ancestor(GTK_WIDGET(listbox), GTK_TYPE_WINDOW));
  GtkWidget *vertical_box_for_client_and_server =
      create_vertical_box(20, 0, 0, 0, 0);

  GtkWidget *label = gtk_widget_get_first_child(GTK_WIDGET(row));
  const char *item_text = gtk_label_get_text(GTK_LABEL(label));
  GtkWidget *new_label = create_wrapped_label(item_text, 30);
  gtk_box_append(GTK_BOX(vertical_box_for_client_and_server), new_label);

  GtkWidget *connect_as_server = create_button(
      "Connect as Server", G_CALLBACK(button_callback), g_strdup(item_text));
  gtk_box_append(GTK_BOX(vertical_box_for_client_and_server),
                 connect_as_server);

  GtkWidget *connect_as_receiver = create_button(
      "Connect as Receiver", G_CALLBACK(button_callback), g_strdup(item_text));
  gtk_box_append(GTK_BOX(vertical_box_for_client_and_server),
                 connect_as_receiver);

  gtk_window_set_child(window, vertical_box_for_client_and_server);
}
