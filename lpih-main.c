#include <gtk/gtk.h>
#include <string.h>

static void
debian_window (GtkWidget *widget,
             gpointer   data)
{
  g_print ("debian_window engaged\n");



}


static void
fedora_window (GtkWidget *widget,
	       gpointer data)
{
g_print ("fedora_window engaged\n");


}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button;
 ///////////////////////////////////////////////////////////////////////////

  /* create a new window, and set its title */
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Linux Post-install Helper");
  gtk_widget_set_size_request(window, 444, 444);
   gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
/////////////////////////////////////////////////////////////////////////



  
  /* Here we construct the container that is going pack our buttons */
  grid = gtk_grid_new ();
  gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE); // Make rows of equal height
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE); // Make columns of equal width
    gtk_grid_set_row_spacing(GTK_GRID(grid), 50); // Add spacing between rows
    gtk_grid_set_column_spacing(GTK_GRID(grid), 100); // Add spacing between columns
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
  /* Pack the container in the window */
  gtk_window_set_child (GTK_WINDOW (window), grid);



  button = gtk_button_new_with_label ("DEBIAN");
    
  gtk_widget_set_size_request(button, 100, 50);

  g_signal_connect (button, "clicked", G_CALLBACK (debian_window), NULL);
    
  /* Place the first button in the grid cell (0, 0), and make it fill
   * just 1 cell horizontally and vertically (ie no spanning)
   */
  gtk_grid_attach (GTK_GRID (grid), button, 0, 0, 1, 1);

  button = gtk_button_new_with_label ("FEDORA");
 
    gtk_widget_set_size_request(button, 100, 50);

g_signal_connect (button, "clicked", G_CALLBACK (fedora_window), NULL);

  /* Place the second button in the grid cell (1, 0), and make it fill
   * just 1 cell horizontally and vertically (ie no spanning)
   */
  gtk_grid_attach (GTK_GRID (grid), button, 1, 0, 1, 1);
  
  button = gtk_button_new_with_label ("QUIT");
  
 gtk_widget_set_name (button, "quit");

      gtk_widget_set_size_request(button, 100, 50);

  g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_window_destroy), window);

  /* Place the Quit button in the grid cell (0, 1), and make it
   * span 2 columns.
   */
  gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 2, 1);
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
