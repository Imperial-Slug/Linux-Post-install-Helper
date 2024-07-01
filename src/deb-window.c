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
#include "deb-window.h"

const gchar* debian_tips_spiel = "  Debian GNU/Linux is one of the oldest and most popular Linux distributions, released in 1993.  It is known for its stability and reliability: which is why it is often used for servers and other critical systems and serves as the base of many other distros, like Ubuntu and Linux Mint: Debian Edition (LMDE).\n\
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


const gchar* DEBIAN_OPENER = "  # Check the boxes according to your needs and run the resulting script in your terminal  \n  # to set up the desired functionality on your Debian system.  You may need to enable non-free  \n  # repositories by editing your '/etc/apt/sources.list' file if some of the proprietary packages  \n  # like Steam and GPU drivers don't install.  See 'tips' for details.  \n\n  sudo apt update; sudo apt upgrade;  \n  sudo apt install build-essential dkms linux-headers-$(uname -r); \n";

const gchar* DEBIAN_STEAM = "  sudo dpkg --add-architecture i386; sudo apt update; \n  sudo apt install steam-devices steam-installer; \n";

const gchar* DEBIAN_GAMING = "  sudo apt install nvidia-driver-libs:i386 mesa-vulkan-drivers libvulkan1;\n  sudo apt install vulkan-tools vulkan-validationlayers gamemode;  \n";

const gchar* DEBIAN_FLATPAK = "  sudo apt install flatpak gnome-software-plugin-flatpak; \n  sudo flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo; \n";

const gchar* DEBIAN_MULTIMEDIA = "  sudo apt install libavcodec-extra;  \n  sudo apt install gstreamer1.0-libav gstreamer1.0-plugins-ugly gstreamer1.0-vaapi;  \n  sudo apt install fonts-crosextra-carlito fonts-crosextra-caladea;  \n";

const gchar* DEBIAN_UFW = "  sudo apt install ufw; sudo ufw enable; \n";
const gchar* DEBIAN_TLP = "  sudo apt install tlp; \n";
const gchar* DEBIAN_VLC = "  sudo apt install vlc; \n";


 int debian_window_open = 0;
 int debian_tips_open = 0;
 
  ////////////////////////////////////////////
 ///// INFORMATIONAL WINDOW: DEBIAN /////////
///////////////////////////////////////////

int on_deb_tips_window_destroy() {
  debian_tips_open = 0;
  
  if(debian_tips_open == 0){
    g_print("debian_tips_open set to 0.\n");
    return 0;
  } else {
            g_print("debian_tips_open is NULL.  Failed to set to 0.\n");
            return 1;
            }
}



 void debian_info_window() {

  if (debian_tips_open != 1) {

    GtkWidget * deb_info_window;
    GtkWidget * deb_info_box;
  

    deb_info_window = gtk_window_new();
    gtk_widget_add_css_class(deb_info_window, "deb_info_window");
    gtk_window_set_title(GTK_WINDOW(deb_info_window), "Debian: tips");
    gtk_window_set_resizable(GTK_WINDOW(deb_info_window), FALSE);
    gtk_window_set_default_size(GTK_WINDOW(deb_info_window), 700, 700);

    GtkWidget * view;
    GtkTextBuffer * buffer;

    deb_info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(deb_info_window), deb_info_box);

    // Create a scrolled window and set the size
    GtkWidget * scroll_info_window = gtk_scrolled_window_new();

    gtk_widget_set_size_request(scroll_info_window, 700, 700);

    view = gtk_text_view_new();
    gtk_widget_set_opacity(view, 0.9);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD_CHAR);
    gtk_widget_add_css_class(view, "deb_info_view");
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    gtk_text_buffer_set_text(buffer, debian_tips_spiel, -1);

    gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
    gtk_widget_set_can_focus(GTK_WIDGET(view), TRUE);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(view), 13);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(view), 13);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll_info_window), view);

    gtk_box_append(GTK_BOX(deb_info_box), scroll_info_window);

    g_signal_connect(deb_info_window, "destroy", G_CALLBACK(on_deb_tips_window_destroy), NULL);

    gtk_widget_set_visible(deb_info_window, TRUE);

  } else {
    g_print("debian_tips window is already open.");

  }

  debian_tips_open = 1;

}

