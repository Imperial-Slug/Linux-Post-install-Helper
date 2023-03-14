#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

// TODO // USE A VARIABLE TO HOLD THE PROPER COMMAND DEPENDING ON THE GPU CHOSEN ///////////////////
static void deb_nvidia_toggled(GtkWidget *widget, gpointer data);
static void deb_steam_toggled(GtkWidget *widget, gpointer data);
static void deb_game_toggled(GtkWidget *widget, gpointer data);
static void deb_flatpak_toggled(GtkWidget *widget, gpointer data);
static void deb_microcode_toggled(GtkWidget *widget, gpointer data);
static void deb_fonts_toggled(GtkWidget *widget, gpointer data);



static void
debian_window (GtkWidget *widget,
             gpointer   data)
{
  GtkWidget *deb_window;
  GtkWidget *deb_box, *deb_nvidia_check, *deb_steam_check,*deb_game_check, *deb_flatpak_check, *deb_microcode_check, *deb_fonts_check;
  deb_window = gtk_window_new();
  gtk_window_set_title(GTK_WINDOW(deb_window), "Linux Post-install Helper: Debian");
  gtk_window_set_resizable (GTK_WINDOW(deb_window), FALSE);
  gtk_window_set_default_size(GTK_WINDOW(deb_window), 700, 700);

  GtkWidget *view;
  GtkTextBuffer *buffer;
  
  deb_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_window_set_child (GTK_WINDOW (deb_window), deb_box);
  
  // CHECKBOXES //////////
  
    deb_nvidia_check = gtk_check_button_new_with_label("  Do you have an Nvidia graphics card?");
    gtk_box_append(GTK_BOX(deb_box), deb_nvidia_check);

    deb_steam_check = gtk_check_button_new_with_label("  Do you plan on using steam?");
    gtk_box_append(GTK_BOX(deb_box), deb_steam_check);

    deb_game_check = gtk_check_button_new_with_label("  Do you plan on playing video games?");
    gtk_box_append(GTK_BOX(deb_box),deb_game_check );
    ////////////////////////////////////////////////
    
    deb_flatpak_check = gtk_check_button_new_with_label("  Do you want to use flatpak applications?");
    gtk_box_append(GTK_BOX(deb_box), deb_flatpak_check);

    deb_microcode_check = gtk_check_button_new_with_label("  Install your processor's latest microcode?");
    gtk_box_append(GTK_BOX(deb_box), deb_microcode_check);

    deb_fonts_check = gtk_check_button_new_with_label("  Install restricted fonts compatibility for Microsoft products?");
    gtk_box_append(GTK_BOX(deb_box),deb_fonts_check );
    
  
  view = gtk_text_view_new ();
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
  gtk_text_buffer_set_text (buffer, "  sudo apt update && sudo apt upgrade; \n  sudo apt update && sudo apt full-upgrade; \n  sudo apt install build-essential dkms linux-headers-$(uname -r); \n", -1);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
  gtk_widget_set_can_focus(GTK_WIDGET(view), FALSE);
  gtk_box_append(GTK_BOX(deb_box), view);
  
  // checkbox logic
  
  g_signal_connect(G_OBJECT(deb_nvidia_check), "toggled", G_CALLBACK(deb_nvidia_toggled), buffer);
  g_signal_connect(G_OBJECT(deb_steam_check), "toggled", G_CALLBACK(deb_steam_toggled), buffer);
  g_signal_connect(G_OBJECT(deb_game_check), "toggled", G_CALLBACK(deb_game_toggled), buffer);
  
  g_signal_connect(G_OBJECT(deb_flatpak_check), "toggled", G_CALLBACK(deb_flatpak_toggled), buffer);
  g_signal_connect(G_OBJECT(deb_microcode_check), "toggled", G_CALLBACK(deb_microcode_toggled), buffer);
  g_signal_connect(G_OBJECT(deb_fonts_check), "toggled", G_CALLBACK(deb_fonts_toggled), buffer);
  
  gtk_widget_show(deb_window);
  
}

////////// DEBIAN NVIDIA CHECKBOX ///////////////

static void deb_nvidia_toggled(GtkWidget *widget, gpointer data) {
  
  gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer *buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state) {
    gtk_text_buffer_get_end_iter(buffer, &iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, &iter, "  sudo apt install nvidia-driver nvidia-driver-libs:i386 nvidia-driver-libs;\n", -1);
  } else {
    GtkTextIter start, end;
    const gchar *search_string = "  sudo apt install nvidia-driver nvidia-driver-libs:i386 nvidia-driver-libs;";

    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);


    while (gtk_text_iter_forward_search(&start, search_string, 0, &start, NULL, NULL))
    {
        gtk_text_buffer_delete(buffer, &start, &end);
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
    }
  }
}

///////////////////////////////////////////////////////////

//// DEBIAN STEAM CHECKBOX ///////


