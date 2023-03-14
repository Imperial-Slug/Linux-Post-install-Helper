#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>


static void nvidia_toggled(GtkWidget *widget, gpointer data);





static void
debian_window (GtkWidget *widget,
             gpointer   data)
{
  GtkWidget *deb_window;
  GtkWidget *deb_box, *deb_nvidia_check, *check2, *check3;
  deb_window = gtk_window_new();
  gtk_window_set_title(GTK_WINDOW(deb_window), "Linux Post-install Helper: Debian");
  gtk_window_set_resizable (GTK_WINDOW(deb_window), FALSE);
  gtk_window_set_default_size(GTK_WINDOW(deb_window), 700, 700);

  GtkWidget *view;
  GtkTextBuffer *buffer;
  
  deb_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_window_set_child (GTK_WINDOW (deb_window), deb_box);
  
  // CHECKBOXES //////////
  
    deb_nvidia_check = gtk_check_button_new_with_label("Do you have an Nvidia graphics card?");
    gtk_box_append(GTK_BOX(deb_box), deb_nvidia_check);

    check2 = gtk_check_button_new_with_label("Checkbox 2");
    gtk_box_append(GTK_BOX(deb_box), check2);

    check3 = gtk_check_button_new_with_label("Checkbox 3");
    gtk_box_append(GTK_BOX(deb_box), check3);
  
  view = gtk_text_view_new ();
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
  gtk_text_buffer_set_text (buffer, "sudo apt update && sudo apt upgrade; sudo apt update && sudo apt full-upgrade; ", -1);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
  gtk_widget_set_can_focus(GTK_WIDGET(view), FALSE);
  gtk_box_append(GTK_BOX(deb_box), view);
  
  // checkbox logic
  
  g_signal_connect(G_OBJECT(deb_nvidia_check), "toggled", G_CALLBACK(nvidia_toggled), buffer);
  
  gtk_widget_show(deb_window);
  
}

static void nvidia_toggled(GtkWidget *widget, gpointer data) {
  
  gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer *buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state) {
    gtk_text_buffer_get_end_iter(buffer, &iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, &iter, "\nsudo apt install nvidia-driver; \n", -1);
  } else {
    gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0); // Move the iterator to the start
    GtkTextIter end_iter;
gtk_text_buffer_get_end_iter(buffer, &end_iter);
gchar *text = gtk_text_buffer_get_text(buffer, &iter, &end_iter, FALSE);
    gchar *substring = "\nsudo apt install nvidia-driver; \n";
    gchar *p = strstr(text, substring);
    if (p) {
      GtkTextIter start;
      gtk_text_iter_set_offset(&start, p - text);
      gtk_text_buffer_delete(buffer, &start, &iter); // Delete the substring
    }
    g_free(text);
  }
}


static void
fedora_window (GtkWidget *widget,
	       gpointer data)
{
  GtkWidget *fed_window;
  GtkWidget *fed_box, *fed_nvidia_check, *check2, *check3;
  fed_window = gtk_window_new();
  gtk_window_set_title(GTK_WINDOW(fed_window), "Linux Post-install Helper: Fedora");
  gtk_window_set_default_size(GTK_WINDOW(fed_window), 700, 700);
  gtk_window_set_resizable (GTK_WINDOW(fed_window), FALSE);
  GtkWidget *view;
  GtkTextBuffer *buffer;
  
  fed_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_window_set_child (GTK_WINDOW (fed_window), fed_box);
  
    fed_nvidia_check = gtk_check_button_new_with_label("Do you have an Nvidia graphics card?");
    gtk_box_append(GTK_BOX(fed_box), fed_nvidia_check);

    check2 = gtk_check_button_new_with_label("Checkbox 2");
    gtk_box_append(GTK_BOX(fed_box), check2);

    check3 = gtk_check_button_new_with_label("Checkbox 3");
    gtk_box_append(GTK_BOX(fed_box), check3);
  
  view = gtk_text_view_new ();
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
  gtk_text_buffer_set_text (buffer, "sudo dnf update && sudo dnf upgrade; sudo dnf update && sudo dnf full-upgrade; ", -1);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
  gtk_widget_set_can_focus(GTK_WIDGET(view), FALSE); //stops cursor from showing in textview when clicked. 
  gtk_box_append(GTK_BOX(fed_box), view);
  
gtk_widget_show(fed_window);  

}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *deb_button;
  GtkWidget *fed_button;
  GtkWidget *quit_button;
  
  
 ///////////////////////////////////////////////////////////////////////////

  /* create a new window, and set its title */
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Linux Post-install Helper");
  gtk_widget_set_size_request(window, 444, 444);
  gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
    
/////////////////////////////////////////////////////////////////////////



  

  grid = gtk_grid_new ();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE); // Make rows of equal height
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE); // Make columns of equal width
    gtk_grid_set_row_spacing(GTK_GRID(grid), 50); // Add spacing between rows
    gtk_grid_set_column_spacing(GTK_GRID(grid), 100); // Add spacing between columns
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

  gtk_window_set_child (GTK_WINDOW (window), grid);

  deb_button = gtk_button_new_with_label ("DEBIAN");
  gtk_widget_set_size_request(deb_button, 100, 50);
  g_signal_connect (deb_button, "clicked", G_CALLBACK (debian_window), NULL);
    
  /* Place the first button in the grid cell (0, 0), and make it fill
   * just 1 cell horizontally and vertically (ie no spanning)
   */
  gtk_grid_attach (GTK_GRID (grid), deb_button, 0, 0, 1, 1);
  fed_button = gtk_button_new_with_label ("FEDORA");
  gtk_widget_set_size_request(fed_button, 100, 50);
  g_signal_connect (fed_button, "clicked", G_CALLBACK (fedora_window), NULL);

  /* Place the second button in the grid cell (1, 0), and make it fill
   * just 1 cell horizontally and vertically (ie no spanning)
   */
  gtk_grid_attach (GTK_GRID (grid), fed_button, 1, 0, 1, 1);
  quit_button = gtk_button_new_with_label ("QUIT");
  
  /////////////////////////////////////////
  
  gtk_widget_set_name (quit_button, "quit");
  gtk_widget_set_size_request(quit_button, 100, 50);
  g_signal_connect_swapped (quit_button, "clicked", G_CALLBACK (gtk_window_destroy), window);

  /* Place the Quit button in the grid cell (0, 1), and make it
   * span 2 columns.
   */
  gtk_grid_attach (GTK_GRID (grid), quit_button, 0, 1, 2, 1);
  gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

  gtk_widget_show (window);

}


int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("petc0016.project.lpisg", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
  
  return status;
}

