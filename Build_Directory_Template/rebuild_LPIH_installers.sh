#!/bin/bash
# This shell script builds the .deb and .rpm files for Imperial-Slug's LPIH Project.
# To make it work, ensure you have navigated to the .deb builder directory's  directory (ie; LPIH should be in the directory you are in, with the DEBIAN and usr directories).
# This script uninstalls the original one first.  It is purely for development purposes when testing changes and quickly re-deploying the .deb and .rpm installer packages.
# If you are testing it on a Fedora machine, you may wish to change the commands to suit your Fedora system, or add your own build script to the project.
# Requires Alien to convert the built .deb to a .rpm: -----> ( sudo apt/dnf install alien )
# The script currently only fully supports building automatically on Debian based distros.  Feel free to add your own Fedora support if I haven't gotten to it by now, and make a pull request.

# Delete previous .deb and .rpm from current directory:
rm -r LPIH.deb LPIH*.rpm  || sudo rm -r LPIH.deb LPIH*.rpm;
# Uninstall the previous version from dev computer.
sudo dpkg --purge LPIH;
# Build the .deb
dpkg-deb --build LPIH;
# Build the .rpm with alien: install alien if not present.
alien -r -c -v LPIH.deb || sudo alien -r -c -v LPIH.deb || sudo apt install alien && alien -r -c -v LPIH.deb || sudo alien -r -c -v LPIH.deb;
# Install the new .deb to the system.  Use the .rpm on Fedora
sudo dpkg -i LPIH.deb || echo "Error: the new build wasn't able to be installed.";

# rm -r rpmbuild  || sudo rm -r rpmbuild;