static void deb_steam_toggled(GtkWidget *widget, gpointer data) {
  
  gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer *buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state) {
    gtk_text_buffer_get_end_iter(buffer, &iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, &iter, "  sudo apt install steam-devices steam-installer; \n", -1);
  } else {
    GtkTextIter start, end;
    const gchar *search_string = "  sudo apt install steam-devices steam-installer;";

    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);


    while (gtk_text_iter_forward_search(&start, search_string, 0, &start, NULL, NULL))
    {
        gtk_text_buffer_delete(buffer, &start, &end);
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
    }
  }
}



//// DEBIAN GAME CHECKBOX ///////


static void deb_game_toggled(GtkWidget *widget, gpointer data) {
  
  gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer *buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state) {
    gtk_text_buffer_get_end_iter(buffer, &iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, &iter, "  sudo dpkg --add-architecture i386; sudo apt install mesa-vulkan-drivers libvulkan1;\n  sudo apt install vulkan-tools vulkan-validationlayers gamemode; \n", -1);
  } else {
    GtkTextIter start, end;
    const gchar *search_string = "  sudo dpkg --add-architecture i386; sudo apt install mesa-vulkan-drivers libvulkan1;\n  sudo apt install vulkan-tools vulkan-validationlayers gamemode;";

    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);


    while (gtk_text_iter_forward_search(&start, search_string, 0, &start, NULL, NULL))
    {
        gtk_text_buffer_delete(buffer, &start, &end);
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
    }
  }
}



//////////////////////////////////////////
/////////////////////////////////////////

////////// DEBIAN FLATPAK CHECKBOX ///////////////

static void deb_flatpak_toggled(GtkWidget *widget, gpointer data) {
  
  gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer *buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state) {
    gtk_text_buffer_get_end_iter(buffer, &iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, &iter, "  sudo apt install flatpak; sudo apt install gnome-software-plugin-flatpak; sudo flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo; \n", -1);
  } else {
    GtkTextIter start, end;
    const gchar *search_string = "  sudo apt install flatpak; sudo apt install gnome-software-plugin-flatpak; sudo flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo;";

    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);


    while (gtk_text_iter_forward_search(&start, search_string, 0, &start, NULL, NULL))
    {
        gtk_text_buffer_delete(buffer, &start, &end);
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
    }
  }
}

///////////////////////////////////////////////////////////

//// DEBIAN MICROCODE CHECKBOX ///////


static void deb_microcode_toggled(GtkWidget *widget, gpointer data) {
  
  gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer *buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state) {
    gtk_text_buffer_get_end_iter(buffer, &iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, &iter, "  sudo apt install amd64-microcode; \n", -1);
  } else {
    GtkTextIter start, end;
    const gchar *search_string = "  sudo apt install amd64-microcode;";

    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);


    while (gtk_text_iter_forward_search(&start, search_string, 0, &start, NULL, NULL))
    {
        gtk_text_buffer_delete(buffer, &start, &end);
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
    }
  }
}



//// DEBIAN FONTS CHECKBOX ///////


static void deb_fonts_toggled(GtkWidget *widget, gpointer data) {
  
  gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer *buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state) {
    gtk_text_buffer_get_end_iter(buffer, &iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, &iter, "  sudo apt install ttf-mscorefonts-installer rar unrar libavcodec-extra; \n sudo apt install gstreamer1.0-libav gstreamer1.0-plugins-ugly gstreamer1.0-vaapi; \n sudo apt install fonts-crosextra-carlito fonts-crosextra-caladea;  \n", -1);
  } else {
    GtkTextIter start, end;
    const gchar *search_string = "  sudo apt install ttf-mscorefonts-installer rar unrar libavcodec-extra; \n sudo apt install gstreamer1.0-libav gstreamer1.0-plugins-ugly gstreamer1.0-vaapi; \n sudo apt install fonts-crosextra-carlito fonts-crosextra-caladea;";

    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);


    while (gtk_text_iter_forward_search(&start, search_string, 0, &start, NULL, NULL))
    {
        gtk_text_buffer_delete(buffer, &start, &end);
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
    }
  }
}

























//////////////////////////////////////////
static void
fedora_window (GtkWidget *widget,
	       gpointer data)
{
  GtkWidget *fed_window;
  GtkWidget *fed_box, *fed_nvidia_check,  *deb_steam_check,*deb_game_check;
  fed_window = gtk_window_new();
  gtk_window_set_title(GTK_WINDOW(fed_window), "Linux Post-install Helper: Fedora");
  gtk_window_set_default_size(GTK_WINDOW(fed_window), 700, 700);
  gtk_window_set_resizable (GTK_WINDOW(fed_window), FALSE);
  GtkWidget *view;
  GtkTextBuffer *buffer;
  
  fed_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_window_set_child (GTK_WINDOW (fed_window), fed_box);
  
    fed_nvidia_check = gtk_check_button_new_with_label("  Do you have an Nvidia graphics card?");
    gtk_box_append(GTK_BOX(fed_box), fed_nvidia_check);

    deb_steam_check = gtk_check_button_new_with_label("Checkbox 2");
    gtk_box_append(GTK_BOX(fed_box), deb_steam_check);

   deb_game_check = gtk_check_button_new_with_label("Checkbox 3");
    gtk_box_append(GTK_BOX(fed_box),deb_game_check);
  
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

