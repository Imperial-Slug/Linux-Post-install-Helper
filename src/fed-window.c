/* fed-window.c
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
#include "fed-window.h"

const gchar* FEDORA_OPENER = "  # Check the boxes according to your needs and run the resulting script in your terminal  \n  # to set up the desired functionality on your Fedora system.  \n\n  sudo dnf update;  \n";

const gchar* FEDORA_REP = "  sudo dnf install https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm;  \n  sudo dnf install https://download1.rpmfusion.org/nonfree/fedora/rpmfusion-nonfree-release-$(rpm -E %fedora).noarch.rpm;  \n  sudo dnf update; \n";

const gchar* FEDORA_STEAM = "  sudo dnf install steam;\n";

const gchar* FEDORA_DNF = "  sudo sh -c 'if test -f /etc/dnf/dnf.conf; then echo \"max_parallel_downloads=20\" >> /etc/dnf/dnf.conf; fi'\n";

const gchar* FEDORA_FLATPAK = "  flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo; \n";

const gchar* FEDORA_CUST = "  sudo dnf install gnome-tweaks gnome-extensions-app; \n";

const gchar* FEDORA_MULTIMEDIA = "  sudo dnf install ffmpeg --allowerasing &&  \n  sudo dnf install gstreamer1-plugins-{bad-\\*,good-\\*,base} gstreamer1-plugin-openh264 gstreamer1-libav --exclude=gstreamer1-plugins-bad-free-devel &&  \n  sudo dnf install lame\\* --exclude=lame-devel && sudo dnf group upgrade --with-optional Multimedia \n";

const gchar* FEDORA_TLP = "  sudo dnf install tlp; \n";

const gchar* FEDORA_VLC = "  sudo dnf install vlc; \n";

 const gchar* fedora_gpu_command = "";
 gboolean fedora_window_open = FALSE;
 gboolean fedora_tips_open = FALSE;
 
 /////////////////////////////////////////////////////
// INFORMATIONAL WINDOW: FEDORA /////////////////////
// //////////////////////////////////////////////////

 void fedora_info_window() {

  if (fedora_tips_open != TRUE) {

    GtkWidget* fed_info_window;
    GtkWidget* fed_info_box;


    fed_info_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(fed_info_window), "Fedora: info");
    gtk_window_set_resizable(GTK_WINDOW(fed_info_window), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(fed_info_window), 800, 700);

    GtkWidget* view;
    GtkTextBuffer* buffer;

    fed_info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(fed_info_window), fed_info_box);

    // Create a scrolled window and set the size
    GtkWidget* scroll_info_window = gtk_scrolled_window_new();

    gtk_widget_set_size_request(scroll_info_window, 800, 700);

    gtk_widget_set_vexpand(scroll_info_window, TRUE);
    gtk_widget_set_hexpand(scroll_info_window, TRUE);  

    view = gtk_text_view_new();
    gtk_widget_set_opacity(view, 0.9);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD_CHAR);
    gtk_widget_add_css_class(view, "fed_info_view");
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

  // Load text file.
    gchar *tips_spiel_fedora = NULL;
    gsize length = 0;
    GError *error = NULL;

    if (g_file_get_contents("../Resources/fed-text.txt", &tips_spiel_fedora, &length, &error)) {
    gtk_text_buffer_set_text(buffer, tips_spiel_fedora, -1);
    g_free(tips_spiel_fedora);
      }  else if (g_file_get_contents("/usr/share/LPIH/text_files/fed-text.txt", &tips_spiel_fedora, &length, &error)) {
    
    gtk_text_buffer_set_text(buffer, tips_spiel_fedora, -1);
    g_free(tips_spiel_fedora); 
    
    } else {
            g_print("Failed to load Fedora tips file: %s\n", error->message);
            g_error_free(error);
        }

    gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
    gtk_widget_set_can_focus(GTK_WIDGET(view), True);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(view), 13);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(view), 13);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll_info_window), view);
    gtk_box_append(GTK_BOX(fed_info_box), scroll_info_window);

    g_signal_connect(fed_info_window, "destroy", G_CALLBACK(on_fed_tips_window_destroy), NULL);

    gtk_window_present(GTK_WINDOW(fed_info_window));

  } else {
    g_print("Error: Fedora Tips already open.");
  }

  fedora_tips_open = TRUE;

}

 gboolean on_fed_tips_window_destroy() {
  fedora_tips_open = FALSE;
   if(fedora_tips_open == FALSE){
    g_print("fedora_tips_open set to FALSE.\n");
    return TRUE;
  } else {
            g_print("fedora_tips_open: Failed to set to FALSE.\n");
            return FALSE;
            }
}

//////////////////////////////////////////
//                                     || 
//      FEDORA WINDOW AND FUNCTIONS   |  |
//                                     || 
//////////////////////////////////////////

GtkWidget* fed_window;

gboolean fedora_window() {

  if (fedora_window_open != TRUE) {


    GtkWidget *fed_box, *fed_gpu_check, *fed_steam_check, *fed_dnf_check, *fed_flatpak_check, *fed_repo_check, *fed_customization_check, *fed_codecs_check, *fed_tlp_check, *fed_vlc_check, *fed_info_button;
    fed_window = gtk_window_new();
    gtk_widget_add_css_class(fed_window, "fed_window");
    gtk_window_set_title(GTK_WINDOW(fed_window), "Linux Post-install Helper: Fedora");
    gtk_window_set_resizable(GTK_WINDOW(fed_window), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(fed_window), 800, 700);
    gtk_widget_set_can_focus(GTK_WIDGET(fed_window), TRUE);
    gtk_widget_set_vexpand(fed_window, TRUE);
    gtk_widget_set_hexpand(fed_window, TRUE);  

    GtkWidget* view;
    GtkTextBuffer* buffer;
    
    GtkWidget* checkbox_scroll_window = gtk_scrolled_window_new();
    gtk_widget_set_size_request(checkbox_scroll_window, 300, 200);
    gtk_widget_set_vexpand(checkbox_scroll_window, TRUE);
    gtk_widget_set_hexpand(checkbox_scroll_window, TRUE);  
    gtk_widget_set_can_focus(GTK_WIDGET(checkbox_scroll_window), TRUE);

    fed_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(fed_window), fed_box);
    gtk_widget_set_can_focus(GTK_WIDGET(fed_box), TRUE);

    // Create a box to contain checkboxes
    GtkWidget* checkbox_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    // Add the box to the scrolled window
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(checkbox_scroll_window), checkbox_box);

    // FEDORA CHECKBOXES //////////

    fed_dnf_check = gtk_check_button_new_with_label("  Optimize the dnf package manager for faster downloads?");
    gtk_box_append(GTK_BOX(checkbox_box), fed_dnf_check);

    fed_repo_check = gtk_check_button_new_with_label("  Enable RPM-fusion repositories for wider range of software?");
    gtk_box_append(GTK_BOX(checkbox_box), fed_repo_check);

    fed_steam_check = gtk_check_button_new_with_label("  Do you plan on using Steam?");
    gtk_box_append(GTK_BOX(checkbox_box), fed_steam_check);

    fed_flatpak_check = gtk_check_button_new_with_label("  Do you want to use flatpak applications?");
    gtk_box_append(GTK_BOX(checkbox_box), fed_flatpak_check);

    fed_customization_check = gtk_check_button_new_with_label("  Install gnome-tweaks and gnome-extensions for desktop customization?");
    gtk_box_append(GTK_BOX(checkbox_box), fed_customization_check);

    fed_codecs_check = gtk_check_button_new_with_label("  Do you want to install multimedia codecs for unsupported media formats?");
    gtk_box_append(GTK_BOX(checkbox_box), fed_codecs_check);

    fed_tlp_check = gtk_check_button_new_with_label("  Install tlp for laptop power management?");
    gtk_box_append(GTK_BOX(checkbox_box), fed_tlp_check);

    fed_vlc_check = gtk_check_button_new_with_label("  Install vlc to play unsupported media formats?");
    gtk_box_append(GTK_BOX(checkbox_box), fed_vlc_check);

    fed_gpu_check = gtk_check_button_new_with_label("  Install applicable GPU drivers?");
    gtk_box_append(GTK_BOX(checkbox_box), fed_gpu_check);
    
    gtk_box_append(GTK_BOX(fed_box), checkbox_scroll_window); 
    

    // Create a scrolled window and set the size
    GtkWidget* scroll_window = gtk_scrolled_window_new();

    gtk_widget_set_size_request(scroll_window, 200, 200);
    gtk_widget_set_vexpand(scroll_window, TRUE);
    gtk_widget_set_hexpand(scroll_window, TRUE);  

    view = gtk_text_view_new();
    gtk_widget_set_opacity(view, 0.9);

    gtk_widget_add_css_class(view, "fed_view");
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    gtk_text_buffer_set_text(buffer, FEDORA_OPENER, -1);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
    gtk_widget_set_can_focus(GTK_WIDGET(view), TRUE);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll_window), view);
    gtk_box_append(GTK_BOX(fed_box), scroll_window);

    // FEDORA INFO WINDOW //

    // Create separate box to hold button to solve sizing issues //////
    GtkWidget* fed_info_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append(GTK_BOX(fed_box), fed_info_box);
    
    // create info button for Fedora window //

    fed_info_button = gtk_button_new_with_label("Info");
    gtk_widget_add_css_class(view, "fed_info_button");
    gtk_widget_set_size_request(fed_info_button, 64, 64);
    gtk_widget_add_css_class(fed_info_button, "fed_info_button");
    gtk_box_append(GTK_BOX(fed_info_box), fed_info_button);

    // Connect checkbox functions to checkboxes //
    g_signal_connect(fed_info_button, "clicked", G_CALLBACK(fedora_info_window), NULL);

    g_signal_connect(G_OBJECT(fed_gpu_check), "toggled", G_CALLBACK(fed_gpu_toggled), buffer);
    g_signal_connect(G_OBJECT(fed_steam_check), "toggled", G_CALLBACK(fed_steam_toggled), buffer);
    g_signal_connect(G_OBJECT(fed_dnf_check), "toggled", G_CALLBACK(fed_dnf_toggled), buffer);
    g_signal_connect(G_OBJECT(fed_flatpak_check), "toggled", G_CALLBACK(fed_flatpak_toggled), buffer);
    g_signal_connect(G_OBJECT(fed_repo_check), "toggled", G_CALLBACK(fed_repo_toggled), buffer);
    g_signal_connect(G_OBJECT(fed_customization_check), "toggled", G_CALLBACK(fed_customization_toggled), buffer);
    g_signal_connect(G_OBJECT(fed_codecs_check), "toggled", G_CALLBACK(fed_codecs_toggled), buffer);
    g_signal_connect(G_OBJECT(fed_tlp_check), "toggled", G_CALLBACK(fed_tlp_toggled), buffer);
    g_signal_connect(G_OBJECT(fed_vlc_check), "toggled", G_CALLBACK(fed_vlc_toggled), buffer);
    g_signal_connect(fed_window, "destroy", G_CALLBACK(on_fed_window_destroy), NULL);

    gtk_widget_set_visible(fed_window, TRUE);

    
  }
  if (gtk_widget_is_visible(fed_window)) {
      fedora_window_open = TRUE;
      return TRUE;

    } else {
      g_print("Fedora window failed to open.");
      return FALSE;
    }
  
}



 //// FEDORA REPO CHECKBOX ///////
gboolean fed_repo_toggled(GtkWidget* widget, gpointer data) {
  check_box_state(FEDORA_REP, widget, data);
  return TRUE;
}
////////// FEDORA GPU CHECKBOX ///////////////
gboolean fed_gpu_toggled(GtkWidget* widget, gpointer data) {
 check_box_state(fedora_gpu_command, widget, data); 
    return TRUE;
    } 

//// FEDORA STEAM CHECKBOX ///////
gboolean fed_steam_toggled(GtkWidget* widget, gpointer data) {
  check_box_state(FEDORA_STEAM, widget, data);
  return TRUE;
}
//// FEDORA DNF SETTINGS CHECKBOX ///////
gboolean fed_dnf_toggled(GtkWidget* widget, gpointer data) {
  check_box_state(FEDORA_DNF, widget, data);
  return TRUE;
}
////////// FEDORA FLATPAK CHECKBOX ///////////////
gboolean fed_flatpak_toggled(GtkWidget* widget, gpointer data) {
  check_box_state(FEDORA_FLATPAK, widget, data);
  return TRUE;
}
//// FEDORA CUSTOMIZATION CHECKBOX ///////
gboolean fed_customization_toggled(GtkWidget* widget, gpointer data) {
  check_box_state(FEDORA_CUST, widget, data);
  return TRUE;
}
////////// FEDORA CODECS CHECKBOX ///////////////
gboolean fed_codecs_toggled(GtkWidget* widget, gpointer data) {
  check_box_state(FEDORA_MULTIMEDIA, widget, data);
  return TRUE;
}
//// FEDORA TLP CHECKBOX ///////
gboolean fed_tlp_toggled(GtkWidget* widget, gpointer data) {
  check_box_state(FEDORA_TLP, widget, data);
  return TRUE;
}
//// FEDORA VLC CHECKBOX ///////
gboolean fed_vlc_toggled(GtkWidget* widget, gpointer data) {
  check_box_state(FEDORA_VLC, widget, data);
  return TRUE;
}

 gboolean on_fed_window_destroy() {
    if (gtk_widget_is_visible(fed_window)) {
        g_print("Fedora LPIH window failed to close.\n");
         return FALSE;

    } else {
      
      g_print("Fedora LPIH window closed successfully.\n");
      return TRUE;
    }
}

