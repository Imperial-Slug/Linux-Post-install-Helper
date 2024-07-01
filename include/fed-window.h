#ifndef FED_WINDOW_H
#define FED_WINDOW_H



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
 void fedora_window(void);
 void on_fed_tips_window_destroy();

// GLOBAL VARIABLES

 extern const gchar* fedora_gpu_command;
 extern const gchar* fedora_tips_spiel;
 extern const gchar* FEDORA_OPENER;
 extern const gchar* FEDORA_REP;
 extern const gchar* FEDORA_STEAM;
 extern const gchar* FEDORA_DNF;
 extern const gchar* FEDORA_FLATPAK;
 extern const gchar* FEDORA_CUST;
 extern const gchar* FEDORA_MULTIMEDIA;
 extern const gchar* FEDORA_TLP;
 extern const gchar* FEDORA_VLC;

#endif // FED_WINDOW_H
