#!/bin/bash
# This shell script builds the .deb and .rpm files for Imperial-Slug's LPIH Project.
# To make it work, ensure you have navigated to the .deb builder directory's  directory (ie; LPIH should be in the directory you are in, with the DEBIAN and usr directories).
# This script uninstalls the original one first.  It is purely for development purposes when testing changes and quickly re-deploying the .deb and .rpm installer packages.
# If you are testing it on a Fedora machine, you may wish to change the commands to suit your Fedora system.

# Uninstall the previous version from dev computer.
sudo dpkg --purge LPIH
# Build the .deb
dpkg-deb --build LPIH
# Build the .rpm
alien -r -c -v LPIH.deb
# Install the new .deb to the system.  Use the .rpm on Fedora
sudo dpkg -i LPIH.deb
