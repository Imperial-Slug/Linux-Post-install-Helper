/* deb-window.c
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

#include "lpih-main.h"

#include "lpih-window.h"

// DEBIAN CONSTANTS // // // //

const gchar * DEBIAN_STEAM = "  sudo dpkg --add-architecture i386; sudo apt update; \n  sudo apt install steam-devices steam-installer; sudo apt install mesa-vulkan-drivers libvulkan1;\n  sudo apt install vulkan-tools vulkan-validationlayers \n";

const gchar * DEBIAN_GAMING = "  sudo apt install gamemode;  \n";

const gchar * DEBIAN_FLATPAK = "  sudo apt install flatpak gnome-software-plugin-flatpak; \n  sudo flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo; \n";

const gchar * DEBIAN_MULTIMEDIA = "  sudo apt install libavcodec-extra;  \n  sudo apt install gstreamer1.0-libav gstreamer1.0-plugins-ugly gstreamer1.0-vaapi;  \n  sudo apt install fonts-crosextra-carlito fonts-crosextra-caladea;  \n";

const gchar * DEBIAN_UFW = "  sudo apt install ufw; sudo ufw enable; \n";
const gchar * DEBIAN_TLP = "  sudo apt install tlp; \n";
const gchar * DEBIAN_VLC = "  sudo apt install vlc; \n";
const gchar * DEBIAN_GH = "  sudo apt install git gh; \n";

const gchar * debian_microcode_command;
const gchar * debian_gpu_command;

// const gchar* DEBIAN_CONTRIB_NONFREE = "sudo sh -c 'if test -f /etc/apt/sources.list; then echo "contrib non-free" >> /etc/apt/sources.list; fi'"

// FEDORA CONSTANTS // // //

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

// Keeping track of whether the Fedora or Debian window is open.
gboolean debian_info_open = FALSE;
gboolean fedora_info_open = FALSE;

typedef struct {

  enum Distro distro_id;
  gchar * info_window_name;
  gchar * info_window_title;
  gchar * notebook_css_name;
  gboolean info_open_flag;

}
InfoWindowData;

typedef struct {
  const gchar * checkbox1_text;
  const gchar * checkbox2_text;
  const gchar * checkbox3_text;
  const gchar * checkbox4_text;
  const gchar * checkbox5_text;
  const gchar * checkbox6_text;
  const gchar * checkbox7_text;
  const gchar * checkbox8_text;
  const gchar * checkbox9_text;
  const gchar * checkbox10_text;
}
TextData;

////////////////////////////////////////////
///// INFORMATIONAL WINDOW:  /////////
///////////////////////////////////////////

gboolean on_info_window_destroy(GtkWidget * widget, gpointer data) {
  if (widget != NULL) {
  
  InfoWindowData * info_window_data = (InfoWindowData * ) data;

  info_window_data -> info_open_flag = FALSE;

  if (info_window_data -> info_open_flag == FALSE) {
    g_print("info_open_flag for %s set to FALSE.\n", info_window_data -> info_window_name);

    return TRUE;
  } else {
    g_print("%s info_open_flag: Failed to set to FALSE.\n", info_window_data -> info_window_name);
    return FALSE;
  }
} else { g_print("on_info_window_destroy: Widget is NULL.\n"); 
    return FALSE;
    }
}

///////////////////////////////////////////
///////// INFORMATIONAL WINDOW:  //////////
///////////////////////////////////////////

void make_info_window(GtkWidget * widget, gpointer data) {
  if (widget != NULL) {
  
  InfoWindowData * info_window_data = (InfoWindowData * ) data;

  if (info_window_data -> info_open_flag != TRUE) {

    g_print("Line 132 Creating info window...\n");
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

    GtkWidget * notebook;
    notebook = gtk_notebook_new();
    if (notebook != NULL) {
      g_print("Notebook created. \n");
    } else {
      g_print("Notebook is NULL!");
    }

    gtk_widget_add_css_class(notebook, info_window_data -> notebook_css_name);

    if (info_window_data -> distro_id == DEBIAN) {
      g_print("Creating debian notebook\n");
      create_notebook_tab(notebook, "deb_tab_view1", "Main", "deb_info_main", "../Resources/deb-info-tab1.txt", "/usr/share/LPIH/text_files/deb-info-tab1.txt");
      create_notebook_tab(notebook, "deb_tab_view2", "Software Management", "deb_info2", "../Resources/deb-info-tab2.txt", "/usr/share/LPIH/text_files/deb-info-tab2.txt");
      create_notebook_tab(notebook, "deb_tab_view3", "Tips", "deb_info3", "../Resources/set_static_ip3.txt", "/usr/share/LPIH/text_files/set_static_ip3.txt");
      g_print("Finished creating Debian notebook.\n");

    } else if (info_window_data -> distro_id == FEDORA) {
      create_notebook_tab(notebook, "fed_tab_view1", "Main", "fed_info_main", "../Resources/fed-info-tab1.txt", "/usr/share/LPIH/text_files/fed-info-tab1.txt");
      create_notebook_tab(notebook, "fed_tab_view2", "Software Management", "fed_info2", "../Resources/fed-info-tab2.txt", "/usr/share/LPIH/text_files/fed-info-tab2.txt");
      create_notebook_tab(notebook, "fed_tab_view3", "Tips", "fed_info3", "../Resources/set_static_ip3.txt", "/usr/share/LPIH/text_files/set_static_ip3.txt");
      g_print("Finished creating Fedora notebook.\n");
    } else {
      g_print("Invalid distro number.  Can't determine which distro's text-files to read.  \n");
    }

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

// Declare info_window struct member values.

void lpih_window(GtkWidget * widget, gpointer window_data) {

  if (widget != NULL) {
    g_print("info_button clicked.  Executing lpih_window.\n");
  }

  LpihWindowData * lpih_window_data = (LpihWindowData * ) window_data;

  if (lpih_window_data -> window_open_flag != TRUE) {

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

    const gchar * checkbox1_text;
    const gchar * checkbox2_text;
    const gchar * checkbox3_text;
    const gchar * checkbox4_text;
    const gchar * checkbox5_text;
    const gchar * checkbox6_text;
    const gchar * checkbox7_text;
    const gchar * checkbox8_text;
    const gchar * checkbox9_text;
    const gchar * checkbox10_text;

  

  

    InfoWindowData * info_window_data;

    info_window_data = g_malloc(sizeof(InfoWindowData));
    if (info_window_data != NULL) {
      g_print("info_window_data memory initalized. \n");
    } else { g_print("info_window_data failed to initialize.");}

    if (lpih_window_data -> distro_id == DEBIAN) {
      g_print("Chose Debian window.\n");

      gchar * info_window_name_debian = "deb_info_window";
      gchar * info_window_title_debian = "LPIH: Debian Info";
      gchar * notebook_css_debian = "deb_notebook";
      enum Distro info_distro_debian = DEBIAN;
      
      info_window_data -> info_window_name = info_window_name_debian;
      info_window_data -> info_window_title = info_window_title_debian;
      info_window_data -> notebook_css_name = notebook_css_debian;
      info_window_data -> distro_id = info_distro_debian;
      info_window_data -> info_open_flag = debian_info_open;

      checkbox1_text = DEBIAN_STEAM;
      checkbox2_text = DEBIAN_FLATPAK;
      checkbox3_text = debian_gpu_command;
      checkbox4_text = DEBIAN_TLP;
      checkbox5_text = DEBIAN_VLC;
      checkbox6_text = DEBIAN_MULTIMEDIA;
      checkbox7_text = debian_microcode_command;
      checkbox8_text = DEBIAN_GAMING;
      checkbox9_text = DEBIAN_UFW;
      checkbox10_text = DEBIAN_GH;

    } else if (lpih_window_data -> distro_id == FEDORA) {
      g_print("Chose Fedora window.  Initializing...");

      gchar * info_window_name = "fed_info_window";
      gchar * info_window_title = "LPIH: Fedora Info";
      gchar * notebook_css = "fed_notebook";
      enum Distro info_distro_fedora = FEDORA;
      
      info_window_data -> info_window_name = info_window_name;
      info_window_data -> info_window_title = info_window_title;
      info_window_data -> notebook_css_name = notebook_css;
      info_window_data -> distro_id = info_distro_fedora;
      info_window_data -> info_open_flag = fedora_info_open;

      checkbox1_text = FEDORA_STEAM;
      checkbox2_text = FEDORA_FLATPAK;
      checkbox3_text = fedora_gpu_command;
      checkbox4_text = FEDORA_TLP;
      checkbox5_text = FEDORA_VLC;
      checkbox6_text = FEDORA_MULTIMEDIA;
      checkbox7_text = FEDORA_CUST;
      checkbox8_text = FEDORA_REP;
      checkbox9_text = FEDORA_DNF;
      checkbox10_text = FEDORA_GH;

    } else {
      g_print("\nCOULDN'T INITIALIZE CHECKBOXES\n");
      checkbox1_text = NULL;
      checkbox2_text = NULL;
      checkbox3_text = NULL;
      checkbox4_text = NULL;
      checkbox5_text = NULL;
      checkbox6_text = NULL;
      checkbox7_text = NULL;
      checkbox8_text = NULL;
      checkbox9_text = NULL;
      checkbox10_text = NULL;
      //info_window_data = g_malloc(sizeof(InfoWindowData));
      // g_free(info_window_data);

    }

    window = gtk_window_new();
    gtk_widget_add_css_class(window, lpih_window_data -> css_label);
    gtk_window_set_title(GTK_WINDOW(window), lpih_window_data -> window_title);
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
    gtk_widget_add_css_class(view, lpih_window_data -> view_css_class);
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    gtk_text_buffer_set_text(buffer, lpih_window_data -> opener, -1);
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
    gtk_widget_add_css_class(info_button, lpih_window_data -> info_button_css_class);
    gtk_box_append(GTK_BOX(info_button_box), info_button);

    // CHECKBOXES //////////
    checkbox1 = gtk_check_button_new_with_label(lpih_window_data -> checkbox1_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox1);
    CheckboxData * checkbox1_data = g_malloc(sizeof(CheckboxData));
    checkbox1_data -> shared_buffer = buffer;
    checkbox1_data -> associated_command = checkbox1_text;

    checkbox2 = gtk_check_button_new_with_label(lpih_window_data -> checkbox2_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox2);
    CheckboxData * checkbox2_data = g_malloc(sizeof(CheckboxData));
    checkbox2_data -> shared_buffer = buffer;
    checkbox2_data -> associated_command = checkbox2_text;

    checkbox3 = gtk_check_button_new_with_label(lpih_window_data -> checkbox3_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox3);
    CheckboxData * checkbox3_data = g_malloc(sizeof(CheckboxData));
    checkbox3_data -> shared_buffer = buffer;
    checkbox3_data -> associated_command = checkbox3_text;

    checkbox4 = gtk_check_button_new_with_label(lpih_window_data -> checkbox4_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox4);
    CheckboxData * checkbox4_data = g_malloc(sizeof(CheckboxData));
    checkbox4_data -> shared_buffer = buffer;
    checkbox4_data -> associated_command = checkbox4_text;

    checkbox5 = gtk_check_button_new_with_label(lpih_window_data -> checkbox5_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox5);
    CheckboxData * checkbox5_data = g_malloc(sizeof(CheckboxData));
    checkbox5_data -> shared_buffer = buffer;
    checkbox5_data -> associated_command = checkbox5_text;

    checkbox6 = gtk_check_button_new_with_label(lpih_window_data -> checkbox6_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox6);
    CheckboxData * checkbox6_data = g_malloc(sizeof(CheckboxData));
    checkbox6_data -> shared_buffer = buffer;
    checkbox6_data -> associated_command = checkbox6_text;

    checkbox7 = gtk_check_button_new_with_label(lpih_window_data -> checkbox7_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox7);
    CheckboxData * checkbox7_data = g_malloc(sizeof(CheckboxData));
    checkbox7_data -> shared_buffer = buffer;
    checkbox7_data -> associated_command = checkbox7_text;

    checkbox8 = gtk_check_button_new_with_label(lpih_window_data -> checkbox8_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox8);
    CheckboxData * checkbox8_data = g_malloc(sizeof(CheckboxData));
    checkbox8_data -> shared_buffer = buffer;
    checkbox8_data -> associated_command = checkbox8_text;

    checkbox9 = gtk_check_button_new_with_label(lpih_window_data -> checkbox9_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox9);
    CheckboxData * checkbox9_data = g_malloc(sizeof(CheckboxData));
    checkbox9_data -> shared_buffer = buffer;
    checkbox9_data -> associated_command = checkbox9_text;
    
     checkbox10 = gtk_check_button_new_with_label(lpih_window_data -> checkbox10_title);
    gtk_box_append(GTK_BOX(checkbox_box), checkbox10);
    CheckboxData * checkbox10_data = g_malloc(sizeof(CheckboxData));
    checkbox10_data -> shared_buffer = buffer;
    checkbox10_data -> associated_command = checkbox10_text;


    lpih_window_data -> checkbox1 = checkbox1;
    lpih_window_data -> checkbox2 = checkbox2;
    lpih_window_data -> checkbox3 = checkbox3;
    lpih_window_data -> checkbox4 = checkbox4;
    lpih_window_data -> checkbox5 = checkbox5;
    lpih_window_data -> checkbox6 = checkbox6;
    lpih_window_data -> checkbox7 = checkbox7;
    lpih_window_data -> checkbox8 = checkbox8;
    lpih_window_data -> checkbox9 = checkbox9;
    lpih_window_data -> checkbox10 = checkbox10;

    // CONNECT WIDGET CLICKS TO CALLBACK FUNCTIONS //
    g_signal_connect(info_button, "clicked", G_CALLBACK(make_info_window), info_window_data);

    g_signal_connect(G_OBJECT(checkbox1), "toggled", G_CALLBACK(check_box_state), checkbox1_data);
    g_signal_connect(G_OBJECT(checkbox2), "toggled", G_CALLBACK(check_box_state), checkbox2_data);
    g_signal_connect(G_OBJECT(checkbox3), "toggled", G_CALLBACK(check_box_state), checkbox3_data);
    g_signal_connect(G_OBJECT(checkbox4), "toggled", G_CALLBACK(check_box_state), checkbox4_data);
    g_signal_connect(G_OBJECT(checkbox5), "toggled", G_CALLBACK(check_box_state), checkbox5_data);
    g_signal_connect(G_OBJECT(checkbox6), "toggled", G_CALLBACK(check_box_state), checkbox6_data);
    g_signal_connect(G_OBJECT(checkbox7), "toggled", G_CALLBACK(check_box_state), checkbox7_data);
    g_signal_connect(G_OBJECT(checkbox8), "toggled", G_CALLBACK(check_box_state), checkbox8_data);
    g_signal_connect(G_OBJECT(checkbox9), "toggled", G_CALLBACK(check_box_state), checkbox9_data);
    g_signal_connect(G_OBJECT(checkbox10), "toggled", G_CALLBACK(check_box_state), checkbox10_data);

    g_signal_connect(window, "destroy", G_CALLBACK(on_lpih_window_destroy), lpih_window_data);

    gtk_window_present(GTK_WINDOW(window));
    lpih_window_data -> window_open_flag = TRUE;

  } //else {  g_free(lpih_window_data); }

}

///// Do this when the main window is closed. ////////
gboolean on_lpih_window_destroy(GtkWidget * window, gpointer data) {
  LpihWindowData * window_data = (LpihWindowData * ) data;
  window_data -> window_open_flag = FALSE;

  if (gtk_widget_is_visible(window)) {
    g_print("LPIH window failed to close.\n");
    return FALSE;

  } else {

    g_print("LPIH window closed successfully.\n");
    //g_free(window_data);
    return TRUE;
  }

}
