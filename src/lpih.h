
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
 
 
