Name: rserpooldemo
Version: 3.2.4
Release: 1
Summary: RSerPool Demo Tool
Group: Applications/Internet
License: GPL-3.0-or-later
URL: https://www.nntb.no/~dreibh/rserpool/rserpool/
Source: https://www.nntb.no/~dreibh/rserpool/rserpool/download/%{name}-%{version}.tar.xz

# FIXME: S390x does not provide the dependency Gimp 3.x, yet.
#        Once this is fixed, the architecture exclusion can be removed:
ExcludeArch: s390x

AutoReqProv: on
BuildRequires: cmake
BuildRequires: dejavu-sans-fonts
BuildRequires: dejavu-sans-mono-fonts
BuildRequires: dejavu-serif-fonts
BuildRequires: gcc
BuildRequires: gcc-c++
BuildRequires: ghostscript
BuildRequires: gimp
BuildRequires: google-noto-cjk-fonts
BuildRequires: google-noto-sans-fonts
BuildRequires: google-noto-serif-fonts
BuildRequires: GraphicsMagick
BuildRequires: perl-Image-ExifTool
BuildRequires: qt6-qtbase-devel
BuildRequires: texlive-epstopdf-bin
BuildRequires: urw-base35-fonts
BuildRoot: %{_tmppath}/%{name}-%{version}-build

Requires: %{name}-tool
Recommends: %{name}-desktop
Recommends: %{name}-management
Suggests: %{name}-development


%description
This package contains the RSerPool Demo Tool
See https://www.nntb.no/~dreibh/rserpool/ for details on RSerPool and the
RSerPoolDemo tool!

%prep
%setup -q

%build
# NOTE: CMAKE_VERBOSE_MAKEFILE=OFF for reduced log output!
%cmake -DCMAKE_INSTALL_PREFIX=/usr -DFLAT_DIRECTORY_STRUCTURE=1 -DBUILD_BOOTSPLASH=1 -DCMAKE_VERBOSE_MAKEFILE=OFF .
%cmake_build

%install
%cmake_install
# ====== Relocate files =====================================================
mkdir -p %{buildroot}/boot/RSerPoolDemo
mv %{buildroot}/usr/share/rserpooldemo/splash/Zollverein-*.jpeg %{buildroot}/boot/RSerPoolDemo
mkdir -p %{buildroot}/etc/rserpooldemo
mv %{buildroot}/usr/share/rserpooldemo/splash/rserpooldemo-version %{buildroot}/etc/rserpooldemo
# ===========================================================================

%files


%package management
Summary: Management tools for the RSerPool Demo system
Group: Applications/Internet
BuildArch: noarch
Requires: acl
Requires: bash
Requires: bash-completion
Requires: bind-utils
Requires: bridge-utils
Requires: bwm-ng
Requires: bzip2
Requires: chrpath
Requires: cloud-utils-growpart
Requires: cmake
Requires: curl
Requires: dnf-automatic
Requires: ethtool
Requires: fail2ban
Requires: gdisk
Requires: git
Requires: gpg
Requires: gpm
Requires: hipercontracer
Requires: htop
Requires: iproute
Requires: iproute-tc
Requires: iptables
Requires: iputils
Requires: joe
Requires: jq
Requires: kernel-modules-extra
Requires: libidn
Requires: make
Requires: man
Requires: netperfmeter
Requires: netplan
Requires: net-tools
Requires: nmap
Requires: openssh-server
Requires: openssl
Requires: parallel
Requires: plocate
Requires: pwgen
Requires: python3
Requires: rsplib-tools
Requires: rsync
Requires: subnetcalc
Requires: sudo
Requires: tar
Requires: tcpdump
Requires: td-system-tools
Requires: td-system-tools-configure-grub
Requires: traceroute
Requires: tree
Requires: tsctp
Requires: unzip
Requires: uuid
Requires: virt-what
Requires: wget
Requires: wireshark-cli
Requires: zip
Recommends: grub2-tools

