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

const gchar* fedora_tips_spiel = "  Debian GNU/Linux is one of the oldest and most popular Linux distributions, released in 1993.  It is known for its stability and reliability: which is why it is often used for servers and other critical systems and serves as the base of many other distros, like Ubuntu and Linux Mint: Debian Edition (LMDE).\n\
\n\
Debian, like other Linux distributions, has a package manager; which handles the installation, updating and removal of software packages on the computer.  Debian's package manager is called apt.  In order to use some apt commands, the user must use the sudo command to elevate their privileges to those of a super-user, example: sudo apt install nvidia-driver, where nvidia-driver is the package you are trying to install.  \n\
\n\
There are some instructions for optional system configuration outlined here that you might find useful.\n\
\n\
SETTING   IP\n\
\n\
Normally, your device's unique IP address is assigned dynamically, meaning it is not necessarily going to be the same everytime it connects to your network.  You can get a slightly more stable and performant connection by setting a   IP address, where your IP is the same everytime it connects to the network.  This will also aid in configuration of firewalls and servers, since you can now specify your computer's connection by its exact IP address.  Having a   IP can be useful for a multitude of reasons, one of them being the implementation of firewall rules.  The instructions for switching to a   IP on Linux in the GNOME desktop environment are as follows:\n\
\n\
1.     Click in the top-right corner of the screen where the power icon is.  Click the gear-shaped icon to access settings.  You can also find the settings application icon by pressing the super key (Windows key or command key on Mac) and either find it visually or search settings in the searchbar that pops up with the GNOME Activities window.  This Activities window can also be accessed by clicking Activities in the tope left of the screen.\n\
\n\
2.     If you are using Wi-Fi, click on Wi-Fi at the top of the scrollable menu on the left of the settings window.  If you are using ethernet, click on Network. \n\
\n\
3.     Click on the gear icon for the network connection you are using.  Take note of the IP you are currently assigned in the IPv4 Address section of this window, under the Details tab.  When you assign a   IP to your connection, you will make one that looks very similar to this one; save for the digits after the last decimal.  So, if your IPv4 is currently 172.178.1.19 then you can likely assign yourself anything from 192.168.1.0 to 192.168.1.255, as long as the address is not in use by another network device.\n\
\n\
4.     Click the IPv4 tab, and change the method from Automatic (DHCP) to Manual.\n\
\n\
5.     In the address field, enter the   IP address you want to use.  \n\
\n\
6.     Under Netmask, enter the subnet mask for your network.  You can enter the ip addr show command to check which netmask to use.  If you are using Wi-Fi, you will be looking for an interface that looks something like wlp32s0.  Ethernet will look something like enp18s0. \n\
Beside the inet line for your interface in the terminal command output, there will be a slash follwed by a number: this is the number of bits in your subnet mask.\n\
\n\
Thus, inet 192.168.1.123/24 indicates a 24-bit netmask, which means 255.255.255.0 is your subnet mask.  \n\
\n\
- If it reads /16 then you will enter 255.255.0.0.  \n\
\n\
- For /8: 255.0.0.0 \n\
\n\
- For /25: 255.255.255.128 and finally \n\
\n\
- For /30 we have 255.255.255.252 for the subnet mask.    \n\
\n\
A subnet mask determines the size of a network: if more bits are available, then more devices can be connected.\n\
\n\
7.  Now we must determine the Gateway address, which is the address of your router.  If you have accessed your router's admin tools from your browser before by typing in an IP address, this would be the same address; and it will generally be a similar IP sequence to what you have been dealing with, but with 1 as the only digit after the last decimal, like:  XXX.XXX.X.1 \n\
So, if we are to keep with our 192.168.1.123 example, our gateway is most likely 192.168.1.1.  However, we want to be sure of this.  To confirm your gateway, use the ip route show command in the terminal.  Look for the default via line with an IP beside it.  This is your gateway address to enter. \n\
\n\
8.    Time to choose your DNS servers.  You can pick whichever one you like.  Cloudflare is popular with many, since it offers security features like DDoS protection, SSL encryption, and a web application firewall.  You can specify 2 addresses, in case one is down (seperate them with a comma in the textfield).  To use Cloudflare as your DNS provider, you can use the addresses 1.1.1.1, 1.0.0.1. For added malware protection, substitute the last 1 in each address with a 2.  OpenDNS is another popular provider that offers its own security features and malware protection, much like Cloudflare.  To use OpenDNS, use 208.67.222.222, 208.67.220.220. \n\
\n\
9.  Save your new   IP address configuration with the Apply button.  Go to your network settings and switch the connection off, then on again.  Try to connect to a webpage.  If it works, you are good to go.  If not, you may have made an error in your IP configuration.  If you need to go back to an automatically assigned IP you can undo the   IP settings by simply switching your connection from Manual to Automatic (DHCP) again in the settings. \n\n\
TROUBLESHOOTING APT PACKAGE MANAGER ON DEBIAN\n\n\
Occasionally, when trying to install software with apt on Debian, you may encounter an error with the message: \"You have held broken packages.\"  This can be fixed much of the time by following up with the following 2 commands: \n\n  sudo apt --fix-broken install\n  sudo apt install <name of package>\n\n";


