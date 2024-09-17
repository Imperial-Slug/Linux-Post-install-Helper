/* lpih-main.c
 *
 * Copyright 2023 Samuel Petch
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

////////////////////////////////////////
////        L I B R A R I E S   ////////
////////////////////////////////////////
#include <gtk/gtk.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "lpih-window.h"

#include "utility.h"

#include "lpih-main.h"

//////////////////////////////
//////////////////////////////

const gchar * DEBIAN_OPENER = "  # Check the boxes according to your needs and run the resulting script in your terminal  \n  # to set up the desired functionality on your Debian system.  You may need to enable non-free  \n  # repositories by editing your '/etc/apt/sources.list' file if some of the proprietary packages  \n  # like Steam and GPU drivers don't install.  See 'info' for details.  \n\n  sudo apt update; sudo apt upgrade;  \n  sudo apt install build-essential dkms linux-headers-$(uname -r); \n";

// For keeping track of single-instance lpih_window
gboolean lpih_instance_running = FALSE;

const gchar * FEDORA_OPENER = "  # Check the boxes according to your needs and run the resulting script in your terminal  \n  # to set up the desired functionality on your Fedora system.  \n\n  sudo dnf update;  \n";
//

////// INITIAL WINDOW ////////////////////////////////////////////////////
// Creates the initial window where the distro is selected.
void activate(GtkApplication * app) {

  if (lpih_instance_running != TRUE) {

    lpih_instance_running = TRUE;

      GtkWidget *window = make_main_window(app);
      gtk_window_present(GTK_WINDOW(window));

      set_gpu_vendor();
      set_cpu_vendor();
   
  } else {
    g_print("Error: instance of LPIH is already running!\n");
  }
}

void on_quit() {
  g_print("Exiting LPIH now.\n");
  g_print("*************************************\n");
  lpih_instance_running = FALSE;
}

gboolean main(int argc,
  char ** argv) {

  GtkApplication * app;
  gboolean status;
  app = gtk_application_new("imperialslug.gtkproject.lpih", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  g_signal_connect(app, "shutdown", G_CALLBACK(on_quit), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}

///////////// END OF FILE /////////////
