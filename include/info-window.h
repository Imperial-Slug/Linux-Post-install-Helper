#ifndef INFO_WINDOW_H
#define INFO_WINDOW_H

extern gboolean debian_info_open;
extern gboolean fedora_info_open;
gboolean init_info_gui(gpointer data);
enum Distro check_distro_from_window_data(gpointer data);
 
#endif // INFO_WINDOW_H