const gchar* FEDORA_OPENER = "  # Check the boxes according to your needs and run the resulting script in your terminal  \n  # to set up the desired functionality on your Fedora system.  \n\n  sudo dnf update; sudo dnf upgrade; \n";


const gchar* FEDORA_REP = "  sudo dnf install https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm;  \n  sudo dnf install https://download1.rpmfusion.org/nonfree/fedora/rpmfusion-nonfree-release-$(rpm -E %fedora).noarch.rpm;  \n  sudo dnf update; \n";

const gchar* FEDORA_STEAM = "  sudo dnf install steam;\n";

const gchar* FEDORA_DNF = "  sudo sh -c 'if test -f /etc/dnf/dnf.conf; then echo \"max_parallel_downloads=20\" >> /etc/dnf/dnf.conf; fi'\n";

const gchar* FEDORA_FLATPAK = "  flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo; \n";

const gchar* FEDORA_CUST = "  sudo dnf install gnome-tweaks gnome-extensions-app; \n";



const gchar* FEDORA_MULTIMEDIA = "  sudo dnf install ffmpeg --allowerasing &&  \n  sudo dnf install gstreamer1-plugins-{bad-\\*,good-\\*,base} gstreamer1-plugin-openh264 gstreamer1-libav --exclude=gstreamer1-plugins-bad-free-devel &&  \n  sudo dnf install lame\\* --exclude=lame-devel && sudo dnf group upgrade --with-optional Multimedia \n";

const gchar* FEDORA_TLP = "  sudo dnf install tlp; \n";

const gchar* FEDORA_VLC = "  sudo dnf install vlc; \n";


 const gchar* fedora_gpu_command;
 int fedora_window_open = 0;
 int fedora_tips_open = 0;
 
 /////////////////////////////////////////////////////
// INFORMATIONAL WINDOW: FEDORA /////////////////////
// //////////////////////////////////////////////////

 void fedora_info_window() {

  if (fedora_tips_open != 1) {

    GtkWidget * fed_info_window;
    GtkWidget * fed_info_box;


    fed_info_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(fed_info_window), "Fedora: tips");
    gtk_window_set_resizable(GTK_WINDOW(fed_info_window), FALSE);
    gtk_window_set_default_size(GTK_WINDOW(fed_info_window), 700, 700);

    GtkWidget * view;
    GtkTextBuffer * buffer;

    fed_info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(fed_info_window), fed_info_box);

    // Create a scrolled window and set the size
    GtkWidget * scroll_info_window = gtk_scrolled_window_new();

    gtk_widget_set_size_request(scroll_info_window, 700, 700);

    view = gtk_text_view_new();
    gtk_widget_set_opacity(view, 0.9);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD_CHAR);
    gtk_widget_add_css_class(view, "fed_info_view");
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    gtk_text_buffer_set_text(buffer, fedora_tips_spiel, -1);

    gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
    gtk_widget_set_can_focus(GTK_WIDGET(view), True);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(view), 13);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(view), 13);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll_info_window), view);
    gtk_box_append(GTK_BOX(fed_info_box), scroll_info_window);

    g_signal_connect(fed_info_window, "destroy", G_CALLBACK(on_fed_tips_window_destroy), NULL);

    gtk_widget_set_visible(fed_info_window, TRUE);

  } else {
    g_print("Error: Fedora Tips already open.");
  }

  fedora_tips_open = 1;

}

 void on_fed_tips_window_destroy() {
  fedora_tips_open = 0;
}

