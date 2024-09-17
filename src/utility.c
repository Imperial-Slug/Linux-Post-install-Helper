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


gboolean check_box_state(GtkWidget * checkbox, gpointer data) {

  //Instantiate checkbox data struct and its members.
  CheckboxData * checkbox_data = (CheckboxData * ) data;
  const gchar * command_string = checkbox_data -> associated_command;
  g_print("Trying to print %s\n", command_string);
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(checkbox_data -> shared_buffer);

  gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(checkbox));

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

gboolean init_css_provider() {

  GtkCssProvider * provider = gtk_css_provider_new();

  gchar * cssFilePathDecided;
  gchar * cssPathForAppInstalled = "/usr/share/LPIH/css/style.css";
  gchar * cssPathForAppInSrc = "../Resources/style.css";

  FILE * cssFileForAppInstalled = fopen(cssPathForAppInstalled, "r");
  FILE * cssFileForAppInSrc = fopen(cssPathForAppInSrc, "r");

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
    return TRUE;

  } else {
    g_print("******ERROR: CSS provider failed to find the CSS file in function gtk_css_provider_load_from_path(). \n");
    return FALSE;
  }
}

// Function to get the graphics card vendor of the user.
const char * getGraphicsCardVendor() {
  return (const char * ) glGetString(GL_VENDOR);
}

// Function that uses inline assembly to get the vendor string of the CPU
// The vendor gchar array is declared at runtime and fed into this function in lpih-main.c.
void get_cpu_vendor(gchar * vendor) {
  // Use inline assembly to execute the cpuid instruction
  __asm__ volatile(
    "cpuid" // Execute the cpuid instruction
    : "=b"(((unsigned int * ) vendor)[0]),
    "=d"(((unsigned int * ) vendor)[1]),
    "=c"(((unsigned int * ) vendor)[2]): "a"(0)
  );

  vendor[13] = '\0';

}


