#ifndef UTILITY_H
#define UTILITY_H

 // Function to get the CPU vendor strings. // // // // // // // // // // // // // // //
 void get_cpu_vendor(char * vendor);
 gboolean init_css_provider();
 gboolean check_box_state(GtkWidget * checkbox, gpointer data);
 const char * getGraphicsCardVendor(void);
 
#endif // UTILITY_H