//////////////////////////////////////////
//                                     || //
//      FEDORA WINDOW AND FUNCTIONS   |  |  ||
//                                     || //
//////////////////////////////////////////

void fedora_window() {

  if (fedora_window_open != 1) {

    GtkWidget * fed_window;
    GtkWidget * fed_box, * fed_gpu_check, * fed_steam_check, * fed_dnf_check, * fed_flatpak_check, * fed_repo_check, * fed_customization_check, * fed_codecs_check, * fed_tlp_check, * fed_vlc_check, * fed_info_button;
    fed_window = gtk_window_new();
    gtk_widget_add_css_class(fed_window, "fed_window");
    gtk_window_set_title(GTK_WINDOW(fed_window), "Linux Post-install Helper: Fedora");
    gtk_window_set_resizable(GTK_WINDOW(fed_window), FALSE);
    gtk_window_set_default_size(GTK_WINDOW(fed_window), 960, 700);
    gtk_widget_set_can_focus(GTK_WIDGET(fed_window), TRUE);

    GtkWidget * view;
    GtkTextBuffer * buffer;

    fed_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(fed_window), fed_box);

    // FEDORA CHECKBOXES //////////

    fed_dnf_check = gtk_check_button_new_with_label("  Optimize the dnf package manager for faster downloads?");
    gtk_box_append(GTK_BOX(fed_box), fed_dnf_check);

    fed_repo_check = gtk_check_button_new_with_label("  Enable RPM-fusion repositories for wider range of software?");
    gtk_box_append(GTK_BOX(fed_box), fed_repo_check);

    fed_steam_check = gtk_check_button_new_with_label("  Do you plan on using steam?");
    gtk_box_append(GTK_BOX(fed_box), fed_steam_check);

    fed_flatpak_check = gtk_check_button_new_with_label("  Do you want to use flatpak applications?");
    gtk_box_append(GTK_BOX(fed_box), fed_flatpak_check);

    fed_customization_check = gtk_check_button_new_with_label("  Install gnome-tweaks and gnome-extensions for desktop customization?");
    gtk_box_append(GTK_BOX(fed_box), fed_customization_check);

    fed_codecs_check = gtk_check_button_new_with_label("  Do you want to install multimedia codecs for unsupported media formats?");
    gtk_box_append(GTK_BOX(fed_box), fed_codecs_check);

    fed_tlp_check = gtk_check_button_new_with_label("  Install tlp for laptop power management?");
    gtk_box_append(GTK_BOX(fed_box), fed_tlp_check);

    fed_vlc_check = gtk_check_button_new_with_label("  Install vlc to play unsupported media formats?");
    gtk_box_append(GTK_BOX(fed_box), fed_vlc_check);

    fed_gpu_check = gtk_check_button_new_with_label("  Install applicable GPU drivers?");
    gtk_box_append(GTK_BOX(fed_box), fed_gpu_check);

    // Create a scrolled window and set the size
    GtkWidget * scroll_window = gtk_scrolled_window_new();

    gtk_widget_set_size_request(scroll_window, 400, 300);

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
    GtkWidget * fed_info_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append(GTK_BOX(fed_box), fed_info_box);
    // create info button for Debian window //

    fed_info_button = gtk_button_new_with_label("Tips");
    gtk_widget_add_css_class(view, "fed_info_button");
    gtk_widget_set_size_request(fed_info_button, 100, 100);
    gtk_widget_add_css_class(fed_info_button, "fed_info_button");
    gtk_box_append(GTK_BOX(fed_info_box), fed_info_button);

    // Connect checkbox functions to checkboxes ///
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

    if (gtk_widget_is_visible(fed_window)) {
      fedora_window_open = 1;

    } else {
      g_print("Fedora window failed to open.");

    }
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

 void on_fed_window_destroy() {
  fedora_window_open = 0;
}

