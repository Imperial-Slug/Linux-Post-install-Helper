#ifndef UTILITY_H
#define UTILITY_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include "lpih-main.h"
#include "fed-window.h"
#include "utility.h"

 // Function to get the CPU vendor strings. // // // // // // // // // // // // // // //
 gboolean get_cpu_vendor(char * vendor);
 gboolean init_css_provider();
 gboolean check_box_state(const gchar * command_string, GtkWidget* widget, gpointer data);
 char * getGraphicsCardVendor(void);
void create_notebook_tab(GtkWidget* notebook, gchar* view_css_label, gchar* tab_label, gchar* tab_css_label, gchar* res_path1, gchar* res_path2);

GtkWidget * make_info_window(gchar * info_window_name, gchar * info_window_title, gchar * notebook_css_name, int distro, gboolean tips_open );

#endif // UTILITY_H
