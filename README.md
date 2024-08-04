
Who is this program for?

It is specifically targeted at new Debian and Fedora Linux users who may not know where to start with getting their systems running the bare necesseties for their use case.  For example, this program will detect whether your GPU is an AMD, Nvidia or Intel and give you the command to install the proper drivers for it.  Simply copy and paste the checkbox-generated Linux commands into your terminal after checking the corresponding checkbox.  

Experienced users may find it useful as a "quick and easy" way to get all of the basics installed as well.

        ___________________DETAILS___________________
        

        MAIN GOAL

The entire goal of the project is to make it easier for new Linux users to get their freshly installed systems going with key software and functionality installed that might otherwise take a while to figure out for new users. 

         INSTALLING THE .deb or .rpm

Linux-Post-install-Helper: to install, download the zip (or just the Build_Directory_Template) and within Build_Directory_Template you will find the .deb and .rpm for installing on Debian or Fedora respectively, with:

        sudo dpkg -i LPIH.deb; # (For Debian)
        # or
        sudo rpm -i LPIH*.rpm;  # (For Fedora)

**** IN DEVELOPMENT ****

Started as my project for a programming language research project at my college in the final semester, but is intended to be free and open source, with many useful new features planned to be added.

PURPOSE: The general purpose of this program will be to make Linux desktop systems easier to configure
after installation for non-technical or non computer-oriented people and for people who can't be bothered with looking for information
online.

THE PROGRAM: A PLACE TO START FOR NEW DEBIAN OR FEDORA LINUX USERS.  The program is written in C (using GTK4 via the GOBject framework).  It aids in the
post-installation activities involved with setting up a GNU/Linux system specifically for GNOME
desktop-environment users on Debian- and Fedora-based desktop/laptop systems.  Support for desktop-environment-agnostic functionality is one of the features that will be added in the future. 

This project is far from finished and will eventually support other distributions, as well as offer more advanced features over the course of time.

Currently, the project is in a perfectly useful form, but could use some quality-of-life improvements for the user, like more error handling in the bash script generated and especially populating the TIPS windows with more tips and information for new/curious GNU/Linux desktop users. The "TIPS" sections will be more functional and useful in forthcoming releases.

Debian users who want to compile the code themselves will need to make sure the following dependencies are installed:

sudo apt install libgtk-4-1 libgtk-4-dev clang pkg-config

Similarly, for Fedora users:

sudo dnf install gtk4 gtk4-devel mesa-libGL-devel clang pkg-config



