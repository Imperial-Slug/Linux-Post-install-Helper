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

#include <gtk/gtk.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <X11/Xlib.h>

#include <GL/gl.h>

static void deb_gpu_toggled(GtkWidget * widget, gpointer data);
static void deb_steam_toggled(GtkWidget * widget, gpointer data);
static void deb_game_toggled(GtkWidget * widget, gpointer data);
static void deb_flatpak_toggled(GtkWidget * widget, gpointer data);
static void deb_microcode_toggled(GtkWidget * widget, gpointer data);
static void deb_fonts_toggled(GtkWidget * widget, gpointer data);
static void deb_ufw_toggled(GtkWidget * widget, gpointer data);
static void deb_tlp_toggled(GtkWidget * widget, gpointer data);
static void deb_vlc_toggled(GtkWidget * widget, gpointer data);
static void on_deb_window_destroy(GtkWidget * deb_window, gpointer user_data);
static void on_deb_tips_window_destroy(GtkWidget * deb_info_window, gpointer user_data);
static void get_cpu_vendor(char * vendor);

static void fed_gpu_toggled(GtkWidget * widget, gpointer data);
static void fed_steam_toggled(GtkWidget * widget, gpointer data);
static void fed_dnf_toggled(GtkWidget * widget, gpointer data);
static void fed_flatpak_toggled(GtkWidget * widget, gpointer data);
static void fed_repo_toggled(GtkWidget * widget, gpointer data);
static void fed_customization_toggled(GtkWidget * widget, gpointer data);
static void fed_codecs_toggled(GtkWidget * widget, gpointer data);
static void fed_tlp_toggled(GtkWidget * widget, gpointer data);
static void fed_vlc_toggled(GtkWidget * widget, gpointer data);
static void on_fed_window_destroy(GtkWidget * fed_window, gpointer user_data);
static void on_fed_tips_window_destroy(GtkWidget * fed_info_window, gpointer user_data);

// Static variables that tell the program what kind of CPU and GPU the user has to use the proper commands on the next screen.

// 1 = AMD, 2 = Intel, 3 = Nvidia.
static int cpu_manufacturer = 0;
static int gpu_manufacturer = 0;

//For keeping track of single-instance windows.
static int lpih_instance_running = 0;
static int debian_window_open = 0;
static int debian_tips_open = 0;
static int fedora_window_open = 0;
static int fedora_tips_open = 0;

const gchar * tips_spiel = "  Debian GNU/Linux is one of the oldest and most popular Linux distributions, released in 1993.  It is known for its stability and reliability: which is why it is often used for servers and other critical systems and serves as the base of many other distros, like Ubuntu and Linux Mint: Debian Edition (LMDE).\n\
\n\
Debian, like other Linux distributions, has a package manager; which handles the installation, updating and removal of software packages on the computer.  Debian's package manager is called apt.  In order to use some apt commands, the user must use the sudo command to elevate their privileges to those of a super-user, example: sudo apt install nvidia-driver, where nvidia-driver is the package you are trying to install.  \n\
\n\
There are some instructions for optional system configuration outlined here that you might find useful.\n\
\n\
SETTING STATIC IP\n\
\n\
Normally, your device's unique IP address is assigned dynamically, meaning it is not necessarily going to be the same everytime it connects to your network.  You can get a slightly more stable and performant connection by setting a static IP address, where your IP is the same everytime it connects to the network.  This will also aid in configuration of firewalls and servers, since you can now specify your computer's connection by its exact IP address.  Having a static IP can be useful for a multitude of reasons, one of them being the implementation of firewall rules.  The instructions for switching to a static IP on Linux in the GNOME desktop environment are as follows:\n\
\n\
1.     Click in the top-right corner of the screen where the power icon is.  Click the gear-shaped icon to access settings.  You can also find the settings application icon by pressing the super key (Windows key or command key on Mac) and either find it visually or search settings in the searchbar that pops up with the GNOME Activities window.  This Activities window can also be accessed by clicking Activities in the tope left of the screen.\n\
\n\
2.     If you are using Wi-Fi, click on Wi-Fi at the top of the scrollable menu on the left of the settings window.  If you are using ethernet, click on Network. \n\
\n\
3.     Click on the gear icon for the network connection you are using.  Take note of the IP you are currently assigned in the IPv4 Address section of this window, under the Details tab.  When you assign a static IP to your connection, you will make one that looks very similar to this one; save for the digits after the last decimal.  So, if your IPv4 is currently 172.178.1.19 then you can likely assign yourself anything from 192.168.1.0 to 192.168.1.255, as long as the address is not in use by another network device.\n\
\n\
4.     Click the IPv4 tab, and change the method from Automatic (DHCP) to Manual.\n\
\n\
5.     In the address field, enter the static IP address you want to use.  \n\
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
8.    Time to choose your DNS servers.  You can pick whichever one you like.  Cloudflare is popular with many, since it offers security features like DDoS protection, SSL encryption, and web application firewall.  You can specify 2 addresses, in case one is down (seperate them with a comma in the textfield).  To use Cloudflare as your DNS provider, you can use the addresses 1.1.1.1, 1.0.0.1. OpenDNS is another popular provider that offers its own security features and malware protection, much like Cloudflare.  To use OpenDNS, use 208.67.222.222, 208.67.220.220. \n\
\n\
9.  Save your new static IP address configuration with the Apply button.  Go to your network settings and switch the connection off, then on again.  Try to connect to a webpage.  If it works, you are good to go.  If not, you may have made an error in your IP configuration.  If you need to go back to an automatically assigned IP you can undo the static IP settings by simply switching your connection from Manual to Automatic (DHCP) again in the settings. \n\n\
TROUBLESHOOTING APT PACKAGE MANAGER ON DEBIAN\n\n\
Occasionally, when trying to install software with apt on Debian, you may encounter an error with the message: \"You have held broken packages.  \"This can be fixed much of the time by following up with the following 2 commands: \n\n  sudo apt --fix-broken install\n  sudo apt install <name of package, no angle brackets>\n\n";


