Name: rserpooldemo
Version: 3.0.0~rc1.13
Release: 1
Summary: RSerPool Demo Tool
Group: Applications/Internet
License: GPLv3
URL: https://www.uni-due.de/~be0001/rserpool/
Source: https://www.uni-due.de/~be0001/rserpool/download/%{name}-%{version}.tar.gz

AutoReqProv: on
BuildRequires: cmake
BuildRequires: dia
BuildRequires: gcc
BuildRequires: gcc-c++
BuildRequires: ghostscript
BuildRequires: gimp
BuildRequires: ImageMagick
BuildRequires: perl-Image-ExifTool
BuildRequires: qt5-qtbase-devel
BuildRequires: texlive-epstopdf-bin
BuildRequires: texlive-pdfcrop-bin texlive-pdftex-bin
BuildRoot: %{_tmppath}/%{name}-%{version}-build

%define _unpackaged_files_terminate_build 0

%description
 This metapackage contains the RSerPool Demo Tool
 See https://www.uni-due.de/~be0001/ for details on RSerPool and the RSerPoolDemo tool!

%prep
%setup -q

%build
%cmake -DCMAKE_INSTALL_PREFIX=/usr -DFLAT_DIRECTORY_STRUCTURE=1 -DBUILD_BOOTSPLASH=1 .
make %{?_smp_mflags}

%install
make DESTDIR=%{buildroot} install

%files
/usr/share/rserpooldemo/Desktop-with-Logo/*
/usr/share/rserpooldemo/Desktop-without-Logo/*
/usr/share/rserpooldemo/Splash/*
/usr/bin/rserpooldemo
/usr/share/man/man1/rserpooldemo.1.gz
/usr/share/rserpooldemo/graphics/*
/usr/share/rserpooldemo/local-scenario/*

%doc

%changelog
* Wed Nov 22 2017 Thomas Dreibholz <dreibh@iem.uni-due.de> - 0.0.0
- Created RPM package.