%description management
This metapackage contains basic software for RSerPoolDemo system management.
See https://www.nntb.no/~dreibh/rserpool/ for details on RSerPool and the
RSerPoolDemo tool!

%files management
%{_sysconfdir}/rserpooldemo/rserpooldemo-version


%package development
Summary: Development tools for the RSerPool Demo system
Group: Applications/Internet
BuildArch: noarch
Requires: %{name}-management = %{version}-%{release}
Requires: autoconf
Requires: automake
Requires: bc
Requires: bison
Requires: boost-devel
Requires: bzip2-devel
Requires: c-ares-devel
Requires: clang
Requires: cmake
Requires: dejavu-fonts-all
Requires: extra-cmake-modules
Requires: flex
Requires: g++
Requires: gcc
Requires: gdb
Requires: GeoIP-devel
Requires: ghostscript
Requires: git-lfs
Requires: google-noto-fonts-all
Requires: GraphicsMagick
Requires: libcurl-devel
Requires: libtool
Requires: lksctp-tools-devel
Requires: mock
Requires: open-sans-fonts
Requires: openssl-devel
Requires: pbuilder
Requires: pdf2svg
Requires: perl-Image-ExifTool
Requires: pkg-config
Requires: python3
Requires: python3-netifaces
Requires: python3-pip
Requires: python3-setuptools
Requires: qt6-linguist
Requires: qt6-qtbase-devel
Requires: R-base
Requires: reprepro
Requires: rpm-build
Requires: rsplib-libcpprspserver-devel
Requires: rsplib-librsplib-devel
Requires: shellcheck
Requires: tidy
Requires: urw-base35-fonts
Requires: xz-devel
Requires: zlib-devel
Recommends: valgrind
Recommends: yamllint

%description development
This metapackage contains basic software for RSerPoolDemo development.
See https://www.nntb.no/~dreibh/rserpool/ for details on RSerPool and the
RSerPoolDemo tool!

%files development


%package desktop
Summary: Desktop setup for the RSerPool Demo system
Group: Applications/Internet
BuildArch: noarch
Requires: %{name}-management = %{version}-%{release}
Recommends: grub2-tools

%description desktop
This package contains the scripts to configure a RSerPoolDemo desktop.
See https://www.nntb.no/~dreibh/rserpool/ for details on RSerPool and the
RSerPoolDemo tool!

