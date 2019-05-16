Name: rserpooldemo
Version: 3.0.0~rc1.27
Release: 1
Summary: RSerPool Demo Tool
Group: Applications/Internet
License: GPLv3
URL: https://www.uni-due.de/~be0001/rserpool/
Source: https://www.uni-due.de/~be0001/rserpool/download/%{name}-%{version}.tar.xz

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
BuildRequires: inkscape
BuildRequires: perl-Image-ExifTool
BuildRequires: qt5-qtbase-devel
BuildRequires: texlive-epstopdf-bin
BuildRequires: urw-base35-fonts
BuildRoot: %{_tmppath}/%{name}-%{version}-build


# TEST ONLY:
# define _unpackaged_files_terminate_build 0


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
# ====== Relocate files =====================================================
mkdir -p %{buildroot}/boot/RSerPoolDemo
mv %{buildroot}/usr/share/rserpooldemo/splash/Zollverein-1024x768.jpeg %{buildroot}/boot/RSerPoolDemo
mkdir -p %{buildroot}/etc/rserpooldemo
mv %{buildroot}/usr/share/rserpooldemo/splash/rserpooldemo-version %{buildroot}/etc/rserpooldemo
# ===========================================================================


%package management
Summary: RSerPool Demo Management
Group: Applications/Internet
BuildArch: noarch
Requires: bash-completion
Requires: bridge-utils
Requires: btrfs-progs
Requires: bc
Requires: bwm-ng
Requires: colordiff
Requires: cronie
Requires: ethtool
Requires: git
Requires: gpm
Requires: hping3
Requires: htop
Requires: ipsec-tools
Requires: joe
Requires: libidn
Requires: lksctp-tools
Requires: mlocate
Requires: net-snmp-utils
Requires: net-tools
Requires: nmap
Requires: ntpdate
Requires: pxz
Requires: reiserfs-utils
Requires: reprepro
Requires: smartmontools
Requires: subnetcalc
Requires: tcpdump
Requires: tftp
Requires: traceroute
Requires: tree
Requires: vconfig
Requires: virt-what
Requires: whois
Recommends: netperfmeter
Recommends: rsplib-docs
Recommends: rsplib-services
Recommends: rsplib-tools
Recommends: wireshark-cli

%description management
This metapackage contains basic software for RSerPoolDemo system management.
See https://www.uni-due.de/~be0001/ for details on RSerPool and the RSerPoolDemo tool!

%files management
%{_sysconfdir}/rserpooldemo/rserpooldemo-version


%package development
Summary: RSerPool Demo Development
Group: Applications/Internet
BuildArch: noarch
Requires: %{name}-management = %{version}-%{release}
Requires: autoconf
Requires: automake
Requires: banner
Requires: bison
Requires: bzip2-devel
Requires: clang
Requires: cmake
Requires: createrepo
Requires: debhelper
Requires: dejavu-sans-fonts
Requires: dejavu-sans-mono-fonts
Requires: dejavu-serif-fonts
Requires: devscripts
Requires: flex
Requires: gcc
Requires: gcc-c++
Requires: gdb
Requires: ghostscript
Requires: gimp
Requires: glib2-devel
Requires: gnupg
Requires: gnuplot
Requires: google-noto-cjk-fonts
Requires: google-noto-sans-fonts
Requires: google-noto-serif-fonts
Requires: GraphicsMagick
Requires: inkscape
Requires: libcurl-devel
Requires: libpcap-devel
Requires: libtool
Requires: lksctp-tools-devel
Requires: make
Requires: mock
Requires: openssl-devel
Requires: pbuilder
Requires: perl-Image-ExifTool
Requires: pkg-config
Requires: qt5-qtbase-devel
Requires: quilt
Requires: R-base
Requires: rpm
Requires: texlive-epstopdf-bin
Requires: urw-base35-fonts
Requires: valgrind

%description development
This meta-package contains basic software for RSerPoolDemo development.
See https://www.uni-due.de/~be0001/ for details on RSerPool and the RSerPoolDemo tool!

%files development


%package desktop
Summary: RSerPool Demo Desktop
Group: Applications/Internet
BuildArch: noarch
Requires: %{name}-management = %{version}-%{release}
Recommends: grub2-tools

%description desktop
This package contains the scripts to configure a RSerPoolDemo desktop.
See https://www.uni-due.de/~be0001/ for details on RSerPool and the RSerPoolDemo tool!

%files desktop
/boot/RSerPoolDemo/Zollverein-1024x768.jpeg
%{_sysconfdir}/grub.d/??_rserpooldemo_desktop_theme
%{_datadir}/rserpooldemo/background/*
%{_datadir}/rserpooldemo/desktop/*
%{_datadir}/rserpooldemo/grub-defaults
%ghost %{_datadir}/rserpooldemo/splash

%post desktop
echo "Updating /etc/default/grub with RSerPool Demo settings:"
echo "-----"
cat /usr/share/rserpooldemo/grub-defaults | \
   ( if grep "biosdevname=0" >/dev/null 2>&1 /proc/cmdline ; then sed "s/^GRUB_CMDLINE_LINUX=\"/GRUB_CMDLINE_LINUX=\"biosdevname=0 /g" ; else cat ; fi ) | \
   ( if grep "net.ifnames=0" >/dev/null 2>&1 /proc/cmdline ; then sed "s/^GRUB_CMDLINE_LINUX=\"/GRUB_CMDLINE_LINUX=\"net.ifnames=0 /g" ; else cat ; fi ) | tee /etc/default/grub.new && \
mv /etc/default/grub.new /etc/default/grub
echo "-----"
if [ -e /usr/sbin/grub2-mkconfig ] ; then /usr/sbin/grub2-mkconfig -o /boot/grub2/grub.cfg || true ; fi

%postun desktop
rm -f /etc/grub.d/??_rserpooldemo_desktop_theme
if [ -e /usr/sbin/grub2-mkconfig ] ; then /usr/sbin/grub2-mkconfig -o /boot/grub2/grub.cfg || true ; fi


%package tool
Summary: RSerPool Demo Tool
Group: Applications/Internet
Requires: %{name}-desktop = %{version}-%{release}
Requires: %{name}-management = %{version}-%{release}
Recommends: rsplib-fgp-cfgfiles
Recommends: rsplib-registrar
Recommends: rsplib-tools

%description tool
This package contains the RSerPoolDemo tool.
See https://www.uni-due.de/~be0001/ for details on RSerPool and the RSerPoolDemo tool!

%files tool
%{_bindir}/rserpooldemo
%{_mandir}/man1/rserpooldemo.1.gz
%{_datadir}/rserpooldemo/graphics/*
%{_datadir}/rserpooldemo/local-scenario/*


%changelog
* Wed Nov 22 2017 Thomas Dreibholz <dreibh@iem.uni-due.de> - 0.0.0
- Created RPM package.
