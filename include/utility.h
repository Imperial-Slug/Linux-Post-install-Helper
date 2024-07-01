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
 int get_cpu_vendor(char * vendor);
 int init_css_provider();
 gboolean check_box_state(const gchar * command_string, GtkWidget* widget, gpointer data);
 const char * getGraphicsCardVendor(void);
 int get_cpu_vendor(char* vendor);

#endif // UTILITY_H
