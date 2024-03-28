Setup tool:
```shell
kienht@kienht-MacBook-Pro:~/CLionProjects/Database/test$ sudo apt install python3-pip
Reading package lists... Done
Building dependency tree
Reading state information... Done
The following packages were automatically installed and are no longer required:
dctrl-tools diffstat enchant example-content gcc-7-aarch64-linux-gnu-base
gcc-7-arm-linux-gnueabihf-base gcc-7-cross-base gcc-8-base:i386
gcc-8-cross-base gir1.2-mutter-2 gnome-software-common imagemagick-6-common
iputils-arping libappstream-glib8 libapt-pkg-perl libart-2.0-2
libasan4-armhf-cross libboost-date-time1.65.1 libboost-filesystem1.65.1
libboost-system1.65.1 libboost-thread1.65.1 libbs2b0 libcdio17
libclass-accessor-perl libclass-method-modifiers-perl libclone-perl
libcommon-sense-perl libcrystalhd3 libdc1394-22 libdca0 libde265-0
libdevel-size-perl libdigest-bubblebabble-perl libdns-export1100
libdouble-conversion1 libdynaloader-functions-perl libegl-mesa0:i386
libegl1:i386 libenchant1c2a libevent-2.1-6 libevent-core-2.1-6
libevent-openssl-2.1-6 libevent-pthreads-2.1-6 libexempi3 libexiv2-14
libexporter-tiny-perl libfcgi-perl libfile-copy-recursive-perl libflite1
libgail-3-0 libgbm1:i386 libgdbm5 libgeoip1 libgl1-mesa-glx:i386
libglvnd-core-dev libgmime-3.0-0 libgnatvsn7 libgnome-desktop-3-17
libgrilo-0.3-0 libgssdp-1.0-3 libgupnp-igd-1.0-4 libgutenprint-common
libhogweed4 libhunspell-1.6-0 libicu-le-hb-dev libicu-le-hb0 libicu60
libilmbase12 libilmbase24 libio-pty-perl libip6tc0 libipc-run-perl libiptc0
libisc-export169 libisc169 libjson-c3 libjson-xs-perl libkate1 liblilv-0-0
liblinux-epoll-perl liblist-moreutils-perl libllvm10:i386 liblouis14
liblouisutdml8 liblqr-1-0 libminiupnpc10 libmng2 libmodplug1 libmutter-2-0
libnet-dns-perl libnet-dns-sec-perl libnet-ip-perl libnetpbm10 libnettle6
libntfs-3g88 libnumber-compare-perl libnvidia-common-470
libnvidia-decode-470:i386 liboauth0 libopenexr22 libpackage-stash-xs-perl
libpath-tiny-perl libperl4-corelibs-perl libperl5.26 libplymouth4
libpoppler73 libprotobuf10 libraw16 libref-util-xs-perl
libreoffice-avmedia-backend-gstreamer libserd-0-0 libsereal-decoder-perl
libsereal-encoder-perl libsereal-perl libsndio6.1:i386 libsord-0-0
libsoundtouch1 libspandsp2 libspeexdsp1:i386 libsratom-0-0 libsrt1
libsub-exporter-progressive-perl libsub-identify-perl libsub-quote-perl
libtest-fatal-perl libtext-glob-perl libtype-tiny-perl
libtypes-serialiser-perl libubsan0-armhf-cross libunicode-utf8-perl
libusbmuxd4 libusrsctp1 libvo-aacenc0 libwayland-bin
libwayland-egl1-mesa:i386 libwayland-server0:i386 libwildmidi2 libx264-152
libx265-146 libxcb-dri2-0-dev libxcb-glx0-dev libxcb-randr0-dev
libxcb-shape0-dev libxcb-sync-dev libxcb-xfixes0-dev libxdamage1:i386
libxml-simple-perl libxml-writer-perl libxnvctrl0 libxshmfence-dev
libyaml-libyaml-perl libzbar0 libzeitgeist-2.0-0 light-themes
linux-headers-generic-hwe-18.04 linux-image-generic-hwe-18.04
mesa-common-dev nvidia-prime patchutils python3-oauth python3-zope.interface
qtchooser qtcore4-l10n ruby-did-you-mean t1utils x11proto-composite-dev
xserver-xorg-core-hwe-18.04 xserver-xorg-hwe-18.04
xserver-xorg-input-all-hwe-18.04 xserver-xorg-input-libinput-hwe-18.04
xserver-xorg-video-fbdev-hwe-18.04 xserver-xorg-video-nouveau-hwe-18.04
xserver-xorg-video-qxl-hwe-18.04 xserver-xorg-video-radeon-hwe-18.04
xserver-xorg-video-vesa-hwe-18.04 xserver-xorg-video-vmware-hwe-18.04
Use 'sudo apt autoremove' to remove them.
The following additional packages will be installed:
python-pip-whl python3-setuptools python3-wheel
Suggested packages:
python-setuptools-doc
The following NEW packages will be installed:
python-pip-whl python3-pip python3-setuptools python3-wheel
0 upgraded, 4 newly installed, 0 to remove and 11 not upgraded.
Need to get 2.389 kB of archives.
After this operation, 4.933 kB of additional disk space will be used.
Do you want to continue? [Y/n] y
Get:1 http://vn.archive.ubuntu.com/ubuntu focal-updates/universe amd64 python-pip-whl all 20.0.2-5ubuntu1.9 [1.805 kB]
Get:2 http://vn.archive.ubuntu.com/ubuntu focal-updates/main amd64 python3-setuptools all 45.2.0-1ubuntu0.1 [330 kB]
Get:3 http://vn.archive.ubuntu.com/ubuntu focal-updates/universe amd64 python3-wheel all 0.34.2-1ubuntu0.1 [23,9 kB]
Get:4 http://vn.archive.ubuntu.com/ubuntu focal-updates/universe amd64 python3-pip all 20.0.2-5ubuntu1.9 [231 kB]
Fetched 2.389 kB in 1s (2.171 kB/s)
Selecting previously unselected package python-pip-whl.
(Reading database ... 267314 files and directories currently installed.)
Preparing to unpack .../python-pip-whl_20.0.2-5ubuntu1.9_all.deb ...
Unpacking python-pip-whl (20.0.2-5ubuntu1.9) ...
Selecting previously unselected package python3-setuptools.
Preparing to unpack .../python3-setuptools_45.2.0-1ubuntu0.1_all.deb ...
Unpacking python3-setuptools (45.2.0-1ubuntu0.1) ...
Selecting previously unselected package python3-wheel.
Preparing to unpack .../python3-wheel_0.34.2-1ubuntu0.1_all.deb ...
Unpacking python3-wheel (0.34.2-1ubuntu0.1) ...
Selecting previously unselected package python3-pip.
Preparing to unpack .../python3-pip_20.0.2-5ubuntu1.9_all.deb ...
Unpacking python3-pip (20.0.2-5ubuntu1.9) ...
Setting up python3-setuptools (45.2.0-1ubuntu0.1) ...
Setting up python3-wheel (0.34.2-1ubuntu0.1) ...
Setting up python-pip-whl (20.0.2-5ubuntu1.9) ...
Setting up python3-pip (20.0.2-5ubuntu1.9) ...
Processing triggers for man-db (2.9.1-1) ...

kienht@kienht-MacBook-Pro:~/CLionProjects/Database/test$ pip install leveldb openpyxl
Collecting leveldb
Downloading leveldb-0.201.tar.gz (236 kB)
|████████████████████████████████| 236 kB 1.9 MB/s
Collecting openpyxl
Downloading openpyxl-3.1.2-py2.py3-none-any.whl (249 kB)
|████████████████████████████████| 249 kB 3.1 MB/s
Collecting et-xmlfile
Downloading et_xmlfile-1.1.0-py3-none-any.whl (4.7 kB)
Building wheels for collected packages: leveldb
Building wheel for leveldb (setup.py) ... done
Created wheel for leveldb: filename=leveldb-0.201-cp38-cp38-linux_x86_64.whl size=2051935 sha256=60e3809ff5eb11838f7e04a3733a6daf4aacb16de4e7272759b08b0a2d664fc9
Stored in directory: /home/kienht/.cache/pip/wheels/bc/dc/f6/b56f7de35f2c5c0c21dc681c1f8f7bd228e7ccab44eb056ce1
Successfully built leveldb
Installing collected packages: leveldb, et-xmlfile, openpyxl
Successfully installed et-xmlfile-1.1.0 leveldb-0.201 openpyxl-3.1.2

```

Usage:
```shell
///convert excel to leveldb
python3 excel2leveldb.py <leveldb_path> <excel_file_path>

///convert leveldb to excel
python3 leveldb2excel.py <leveldb_path> <output_excel_file>

```