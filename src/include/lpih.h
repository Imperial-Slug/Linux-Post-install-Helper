
#ifndef LPIH_H
#define LPIH_H


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
 gboolean on_deb_window_destroy(void);
 int on_deb_tips_window_destroy();

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
 void on_fed_window_destroy();
 void on_fed_tips_window_destroy();

 // Variables that tell the program what kind of CPU and GPU the user has.

 
 const gchar* fedora_gpu_command;
 const gchar* debian_gpu_command;
 const gchar* debian_microcode_command;

#endif
