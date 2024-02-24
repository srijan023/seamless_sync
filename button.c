#include<gtk/gtk.h>
void print_button_clicked(GtkWidget *widget,gpointer data){
  GtkWidget *label=GTK_WIDGET(data);
  
//  g_print("%d\n",GPOINTER_TO_INT(data));
  gtk_label_set_text(GTK_LABEL(data), "CHanged!!!!!!!!!!!!!!");

}