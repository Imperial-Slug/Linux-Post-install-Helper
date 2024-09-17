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



gpointer cpu_vendor_name = NULL;

const gchar * DEBIAN_OPENER = "  # Check the boxes according to your needs and run the resulting script in your terminal  \n  # to set up the desired functionality on your Debian system.  You may need to enable non-free  \n  # repositories by editing your '/etc/apt/sources.list' file if some of the proprietary packages  \n  # like Steam and GPU drivers don't install.  See 'info' for details.  \n\n  sudo apt update; sudo apt upgrade;  \n  sudo apt install build-essential dkms linux-headers-$(uname -r); \n";
// For keeping track of single-instance lpih_window
gboolean lpih_instance_running = FALSE;
const gchar * FEDORA_OPENER = "  # Check the boxes according to your needs and run the resulting script in your terminal  \n  # to set up the desired functionality on your Fedora system.  \n\n  sudo dnf update;  \n";
//


////// INITIAL WINDOW ////////////////////////////////////////////////////
// Creates the initial window where the distro is selected.
void activate(GtkApplication * app) {

  if (lpih_instance_running != TRUE) {

    lpih_instance_running = TRUE;

        GtkWidget *window = make_main_window(app);
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
