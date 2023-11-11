Name:		lpih
Version:	1.0
Release:	1%{?dist}
Summary:        Linux post install helper
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
%{_datadir}/icons/hicolor/512x512/apps/petc0016.project.lpisg.png
%{_datadir}/applications/petc0016.project.lpisg.desktop

%changelog
* Fri Nov 10 2023 Sam
- initial