void init_css_provider() {

  GtkCssProvider * provider = gtk_css_provider_new();
  GtkCssProvider * provider2 = gtk_css_provider_new();
  const char * filepath1 = "/usr/share/LPIH/css/style.css";
  const char * filepath2 = "style.css";
  gtk_css_provider_load_from_path(provider, filepath1);
  gtk_css_provider_load_from_path(provider2, filepath2);
  gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider2), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }

//Function to get the graphics card vendor of the user.
const char * getGraphicsCardVendor() {
  return (const char * ) glGetString(GL_VENDOR);
}

void get_cpu_vendor(char * vendor) {
  unsigned int eax, ebx, ecx, edx;

  // Call cpuid with input 0 to get vendor string
  __asm__("cpuid": "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx): "a"(0));

  // Copy the vendor string to the output buffer
  ((int * ) vendor)[0] = ebx;
  ((int * ) vendor)[1] = edx;
  ((int * ) vendor)[2] = ecx;
  vendor[12] = '\0';
}

//////////////////////////////////////////
// INFORMATIONAL WINDOW: DEBIAN /////////
// /////////////////////////////////////

static void on_deb_tips_window_destroy(GtkWidget * deb_info_window, gpointer user_data) {
  debian_tips_open = 0;
}

static void
debian_info_window(GtkWidget * widget,
  gpointer data) {

  if (debian_tips_open != 1) {

    GtkWidget * deb_info_window;
    GtkWidget * deb_info_box;
    GtkWidget * deb_info_button;

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
    gtk_widget_add_css_class(view, "deb_info_window_view");
    gtk_widget_set_opacity(view, 0.9);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD_CHAR);
    gtk_widget_add_css_class(view, "deb_info_view");
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    gtk_text_buffer_set_text(buffer, tips_spiel, -1);

    gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
    gtk_widget_set_can_focus(GTK_WIDGET(view), TRUE);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(view), 13);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(view), 13);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll_info_window), view);

    gtk_box_append(GTK_BOX(deb_info_box), scroll_info_window);

    g_signal_connect(deb_info_window, "destroy", G_CALLBACK(on_deb_tips_window_destroy), NULL);

        gtk_widget_set_visible(deb_info_window, TRUE);

  } else {
    g_print("Error: debian_tips window is already open.");
  }

  debian_tips_open = 1;

}


