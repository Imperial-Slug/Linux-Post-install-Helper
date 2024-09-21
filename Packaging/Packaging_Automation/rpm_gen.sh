#!/bin/bash

version='0.3.1'
sources_dir="/home/${USER}/rpmbuild/SOURCES/LPIH-${version}"
spec_dir="/home/${USER}/rpmbuild/SPECS"
starting_dir=$(realpath ../)

check_complete() {
if [ $? != 0 ]; then
echo "**************"
echo "Failed to copy ${1}. "
else
echo "Copied ${1} to ${sources_dir}"
fi
}

echo "Ensure that SOURCES/LPIH-${version}/makefile.am has all the most up-to-date source code files added to it, and that the LPIH.spec file has been updated accordingly for all additional source code and resource files."
echo "*****************************"

# Moves the rpmbuild dir structure from the Linux-Post-install-Helper project root dir to the home dir so it can be used properly.

cp -R ../rpmbuild /home/${USER}/rpmbuild
if [ $? != 0 ]; then
echo "**************"
echo "Failed to copy rpmbuild directory to home directory."
else
echo "Copied rpm structure to home directory."
fi

# Copies the up-to-date source code and resource files to the rpmbuild /SOURCES/LPIH-${version} directory.

cp ../../src/lpih ${sources_dir}
check_complete "lpih binary"
cp ../../src/*.c ${sources_dir}
check_complete "source code files"
cp ../../Resources/* ${sources_dir}
check_complete "resource files"

cd ${sources_dir}
:
aclocal
autoconf
automake --add-missing
./configure

cd .. 
:
tar -czvf LPIH-${version}.tar.gz LPIH-${version}

cd ${spec_dir}
:
rpmbuild -ba --nodebuginfo LPIH.spec

if [ $? != 0 ]; then
echo "**************"
echo "Failed to create .rpm for LPIH.  Make sure you adjusted the version numbers for the folders in the SOURCES directory in the /home/${USER}/rpmbuild directory."
else
echo "Successfully created .rpm for LPIH.  Find it in /home/${USER}/rpmbuild"

fi

