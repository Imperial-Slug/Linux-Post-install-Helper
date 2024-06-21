#!/bin/bash
# This shell script builds the .deb and .rpm files for Imperial-Slug's LPIH Project.
# To make it work, ensure you have navigated to the .deb builder directory's  directory (ie; LPIH should be in the directory you are in, with the DEBIAN and usr directories).
# This script uninstalls the original one first.  It is purely for development purposes when testing packaging changes and quickly re-deploying the .deb and .rpm installer packages for new builds.

# Delete previous .deb and .rpm from current directory:
rm -r LPIH.deb LPIH*.rpm && sudo rm -r LPIH.deb LPIH*.rpm || echo "Error deleting old packaging files.";
# Uninstall the previous version from dev computer.
sudo dpkg --purge LPIH;
# Build the .deb
dpkg-deb --build LPIH
# For creating the rpm
# rpmbuild -ba --nodebuginfo lpih.spec;
# Install the new .deb to the system.  Use the .rpm on Fedora
sudo dpkg -i LPIH.deb || echo "Error: the new build wasn't able to be installed.";


