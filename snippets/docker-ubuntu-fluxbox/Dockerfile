# lightweight ubuntu box using fluxbox
FROM ubuntu:latest

ENV http_proxy=http://10.10.10.10:8080/ https_proxy=https://10.10.10.10:8080/

# install stuff
RUN DEBIAN_FRONTEND=noninteractive;                             \
    for i in `seq 5`; do apt-get update; done;                  \
    for i in `seq 5`; do apt-get install -y apt-utils; done;    \
    for i in `seq 10`; do apt-get -d install -y xvfb fluxbox x11vnc xinit iputils-ping net-tools ssh terminator vim; done;  \
    apt-get install -y xvfb fluxbox x11vnc xinit iputils-ping net-tools ssh terminator vim;

# config
RUN x11vnc -storepasswd 1234 ~/x11vncpasswd;    \
    echo "#! /bin/bash" >> ~/.xinitrc;          \
    echo "exec startfluxbox" >> ~/.xinitrc;     \
    echo "#! /bin/bash" >> ~/envup;             \
    echo "export DISPLAY=:1" >> ~/envup;        \
    echo "startx -- `which Xvfb` :1 -screen 0 1024x768x24 &" >> ~/envup;    \
    echo "x11vnc -display :1.0 -xkb -noxrecord -noxfixes -noxdamage -usepw -rfbauth ~/x11vncpasswd -forever &" >> ~/envup;  \
    echo "bash" >> ~/envup;     \
    chmod +x ~/envup;           \
    echo "export DISPLAY=:1" >> ~/.bashrc;
