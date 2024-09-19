#ifndef UTILITY_H
#define UTILITY_H

enum vendor_name {
  AMD = 1, Intel = 2, Nvidia = 3, Unknown = 4
};

 // Function to get the CPU vendor strings. // // // // // // // // // // // // // // //
 void get_cpu_vendor(char * vendor);
 gboolean init_css_provider();
 const char * getGraphicsCardVendor(void);
 void set_gpu_vendor(void);
 void set_cpu_vendor(void);
 
#endif // UTILITY_H
