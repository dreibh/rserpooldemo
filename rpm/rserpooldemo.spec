Name: rserpooldemo
Version: 3.0.0~rc1.14
Release: 1
Summary: RSerPool Demo Tool
Group: Applications/Internet
License: GPLv3
URL: https://www.uni-due.de/~be0001/rserpool/
Source: https://www.uni-due.de/~be0001/rserpool/download/%{name}-%{version}.tar.gz

AutoReqProv: on
BuildRequires: cmake
BuildRequires: inkscape
BuildRequires: gcc
BuildRequires: gcc-c++
BuildRequires: gimp
BuildRequires: ImageMagick
BuildRequires: perl-Image-ExifTool
BuildRequires: qt5-qtbase-devel
BuildRequires: texlive-epstopdf-bin
BuildRoot: %{_tmppath}/%{name}-%{version}-build

%define _unpackaged_files_terminate_build 0

%description
This package contains the RSerPool Demo Tool
See https://www.uni-due.de/~be0001/ for details on RSerPool and the RSerPoolDemo tool!

%prep
%setup -q

%build
%cmake -DCMAKE_INSTALL_PREFIX=/usr -DFLAT_DIRECTORY_STRUCTURE=1 -DBUILD_BOOTSPLASH=1 .
make %{?_smp_mflags}

%install
make DESTDIR=%{buildroot} install


%package management
Summary: RSerPool Demo Management
Group: Applications/Internet
Requires: %{name} = %{version}-%{release}
Requires: joe
Requires: git
Requires: gpm

%description management
This metapackage contains basic software for RSerPoolDemo system management.
See https://www.uni-due.de/~be0001/ for details on RSerPool and the RSerPoolDemo tool!


%package development
Summary: RSerPool Demo Development
Group: Applications/Internet
Requires: %{name} = %{version}-%{release}
Requires: autoconf
Requires: automake
Requires: bison
Requires: clang
Requires: cmake
Requires: flex
Requires: gcc
Requires: gcc-c++
Requires: gdb
Requires: inkscape
Requires: make
Requires: mock
Requires: qt5-qtbase-devel
Requires: bc
Requires: perl-Image-ExifTool
Requires: gimp
Requires: ImageMagick
Requires: ghostscript
Requires: texlive-epstopdf-bin
Requires: valgrind

%description development
This meta-package contains basic software for RSerPoolDemo development.
See https://www.uni-due.de/~be0001/ for details on RSerPool and the RSerPoolDemo tool!

%package desktop
Summary: RSerPool Demo Desktop
Group: Applications/Internet
Requires: %{name} = %{version}-%{release}
Requires: %{name}-management

%description desktop
This package contains the scripts to configure a RSerPoolDemo desktop.
See https://www.uni-due.de/~be0001/ for details on RSerPool and the RSerPoolDemo tool!


%files
/usr/share/rserpooldemo/Desktop-with-Logo/*
/usr/share/rserpooldemo/Desktop-without-Logo/*
/usr/share/rserpooldemo/Splash/*


%package tool
Summary: RSerPool Demo Tool
Group: Applications/Internet
Requires: %{name} = %{version}-%{release}
Requires: %{name}-management
Requires: rsplib-fgp-cfgfiles
Requires: %{name}-desktop
Recommends: rsplib-registrar

%description tool
This package contains the RSerPoolDemo tool.
See https://www.uni-due.de/~be0001/ for details on RSerPool and the RSerPoolDemo tool!


%files
/usr/bin/rserpooldemo
/usr/share/man/man1/rserpooldemo.1.gz
/usr/share/rserpooldemo/graphics/*
/usr/share/rserpooldemo/local-scenario/*


%changelog
* Wed Nov 22 2017 Thomas Dreibholz <dreibh@iem.uni-due.de> - 0.0.0
- Created RPM package.
