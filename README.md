**** IN DEVELOPMENT ****

This project, while mostly a learning catalyst/hobby for myself, is also meant to be useful to others. Namely, Debian and Fedora Linux desktop users who are unsure of what packages they need to install to acheive a desired basic functionality.

Who is this program for?

It is specifically targeted at new Debian and Fedora Linux users who may not know where to start with getting their systems running the bare necesseties for their use case.  For example, this program will detect whether your GPU is an AMD, Nvidia or Intel and give you the command to install the proper drivers for it.  Simply copy and paste the checkbox-generated Linux commands into your terminal after checking the corresponding checkbox.

Experienced users may find it useful as a "quick and easy" way to get all of the basics installed following a fresh installation of Debian or Fedora.

This project is far from finished and will eventually support other distributions, as well as offer more advanced features.


DEPENDENCIES

Debian users who want to compile the code themselves will need to make sure the following dependencies are installed:

sudo apt install libgtk-4-1 libgtk-4-dev clang pkg-config

Similarly, for Fedora users:

sudo dnf install gtk4 gtk4-devel mesa-libGL-devel clang pkg-config


INSTALLING VIA THE DEB AND RPM PACKAGES (RECOMMENDED FOR NEW OR NON-TECHNICAL USERS)

There are .deb and .rpm packages provided under "releases." .deb is for Debian systems, and .rpm is for Fedora.

To install them:

        sudo dpkg -i LPIH.deb; # (For Debian)
        # or
        sudo rpm -i LPIH*.rpm;  # (For Fedora)
        
        
COMPILING FROM SOURCE
        
Some may find it desirable to compile their own binary in the tarball so the files can be swiftly disposed of afterwards.  The program is organized so it can be run from the src directory in the source-code tarball OR by being installed via the .deb or .rpm packages.  

Simply download the source code tarball, navigate to Linux-Post-install-helper/src, ensure your dependencies are installed (read above) and run "make" in that directory to produce a compiled executable which you can run by running "./lpih" in your terminal in the src directory, or right-click the executable and select "run as program."  

You may need to add the "execute" permission to the binary to do it this way. In that case:

chmod +x lpih

Or, in the GNOME desktop environment, you can right-click on the executable and toggle on the "runnable as program" option in the GUI to turn on the "execute" permission.




