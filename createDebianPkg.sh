#! /bin/sh

rm -rf /tmp/insanerzshooter
mkdir -p /tmp/insanerzshooter/DEBIAN
mkdir -p /tmp/insanerzshooter/usr/games
mkdir -p /tmp/insanerzshooter/usr/share/insanerzshooter/
mkdir -p /tmp/insanerzshooter/usr/share/applications
cp res/* -r /tmp/insanerzshooter/usr/share/insanerzshooter/
cp hiscore.dat /tmp/insanerzshooter/usr/share/insanerzshooter/
cp bin/Release/InsanerzShooter /tmp/insanerzshooter/usr/games/insanerzshooter
cp FreeSans_bold.ttf /tmp/insanerzshooter/usr/share/insanerzshooter/ 
cp control /tmp/insanerzshooter/DEBIAN/
cp insanerzshooter.desktop /tmp/insanerzshooter/usr/share/applications
dpkg-deb -b /tmp/insanerzshooter/ .
