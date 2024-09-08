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
gboolean check_box_state(const gchar * command_string, GtkWidget * checkbox, gpointer data) {

  gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(checkbox));
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(data);

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
char * getGraphicsCardVendor() {
  return (char * ) glGetString(GL_VENDOR);
}

// Function that uses inline assembly to get the vendor string of the CPU
// The vendor gchar array is declared at runtime and fed into this function in lpih-main.c.
gboolean get_cpu_vendor(gchar * vendor) {
  // Use inline assembly to execute the cpuid instruction
  __asm__ volatile(
    "cpuid" // Execute the cpuid instruction
    : "=b"(((unsigned int * ) vendor)[0]),
    "=d"(((unsigned int * ) vendor)[1]),
    "=c"(((unsigned int * ) vendor)[2]): "a"(0)
  );

  vendor[13] = '\0';

  if (vendor != NULL) {
    g_print("CPU vendor loaded.\n");
    return TRUE;
  } else {
    g_print("CPU vendor loaded.\n");
    return FALSE;
  }
}

void create_notebook_tab(GtkWidget * notebook, gchar * view_css_label, gchar * tab_label, gchar * tab_css_label, gchar * res_path1, gchar * res_path2) {

  GtkWidget * view;
  GtkTextBuffer * buffer;
  GtkWidget * scroll_info_window = gtk_scrolled_window_new();

  gtk_widget_set_size_request(scroll_info_window, 300, 200);
  gtk_widget_set_vexpand(scroll_info_window, TRUE);
  gtk_widget_set_hexpand(scroll_info_window, TRUE);

  view = gtk_text_view_new();
  gtk_widget_set_opacity(view, 0.9);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD_CHAR);
  gtk_widget_add_css_class(view, view_css_label);
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

  GtkWidget * tab_label_view;
  GtkTextBuffer * tab_buffer;

  tab_label_view = gtk_text_view_new();
  gtk_widget_add_css_class(tab_label_view, tab_css_label);
  tab_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tab_label_view));
  gtk_text_buffer_set_text(tab_buffer, tab_label, -1);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(tab_label_view), FALSE);
  gtk_widget_set_vexpand(tab_label_view, TRUE);
  gtk_widget_set_hexpand(tab_label_view, TRUE);

  gchar * tab_text = NULL;
  gsize length = 0;
  GError * error = NULL;

  if (g_file_get_contents(res_path1, & tab_text, & length, & error)) {
    gtk_text_buffer_set_text(buffer, tab_text, -1);
    g_free(tab_text);
  } else if (g_file_get_contents(res_path2, & tab_text, & length, & error)) {

    gtk_text_buffer_set_text(buffer, tab_text, -1);
    g_free(tab_text);

  } else {
    g_print("Failed to load info file: %s\n", error -> message);
    g_error_free(error);
  }

  gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view), FALSE);
  gtk_widget_set_can_focus(GTK_WIDGET(view), TRUE);
  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(view), 13);
  gtk_text_view_set_right_margin(GTK_TEXT_VIEW(view), 13);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll_info_window), view);

  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), scroll_info_window, tab_label_view);

}