void debian_window() {

  if (debian_window_open != 1) {

    GtkWidget * deb_window, * deb_box, * deb_gpu_check, * deb_steam_check, * deb_game_check, * deb_flatpak_check, * deb_microcode_check, * deb_fonts_check, * deb_ufw_check, * deb_tlp_check, * deb_vlc_check, * deb_info_button;

    deb_window = gtk_window_new();
    gtk_widget_add_css_class(deb_window, "deb_window");
    gtk_window_set_title(GTK_WINDOW(deb_window), "Linux Post-install Helper: Debian");
    gtk_window_set_resizable(GTK_WINDOW(deb_window), FALSE);
    gtk_window_set_default_size(GTK_WINDOW(deb_window), 960, 700);
    gtk_widget_set_can_focus(GTK_WIDGET(deb_window), TRUE);

    GtkWidget * view;
    GtkTextBuffer * buffer;

    deb_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(deb_window), deb_box);
    gtk_widget_set_can_focus(GTK_WIDGET(deb_box), TRUE);

    // CHECKBOXES //////////

    deb_steam_check = gtk_check_button_new_with_label("  Do you plan on using steam?");
    gtk_box_append(GTK_BOX(deb_box), deb_steam_check);

    deb_game_check = gtk_check_button_new_with_label("  Do you plan on playing video games?");
    gtk_box_append(GTK_BOX(deb_box), deb_game_check);

    deb_flatpak_check = gtk_check_button_new_with_label("  Do you want to use flatpak applications?");
    gtk_box_append(GTK_BOX(deb_box), deb_flatpak_check);

    deb_microcode_check = gtk_check_button_new_with_label("  Install your processor's latest microcode?");
    gtk_box_append(GTK_BOX(deb_box), deb_microcode_check);

    deb_fonts_check = gtk_check_button_new_with_label("  Install restricted fonts compatibility for Microsoft products and multimedia compatibility packages?");
    gtk_box_append(GTK_BOX(deb_box), deb_fonts_check);
    
    deb_ufw_check = gtk_check_button_new_with_label("  Do you want to install ufw? (uncomplicated firewall)");
    gtk_box_append(GTK_BOX(deb_box), deb_ufw_check);

    deb_tlp_check = gtk_check_button_new_with_label("  Install tlp for laptop power management?");
    gtk_box_append(GTK_BOX(deb_box), deb_tlp_check);

    deb_vlc_check = gtk_check_button_new_with_label("  Install vlc to play unsupported media formats?");
    gtk_box_append(GTK_BOX(deb_box), deb_vlc_check);

    deb_gpu_check = gtk_check_button_new_with_label("  Install applicable GPU drivers?");
    gtk_box_append(GTK_BOX(deb_box), deb_gpu_check);

    // Create a scrolled window and set the size
    GtkWidget * scroll_window = gtk_scrolled_window_new();

    gtk_widget_set_size_request(scroll_window, 400, 300);
    gtk_widget_set_can_focus(GTK_WIDGET(scroll_window), TRUE);

    view = gtk_text_view_new();
    gtk_widget_set_opacity(view, 0.9);
    gtk_widget_add_css_class(view, "deb_view");
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    gtk_text_buffer_set_text(buffer, DEBIAN_OPENER, -1);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
    gtk_widget_set_can_focus(GTK_WIDGET(view), TRUE);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll_window), view);
    gtk_box_append(GTK_BOX(deb_box), scroll_window);


    // Create separate box to hold button to solve sizing issues //////
    GtkWidget * deb_info_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append(GTK_BOX(deb_box), deb_info_box);
    // create info button for Debian window //

    deb_info_button = gtk_button_new_with_label("Tips");
    gtk_widget_add_css_class(view, "deb_info_button");
    gtk_widget_set_size_request(deb_info_button, 100, 100);
    gtk_widget_add_css_class(deb_info_button, "deb_info_button");
    gtk_box_append(GTK_BOX(deb_info_box), deb_info_button);

    // CONNECT WIDGET CLICKS TO CALLBACK FUNCTIONS //

    g_signal_connect(deb_info_button, "clicked", G_CALLBACK(debian_info_window), NULL);

    g_signal_connect(G_OBJECT(deb_gpu_check), "toggled", G_CALLBACK(deb_gpu_toggled), buffer);
    g_signal_connect(G_OBJECT(deb_steam_check), "toggled", G_CALLBACK(deb_steam_toggled), buffer);
    g_signal_connect(G_OBJECT(deb_game_check), "toggled", G_CALLBACK(deb_game_toggled), buffer);
    g_signal_connect(G_OBJECT(deb_flatpak_check), "toggled", G_CALLBACK(deb_flatpak_toggled), buffer);
    g_signal_connect(G_OBJECT(deb_microcode_check), "toggled", G_CALLBACK(deb_microcode_toggled), buffer);
    g_signal_connect(G_OBJECT(deb_fonts_check), "toggled", G_CALLBACK(deb_fonts_toggled), buffer);
    g_signal_connect(G_OBJECT(deb_ufw_check), "toggled", G_CALLBACK(deb_ufw_toggled), buffer);
    g_signal_connect(G_OBJECT(deb_tlp_check), "toggled", G_CALLBACK(deb_tlp_toggled), buffer);
    g_signal_connect(G_OBJECT(deb_vlc_check), "toggled", G_CALLBACK(deb_vlc_toggled), buffer);
    g_signal_connect(deb_window, "destroy", G_CALLBACK(on_deb_window_destroy), NULL);

    gtk_widget_set_visible(deb_window, TRUE);

    ///////////////////////
    if (gtk_widget_is_visible(deb_window)) {
      debian_window_open = 1;

    } else {
      g_print("Debian LPIH window failed to open.");
     
    }
    ///////////////////////
  }
  
}

