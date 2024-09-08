Name:		LPIH
Version:	0.2.1
Release:	0.2.1%{?dist}
Summary:        Linux post-install Helper
License:	GPL 3.0

ExclusiveArch:  x86_64
URL:		https://www.github.com/Imperial-Slug/Linux-Post-install-helper
Source0:	%{name}-%{version}.tar.gz
BuildRequires:  clang make gtk4 gtk4-devel mesa-libGL-devel pkgconfig

%description
Linux post install helper.

%prep
rm -rf %{_builddir}/%{name}-%{version}
%autosetup

%build
make

%install
make lpih DESTDIR=%{buildroot}

mkdir -p  %{buildroot}/%{_bindir} %{buildroot}/%{_datadir}/LPIH/css/ %{buildroot}/%{_datadir}/icons/hicolor/512x512/apps/ %{buildroot}/%{_datadir}/applications/  %{buildroot}/%{_datadir}/LPIH/text_files/

cp lpih %{buildroot}/%{_bindir}/lpih
cp style.css %{buildroot}/%{_datadir}/LPIH/css/style.css
cp imperialslug.gtkproject.lpih.png %{buildroot}/%{_datadir}/icons/hicolor/512x512/apps/imperialslug.gtkproject.lpih.png
cp imperialslug.gtkproject.lpih.desktop %{buildroot}/%{_datadir}/applications/imperialslug.gtkproject.lpih.desktop
cp fed-text.txt %{buildroot}/%{_datadir}/LPIH/text_files/fed-text.txt
cp deb-text.txt %{buildroot}/%{_datadir}/LPIH/text_files/deb-text.txt

%files
%{_bindir}/lpih
%{_datadir}/LPIH/css/style.css
%{_datadir}/icons/hicolor/512x512/apps/imperialslug.gtkproject.lpih.png
%{_datadir}/applications/imperialslug.gtkproject.lpih.desktop
%{_datadir}/LPIH/text_files/fed-text.txt
%{_datadir}/LPIH/text_files/deb-text.txt

%changelog
* Fri Jun 7 2024 Sam
- Updated Version number for new package
* Sat Jun 1 2024 Sam
- Updated names of png icons based on new project name.
* Fri Nov 10 2023 Sam
- initial
