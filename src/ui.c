#include "getMyIp.c"
#include <gtk/gtk.h>

// Function Prototypes
static void activate(GtkApplication *app, gpointer data);
GtkWidget *create_main_window(GtkApplication *app);
GtkWidget *create_vertical_box(int spacing, int t_margin, int b_margin,
                               int s_margin, int e_margin);
GtkWidget *create_wrapped_label(const char *text);
GtkWidget *create_image(const char *path, int width, int height);
GtkWidget *create_button(const char *label, GCallback on_button_clicked,
                         gpointer data);
GtkWidget *create_window(GtkApplication *app);
GtkWidget *create_grid();
void create_sidebar(GtkWidget *box, GtkWidget *main_content,
                    const char *items[], int n_items);

GtkWidget *create_window(GtkApplication *app) {
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Seamless Sync");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 600);
  return window;
}

static void on_item_selected(GtkListBox *listbox, GtkListBoxRow *row,
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

void create_sidebar(GtkWidget *box, GtkWidget *main_content,
                    const char *items[], int n_items) {
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
  g_signal_connect(sidebar, "row-selected", G_CALLBACK(on_item_selected),
                   main_content);

  // Make the sidebar scrollable
  GtkWidget *scrolled_window = gtk_scrolled_window_new();
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), sidebar);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                 GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_widget_set_size_request(scrolled_window, 120, -1);
  gtk_widget_set_vexpand(scrolled_window, TRUE);

  gtk_box_append(GTK_BOX(sidebar_container), scrolled_window);
  gtk_box_append(GTK_BOX(box), sidebar_container);
}

// Button click callback function to show different content
static void on_button_clicked(GtkWidget *widget, gpointer data) {
  GtkWindow *window = GTK_WINDOW(data);

  // Create the new main content area
  GtkWidget *main_content = create_vertical_box(10, 20, 20, 20, 20);
  const char *items[] = {
      "apple",      "ball",      "cat",    "Dog",      "Egg",       "Fish",
      "Gold",       "fig",       "grape",  "honeydew", "kiwi",      "lemon",
      "mango",      "nectarine", "orange", "pear",     "quince",    "raspberry",
      "strawberry", "tangerine", "ugli",   "vinegar",  "watermelon"};

  // Create the box containing the sidebar and the main content area
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  create_sidebar(box, main_content, items, sizeof(items) / sizeof(items[0]));
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

GtkWidget *create_button(const char *label, GCallback on_button_clicked,
                         gpointer data) {
  GtkWidget *button = gtk_button_new_with_label(label);
  g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), data);
  return button;
}

GtkWidget *create_image(const char *path, int width, int height) {
  GtkWidget *image = gtk_image_new_from_file(path);
  gtk_widget_set_size_request(image, width, height);
  return image;
}

GtkWidget *create_wrapped_label(const char *text) {
  GtkWidget *label = gtk_label_new(text);
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
  gtk_label_set_wrap(GTK_LABEL(label), TRUE);
  gtk_label_set_wrap_mode(GTK_LABEL(label), PANGO_WRAP_WORD);
  gtk_label_set_max_width_chars(GTK_LABEL(label), 30);
  return label;
}

GtkWidget *create_vertical_box(int spacing, int t_margin, int b_margin,
                               int s_margin, int e_margin) {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);
  gtk_widget_set_margin_top(box, t_margin);
  gtk_widget_set_margin_bottom(box, b_margin);
  gtk_widget_set_margin_start(box, s_margin);
  gtk_widget_set_margin_end(box, e_margin);
  gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
  gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
  return box;
}

static GtkWidget *create_menu() {
  // Create a new popover menu
  GtkWidget *popover = gtk_popover_new();
  GtkWidget *menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  // Create menu items
  GtkWidget *option1 = gtk_button_new_with_label("Option 1");
  GtkWidget *option2 = gtk_button_new_with_label("Option 2");
  GtkWidget *option3 = gtk_button_new_with_label("Option 3");

  // Add the menu items to the box
  gtk_box_append(GTK_BOX(menu_box), option1);
  gtk_box_append(GTK_BOX(menu_box), option2);
  gtk_box_append(GTK_BOX(menu_box), option3);

  // Set the box as the child of the popover
  gtk_popover_set_child(GTK_POPOVER(popover), menu_box);

  return popover;
}

// Create a main window i.e. first window to show when application launches
GtkWidget *create_main_window(GtkApplication *app) {
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Seamless Sync");
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 800);

  // Create header bar with menu button
  GtkWidget *header = gtk_header_bar_new();
  gtk_window_set_titlebar(GTK_WINDOW(window), header);
  GtkWidget *menu_button = gtk_menu_button_new();
  GtkWidget *popover = create_menu();
  gtk_menu_button_set_popover(GTK_MENU_BUTTON(menu_button), popover);
  gtk_widget_set_tooltip_text(menu_button, "Menu");
  gtk_header_bar_pack_end(GTK_HEADER_BAR(header), menu_button);

  GtkWidget *box = create_vertical_box(10, 20, 20, 20, 20);
  GtkWidget *label = create_wrapped_label(
      "To use Seamless Sync, verify your identity using the "
      "authentication methods you've turned on. Make sure other "
      "device has Seamless Sync turned on.");
  GtkWidget *image = create_image("../include/image.png", 200, 200);
  GtkWidget *button =
      create_button("Start", G_CALLBACK(on_button_clicked), window);

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
