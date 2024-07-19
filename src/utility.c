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
#include <X11/Xlib.h>
#include <GL/gl.h>
#include "utility.h"

// This function is used to add and remove the commands from the GUI based on the status of the checkboxes. 
gboolean check_box_state(const gchar* command_string, GtkWidget* widget, gpointer data) {

  gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer* buffer = GTK_TEXT_BUFFER(data);

  GtkTextIter iter;
  if (state) {
    gtk_text_buffer_get_end_iter(buffer, & iter);
    gtk_text_buffer_insert(buffer, & iter, command_string, -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar* search_string = command_string;

    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
  return TRUE;
}


int init_css_provider() {

  GtkCssProvider* provider = gtk_css_provider_new();

  const char* cssFilePathDecided;
  const char* cssPathForAppInstalled = "/usr/share/LPIH/css/style.css";
  const char* cssPathForAppInSrc = "../Resources/style.css";

  FILE* cssFileForAppInstalled = fopen(cssPathForAppInstalled, "r");
  FILE* cssFileForAppInSrc = fopen(cssPathForAppInSrc, "r");

  if (cssFileForAppInSrc) {
    g_print("The CSS file was found beside the executable.\n");
    cssFilePathDecided = cssPathForAppInSrc;
    fclose(cssFileForAppInSrc);

  } else if (cssPathForAppInstalled) {
    g_print("The CSS file was found in /usr/share/LPIH/css");
    cssFilePathDecided = cssPathForAppInstalled;
    fclose(cssFileForAppInstalled);
  } else {
    g_print("*********************\nERROR: Can't find the CSS file!  Please report this to the LPIH maintainer so it can be fixed.\n*********************\n\n");
  }

  gtk_css_provider_load_from_path(provider, cssFilePathDecided);
  gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  if (provider != NULL) {
    g_print("The CSS provider has found the CSS file.  \n");
    return 0;

  } else {
    g_print("******ERROR: CSS provider failed to find the CSS file in function gtk_css_provider_load_from_path(). \n");
    return 2;
  }

}

// Function to get the graphics card vendor of the user.
const char* getGraphicsCardVendor() {
  return (const char* ) glGetString(GL_VENDOR);
}

int get_cpu_vendor(char* vendor) {
  unsigned int eax, ebx, ecx, edx;

  // Call cpuid with input 0 to get vendor string
  __asm__("cpuid": "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx): "a"(0));

  // Copy the vendor string to the output buffer
  ((int* ) vendor)[0] = ebx;
  ((int* ) vendor)[1] = edx;
  ((int* ) vendor)[2] = ecx;
  vendor[12] = '\0';
  
  if (vendor != NULL) {
    g_print("CPU vendor loaded.\n");
    return 0;
  } 
  
  else {
  g_print("*********ERROR: Problem getting cpu vendor info.*****\n\n");
  return 1;
  }
  
}

 
