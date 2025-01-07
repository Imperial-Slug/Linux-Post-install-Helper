/* utility.c
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

#include <string.h>

#include <GL/gl.h>

#include "lpih-window.h"

#include "utility.h"

// This file contains functions outside of the realm of window generation.  You will find in here the functions to get the checkbox handling function, the functions to identify the CPU and GPU vendors, and the css initializer function.

void set_cpu_vendor(void) {

  gpointer cpu_vendor_name = NULL;
  gchar vendor[15];
  get_cpu_vendor(vendor);
  cpu_vendor_name = g_malloc(15);

  if (strstr(vendor, "AMD") != NULL) {
    *(enum vendor_name * ) cpu_vendor_name = AMD;

  } else if (strstr(vendor, "Intel") != NULL) {
    *(enum vendor_name * ) cpu_vendor_name = Intel;

  } else {
    g_print("*****ERROR: The CPU vendor could not be determined for this computer.\n");
    g_print("*************************************\n\n");
    *(enum vendor_name * ) cpu_vendor_name = Unknown;

  }

  g_print("The CPU vendor for this machine is %s.\n", vendor);
  g_print("*************************************\n\n");

  // Determine Debian microcode command
  if ( * (enum vendor_name * ) cpu_vendor_name == AMD) {
    debian_microcode_command = "  sudo apt install amd64-microcode;\n";
  } else if ( * (enum vendor_name * ) cpu_vendor_name == Intel) {
    debian_microcode_command = "  sudo apt install intel-microcode;\n";
    g_print("Debian microcode command is: %s", debian_microcode_command);
  } else {
    g_print("*****ERROR: Something went wrong trying to get the CPU vendor_name.*****\n");
  }

  g_free(cpu_vendor_name);
}

// Function that uses inline assembly to get the vendor string of the CPU
// The vendor gchar array is declared at runtime and fed into this function in lpih-main.c.

void get_cpu_vendor(gchar * vendor) {
    // Use inline assembly to execute the "cpuid" instruction and get the CPU vendor.
    unsigned int registers[4] = {0};

    __asm__ volatile(
        "cpuid"
        : "=b"(registers[1]), // EBX
          "=d"(registers[2]), // EDX
          "=c"(registers[3])  // ECX
        : "a"(registers[0])   // EAX = 0
    );

    // Copy results to vendor array and NULL-terminate.
    memcpy(vendor, &registers[1], 4); // Copy EBX
    memcpy(vendor + 4, &registers[2], 4); // Copy EDX
    memcpy(vendor + 8, &registers[3], 4); // Copy ECX
    vendor[12] = '\0'; // Null-terminate
}



void set_gpu_vendor(void) {

  const char * gpu_vendor = (const char * ) glGetString(GL_VENDOR);
  const gchar * deb_nvidia_gpu = "  sudo apt install nvidia-driver nvidia-driver-libs nvidia-driver-libs:i386;\n";
  const gchar * fed_nvidia_gpu = "  sudo dnf install akmod-nvidia xorg-x11-drv-nvidia-cuda \n";
  const gchar * fed_amd_gpu = "  sudo dnf install xorg-x11-drv-amdgpu vulkan-tools mesa-vulkan-drivers \n";
  const gchar * deb_amd_gpu = "  sudo apt install firmware-linux firmware-linux-nonfree libdrm-amdgpu1 xserver-xorg-video-amdgpu;\n";
  const gchar * deb_intel_gpu = "  # Intel GPU drivers should already be installed. \n";
  //  gchar * fed_intel_gpu = "  # Intel GPU drivers should already be installed. \n";

  if (strstr(gpu_vendor, "NVIDIA") != NULL) {

    debian_gpu_command = deb_nvidia_gpu;
    fedora_gpu_command = fed_nvidia_gpu;
  } else if (strstr(gpu_vendor, "AMD") != NULL) {

    debian_gpu_command = deb_amd_gpu;
    fedora_gpu_command = fed_amd_gpu;
  } else if (strstr(gpu_vendor, "Intel") != NULL) {

    debian_gpu_command = deb_intel_gpu;
    fedora_gpu_command = debian_gpu_command;
  } else {

    debian_gpu_command = deb_intel_gpu;
    fedora_gpu_command = deb_intel_gpu;

  }
  g_print("The GPU vendor for this machine is %s.\n", gpu_vendor);
}

gboolean init_css_provider() {

  GtkCssProvider * provider = gtk_css_provider_new();

  gchar * cssFilePathDecided;
  gchar * cssPathForAppInstalled = "/usr/share/LPIH/css/style.css";
  gchar * cssPathForAppInSrc = "../Resources/style.css";

  FILE * cssFileForAppInstalled = fopen(cssPathForAppInstalled, "r");
  FILE * cssFileForAppInSrc = fopen(cssPathForAppInSrc, "r");

  // Handling CSS file being read from source tarball / project directory:
  if (cssFileForAppInSrc) {
    g_print("The CSS file was found beside the executable.\n");
    cssFilePathDecided = cssPathForAppInSrc;
    fclose(cssFileForAppInSrc);

    // Handling CSS file being read from the directory it gets installed to with the .deb and .rpm packages:
  } else if (cssPathForAppInstalled) {
    g_print("The CSS file was found in /usr/share/LPIH/css");
    cssFilePathDecided = cssPathForAppInstalled;
    fclose(cssFileForAppInstalled);
  } else {
    g_print("*********************\nERROR: Can't find the CSS file! \n*********************\n\n");
  }

  gtk_css_provider_load_from_path(provider, cssFilePathDecided);
  gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  if (provider != NULL) {
    g_print("The CSS provider has found the CSS file.  \n");
    return TRUE;

  } else {
    g_print("******ERROR: CSS provider failed to load the CSS file in function gtk_css_provider_load_from_path(). \n");
    return FALSE;
  }
}
