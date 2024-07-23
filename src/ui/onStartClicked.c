#include "../SSDPListenerConnection.c"
#include "sideBar.c"

void on_start_clicked(GtkWidget *widget, gpointer data) {
  GtkWindow *window = GTK_WINDOW(data);

  // Create the new main content area
  GtkWidget *main_content = create_vertical_box(10, 20, 20, 20, 20);
  const char *items[] = {
      "apple",      "ball",      "cat",    "Dog",      "Egg",       "Fish",
      "Gold",       "fig",       "grape",  "honeydew", "kiwi",      "lemon",
      "mango",      "nectarine", "orange", "pear",     "quince",    "raspberry",
      "strawberry", "tangerine", "ugli",   "vinegar",  "watermelon"};

  pthread_t SSDPThread;
  int doLooping = 1;
  pthread_create(&SSDPThread, NULL, SSDPListen, &doLooping);
  // sleep(10);
  doLooping = 0;
  struct ssdpMessage *revMsg;
  pthread_join(SSDPThread, (void **)&revMsg);

  // Create the box containing the sidebar and the main content area
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  // create_sidebar(box, main_content, items, sizeof(items) / sizeof(items[0]));
  create_sidebar(box, main_content, revMsg->arr, revMsg->size);

  gtk_box_append(GTK_BOX(box), main_content);

  // Remove the old child widget from the window
  GtkWidget *old_child = gtk_window_get_child(window);
  if (old_child) {
    gtk_window_set_child(window, NULL); // Remove old child
    gtk_widget_unparent(old_child);     // Unparent old child
  }

  // set the new child widget for the window
  gtk_window_set_child(window, box);
}
