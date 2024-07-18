#include "getMyIp.c"
#include <gtk/gtk.h>

// Function Prototypes
GtkWidget *create_window(GtkApplication *app);
GtkWidget *create_vertical_box();
GtkWidget *create_grid();
GtkWidget *create_wrapped_label(const char *text);
GtkWidget *create_image(const char *path, int width, int height);
GtkWidget *create_button(const char *label, GCallback callback, gpointer data);
GtkWidget *create_main_window(GtkApplication *app);
static void activate(GtkApplication *app, gpointer data);

GtkWidget *create_window(GtkApplication *app) {
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Seamless Sync");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 600);
  return window;
}

GtkWidget *create_vertical_box() {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_margin_top(box, 20);
  gtk_widget_set_margin_bottom(box, 20);
  gtk_widget_set_margin_start(box, 20);
  gtk_widget_set_margin_end(box, 20);
  gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
  gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
  return box;
}

/* GtkWidget *create_grid() {
  GtkWidget *grid = gtk_grid_new();
  gtk_widget_set_margin_end(GTK_WIDGET(grid), 75);
  gtk_widget_set_margin_start(GTK_WIDGET(grid), 75);
  gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
  gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
  return grid;
} */

GtkWidget *create_wrapped_label(const char *text) {
  GtkWidget *label = gtk_label_new(text);
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
  gtk_label_set_wrap(GTK_LABEL(label), TRUE);
  gtk_label_set_wrap_mode(GTK_LABEL(label), PANGO_WRAP_WORD);
  gtk_label_set_max_width_chars(GTK_LABEL(label), 30);
  return label;
}

GtkWidget *create_image(const char *path, int width, int height) {
  GtkWidget *image = gtk_image_new_from_file(path);
  gtk_widget_set_size_request(image, width, height);
  return image;
}

GtkWidget *create_button(const char *label, GCallback callback, gpointer data) {
  GtkWidget *button = gtk_button_new_with_label(label);
  g_signal_connect(button, "clicked", G_CALLBACK(callback), data);
  return button;
}

// Button click callback function to show different content
static void on_button_clicked(GtkWidget *widget, gpointer data) {
  GtkWindow *window = GTK_WINDOW(data);

  GtkWidget *label = gtk_label_new(findMyIP().message);
  GtkWidget *old_child = gtk_window_get_child(window);

  // Remove the old child widget from the window
  if (old_child) {
    gtk_window_set_child(window, NULL); // Remove old child
    gtk_widget_unparent(old_child);     // Unparent old child
  }

  // set the new child widget for the window
  gtk_window_set_child(window, label);
}

// Create a main window i.e. first window to show when application launches
GtkWidget *create_main_window(GtkApplication *app) {
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Seamless Sync");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 600);

  GtkWidget *box = create_vertical_box();
  GtkWidget *label = create_wrapped_label(
      "To use Seamless Sync, verify your identity using the "
      "authentication methods you've turned on. Make sure other "
      "device has Seamless Sync turned on.");
  GtkWidget *image = create_image("../include/image.png", 200, 200);
  GtkWidget *button =
      create_button("Verify", G_CALLBACK(on_button_clicked), window);

  gtk_box_append(GTK_BOX(box), label);
  gtk_box_append(GTK_BOX(box), image);
  gtk_box_append(GTK_BOX(box), button);
  gtk_window_set_child(GTK_WINDOW(window), box);

  return window;
}

// Activate function called when the application starts
static void activate(GtkApplication *app, gpointer data) {
  GtkWidget *window = create_main_window(app);
  gtk_window_present(GTK_WINDOW(window));
}
