
#ifndef LPIH_H
#define LPIH_H
#endif

//\\//\\//\\//\\//\\// DEBIAN-WINDOW FUNCTIONS //\\//\\//\\//\\//\\//\\//\\//|||||||||
 int deb_gpu_toggled(GtkWidget * widget, gpointer data);
 int deb_steam_toggled(GtkWidget * widget, gpointer data);
 int deb_game_toggled(GtkWidget * widget, gpointer data);
 int deb_flatpak_toggled(GtkWidget * widget, gpointer data);
 int deb_microcode_toggled(GtkWidget * widget, gpointer data);
 int deb_fonts_toggled(GtkWidget * widget, gpointer data);
 int deb_ufw_toggled(GtkWidget * widget, gpointer data);
 int deb_tlp_toggled(GtkWidget * widget, gpointer data);
 int deb_vlc_toggled(GtkWidget * widget, gpointer data);
 int on_deb_window_destroy(GtkWidget * deb_window, gpointer user_data);
 int on_deb_tips_window_destroy(GtkWidget * deb_info_window, gpointer user_data);

 // Function to get the CPU vendor strings. // // // // // // // // // // // // // // //
 int get_cpu_vendor(char * vendor);
 int init_css_provider();


 //\\//\\//\\//\\//\\// FEDORA-WINDOW FUNCTIONS //\\//\\//\\//\\//\\//\\//\\//||||||||||
 int fed_gpu_toggled(GtkWidget * widget, gpointer data);
 int fed_steam_toggled(GtkWidget * widget, gpointer data);
 int fed_dnf_toggled(GtkWidget * widget, gpointer data);
 int fed_flatpak_toggled(GtkWidget * widget, gpointer data);
 int fed_repo_toggled(GtkWidget * widget, gpointer data);
 int fed_customization_toggled(GtkWidget * widget, gpointer data);
 int fed_codecs_toggled(GtkWidget * widget, gpointer data);
 int fed_tlp_toggled(GtkWidget * widget, gpointer data);
 int fed_vlc_toggled(GtkWidget * widget, gpointer data);
 int on_fed_window_destroy(GtkWidget * fed_window, gpointer user_data);
 int on_fed_tips_window_destroy(GtkWidget * fed_info_window, gpointer user_data);

 // Variables that tell the program what kind of CPU and GPU the user has.

 // 1 = AMD, 2 = Intel, 3 = Nvidia.
 int cpu_manufacturer = 0;
 int gpu_manufacturer = 0;

 // For keeping track of single-instance windows.
 int lpih_instance_running = 0;
 int debian_window_open = 0;
 int debian_tips_open = 0;
 int fedora_window_open = 0;
 int fedora_tips_open = 0;
 const gchar* fedora_gpu_command;
 const gchar* debian_gpu_command;
 const gchar* debian_microcode_command;

 const gchar * tips_spiel = "  Debian GNU/Linux is one of the oldest and most popular Linux distributions, released in 1993.  It is known for its stability and reliability: which is why it is often used for servers and other critical systems and serves as the base of many other distros, like Ubuntu and Linux Mint: Debian Edition (LMDE).\n\
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

const gchar* DEBIAN_FLATPACK = "  sudo apt install flatpak gnome-software-plugin-flatpak; \n  sudo flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo; \n";

const gchar* DEBIAN_MULTIMEDIA = "  sudo apt install libavcodec-extra;  \n  sudo apt install gstreamer1.0-libav gstreamer1.0-plugins-ugly gstreamer1.0-vaapi;  \n  sudo apt install fonts-crosextra-carlito fonts-crosextra-caladea;  \n";

const gchar* DEBIAN_UFW = "  sudo apt install ufw; sudo ufw enable; \n";
const gchar* DEBIAN_TLP = "  sudo apt install tlp; \n";
const gchar* DEBIAN_VLC = "  sudo apt install vlc; \n";










const gchar* FEDORA_MULTIMEDIA_COMMAND = "  sudo dnf install ffmpeg --allowerasing &&  \n  sudo dnf install gstreamer1-plugins-{bad-\\*,good-\\*,base} gstreamer1-plugin-openh264 gstreamer1-libav --exclude=gstreamer1-plugins-bad-free-devel &&  \n  sudo dnf install lame\\* --exclude=lame-devel && sudo dnf group upgrade --with-optional Multimedia \n";


