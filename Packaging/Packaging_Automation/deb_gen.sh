#!/bin/bash

# Meant to be run from the LPIH project structure root dir.

version='0.3.1'
sources_dir="../LPIH-${version}/usr"
bin_dir="${sources_dir}/bin"
resource_dir="../../Resources"
desktop_file_dir="${sources_dir}/share/applications"
icon_dir="${sources_dir}/share/icons/hicolor/512x512/apps"
css_dir="${sources_dir}/share/LPIH/css"
text_file_dir="${sources_dir}/share/LPIH/text_files"
packaging_root=$(realpath ../)
deb_structure_root="/home/${USER}/Linux-Post-install-Helper/Packaging/LPIH-${version}"
deb_file="../LPIH-${version}.deb"



check_complete() {
if [ $? != 0 ]; then
echo "**************"
echo "Failed to copy ${1}. "
else
echo "Copied ${1} to .deb directory structure."
fi
}

# Copies the up-to-date source code and resource files to the .deb packaging dir structure.

cp ../../src/lpih ${bin_dir}
# If the binary file is copied, then echo success message, else try to compile the binary.  If the compilation fails, try installing the dependencies.  Otherwise, carry on.
if [ $? != 0 ]; then
   make -C ../../src

	if [ $? != 0 ]; then
	   echo "There was a problem compiling the LPIH binary."
	   echo "Make sure all your dependencies are installed."
	   echo ""
	   sudo apt install libgtk-4-1 libgtk-4-dev
	   	if [ $? != 0 ]; then
	   	   echo "Dependency installation failed or cancelled.  Exiting."
	   	   exit 0
	   	   fi
	   		make -C ../../src
				if [ $? != 0 ]; then
				   echo ""
				   echo "Failed to compile LPIH after installing dependencies.  Giving up."
			           exit 0
		                   fi
	else
	echo ""
	echo "LPIH binary compiled successfully."
	fi

   else
   echo ""
   echo "LPIH binary copied to .deb directory structure."
   fi


cp ${resource_dir}/*.css ${css_dir}
check_complete "CSS file"

cp ${resource_dir}/*.txt ${text_file_dir}
check_complete "text files"

cp ${resource_dir}/*.desktop ${desktop_file_dir}
check_complete "desktop file"

cp ${resource_dir}/*.png ${icon_dir}
check_complete "icon"

cd ${packaging_root}
:
if [ -f "${deb_file}" ]; then
   rm -rf "${deb_file}"
   else dpkg-deb --build ${deb_structure_root}
	if [ $? != 0 ]; then
           echo "**************"
           echo "Failed to build LPIH-${version}.deb. "
	else
	echo "LPIH-${version}.deb has been created from the new source and resource files in ${packaging_root}."
	echo ""
	fi
fi

exit 0



