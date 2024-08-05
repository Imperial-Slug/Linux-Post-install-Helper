You just have to run "make" in this directory, if you want.      :)

Make sure you have the GTK development libraries installed for your distro.

If it doesn't compile, here are the packages you might be missing by distro type, as per the GTK4 documentation: https://www.gtk.org/docs/installations/linux:

 Debian/Ubuntu:	libgtk-4-1 libgtk-4-dev	gtk-4-examples  <--- (GTK4 examples optional)
 Fedora:	gtk4	gtk4-devel   	mesa-libGL-devel        <--- (for -lGL opengl C headers)
 Arch:	gtk4

Use this execution option for extra debugging from the terminal: 

G_MESSAGES_DEBUG=all ./lpih





