#!/bin/bash

# Meant to be run from the LPIH project structure root dir.

version='0.2'
sources_dir="/home/${USER}/rpmbuild/SOURCES/LPIH-${version}"
spec_dir="/home/${USER}/rpmbuild/SPECS"

echo "Ensure that SOURCES/LPIH-${version}/makefile.am has all the most up-to-date source code files added to it, and that the LPIH.spec file has been updated accordingly for all additional source code and resource files."
echo "*****************************"
echo ""

# Moves the rpmbuild dir structure from the Linux-Post-install-Helper project root dir to the home dir so it can be used properly.

cp ../Packaging/rpmbuild ~/rpmbuild

# Copies the up-to-date source code and resource files to the rpmbuild /SOURCES/LPIH-${version} directory.

cp ../src/lpih ${sources_dir}
cp ../src/*.c ${sources_dir}
cp ../Resources/* ${sources_dir}

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



