# Linux-Post-install-Helper
Project for Programming Language Research Project at Algonquin College, final semester.

PURPOSE: The general purpose of this program will be to make Linux desktop systems easier to configure
after installation for non-tech-oriented people; or people who can't be bothered with looking for information
online. In addition to bash script generation, the program will visually guide the user through common post-
install hurdles that new users run into.

THE PROGRAM: The program will be an application in C (using GTK4 via the GOBject framework) that aids in the
post-installation activities involved with setting up a GNU/Linux desktop system specifically for GNOME
desktop-environment users on Debian- and Fedora-based desktop/laptop systems. There will be a slight focus
on gamers new to Linux who use Steam, since this is a huge hurdle in increasing Linux desktop marketshare.

DETAILS/FLOW: The program, following installation, will first be used for guiding the user through some
basic optional setup, like network options and aesthetics-related actvities. More integral things like graphics
drivers and Steam integration will be handled after these initial setup prompts.
Once the user makes it through all the GUI prompts, the program will then use the user input from the
GUI/program written in C to generate, assemble and execute a set of bash scripts for adding any software,
repositories or settings that don't come pre-installed on the Linux distro that the user needs. The goal is to
make it less scary and troublesome getting all of a user's "stuff" working properly on a newly installed system,
since this is a major point of frustration/failure for many new Linux users.
The program will be for Debian- and Fedora-based Linux distros running the standard GNOME desktop
environment. The goal is to teach/inform as well as help the user with post-install.

PERSONAL CONTEXT (reason for creating):
Over the course of the past year I have used several different GNU/Linux desktop distributions like
Ubuntu, Mint, Fedora, Arch, Oracle Linux in a VM, Debian Stable, and now finally Debian Testing.
After using GNU/Linux desktop distributions so regularly as my "daily driver", I cannot help but think it to be a
crime against humanity that this free, open source, non-profit and beautiful alternative operating system/kernel
are not in wider use by the common user; and furthermore that Linux's integration into the wider sphere of the
desktop world seems to have been actively stymied by corporate, profit-centric interests: effectively depriving
the world of the same security, stability and reliability that these same corporations' headless Linux servers enjoy
by using it.
This strange disparity of reasoning that I have come to see motivated me to use my meager-yet-growing
programming skills to create an application that takes care of any post-installation headaches that commonly
scare new Linux desktop users away, and that might make it more inviting for others to try Linux; taking some of
the desktop marketshare away from profit-focused, proprietary operating systems that only exist to make money
for someone far removed from everyday life.
