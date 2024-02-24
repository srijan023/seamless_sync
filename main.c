#include <gtk/gtk.h>
#include "button.c"

  // GtkWidget *label;

static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  // gtk_label_set_text(GTK_LABEL(label), "CHanged!!!!!!!!!!!!!!");
  g_print ("Hello World\n");
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *button;
  // GtkWidget *button;
  GtkWidget *grid;
  // GtkWidget *box;
  // GtkWidget *box1;

  GtkWidget *label;

  

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Hello");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

 

  grid= gtk_grid_new();
  gtk_window_set_child(GTK_WINDOW(window),grid);
  label=gtk_label_new("Text here!");
  gtk_grid_attach(GTK_GRID(grid),label,0,1,2,1);
  // box=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  // gtk_widget_set_halign(box,GTK_ALIGN_CENTER);
  // gtk_widget_set_valign(box,GTK_ALIGN_CENTER);
  // gtk_window_set_child(GTK_WINDOW(window),box);
  
  // box1=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  // gtk_widget_set_halign(box1,GTK_ALIGN_CENTER);
  // gtk_widget_set_valign(box1,GTK_ALIGN_CENTER);
  // gtk_window_set_child(GTK_WINDOW(window),box1);
   button = gtk_button_new_with_label ("Button1");
  g_signal_connect (button, "clicked", G_CALLBACK (print_button_clicked), label);
  // gtk_box_append(GTK_BOX(box1),button);
   gtk_grid_attach(GTK_GRID(grid),button, 0,0,1,1);

   


  button = gtk_button_new_with_label ("Hello World");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  // gtk_box_append(GTK_BOX(box),button);
 
  gtk_grid_attach(GTK_GRID(grid),button, 1,0,1,1);

  // gtk_window_set_child (GTK_WINDOW (window), button);




// button = gtk_button_new_with_label ("Button1");
//   g_signal_connect (button, "clicked", G_CALLBACK (print_button_clicked), NULL);
//   gtk_window_set_child (GTK_WINDOW (window), button);


  gtk_window_present (GTK_WINDOW (window));
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}