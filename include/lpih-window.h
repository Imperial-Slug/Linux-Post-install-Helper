#ifndef LPIH_WINDOW_H
#define LPIH_WINDOW_H

gboolean on_lpih_window_destroy(GtkWidget * widget, gpointer data);
void lpih_window(GtkWidget * widget, gpointer window_data);
gboolean on_info_window_destroy(GtkWidget* widget, gpointer data);

enum Distro { DEBIAN = 1, FEDORA = 2 };
void make_info_window(GtkWidget *widget, gpointer data);

enum CheckboxNumber { 
CHECKBOX_1 = 1, 
CHECKBOX_2 = 2, 
CHECKBOX_3 = 3, 
CHECKBOX_4 = 4, 
CHECKBOX_5 = 5, 
CHECKBOX_6 = 6, 
CHECKBOX_7 = 7, 
CHECKBOX_8 = 8, 
CHECKBOX_9 = 9, 
CHECKBOX_10 = 10, 
CHECKBOX_11 = 11
  };

void create_notebook_tab(GtkWidget* notebook, gchar* view_css_label, gchar* tab_label, gchar* tab_css_label, gchar* res_path1, gchar* res_path2);

// GLOBAL VARIABLES // // //
gboolean check_box_state(GtkWidget *checkbox, gpointer data);
extern
const gchar * debian_gpu_command;
extern
const gchar * DEBIAN_OPENER;

 extern 
 const gchar* fedora_gpu_command;

 extern
 const gchar* FEDORA_OPENER;
 
 typedef struct {

  enum Distro distro_id;
  gchar * info_window_name;
  gchar * info_window_title;
  gchar * notebook_css_name;
  gboolean info_open_flag;

}
InfoWindowData;
 
 typedef struct {

enum Distro distro_id;
gboolean window_open_flag; 
gchar * css_label;
gchar * window_title;
gchar * view_css_class;
const gchar * opener;
gchar * info_button_css_class;
gchar * checkbox1_title;
gchar * checkbox2_title;
gchar * checkbox3_title;
gchar * checkbox4_title;
gchar * checkbox5_title;
gchar * checkbox6_title;
gchar * checkbox7_title;
gchar * checkbox8_title;
gchar * checkbox9_title;
gchar * checkbox10_title;
gchar * checkbox11_title;

GtkWidget * checkbox1;
GtkWidget * checkbox2;
GtkWidget * checkbox3;
GtkWidget * checkbox4;
GtkWidget * checkbox5;
GtkWidget * checkbox6;
GtkWidget * checkbox7;
GtkWidget * checkbox8;
GtkWidget * checkbox9;  
GtkWidget * checkbox10;
GtkWidget * checkbox11;  
} LpihWindowData;

extern gboolean debian_window_open;
extern gboolean fedora_window_open;


extern const gchar *debian_microcode_command;
extern const gchar *debian_gpu_command;
extern const gchar *fedora_gpu_command;
gboolean check_box_state(GtkWidget * checkbox, gpointer data);


typedef struct {
GtkTextBuffer * shared_buffer;
enum Distro distro;

} CheckboxData;


typedef struct {
gboolean window_open_flag;
enum Distro distro;

 } MainWindowData;
 
 
extern MainWindowData *deb_window_data;
extern MainWindowData *fed_window_data;

GtkWidget * make_main_window(GtkApplication *app);
#endif // LPIH_WINDOW_H