////////// DEBIAN GPU DRIVERS CHECKBOX ///////////////
gboolean deb_gpu_toggled(GtkWidget* widget, gpointer data) {
  check_box_state(debian_gpu_command, widget, data);
  return TRUE;
}
//// DEBIAN STEAM CHECKBOX ///////
gboolean deb_steam_toggled(GtkWidget* widget, gpointer data) {
  check_box_state(DEBIAN_STEAM, widget, data);
    return TRUE;
}
//// DEBIAN GAME CHECKBOX ///////
gboolean deb_game_toggled(GtkWidget* widget, gpointer data) {
  check_box_state(DEBIAN_GAMING, widget, data);
  return TRUE;
}
////////// DEBIAN FLATPAK CHECKBOX ///////////////
gboolean deb_flatpak_toggled(GtkWidget* widget, gpointer data) {
  check_box_state(DEBIAN_FLATPAK, widget, data);
  return TRUE;
}
//// DEBIAN MICROCODE CHECKBOX ///////
gboolean deb_microcode_toggled(GtkWidget* widget, gpointer data) {
  check_box_state(debian_microcode_command, widget, data);
  return TRUE;
}
//// DEBIAN FONTS CHECKBOX ///////
gboolean deb_fonts_toggled(GtkWidget* widget, gpointer data) {
  check_box_state(DEBIAN_MULTIMEDIA, widget, data);
  return TRUE;
}
////////// DEBIAN UFW CHECKBOX ///////////////
gboolean deb_ufw_toggled(GtkWidget* widget, gpointer data) {
  check_box_state(DEBIAN_UFW, widget, data);
  return TRUE;
}
//// DEBIAN TLP CHECKBOX ///////
gboolean deb_tlp_toggled(GtkWidget* widget, gpointer data) {
  check_box_state(DEBIAN_TLP, widget, data);
  return TRUE;
}
//// DEBIAN VLC CHECKBOX ///////
gboolean deb_vlc_toggled(GtkWidget* widget, gpointer data) {
  check_box_state(DEBIAN_VLC, widget, data);
  return TRUE;
}

///// Do this when the main window is closed. ////////
gboolean on_deb_window_destroy(void) {
  debian_window_open = FALSE;
  return TRUE;
}