static void
debian_window(GtkWidget * widget,
  gpointer data) {

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
    ////////////////////////////////////////////////

    deb_flatpak_check = gtk_check_button_new_with_label("  Do you want to use flatpak applications?");
    gtk_box_append(GTK_BOX(deb_box), deb_flatpak_check);

    deb_microcode_check = gtk_check_button_new_with_label("  Install your processor's latest microcode?");
    gtk_box_append(GTK_BOX(deb_box), deb_microcode_check);

    deb_fonts_check = gtk_check_button_new_with_label("  Install restricted fonts compatibility for Microsoft products and multimedia compatibility packages?");
    gtk_box_append(GTK_BOX(deb_box), deb_fonts_check);
    ///////////////////////////////////////////////////////
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
    gtk_text_buffer_set_text(buffer, "  # Check the boxes according to your needs and run the resulting script in your terminal  \n  # to set up the desired functionality on your Debian system.  You may need to enable non-free  \n  # repositories by editing your '/etc/apt/sources.list' file if some of the proprietary packages  \n  # like Steam and GPU drivers don't install.  See 'tips' for details.  \n\n  sudo apt update; sudo apt upgrade;  \n  sudo apt install build-essential dkms linux-headers-$(uname -r); \n", -1);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
    gtk_widget_set_can_focus(GTK_WIDGET(view), TRUE);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll_window), view);
    gtk_box_append(GTK_BOX(deb_box), scroll_window);
    

    // DEBIAN INFO WINDOW //

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

  }
  debian_window_open = 1;
}

////////// DEBIAN gpu CHECKBOX ///////////////

