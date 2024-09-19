/* lpih-window.c
 *
 * Copyright 2023 Samuel Petch
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gtk/gtk.h>

#include <stdio.h>

#include <stdlib.h>

#include <stdint.h>

#include "utility.h"

#include "lpih-window.h"

#include "info-window.h"

// DEBIAN CONSTANTS // // // // // // // // // // // // // // // // // // // // //

const gchar * DEBIAN_STEAM = "  sudo dpkg --add-architecture i386; sudo apt update; \n  sudo apt install steam-devices steam-installer; sudo apt install mesa-vulkan-drivers libvulkan1;\n  sudo apt install vulkan-tools vulkan-validationlayers \n";

const gchar * DEBIAN_GAMING = "  sudo apt install gamemode;  \n";

const gchar * DEBIAN_FLATPAK = "  sudo apt install flatpak gnome-software-plugin-flatpak; \n  sudo flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo; \n";

const gchar * DEBIAN_MULTIMEDIA = "  sudo apt install libavcodec-extra;  \n  sudo apt install gstreamer1.0-libav gstreamer1.0-plugins-ugly gstreamer1.0-vaapi;  \n  sudo apt install fonts-crosextra-carlito fonts-crosextra-caladea;  \n";

const gchar * DEBIAN_UFW = "  sudo apt install ufw; sudo ufw enable; \n";
const gchar * DEBIAN_TLP = "  sudo apt install tlp; \n";
const gchar * DEBIAN_VLC = "  sudo apt install vlc; \n";
const gchar * DEBIAN_GH = "  sudo apt install git gh; \n";

gboolean debian_window_open = FALSE;
gboolean fedora_window_open = FALSE;

const gchar * debian_microcode_command;
const gchar * debian_gpu_command;

// const gchar* DEBIAN_CONTRIB_NONFREE = "sudo sh -c 'if test -f /etc/apt/sources.list; then echo "contrib non-free" >> /etc/apt/sources.list; fi'"

// FEDORA CONSTANTS // // // // // // // // // // // // // // // // // // // // //

const gchar * FEDORA_REP = "  sudo dnf install https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm;  \n  sudo dnf install https://download1.rpmfusion.org/nonfree/fedora/rpmfusion-nonfree-release-$(rpm -E %fedora).noarch.rpm;  \n  sudo dnf update; \n";

const gchar * FEDORA_STEAM = "  sudo dnf install steam;\n";

const gchar * FEDORA_DNF = "  sudo sh -c 'if test -f /etc/dnf/dnf.conf; then echo \"max_parallel_downloads=20\" >> /etc/dnf/dnf.conf; fi'\n";

const gchar * FEDORA_FLATPAK = "  flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo; \n";

const gchar * FEDORA_CUST = "  sudo dnf install gnome-tweaks gnome-extensions-app; \n";

const gchar * FEDORA_MULTIMEDIA = "  sudo dnf install ffmpeg --allowerasing &&  \n  sudo dnf install gstreamer1-plugins-{bad-\\*,good-\\*,base} gstreamer1-plugin-openh264 gstreamer1-libav --exclude=gstreamer1-plugins-bad-free-devel &&  \n  sudo dnf install lame\\* --exclude=lame-devel && sudo dnf group upgrade --with-optional Multimedia \n";

const gchar * FEDORA_TLP = "  sudo dnf install tlp; \n";

const gchar * FEDORA_VLC = "  sudo dnf install vlc; \n";

const gchar * FEDORA_GH = "  sudo dnf install git gh; \n";

const gchar * fedora_gpu_command;

// // // // // // // // // // // // // // // // // // // // //

enum Distro distro_debian = DEBIAN;

gchar * css_label_debian = "deb_window";
gchar * window_title_debian = "Linux Post-install Helper: Debian";
gchar * view_css_class_debian = "deb_view";
gchar * info_button_css_class_debian = "deb_info_button";
gchar * info_window_css_class_debian = "fed_info_button";

gchar * checkbox1_title_debian = "  Do you plan on using Steam?";
gchar * checkbox2_title_debian = "  Do you want to use flatpak applications?";
gchar * checkbox3_title_debian = "  Install applicable GPU drivers?";
gchar * checkbox4_title_debian = "  Install tlp for laptop power management?";
gchar * checkbox5_title_debian = "  Install vlc to play unsupported media formats?";
gchar * checkbox6_title_debian = "  Install restricted fonts compatibility for Microsoft products and multimedia compatibility packages?";
gchar * checkbox7_title_debian = "  Install your processor's latest microcode?";
gchar * checkbox8_title_debian = "  Do you want to install gamemode?";
gchar * checkbox9_title_debian = "  Do you want to install ufw? (uncomplicated firewall)";
gchar * checkbox10_title_debian = "  Do you want to install git and github command-line tools?";

// // // // // // // // // // // // // // // // // // // // //

gchar * css_label_fedora = "fed_window";
gchar * window_title_fedora = "Linux Post-install Helper: Fedora";
gchar * view_css_class_fedora = "fed_view";
gchar * info_button_css_class_fedora = "fed_info_button";

enum Distro distro_fedora = FEDORA;

gchar * checkbox1_title_fedora = "  Optimize the dnf package manager for faster downloads?";
gchar * checkbox2_title_fedora = "  Enable RPM-fusion repositories for wider range of software?";
gchar * checkbox3_title_fedora = "  Install applicable GPU drivers?";
gchar * checkbox4_title_fedora = "  Install tlp for laptop power management?";
gchar * checkbox5_title_fedora = "  Install vlc to play unsupported media formats?";
gchar * checkbox6_title_fedora = "  Install restricted fonts compatibility for Microsoft products and multimedia compatibility packages?";
gchar * checkbox7_title_fedora = "  Install gnome-tweaks and gnome-extensions for desktop customization?";
gchar * checkbox8_title_fedora = "  Do you plan on using Steam?";
gchar * checkbox9_title_fedora = "  Do you want to use flatpak applications?";
gchar * checkbox10_title_fedora = "  Do you want to install git and github command-line tools?";

// Keeping track of whether the Fedora and Debian windows are open.
gboolean debian_info_open = FALSE;
gboolean fedora_info_open = FALSE;

enum CheckboxNumber get_number_from_checkbox_label(gchar *checkbox_label, enum Distro distro) {
   
   if (distro == DEBIAN) { 
      enum CheckboxNumber checkbox_number;

   g_print("Line 141 lpih-window: checkbox_label = %s", checkbox_label);

 if (strstr(checkbox_label, "Steam") != NULL) {
  checkbox_number = 1;
  g_print("Checkbox1 clicked.\n");
  return checkbox_number;
 } else  if (strstr(checkbox_label, "flatpak") != NULL)  {
  checkbox_number = 2;
  g_print("Checkbox2 clicked.\n");
  return checkbox_number;
 } else  if (strstr(checkbox_label, "GPU") != NULL) { 
  checkbox_number = 3;
  g_print("Checkbox3 clicked.\n");
  return checkbox_number;
 } else if (strstr(checkbox_label, "power") != NULL) { 
  checkbox_number = 4;
  g_print("Checkbox4clicked.\n");
  return checkbox_number;
 }else if (strstr(checkbox_label, "unsupported media") != NULL) {
  checkbox_number = 5;
  g_print("Checkbox5 clicked.\n");
  return checkbox_number;
 }else if (strstr(checkbox_label, "compatibility") != NULL) {
  checkbox_number = 6;
  g_print("Checkbox6 clicked.\n");
  return checkbox_number;
 }else if  (strstr(checkbox_label, "microcode") != NULL) {
  checkbox_number = 7;
  g_print("Checkbox7 clicked.\n");
  return checkbox_number;
 } else if (strstr(checkbox_label, "gamemode") != NULL) {
  checkbox_number = 8;
  g_print("Checkbox8 clicked.\n");
  return checkbox_number;
 } else if (strstr(checkbox_label, "ufw") != NULL) {
  checkbox_number = 9;
  g_print("Checkbox9 clicked.\n");
  return checkbox_number;
 }else if (strstr(checkbox_label, "github") != NULL) {
  checkbox_number = 10;
  g_print("Checkbox10 clicked.\n");
  return checkbox_number;
 } else { g_print("***********ERROR: Failed to identify the checkbox that was clicked.\n"); }

    } else if (distro == FEDORA){
    
       enum CheckboxNumber checkbox_number;
       
    if (strstr(checkbox_label, "dnf") != NULL) {
  checkbox_number = 1;
  g_print("Checkbox1 clicked.\n");
  return checkbox_number;
 } else  if (strstr(checkbox_label, "RPM") != NULL)  {
  checkbox_number = 2;
  g_print("Checkbox2 clicked.\n");
  return checkbox_number;
 } else  if (strstr(checkbox_label, "GPU") != NULL) { 
  checkbox_number = 3;
  g_print("Checkbox3 clicked.\n");
  return checkbox_number;
 } else if (strstr(checkbox_label, "power") != NULL) { 
  checkbox_number = 4;
  g_print("Checkbox4clicked.\n");
  return checkbox_number;
 }else if (strstr(checkbox_label, "unsupported") != NULL) {
  checkbox_number = 5;
  g_print("Checkbox5 clicked.\n");
  return checkbox_number;
 }else if (strstr(checkbox_label, "compatibility") != NULL) {
  checkbox_number = 6;
  g_print("Checkbox6 clicked.\n");
  return checkbox_number;
 }else if  (strstr(checkbox_label, "gnome-tweaks") != NULL) {
  checkbox_number = 7;
  g_print("Checkbox7 clicked.\n");
  return checkbox_number;
 } else if (strstr(checkbox_label, "Steam") != NULL) {
  checkbox_number = 8;
  g_print("Checkbox8 clicked.\n");
  return checkbox_number;
 } else if (strstr(checkbox_label, "flatpak") != NULL) {
  checkbox_number = 9;
  g_print("Checkbox9 clicked.\n");
  return checkbox_number;
 }else if (strstr(checkbox_label, "github") != NULL) {
  checkbox_number = 10;
  g_print("Checkbox10 clicked.\n");
  return checkbox_number;
 } else { g_print("***********ERROR: Failed to identify the checkbox that was clicked.\n"); }
    }
    
  return 0;
 }


// Drill up through the widgets until we get to the nth one.  Return its title.
const gchar* get_nth_parent_window_title(GtkWidget *checkbox, uint8_t nth_parent){
// parent holds the widget the loop has drilled up to.
 uint8_t increment = 0;
 GtkWidget *current_parent = checkbox;
 
 // Get the next parent of the widget until we get to one that we can get a title from.
while (increment <= nth_parent) {
 current_parent = gtk_widget_get_parent(GTK_WIDGET(current_parent));  
 increment++; 
    
  }
 
 
  const gchar *nth_parent_title = gtk_window_get_title(GTK_WINDOW(current_parent));
  

  
  return nth_parent_title;
 
}


enum Distro get_distro_from_parent_title(GtkWidget *widget) {
 
 const gchar *fifth_parent_title = get_nth_parent_window_title(widget, 4);
 
 g_print("The fifth_parent_title of this window is %s \n", fifth_parent_title);
 
 if (strstr(fifth_parent_title, "Debian") != NULL) {
 enum Distro debian = DEBIAN;  
 return debian;
} else if (strstr(fifth_parent_title, "Fedora") != NULL) {
 enum Distro fedora = FEDORA;
 return fedora;
} else 
g_print("Failed to get distro line 197 lpih-window\n");
return 0;

}

gboolean check_box_state(GtkWidget * checkbox, gpointer data) {
    
   GtkTextBuffer *buffer = GTK_TEXT_BUFFER(data);
   gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(checkbox));
   const gchar *checkbox_label = gtk_check_button_get_label(GTK_CHECK_BUTTON(checkbox));
   enum Distro distro = get_distro_from_parent_title(checkbox);
   const gchar * command_string;
  
   enum CheckboxNumber checkbox_number = get_number_from_checkbox_label((gchar *)checkbox_label, distro);
    
    if(distro == DEBIAN) {
          
         switch(checkbox_number)
         {
          case 1:
          command_string = DEBIAN_STEAM;
          break;
          case 2:          
          command_string = DEBIAN_FLATPAK;
          break;
          case 3:        
          command_string = debian_gpu_command;
          break;
          case 4:   
          command_string = DEBIAN_TLP;
          break;
          case 5:   
          command_string = DEBIAN_VLC;
          break;
          case 6:
          command_string = DEBIAN_MULTIMEDIA;
          break;   
          case 7:        
          command_string = debian_microcode_command;
          break;         
          case 8:        
          command_string = DEBIAN_GAMING;
          break;
          case 9:
          command_string = DEBIAN_UFW;
          break;
          case 10:
          command_string = DEBIAN_GH;
          break;
          default:
          g_print("Switch statement failed to read the top_widget_title of the checkbox being clicked.\n");
          break;
        }
  }
 
  else if(distro == FEDORA){
    
      switch(checkbox_number)
         {
          case 1:
          command_string = FEDORA_DNF;
          break;
          case 2:          
          command_string = FEDORA_REP;
          break;
          case 3:        
          command_string = fedora_gpu_command;
          break;
          case 4:   
          command_string = FEDORA_TLP;
          break;
          case 5:   
          command_string = FEDORA_VLC;
          break;
          case 6:
          command_string = FEDORA_MULTIMEDIA;
          break;   
          case 7:        
          command_string = FEDORA_CUST;
          break;         
          case 8:        
          command_string = FEDORA_STEAM;
          break;
          case 9:
          command_string = FEDORA_FLATPAK;
          break;
          case 10:
          command_string = FEDORA_GH;
          break;
          default:
          g_print("Switch statement failed to read the top_widget_title of the checkbox being clicked.\n");
          break;
         }
  
  }

    g_print("Trying to print %s\n", command_string);
  GtkTextIter iter;
  if (state) {
    gtk_text_buffer_get_end_iter(buffer, & iter);
    gtk_text_buffer_insert(buffer, & iter, command_string, -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar * search_string = command_string;

    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
  return TRUE;
}


GtkWidget * make_main_window(GtkApplication *app){

     MainWindowData *deb_window_data = g_malloc(sizeof(MainWindowData));   
     MainWindowData *fed_window_data = g_malloc(sizeof(MainWindowData));   
    /////////////////////////////////////////////////////////////////////////

    deb_window_data->distro = DEBIAN;
    deb_window_data->window_open_flag = debian_window_open;
    
    fed_window_data->distro = FEDORA;
    fed_window_data->window_open_flag = fedora_window_open;
   
    GtkWidget * window;
    GtkWidget * grid;
    GtkWidget * deb_button;
    GtkWidget * fed_button;
    GtkWidget * quit_button;

    window = gtk_application_window_new(app);

    gtk_widget_add_css_class(window, "main_window");
    gtk_window_set_title(GTK_WINDOW(window), "Linux Post-install Helper For Debian and Fedora");
    gtk_widget_set_size_request(window, 512, 256);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

    grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE); // Make rows of equal height
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE); // Make columns of equal width
    gtk_grid_set_row_spacing(GTK_GRID(grid), 50); // Add spacing between rows
    gtk_grid_set_column_spacing(GTK_GRID(grid), 100); // Add spacing between columns
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_window_set_child(GTK_WINDOW(window), grid);
    deb_button = gtk_button_new_with_label("DEBIAN");
    gtk_widget_add_css_class(deb_button, "deb");
    gtk_widget_set_size_request(deb_button, 128, 64);
    g_signal_connect(deb_button, "clicked", G_CALLBACK(lpih_window), deb_window_data);

    gtk_grid_attach(GTK_GRID(grid), deb_button, 0, 0, 1, 1);
    fed_button = gtk_button_new_with_label("FEDORA");
    gtk_widget_add_css_class(fed_button, "fed");
    gtk_widget_set_size_request(fed_button, 128, 64);

    g_signal_connect(fed_button, "clicked", G_CALLBACK(lpih_window), fed_window_data);

    gtk_grid_attach(GTK_GRID(grid), fed_button, 1, 0, 1, 1);
    quit_button = gtk_button_new_with_label("QUIT");
    gtk_widget_add_css_class(quit_button, "quit");
    gtk_widget_set_name(quit_button, "quit");
    gtk_widget_set_size_request(quit_button, 128, 64);
    g_signal_connect_swapped(quit_button, "clicked", G_CALLBACK(gtk_window_destroy), window);
    gtk_grid_attach(GTK_GRID(grid), quit_button, 0, 1, 2, 1);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
    init_css_provider();
    gtk_widget_set_can_focus(GTK_WIDGET(window), TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(grid), TRUE);

    return GTK_WIDGET(window);

}

// Creates the main Debian or Fedora windows  when clicking the buttons on the main menu.
void lpih_window(GtkWidget * widget, gpointer data) {

MainWindowData *main_window_data = (MainWindowData *)data;

  if (widget != NULL) {
    g_print("info_button clicked.  Executing lpih_window function.\n");
  }

  if (main_window_data -> window_open_flag != TRUE) {

    GtkWidget * window;
    GtkWidget * box;
    GtkWidget * checkbox1;
    GtkWidget * checkbox2;
    GtkWidget * checkbox3;
    GtkWidget * checkbox4;
    GtkWidget * checkbox5;
    GtkWidget * checkbox6;
    GtkWidget * checkbox7;
    GtkWidget * checkbox8;
    GtkWidget * checkbox9;
    GtkWidget * checkbox10;
    GtkWidget * info_button;

      LpihWindowData * window_data = g_malloc(sizeof(LpihWindowData));

    if (main_window_data->distro == DEBIAN) {
      g_print("Chose Debian window.\n");

    window_data -> window_open_flag = debian_window_open;
    window_data -> css_label = css_label_debian;
    window_data -> window_title = window_title_debian;
    window_data -> view_css_class = view_css_class_debian;
    window_data -> opener = DEBIAN_OPENER;
    window_data -> info_button_css_class = info_button_css_class_debian;
    window_data -> distro_id = distro_debian;
    window_data -> checkbox1_title = checkbox1_title_debian;
    window_data -> checkbox2_title = checkbox2_title_debian;
    window_data -> checkbox3_title = checkbox3_title_debian;
    window_data -> checkbox4_title = checkbox4_title_debian;
    window_data -> checkbox5_title = checkbox5_title_debian;
    window_data -> checkbox6_title = checkbox6_title_debian;
    window_data -> checkbox7_title = checkbox7_title_debian;
    window_data -> checkbox8_title = checkbox8_title_debian;
    window_data -> checkbox9_title = checkbox9_title_debian;
    window_data -> checkbox10_title = checkbox10_title_debian;

    } else if (main_window_data->distro == FEDORA) {
      g_print("Chose Fedora window.  Initializing...");

    window_data -> distro_id = distro_fedora;
    window_data -> css_label = css_label_fedora;
    window_data -> window_title = window_title_fedora;
    window_data -> view_css_class = view_css_class_fedora;
    window_data -> opener = FEDORA_OPENER;
    window_data -> info_button_css_class = info_button_css_class_fedora;
    window_data -> window_open_flag = fedora_window_open;
    window_data -> checkbox1_title = checkbox1_title_fedora;
    window_data -> checkbox2_title = checkbox2_title_fedora;
    window_data -> checkbox3_title = checkbox3_title_fedora;
    window_data -> checkbox4_title = checkbox4_title_fedora;
    window_data -> checkbox5_title = checkbox5_title_fedora;
    window_data -> checkbox6_title = checkbox6_title_fedora;
    window_data -> checkbox7_title = checkbox7_title_fedora;
    window_data -> checkbox8_title = checkbox8_title_fedora;
    window_data -> checkbox9_title = checkbox9_title_fedora;
    window_data -> checkbox10_title = checkbox10_title_fedora;
    
    } else {
      g_print("\nCOULDN'T INITIALIZE CHECKBOXES\n");

    }

    window = gtk_window_new();
    gtk_widget_add_css_class(window, window_data -> css_label);
    gtk_window_set_title(GTK_WINDOW(window), window_data -> window_title);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 700);
    gtk_widget_set_can_focus(GTK_WIDGET(window), TRUE);
    gtk_widget_set_vexpand(window, TRUE);
    gtk_widget_set_hexpand(window, TRUE);

    GtkWidget * view;
    GtkTextBuffer * buffer;

    GtkWidget * checkbox_scroll_window = gtk_scrolled_window_new();
    gtk_widget_set_size_request(checkbox_scroll_window, 300, 200);
    gtk_widget_set_vexpand(checkbox_scroll_window, TRUE);
    gtk_widget_set_hexpand(checkbox_scroll_window, TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(checkbox_scroll_window), TRUE);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(window), box);
    gtk_widget_set_can_focus(GTK_WIDGET(box), TRUE);

    // Create a box to contain the checkboxes
    GtkWidget * checkbox_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    // Add the box to the scrolled window
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(checkbox_scroll_window), checkbox_box);
    // TODO: add these to an array to append them to the window/box in the future.

    gtk_box_append(GTK_BOX(box), checkbox_scroll_window);

    // Create main scrolled window and set the size
    GtkWidget * scroll_window = gtk_scrolled_window_new();

    gtk_widget_set_size_request(scroll_window, 200, 200);
    gtk_widget_set_vexpand(scroll_window, TRUE);
    gtk_widget_set_hexpand(scroll_window, TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(scroll_window), TRUE);

    view = gtk_text_view_new();
    gtk_widget_set_opacity(view, 0.9);
    gtk_widget_add_css_class(view, window_data -> view_css_class);
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    gtk_text_buffer_set_text(buffer, window_data -> opener, -1);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
    gtk_widget_set_can_focus(GTK_WIDGET(view), TRUE);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll_window), view);
    gtk_box_append(GTK_BOX(box), scroll_window);

    // Create separate box to hold button to solve sizing issues //////
    GtkWidget * info_button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append(GTK_BOX(box), info_button_box);

    // Create info button for the window //

    info_button = gtk_button_new_with_label("Info");
    gtk_widget_set_size_request(info_button, 64, 64);
    gtk_widget_add_css_class(info_button, window_data -> info_button_css_class);
    gtk_box_append(GTK_BOX(info_button_box), info_button);

    // INITIALIZE THE WINDOW'S CHECKBOXES // // // // // // // // // // // // // // // // // // // // 
    checkbox1 = gtk_check_button_new_with_label(window_data -> checkbox1_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox1);
  
    checkbox2 = gtk_check_button_new_with_label(window_data -> checkbox2_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox2);

    checkbox3 = gtk_check_button_new_with_label(window_data -> checkbox3_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox3);
  
    checkbox4 = gtk_check_button_new_with_label(window_data -> checkbox4_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox4);

    checkbox5 = gtk_check_button_new_with_label(window_data -> checkbox5_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox5);

    checkbox6 = gtk_check_button_new_with_label(window_data -> checkbox6_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox6);

    checkbox7 = gtk_check_button_new_with_label(window_data -> checkbox7_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox7);

    checkbox8 = gtk_check_button_new_with_label(window_data -> checkbox8_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox8);
  
    checkbox9 = gtk_check_button_new_with_label(window_data -> checkbox9_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox9);

    checkbox10 = gtk_check_button_new_with_label(window_data -> checkbox10_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox10);

    window_data -> checkbox1 = checkbox1;
    window_data -> checkbox2 = checkbox2;
    window_data -> checkbox3 = checkbox3;
    window_data -> checkbox4 = checkbox4;
    window_data -> checkbox5 = checkbox5;
    window_data -> checkbox6 = checkbox6;
    window_data -> checkbox7 = checkbox7;
    window_data -> checkbox8 = checkbox8;
    window_data -> checkbox9 = checkbox9;
    window_data -> checkbox10 = checkbox10;

    // CONNECT WIDGET CLICKS TO CALLBACK FUNCTIONS //
    g_signal_connect(info_button, "clicked", G_CALLBACK(make_info_window), main_window_data);
    
    g_signal_connect(G_OBJECT(checkbox1), "toggled", G_CALLBACK(check_box_state), buffer);
    g_signal_connect(G_OBJECT(checkbox2), "toggled", G_CALLBACK(check_box_state), buffer);
    g_signal_connect(G_OBJECT(checkbox3), "toggled", G_CALLBACK(check_box_state), buffer);
    g_signal_connect(G_OBJECT(checkbox4), "toggled", G_CALLBACK(check_box_state), buffer);
    g_signal_connect(G_OBJECT(checkbox5), "toggled", G_CALLBACK(check_box_state), buffer);
    g_signal_connect(G_OBJECT(checkbox6), "toggled", G_CALLBACK(check_box_state), buffer);
    g_signal_connect(G_OBJECT(checkbox7), "toggled", G_CALLBACK(check_box_state), buffer);
    g_signal_connect(G_OBJECT(checkbox8), "toggled", G_CALLBACK(check_box_state), buffer);
    g_signal_connect(G_OBJECT(checkbox9), "toggled", G_CALLBACK(check_box_state), buffer);
    g_signal_connect(G_OBJECT(checkbox10), "toggled", G_CALLBACK(check_box_state), buffer);

    g_signal_connect(window, "destroy", G_CALLBACK(on_lpih_window_destroy), window_data);

    gtk_window_present(GTK_WINDOW(window));
    window_data -> window_open_flag = TRUE;

  } else {  g_print("Window already open!\n");  }

}

///// Do this when the main window is closed. ////////
gboolean on_lpih_window_destroy(GtkWidget * window, gpointer data) {
  LpihWindowData * window_data = (LpihWindowData * ) data;
  window_data -> window_open_flag = FALSE;

  if (gtk_widget_is_visible(window)) {
    g_print("LPIH window failed to close.  Freeing memory...\n");
    g_free(window_data);
    return FALSE;

  } else {

    g_print("LPIH window closed successfully.\n");
    g_free(window_data);
    
    return TRUE;
  }
}
