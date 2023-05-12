Linux-Post-install-Helper: to install, download the zip (or just the Build_Directory_Template) and within Build_Directory_Template you will find the .deb and /rpm for installing on Debian or Fedora respectively, with:

sudo dpkg -i LPIH.deb; # (For Debian)
# or
sudo rpm -i LPIH*.rpm;  # (For Fedora)

**** IN DEVELOPMENT ****

Started as my project for a programming language research project at my college in the final semester, but is intended to be free and open source; with many useful new features planned to be added.

PURPOSE: The general purpose of this program will be to make Linux desktop systems easier to configure
after installation for non-tech-oriented people; or people who can't be bothered with looking for information
online. In addition to bash script generation, the program will visually guide the user through common post-
install hurdles that new users run into.

THE PROGRAM: The program will be an application in C (using GTK4 via the GOBject framework) that aids in the
post-installation activities involved with setting up a GNU/Linux desktop system specifically for GNOME
desktop-environment users on Debian- and Fedora-based desktop/laptop systems; though users of other desktop environments will still find they may benefit from the application.  Support for DE-agnostic functionality is one of the features that will be added. There is a slight focus
on gamers new to Linux who use Steam, since this is a huge hurdle in increasing Linux desktop usage.

The program will be for Debian- and Fedora-based Linux distros running the standard GNOME desktop
environment. The goal is to teach/inform as well as help the user with post-install activities.

This project is far from finished and will eventually support other distributions, as well as offer more and more advanced features over the course of time.

Currently, the project is in a perfectly useful form, but could use some quality-of-life improvements for the user, like more error handling in the bash script generated and especially populating the TIPS windows with more tips and information for new/curious GNU/Linux desktop users.

Again, the entire goal of the project is to make it easier for new Linux users to get their freshly installed systems going with key software and functionality installed that might otherwise take a while to figure out for new users.  

A big example of a problem solved for new users in this program is the GPU drivers.  It is quite well known in the Linux community that certain brands of GPU can confuse users, owing to the GPU drivers not coming installed by default in many GNU/Linux distributions.  Linux Post-install Helper automatically detects the users's GPU and CPU make and adjusts the generated Linux/bash commands to suit it.  

Additionally, the commands to install Steam are generated, along with the necessary command to enable 32-bit architecture for Steam to work properly.  

This program is mostly targeted at people new to Linux and/or people who don't want to spend the time searching for the proper commands to configure their Linux system and get past some common hurdles to new Linux users.

If you want to contribute: feel free! That's what the build directory is for.  It has an automated build script to run within the template build directory, intended to act as the directory where you build the new packages every time you want to test a new build of the project that you've wrought.  Go ahead and improve it too. It's open source and free under the GNU GENERAL PUBLIC LICENSE.