static void deb_gpu_toggled(GtkWidget * widget, gpointer data) {

  gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(data);

  const gchar * debian_gpu_command;
  if (gpu_manufacturer == 1) {
    debian_gpu_command = "  sudo apt install nvidia-driver nvidia-driver-libs;\n";
  } else if (gpu_manufacturer == 2) {
    debian_gpu_command = "  sudo apt install firmware-linux firmware-linux-nonfree libdrm-amdgpu1 xserver-xorg-video-amdgpu;\n";
  } else if (gpu_manufacturer == 3) {
    debian_gpu_command = "  # Intel GPU drivers already installed. \n";

  }
  
  else if (gpu_manufacturer == 0) {
    debian_gpu_command = "  sudo apt install nvidia-driver nvidia-driver-libs;\n";

  }

  static GtkTextIter iter; // A static variable to store the iterator position
  if (state) {
    gtk_text_buffer_get_end_iter(buffer, & iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, & iter, debian_gpu_command, -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar * search_string = debian_gpu_command;

    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
}

//// DEBIAN STEAM CHECKBOX ///////

static void deb_steam_toggled(GtkWidget * widget, gpointer data) {

  gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state) {
    gtk_text_buffer_get_end_iter(buffer, & iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, & iter, "  sudo dpkg --add-architecture i386; sudo apt update; \n  sudo apt install steam-devices steam-installer; \n", -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar * search_string = "  sudo dpkg --add-architecture i386; sudo apt update; \n  sudo apt install steam-devices steam-installer; \n";

    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
}

//// DEBIAN GAME CHECKBOX ///////

static void deb_game_toggled(GtkWidget * widget, gpointer data) {

  gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state) {
    gtk_text_buffer_get_end_iter(buffer, & iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, & iter, "  sudo apt install nvidia-driver-libs:i386 mesa-vulkan-drivers libvulkan1;\n  sudo apt install vulkan-tools vulkan-validationlayers gamemode;  \n", -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar * search_string = "  sudo apt install nvidia-driver-libs:i386 mesa-vulkan-drivers libvulkan1;\n  sudo apt install vulkan-tools vulkan-validationlayers gamemode;  \n";

    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
}

////////// DEBIAN FLATPAK CHECKBOX ///////////////

static void deb_flatpak_toggled(GtkWidget * widget, gpointer data) {

  gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state) {
    gtk_text_buffer_get_end_iter(buffer, & iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, & iter, "  sudo apt install flatpak gnome-software-plugin-flatpak; \n  sudo flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo; \n", -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar * search_string = "  sudo apt install flatpak gnome-software-plugin-flatpak; \n  sudo flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo; \n";

    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
}

//// DEBIAN MICROCODE CHECKBOX ///////

static void deb_microcode_toggled(GtkWidget * widget, gpointer data) {

  gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position

  const gchar * debian_microcode_command;

  if (cpu_manufacturer == 2) {
    debian_microcode_command = "  sudo apt install amd64-microcode;\n";
  } else if (cpu_manufacturer == 3) {
    debian_microcode_command = "  sudo apt install intel-microcode;\n";
  } else {
    g_print("Something went wrong trying to get the cpu manufacturer.");
  }

  if (state) {
    gtk_text_buffer_get_end_iter(buffer, & iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, & iter, debian_microcode_command, -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar * search_string = debian_microcode_command;

    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
}

//// DEBIAN FONTS CHECKBOX ///////

static void deb_fonts_toggled(GtkWidget * widget, gpointer data) {

  gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state) {
    gtk_text_buffer_get_end_iter(buffer, & iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, & iter, "  sudo apt install libavcodec-extra;  \n  sudo apt install gstreamer1.0-libav gstreamer1.0-plugins-ugly gstreamer1.0-vaapi;  \n  sudo apt install fonts-crosextra-carlito fonts-crosextra-caladea;  \n", -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar * search_string = "  sudo apt install libavcodec-extra;  \n  sudo apt install gstreamer1.0-libav gstreamer1.0-plugins-ugly gstreamer1.0-vaapi;  \n  sudo apt install fonts-crosextra-carlito fonts-crosextra-caladea;  \n";

    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
}

////////// DEBIAN UFW CHECKBOX ///////////////

static void deb_ufw_toggled(GtkWidget * widget, gpointer data) {

  gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state) {
    gtk_text_buffer_get_end_iter(buffer, & iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, & iter, "  sudo apt install ufw; sudo ufw enable; \n", -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar * search_string = "  sudo apt install ufw; sudo ufw enable; \n";

    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
}

//// DEBIAN TLP CHECKBOX ///////

static void deb_tlp_toggled(GtkWidget * widget, gpointer data) {

  gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state) {
    gtk_text_buffer_get_end_iter(buffer, & iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, & iter, "  sudo apt install tlp; \n", -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar * search_string = "  sudo apt install tlp; \n";

    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
}

//// DEBIAN VLC CHECKBOX ///////

static void deb_vlc_toggled(GtkWidget * widget, gpointer data) {

  gboolean state = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state) {
    gtk_text_buffer_get_end_iter(buffer, & iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, & iter, "  sudo apt install vlc; \n", -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar * search_string = "  sudo apt install vlc; \n";

    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
}

static void on_deb_window_destroy(GtkWidget * deb_window, gpointer user_data) {
  debian_window_open = 0;
}

/////////////////////////////////////////////////////
// INFORMATIONAL WINDOW: FEDORA /////////////////////
// //////////////////////////////////////////////////

static void
fedora_info_window(GtkWidget * widget,
  gpointer data) {

  if (fedora_tips_open != 1) {

    GtkWidget * fed_info_window;
    GtkWidget * fed_info_box;
    GtkWidget * fed_info_button;

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
    gtk_widget_add_css_class(view, "fed_info_window_view");
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    gtk_text_buffer_set_text(buffer, tips_spiel, -1);

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

static void on_fed_tips_window_destroy(GtkWidget * fed_info_window, gpointer user_data) {
  fedora_tips_open = 0;
}

//////////////////////////////////////////
//                                     || //
//      FEDORA WINDOW AND FUNCTIONS   |  |  ||
//                                     || //
//////////////////////////////////////////

static void
fedora_window(GtkWidget * widget,
  gpointer data) {

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

    // CHECKBOXES //////////

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
    gtk_text_buffer_set_text(buffer, "  # Check the boxes according to your needs and run the resulting script in your terminal  \n  # to set up the desired functionality on your Fedora system.  \n\n  sudo dnf update; sudo dnf upgrade; \n", -1);
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

  }
  fedora_window_open = 1;
}

//// FEDORA repo CHECKBOX ///////

static void fed_repo_toggled(GtkWidget * widget, gpointer data) {

  gboolean state_f = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state_f) {
    gtk_text_buffer_get_end_iter(buffer, & iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, & iter, "  sudo dnf install https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm;  \n  sudo dnf install https://download1.rpmfusion.org/nonfree/fedora/rpmfusion-nonfree-release-$(rpm -E %fedora).noarch.rpm;  \n  sudo dnf update; \n", -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar * search_string = "  sudo dnf install https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm;  \n  sudo dnf install https://download1.rpmfusion.org/nonfree/fedora/rpmfusion-nonfree-release-$(rpm -E %fedora).noarch.rpm;  \n  sudo dnf update; \n";

    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
}

////////// FEDORA gpu CHECKBOX ///////////////

static void fed_gpu_toggled(GtkWidget * widget, gpointer data) {

  gboolean state_f = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(data);

  const gchar * fedora_gpu_command;
  if (gpu_manufacturer == 1) {
    fedora_gpu_command = "  sudo dnf install akmod-nvidia xorg-x11-drv-nvidia-cuda \n";
  } else if (gpu_manufacturer == 2) {
    fedora_gpu_command = "  sudo dnf install xorg-x11-drv-amdgpu vulkan-tools mesa-vulkan-drivers \n";
  } else if (gpu_manufacturer == 3) {
    fedora_gpu_command = "  # Intel GPU drivers already installed. \n";

  }

  static GtkTextIter iter; // A static variable to store the iterator position
  if (state_f) {
    gtk_text_buffer_get_end_iter(buffer, & iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, & iter, fedora_gpu_command, -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar * search_string = fedora_gpu_command;

    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
}

//// FEDORA STEAM CHECKBOX ///////

static void fed_steam_toggled(GtkWidget * widget, gpointer data) {

  gboolean state_f = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state_f) {
    gtk_text_buffer_get_end_iter(buffer, & iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, & iter, "  sudo dnf install steam;\n", -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar * search_string = "  sudo dnf install steam;\n";

    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
}

//// FEDORA GAME CHECKBOX ///////

static void fed_dnf_toggled(GtkWidget * widget, gpointer data) {

  gboolean state_f = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state_f) {
    gtk_text_buffer_get_end_iter(buffer, & iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, & iter, "  sudo sh -c 'if test -f /etc/dnf/dnf.conf; then echo \"max_parallel_downloads=20\" >> /etc/dnf/dnf.conf; fi'\n", -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar * search_string = "  sudo sh -c 'if test -f /etc/dnf/dnf.conf; then echo \"max_parallel_downloads=20\" >> /etc/dnf/dnf.conf; fi'\n";

    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
}

////////// FEDORA FLATPAK CHECKBOX ///////////////

static void fed_flatpak_toggled(GtkWidget * widget, gpointer data) {

  gboolean state_f = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state_f) {
    gtk_text_buffer_get_end_iter(buffer, & iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, & iter, "  flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo; \n", -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar * search_string = "  flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo; \n";

    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
}

//// FEDORA customization CHECKBOX ///////

static void fed_customization_toggled(GtkWidget * widget, gpointer data) {

  gboolean state_f = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state_f) {
    gtk_text_buffer_get_end_iter(buffer, & iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, & iter, "  sudo dnf install gnome-tweaks gnome-extensions-app; \n", -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar * search_string = "  sudo dnf install gnome-tweaks gnome-extensions-app; \n";
    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
}

////////// FEDORA CODECS CHECKBOX ///////////////

static void fed_codecs_toggled(GtkWidget * widget, gpointer data) {

  gboolean state_f = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state_f) {
    gtk_text_buffer_get_end_iter(buffer, & iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, & iter, "  sudo dnf install ffmpeg --allowerasing &&  \n  sudo dnf install gstreamer1-plugins-{bad-\\*,good-\\*,base} gstreamer1-plugin-openh264 gstreamer1-libav --exclude=gstreamer1-plugins-bad-free-devel &&  \n  sudo dnf install lame\\* --exclude=lame-devel && sudo dnf group upgrade --with-optional Multimedia \n", -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar * search_string = "  sudo dnf install ffmpeg --allowerasing &&  \n  sudo dnf install gstreamer1-plugins-{bad-\\*,good-\\*,base} gstreamer1-plugin-openh264 gstreamer1-libav --exclude=gstreamer1-plugins-bad-free-devel &&  \n  sudo dnf install lame\\* --exclude=lame-devel && sudo dnf group upgrade --with-optional Multimedia \n";

    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
}

//// FEDORA TLP CHECKBOX ///////

static void fed_tlp_toggled(GtkWidget * widget, gpointer data) {

  gboolean state_f = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state_f) {
    gtk_text_buffer_get_end_iter(buffer, & iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, & iter, "  sudo dnf install tlp; \n", -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar * search_string = "  sudo dnf install tlp; \n";

    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
}

//// FEDORA VLC CHECKBOX ///////

static void fed_vlc_toggled(GtkWidget * widget, gpointer data) {

  gboolean state_f = gtk_check_button_get_active(GTK_CHECK_BUTTON(widget));
  GtkTextBuffer * buffer = GTK_TEXT_BUFFER(data);
  static GtkTextIter iter; // A static variable to store the iterator position
  if (state_f) {
    gtk_text_buffer_get_end_iter(buffer, & iter); // Store the end iterator position
    gtk_text_buffer_insert(buffer, & iter, "  sudo dnf install vlc; \n", -1);
  } else {
    GtkTextIter start, end, match_start, match_end;
    const gchar * search_string = "  sudo dnf install vlc; \n";

    gtk_text_buffer_get_start_iter(buffer, & start);
    gtk_text_buffer_get_end_iter(buffer, & end);

    if (gtk_text_iter_forward_search( & start, search_string, 0, & match_start, & match_end, NULL)) {
      gtk_text_buffer_delete(buffer, & match_start, & match_end);
    }
  }
}

static void on_fed_window_destroy(GtkWidget * fed_window, gpointer user_data) {
  fedora_window_open = 0;
}

////// INITIAL WINDOW ////////////////////////////////////////////////////

static void activate(GtkApplication * app,
  gpointer user_data) {

//For singleton
  if (lpih_instance_running != 1) {

    lpih_instance_running = 1;

    GtkWidget * window;
    GtkWidget * grid;
    GtkWidget * deb_button;
    GtkWidget * fed_button;
    GtkWidget * quit_button;

    /* create a new window, and set its title */
    window = gtk_application_window_new(app);
    gtk_widget_add_css_class(window, "main_window");
    gtk_window_set_title(GTK_WINDOW(window), "Linux Post-install Helper For Debian and Fedora");
    gtk_widget_set_size_request(window, 444, 444);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    /////////////////////////////////////////////////////////////////////////

    grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE); // Make rows of equal height
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE); // Make columns of equal width
    gtk_grid_set_row_spacing(GTK_GRID(grid), 50); // Add spacing between rows
    gtk_grid_set_column_spacing(GTK_GRID(grid), 100); // Add spacing between columns
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    gtk_window_set_child(GTK_WINDOW(window), grid);

    deb_button = gtk_button_new_with_label("DEBIAN");
    gtk_widget_add_css_class(deb_button, "deb");
    gtk_widget_set_size_request(deb_button, 100, 50);
    g_signal_connect(deb_button, "clicked", G_CALLBACK(debian_window), NULL);

    gtk_grid_attach(GTK_GRID(grid), deb_button, 0, 0, 1, 1);

    fed_button = gtk_button_new_with_label("FEDORA");
    gtk_widget_add_css_class(fed_button, "fed");
    gtk_widget_set_size_request(fed_button, 100, 50);
    g_signal_connect(fed_button, "clicked", G_CALLBACK(fedora_window), NULL);

    gtk_grid_attach(GTK_GRID(grid), fed_button, 1, 0, 1, 1);

    quit_button = gtk_button_new_with_label("QUIT");
    gtk_widget_add_css_class(quit_button, "quit");
    gtk_widget_set_name(quit_button, "quit");
    gtk_widget_set_size_request(quit_button, 100, 50);
    g_signal_connect_swapped(quit_button, "clicked", G_CALLBACK(gtk_window_destroy), window);

    gtk_grid_attach(GTK_GRID(grid), quit_button, 0, 1, 2, 1);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
    init_css_provider();
    gtk_widget_set_can_focus(GTK_WIDGET(window), TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(grid), TRUE);
    
    
    gtk_widget_set_visible(window, TRUE);

    // Automatically establishing the user's GPU vendor on init of the program.       
    const char * gpu_vendor = getGraphicsCardVendor();

    if (strstr(gpu_vendor, "NVIDIA") != NULL) {
      gpu_manufacturer = 1;
    } else if (strstr(gpu_vendor, "AMD") != NULL) {
      gpu_manufacturer = 2;
    } else if (strstr(gpu_vendor, "Intel") != NULL) {
      gpu_manufacturer = 3;
    } else {
      g_print("The GPU vendor could not be determined for this ");
      gpu_manufacturer = 0;
    }

    char vendor[13];
    get_cpu_vendor(vendor);
    
    if (strstr(vendor, "AMD") != NULL) {
      cpu_manufacturer = 2;
    } else if (strstr(vendor, "Intel") != NULL) {
      cpu_manufacturer = 3;
    } else {
      g_print("The CPU vendor could not be determined for this computer.");
      cpu_manufacturer = 0;
    }

    g_print("The GPU manufacturer for this machine is %d, %s. \n", gpu_manufacturer, gpu_vendor);
    g_print("The CPU manufacturer for this machine is %d, %s. \n", cpu_manufacturer, vendor);

  } else {
    g_print("Error: instance of LPIH is already running!");
  }

}

static void on_quit(GtkApplication * app, gpointer user_data) {
  g_print("Shutting down LPIH now.\n");
  lpih_instance_running = 0;

}

int
main(int argc,
  char ** argv) {

  GtkApplication * app;
  int status;
  app = gtk_application_new("petc0016.project.lpisg", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  g_signal_connect(app, "shutdown", G_CALLBACK(on_quit), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
