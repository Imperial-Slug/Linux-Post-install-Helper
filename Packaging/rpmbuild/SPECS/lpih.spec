Name:		lpih
Version:	1.0
Release:	2%{?dist}
Summary:        Linux post-install Helper
License:	GPL 3.0

ExclusiveArch:  x86_64
URL:		https://www.github.com/Imperial-Slug/Linux-Post-install-helper
Source0:	%{name}-%{version}.tar.gz
BuildRequires:  gcc gtk4 gtk4-devel mesa-libGL-devel pkgconfig

%description
Linux post install helper.

%prep
rm -rf %{_builddir}/%{name}-%{version}
%autosetup

%build
make

%install
make install DESTDIR=%{buildroot}

%files
/usr/bin/lpih
%{_datadir}/LPIH/css/style.css
%{_datadir}/icons/hicolor/512x512/apps/imperialslug.gtkproject.lpih.png
%{_datadir}/applications/imperialslug.gtkproject.lpih.desktop

%changelog
* Fri Jun 7 2024 Sam
- Updated Version number for new package
* Sat Jun 1 2024 Sam
- Updated names of png icons based on new project name.
* Fri Nov 10 2023 Sam
- initial
