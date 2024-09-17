/* info-window.c
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

#include "utility.h"

#include "lpih-window.h"

#include "info-window.h"

///////////////////////////////////////////
///////// INFORMATIONAL WINDOW:  //////////
///////////////////////////////////////////



gboolean on_info_window_destroy(GtkWidget * widget, gpointer data) {
  if (widget != NULL) {
  
  InfoWindowData * info_window_data = (InfoWindowData * ) data;

  info_window_data -> info_open_flag = FALSE;

  if (info_window_data -> info_open_flag == FALSE) {
    g_print("info_open_flag for %s set to FALSE.  Freeing memory...\n", info_window_data -> info_window_name);
    g_free(info_window_data);
    return TRUE;
  } else {
    g_print("%s info_open_flag: Failed to set to FALSE.  Freeing memory...\n", info_window_data -> info_window_name);
    g_free(info_window_data);
    return FALSE;
  }
} else { g_print("on_info_window_destroy: Widget is NULL. Freeing memory...\n"); 
     
    return FALSE;
    }
}


GtkWidget * make_notebook(gpointer data) {
InfoWindowData *info_window_data = (InfoWindowData *)data;
  GtkWidget * notebook;
    notebook = gtk_notebook_new();
    if (notebook != NULL) {
      g_print("Notebook initialized. \n");
    } else {
      g_print("Notebook is NULL!");
      return NULL;
    }

    gtk_widget_add_css_class(notebook, info_window_data -> notebook_css_name);

    if (info_window_data -> distro_id == DEBIAN) {
      g_print("Creating debian notebook\n");
      create_notebook_tab(notebook, "deb_tab_view1", "Main", "deb_info_main", "../Resources/deb-info-tab1.txt", "/usr/share/LPIH/text_files/deb-info-tab1.txt");
      create_notebook_tab(notebook, "deb_tab_view2", "Software Management", "deb_info2", "../Resources/deb-info-tab2.txt", "/usr/share/LPIH/text_files/deb-info-tab2.txt");
      create_notebook_tab(notebook, "deb_tab_view3", "Tips", "deb_info3", "../Resources/set_static_ip3.txt", "/usr/share/LPIH/text_files/set_static_ip3.txt");
      g_print("Finished creating Debian notebook.\n");
      return notebook;

    } else if (info_window_data -> distro_id == FEDORA) {
      create_notebook_tab(notebook, "fed_tab_view1", "Main", "fed_info_main", "../Resources/fed-info-tab1.txt", "/usr/share/LPIH/text_files/fed-info-tab1.txt");
      create_notebook_tab(notebook, "fed_tab_view2", "Software Management", "fed_info2", "../Resources/fed-info-tab2.txt", "/usr/share/LPIH/text_files/fed-info-tab2.txt");
      create_notebook_tab(notebook, "fed_tab_view3", "Tips", "fed_info3", "../Resources/set_static_ip3.txt", "/usr/share/LPIH/text_files/set_static_ip3.txt");
      g_print("Finished creating Fedora notebook.\n");
      return notebook;
      
    } else {
      g_print("Invalid distro number.  Can't determine which distro's text-files to read.  \n");
      return NULL;
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

  if (g_file_get_contents(res_path1, &tab_text, &length, &error)) {
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

void make_info_window(GtkWidget * widget, gpointer data) {
  if (widget != NULL) {
  
      MainWindowData *main_window_data = (MainWindowData *)data;
    
    InfoWindowData * info_window_data = g_malloc(sizeof(InfoWindowData));
    
    if (info_window_data != NULL) {
      g_print("info_window_data memory initalized. \n");
    } else { g_print("info_window_data failed to initialize.");}
    
   if(main_window_data->distro == DEBIAN) {
   
      gchar * info_window_name_debian = "deb_info_window";
      gchar * info_window_title_debian = "LPIH: Debian Info";
      gchar * notebook_css_debian = "deb_notebook";
      enum Distro info_distro_debian = DEBIAN;
      
      info_window_data -> info_window_name = info_window_name_debian;
      info_window_data -> info_window_title = info_window_title_debian;
      info_window_data -> notebook_css_name = notebook_css_debian;
      info_window_data -> distro_id = info_distro_debian;
      info_window_data -> info_open_flag = debian_info_open;
      
      }
      
        else if(main_window_data->distro == FEDORA) {
      
      gchar * info_window_name = "fed_info_window";
      gchar * info_window_title = "LPIH: Fedora Info";
      gchar * notebook_css = "fed_notebook";
      enum Distro info_distro_fedora = FEDORA;
      
      info_window_data -> info_window_name = info_window_name;
      info_window_data -> info_window_title = info_window_title;
      info_window_data -> notebook_css_name = notebook_css;
      info_window_data -> distro_id = info_distro_fedora;
      info_window_data -> info_open_flag = fedora_info_open;

} else {

g_print("Couldn't get distro of info window to be created.\n");

}

  if (info_window_data -> info_open_flag != TRUE) {

    GtkWidget * info_window;

    info_window = gtk_window_new();
    if (info_window != NULL) {
      g_print("info_window created \n");
    } else {
      g_print("Info window creation failed!\n");
    }

    gtk_widget_add_css_class(info_window, info_window_data -> info_window_name);
    gtk_window_set_title(GTK_WINDOW(info_window), info_window_data -> info_window_title);
    gtk_window_set_resizable(GTK_WINDOW(info_window), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(info_window), 700, 700);
    gtk_widget_set_vexpand(info_window, TRUE);
    gtk_widget_set_hexpand(info_window, TRUE);

    GtkWidget * notebook = make_notebook(info_window_data);

    gtk_window_set_child(GTK_WINDOW(info_window), GTK_WIDGET(notebook));
    g_signal_connect(info_window, "destroy", G_CALLBACK(on_info_window_destroy), info_window_data);
    gtk_widget_set_visible(info_window, TRUE);
    info_window_data -> info_open_flag = TRUE;
    g_print("Info window created!  \n");

  } else {
    g_print("This info window is already open.\n");

  }
 } else { g_print("make_info_window: info_button caller widget is NULL.\n");}
}


