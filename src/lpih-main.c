/* lpih-main.c
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

////////////////////////////////////////
////        L I B R A R I E S   ////////
////////////////////////////////////////
#include <gtk/gtk.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "lpih-window.h"

#include "utility.h"

#include "lpih-main.h"

//////////////////////////////
//////////////////////////////

gboolean debian_window_open = FALSE;
gboolean fedora_window_open = FALSE;

gpointer cpu_vendor_name = NULL;

const gchar * DEBIAN_OPENER = "  # Check the boxes according to your needs and run the resulting script in your terminal  \n  # to set up the desired functionality on your Debian system.  You may need to enable non-free  \n  # repositories by editing your '/etc/apt/sources.list' file if some of the proprietary packages  \n  # like Steam and GPU drivers don't install.  See 'info' for details.  \n\n  sudo apt update; sudo apt upgrade;  \n  sudo apt install build-essential dkms linux-headers-$(uname -r); \n";
// For keeping track of single-instance lpih_window
gboolean lpih_instance_running = FALSE;
const gchar * FEDORA_OPENER = "  # Check the boxes according to your needs and run the resulting script in your terminal  \n  # to set up the desired functionality on your Fedora system.  \n\n  sudo dnf update;  \n";
//
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

enum Distro distro_debian = DEBIAN;

// Initialize fedora_window_data

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

////// INITIAL WINDOW ////////////////////////////////////////////////////
// Creates the initial window where the distro is selected.
void activate(GtkApplication * app) {

  if (lpih_instance_running != TRUE) {

    lpih_instance_running = TRUE;

    GtkWidget * window;
    GtkWidget * grid;
    GtkWidget * deb_button;
    GtkWidget * fed_button;
    GtkWidget * quit_button;

    /* create a new window, and set its title*/
    window = gtk_application_window_new(app);

    gtk_widget_add_css_class(window, "main_window");
    gtk_window_set_title(GTK_WINDOW(window), "Linux Post-install Helper For Debian and Fedora");
    gtk_widget_set_size_request(window, 512, 256);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

    /////////////////////////////////////////////////////////////////////////

    LpihWindowData * debian_window_data = g_malloc(sizeof(LpihWindowData));

    LpihWindowData * fedora_window_data = g_malloc(sizeof(LpihWindowData));

    g_print("\nstarting creation of main data structs.\n");
    // Initialize debian_window_data

    g_print(" .....133  \n");

    debian_window_data -> window_open_flag = debian_window_open;
    debian_window_data -> css_label = css_label_debian;
    debian_window_data -> window_title = window_title_debian;
    debian_window_data -> view_css_class = view_css_class_debian;
    debian_window_data -> opener = DEBIAN_OPENER;
    debian_window_data -> info_button_css_class = info_button_css_class_debian;
    debian_window_data -> distro_id = distro_debian;
    debian_window_data -> checkbox1_title = checkbox1_title_debian;
    debian_window_data -> checkbox2_title = checkbox2_title_debian;
    debian_window_data -> checkbox3_title = checkbox3_title_debian;
    debian_window_data -> checkbox4_title = checkbox4_title_debian;
    debian_window_data -> checkbox5_title = checkbox5_title_debian;
    debian_window_data -> checkbox6_title = checkbox6_title_debian;
    debian_window_data -> checkbox7_title = checkbox7_title_debian;
    debian_window_data -> checkbox8_title = checkbox8_title_debian;
    debian_window_data -> checkbox9_title = checkbox9_title_debian;
    debian_window_data -> checkbox10_title = checkbox10_title_debian;

    fedora_window_data -> distro_id = distro_fedora;
    fedora_window_data -> css_label = css_label_fedora;
    fedora_window_data -> window_title = window_title_fedora;
    fedora_window_data -> view_css_class = view_css_class_fedora;
    fedora_window_data -> opener = FEDORA_OPENER;
    fedora_window_data -> info_button_css_class = info_button_css_class_fedora;
    fedora_window_data -> window_open_flag = fedora_window_open;
    fedora_window_data -> checkbox1_title = checkbox1_title_fedora;
    fedora_window_data -> checkbox2_title = checkbox2_title_fedora;
    fedora_window_data -> checkbox3_title = checkbox3_title_fedora;
    fedora_window_data -> checkbox4_title = checkbox4_title_fedora;
    fedora_window_data -> checkbox5_title = checkbox5_title_fedora;
    fedora_window_data -> checkbox6_title = checkbox6_title_fedora;
    fedora_window_data -> checkbox7_title = checkbox7_title_fedora;
    fedora_window_data -> checkbox8_title = checkbox8_title_fedora;
    fedora_window_data -> checkbox9_title = checkbox9_title_fedora;
    fedora_window_data -> checkbox10_title = checkbox10_title_fedora;

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
    g_signal_connect(deb_button, "clicked", G_CALLBACK(lpih_window), debian_window_data);

    gtk_grid_attach(GTK_GRID(grid), deb_button, 0, 0, 1, 1);

    fed_button = gtk_button_new_with_label("FEDORA");
    gtk_widget_add_css_class(fed_button, "fed");
    gtk_widget_set_size_request(fed_button, 128, 64);

    g_signal_connect(fed_button, "clicked", G_CALLBACK(lpih_window), fedora_window_data);

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

    gtk_window_present(GTK_WINDOW(window));

    const char * gpu_vendor = getGraphicsCardVendor(); // Automatically establishing the user's GPU vendor on init of the program.       
    cpu_vendor_name = g_malloc(sizeof(enum vendor_name));

    if (strstr(gpu_vendor, "NVIDIA") != NULL) {

      debian_gpu_command = "  sudo apt install nvidia-driver nvidia-driver-libs nvidia-driver-libs:i386;\n";
      fedora_gpu_command = "  sudo dnf install akmod-nvidia xorg-x11-drv-nvidia-cuda \n";
    } else if (strstr(gpu_vendor, "AMD") != NULL) {

      debian_gpu_command = "  sudo apt install firmware-linux firmware-linux-nonfree libdrm-amdgpu1 xserver-xorg-video-amdgpu;\n";
      fedora_gpu_command = "  sudo dnf install xorg-x11-drv-amdgpu vulkan-tools mesa-vulkan-drivers \n";
    } else if (strstr(gpu_vendor, "Intel") != NULL) {

      debian_gpu_command = "  # Intel GPU drivers should already be installed. \n";
      fedora_gpu_command = debian_gpu_command;
    } else {
    
    debian_gpu_command = "  # No drivers detected.  Either this is a VM or this is an error. \n";
      fedora_gpu_command = debian_gpu_command;
    
    }

    gchar vendor[15];
    get_cpu_vendor(vendor);

    if (strstr(vendor, "AMD") != NULL) {
      *(enum vendor_name * ) cpu_vendor_name = AMD;

    } else if (strstr(vendor, "Intel") != NULL) {
      *(enum vendor_name * ) cpu_vendor_name = Intel;

    } else {
      g_print("*****ERROR: The CPU vendor could not be determined for this computer.\n");
      g_print("*************************************\n\n");
      *(enum vendor_name * ) cpu_vendor_name = Unknown;

    }

    // Print mfgs
    g_print("The GPU vendor for this machine is %s.\n", gpu_vendor);
    g_print("The CPU vendor for this machine is %s.\n", vendor);
    g_print("*************************************\n\n");

    // Determine Debian microcode command
    if ( * (enum vendor_name * ) cpu_vendor_name == AMD) {
      debian_microcode_command = "  sudo apt install amd64-microcode;\n";
    } else if ( * (enum vendor_name * ) cpu_vendor_name == Intel) {
      debian_microcode_command = "  sudo apt install intel-microcode;\n";
      g_print("Debian microcode command is: %s", debian_microcode_command);
    } else {
      g_print("*****ERROR: Something went wrong trying to get the cpu vendor_name.*****\n");
    }

    // g_free(debian_window_data);
    // g_free(fedora_window_data);

  } else {
    g_print("Error: instance of LPIH is already running!\n");
  }

  g_free(cpu_vendor_name);

}

void on_quit() {
  g_print("Exiting LPIH now.\n");
  g_print("*************************************\n");
  lpih_instance_running = FALSE;

}

gboolean main(int argc,
  char ** argv) {

  GtkApplication * app;
  gboolean status;
  app = gtk_application_new("imperialslug.gtkproject.lpih", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  g_signal_connect(app, "shutdown", G_CALLBACK(on_quit), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}

///////////// END OF FILE /////////////
