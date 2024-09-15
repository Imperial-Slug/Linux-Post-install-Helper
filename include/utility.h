#ifndef UTILITY_H
#define UTILITY_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include "lpih-main.h"
#include "lpih-window.h"
#include "utility.h"

 // Function to get the CPU vendor strings. // // // // // // // // // // // // // // //
 void get_cpu_vendor(char * vendor);
 gboolean init_css_provider();
 gboolean check_box_state(GtkWidget * checkbox, gpointer data);
 const char * getGraphicsCardVendor(void);




#endif // UTILITY_H
