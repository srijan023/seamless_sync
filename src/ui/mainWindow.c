#include "button.c"
#include "headerBar.c"
#include "image.c"
#include "onStartClicked.c"
#include "wrappedLabel.c"

// Create a main window i.e. first window to show when application launches
GtkWidget *create_main_window(GtkApplication *app) {
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Seamless Sync");
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 800);

  GtkWidget *header = create_header_bar();
  gtk_window_set_titlebar(GTK_WINDOW(window), header);

  GtkWidget *box = create_vertical_box(10, 20, 20, 20, 20);
  GtkWidget *label = create_wrapped_label(
      "To use Seamless Sync, verify your identity using the "
      "authentication methods you've turned on. Make sure other "
      "device has Seamless Sync turned on.");
  GtkWidget *image = create_image("../include/image.png", 200, 200);
  GtkWidget *button =
      create_button("Start", G_CALLBACK(on_start_clicked), window);

  gtk_box_append(GTK_BOX(box), label);
  gtk_box_append(GTK_BOX(box), image);
  gtk_box_append(GTK_BOX(box), button);
  gtk_window_set_child(GTK_WINDOW(window), box);

  return window;
}
