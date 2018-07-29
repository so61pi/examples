# lightweight ubuntu box using fluxbox
FROM ubuntu:latest

ENV http_proxy=http://10.10.10.10:8080/ https_proxy=https://10.10.10.10:8080/

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get install -y apt-utils
RUN apt-get install -y locales
RUN dpkg-reconfigure locales && locale-gen en_US.UTF-8 && update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
RUN apt-get upgrade -y

RUN apt-get install -y xvfb fluxbox x11vnc xinit iputils-ping net-tools ssh terminator vim

RUN apt-get clean -y
RUN rm -rf /var/lib/apt/lists/*

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
