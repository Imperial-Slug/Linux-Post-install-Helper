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
#include "deb-window.h"
#include "utility.h"
#include "lpih-main.h"

 //////////////////////////////
//////////////////////////////

 const gchar* debian_gpu_command;
 const gchar* debian_microcode_command;

// 1 = AMD, 2 = Intel, 3 = Nvidia.
 int cpu_manufacturer = 0;
 int gpu_manufacturer = 0;

 // For keeping track of single-instance windows.
 int lpih_instance_running = 0;
 
 
////// INITIAL WINDOW ////////////////////////////////////////////////////

 void activate(GtkApplication* app) {

  if (lpih_instance_running != 1) {

    lpih_instance_running = 1;

    GtkWidget* window;
    GtkWidget* grid;
    GtkWidget* deb_button;
    GtkWidget* fed_button;
    GtkWidget* quit_button;

    /* create a new window, and set its title*/
    window = gtk_application_window_new(app);

    gtk_widget_add_css_class(window, "main_window");
    gtk_window_set_title(GTK_WINDOW(window), "Linux Post-install Helper For Debian and Fedora");
    gtk_widget_set_size_request(window, 444, 444);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    /////////////////////////////////////////////////////////////////////////

    grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE); // Make rows of equal height
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE); // Make columns of equal width
    gtk_grid_set_row_spacing(GTK_GRID(grid), 50); // Add spacing between rows
    gtk_grid_set_column_spacing(GTK_GRID(grid), 100); // Add spacing between columns
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    gtk_window_set_child(GTK_WINDOW(window), grid);

    deb_button = gtk_button_new_with_label("DEBIAN");
    gtk_widget_add_css_class(deb_button, "deb");
    gtk_widget_set_size_request(deb_button, 100, 50);
    g_signal_connect(deb_button, "clicked", G_CALLBACK(debian_window), NULL);

    gtk_grid_attach(GTK_GRID(grid), deb_button, 0, 0, 1, 1);

    fed_button = gtk_button_new_with_label("FEDORA");
    gtk_widget_add_css_class(fed_button, "fed");
    gtk_widget_set_size_request(fed_button, 100, 50);
    g_signal_connect(fed_button, "clicked", G_CALLBACK(fedora_window), NULL);

    gtk_grid_attach(GTK_GRID(grid), fed_button, 1, 0, 1, 1);

    quit_button = gtk_button_new_with_label("QUIT");
    gtk_widget_add_css_class(quit_button, "quit");
    gtk_widget_set_name(quit_button, "quit");
    gtk_widget_set_size_request(quit_button, 100, 50);
    g_signal_connect_swapped(quit_button, "clicked", G_CALLBACK(gtk_window_destroy), window);

    gtk_grid_attach(GTK_GRID(grid), quit_button, 0, 1, 2, 1);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
    init_css_provider();
    gtk_widget_set_can_focus(GTK_WIDGET(window), TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(grid), TRUE);

    gtk_widget_set_visible(window, TRUE);

    // Automatically establishing the user's GPU vendor on init of the program.       
    const char* gpu_vendor = getGraphicsCardVendor();

    if (strstr(gpu_vendor, "NVIDIA") != NULL) {
      gpu_manufacturer = 1;
    } else if (strstr(gpu_vendor, "AMD") != NULL) {
      gpu_manufacturer = 2;
    } else if (strstr(gpu_vendor, "Intel") != NULL) {
      gpu_manufacturer = 3;
 
    }  else {
      g_print("The GPU vendor could not be determined for this GPU. If this is a VM, it will likely be using it's own graphics drivers unless you are using pass-through. \n");
      gpu_manufacturer = 0;
    }


    if (gpu_manufacturer == 1) {
    debian_gpu_command = "  sudo apt install nvidia-driver nvidia-driver-libs;\n";
    fedora_gpu_command = "  sudo dnf install akmod-nvidia xorg-x11-drv-nvidia-cuda \n";
   
  } else if (gpu_manufacturer == 2) {
    debian_gpu_command = "  sudo apt install firmware-linux firmware-linux-nonfree libdrm-amdgpu1 xserver-xorg-video-amdgpu;\n";
    fedora_gpu_command = "  sudo dnf install xorg-x11-drv-amdgpu vulkan-tools mesa-vulkan-drivers \n";
   
  } else if (gpu_manufacturer == 3) {
    debian_gpu_command = "  # Intel GPU drivers already installed. \n";
    fedora_gpu_command = debian_gpu_command;
   
  } else if (gpu_manufacturer == 0) {
    debian_gpu_command = "  # The GPU vendor could not be determined for this GPU. If this is a VM, it will likely be using it's own graphics drivers unless you are using pass-through.\n";
    fedora_gpu_command = debian_gpu_command;
   
}

    char vendor[12];
    get_cpu_vendor(vendor);

    if (strstr(vendor, "AMD") != NULL) {
      cpu_manufacturer = 2;
      strcpy(vendor, "AMD");
    } else if (strstr(vendor, "Intel") != NULL) {
      cpu_manufacturer = 3;
      strcpy(vendor, "Intel");
    } else {
      g_print("*****ERROR: The CPU vendor could not be determined for this computer.\n");
      g_print("*************************************\n\n");
      cpu_manufacturer = 0;
      strcpy(vendor, "Unknown");
    }

// Print mfgs
    g_print("The GPU manufacturer for this machine is %d, %s.\n", gpu_manufacturer, gpu_vendor);
    g_print("The CPU manufacturer for this machine is %d, %s.\n", cpu_manufacturer, vendor);
    g_print("*************************************\n\n");

// Determine Debian microcode command
 if (cpu_manufacturer == 2) {
    debian_microcode_command = "  sudo apt install amd64-microcode;\n";
  } else if (cpu_manufacturer == 3) {
    debian_microcode_command = "  sudo apt install intel-microcode;\n";
  } else {
    g_print("*****ERROR: Something went wrong trying to get the cpu manufacturer.*****\n");
  }

  } else {
    g_print("Error: instance of LPIH is already running!\n");
  }

}

 void on_quit() {
  g_print("Exiting LPIH now.\n");
  g_print("*************************************\n");
  lpih_instance_running = 0;

}





int main(int argc,
  char** argv) {

  GtkApplication* app;
  int status;
  app = gtk_application_new("imperialslug.gtkproject.lpih", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  g_signal_connect(app, "shutdown", G_CALLBACK(on_quit), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}

///////////// END OF FILE /////////////