%files desktop
/boot/RSerPoolDemo/Zollverein-*.jpeg
%{_sysconfdir}/grub.d/??_rserpooldemo_desktop_theme
%{_datadir}/rserpooldemo/background/*
%{_datadir}/rserpooldemo/desktop/*
%{_datadir}/rserpooldemo/grub-defaults
%{_sysconfdir}/system-info.d/30-rsplib
%{_sysconfdir}/system-maintenance.d/30-rsplib
%ghost %{_datadir}/rserpooldemo/splash

%post desktop
if [ -e /usr/sbin/grub2-mkconfig ] ; then /usr/sbin/grub2-mkconfig -o /boot/grub2/grub.cfg || true ; fi

%postun desktop
rm -f /etc/grub.d/??_rserpooldemo_desktop_theme
if [ -e /usr/sbin/grub2-mkconfig ] ; then /usr/sbin/grub2-mkconfig -o /boot/grub2/grub.cfg || true ; fi


%package scenario-cfgfiles
Summary: Scenario configuration files for the RSerPool Demo system
Group: Applications/Internet
BuildArch: noarch
Recommends: rsplib-fgp-cfgfiles

%description scenario-cfgfiles
This package contains the scenario files for the RSerPoolDemo tool.
See https://www.nntb.no/~dreibh/rserpool/ for details on RSerPool and the
RSerPoolDemo tool!

%files scenario-cfgfiles
%{_datadir}/rserpooldemo/graphics/*
%{_datadir}/rserpooldemo/local-scenario/*
%{_datadir}/rserpooldemo/nornet-mec-scenario/*


%package tool
Summary: GUI tool for the RSerPool Demo system
Group: Applications/Internet
Requires: %{name}-desktop = %{version}-%{release}
Requires: %{name}-management = %{version}-%{release}
Requires: %{name}-scenario-cfgfiles = %{version}-%{release}
Requires: rsplib-fractalpooluser
Requires: rsplib-registrar
Requires: rsplib-services
Requires: rsplib-tools
Recommends: fractgen

%description tool
This package contains the RSerPoolDemo tool.
See https://www.nntb.no/~dreibh/rserpool/ for details on RSerPool and the
RSerPoolDemo tool!

%files tool
%{_bindir}/rserpooldemo
%{_datadir}/applications/rserpooldemo.desktop
%{_datadir}/icons/hicolor/512x512/apps/rserpooldemo.png
%{_datadir}/mime/packages/rserpooldemo.xml
%{_mandir}/man1/rserpooldemo.1.gz


%changelog
* Fri Nov 21 2025 Thomas Dreibholz <thomas.dreibholz@gmail.com> - 3.2.4-1
- New upstream release.
* Sun Sep 21 2025 Thomas Dreibholz <thomas.dreibholz@gmail.com> - 3.2.3-1
- New upstream release.
* Tue Jul 08 2025 Thomas Dreibholz <thomas.dreibholz@gmail.com> - 3.2.2
- New upstream release.
* Sat Jul 05 2025 Thomas Dreibholz <thomas.dreibholz@gmail.com> - 3.2.1
- New upstream release.
* Fri Dec 13 2024 Thomas Dreibholz <thomas.dreibholz@gmail.com> - 3.2.0
- New upstream release.
* Wed Aug 07 2024 Thomas Dreibholz <thomas.dreibholz@gmail.com> - 3.1.7
- New upstream release.
* Sat Jun 08 2024 Thomas Dreibholz <thomas.dreibholz@gmail.com> - 3.1.6
- New upstream release.
* Thu Apr 11 2024 Thomas Dreibholz <thomas.dreibholz@gmail.com> - 3.1.5
- New upstream release.
* Tue Dec 19 2023 Thomas Dreibholz <thomas.dreibholz@gmail.com> - 3.1.4
- New upstream release.
* Wed Dec 06 2023 Thomas Dreibholz <thomas.dreibholz@gmail.com> - 3.1.3
- New upstream release.
* Wed Feb 08 2023 Thomas Dreibholz <thomas.dreibholz@gmail.com> - 3.1.2
- New upstream release.
* Sun Sep 11 2022 Thomas Dreibholz <dreibh@iem.uni-due.de> - 3.1.1
- New upstream release.
* Mon Aug 22 2022 Thomas Dreibholz <dreibh@iem.uni-due.de> - 3.1.0
- New upstream release.
* Wed Feb 16 2022 Thomas Dreibholz <dreibh@iem.uni-due.de> - 3.0.6
- New upstream release.
* Mon Jun 14 2021 Thomas Dreibholz <dreibh@iem.uni-due.de> - 3.0.5
- New upstream release.
* Wed Dec 16 2020 Thomas Dreibholz <dreibh@iem.uni-due.de> - 3.0.4
- New upstream release.
* Fri Nov 13 2020 Thomas Dreibholz <dreibh@iem.uni-due.de> - 3.0.3
- New upstream release.
* Fri Oct 09 2020 Thomas Dreibholz <dreibh@iem.uni-due.de> - 3.0.2
- New upstream release.
* Tue Oct 06 2020 Thomas Dreibholz <dreibh@iem.uni-due.de> - 3.0.1
- New upstream release.
* Tue May 05 2020 Thomas Dreibholz <dreibh@iem.uni-due.de> - 3.0.0
- New upstream release.
* Wed Nov 22 2017 Thomas Dreibholz <dreibh@iem.uni-due.de> - 0.0.0
- Created RPM package.
