#ifndef DEB_WINDOW_H
#define DEB_WINDOW_H

//\\//\\//\\//\\//\\// DEBIAN-WINDOW FUNCTIONS //\\//\\//\\//\\//\\//\\//\\//|||||||||
 gboolean deb_gpu_toggled(GtkWidget * widget, gpointer data);
 gboolean deb_steam_toggled(GtkWidget * widget, gpointer data);
 gboolean deb_game_toggled(GtkWidget * widget, gpointer data);
 gboolean deb_flatpak_toggled(GtkWidget * widget, gpointer data);
 gboolean deb_microcode_toggled(GtkWidget * widget, gpointer data);
 gboolean deb_fonts_toggled(GtkWidget * widget, gpointer data);
 gboolean deb_ufw_toggled(GtkWidget * widget, gpointer data);
 gboolean deb_tlp_toggled(GtkWidget * widget, gpointer data);
 gboolean deb_vlc_toggled(GtkWidget * widget, gpointer data);
 gboolean on_deb_window_destroy(void);
 void debian_window(void);
 int on_deb_tips_window_destroy(void);
 
 // GLOBAL VARIABLES
 
 extern const gchar* debian_gpu_command;
 extern const gchar* debian_microcode_command;
 extern const gchar* debian_tips_spiel;
 extern const gchar* DEBIAN_OPENER;
 extern const gchar* DEBIAN_STEAM;
 extern const gchar* DEBIAN_GAMING;
 extern const gchar* DEBIAN_FLATPAK;
 extern const gchar* DEBIAN_MULTIMEDIA;
 extern const gchar* DEBIAN_UFW;
 extern const gchar* DEBIAN_TLP;
 extern const gchar* DEBIAN_VLC;
 



#endif // DEB_WINDOW_H
