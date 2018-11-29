# lightweight ubuntu box using fluxbox
FROM ubuntu:latest

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
 && apt-get install -y apt-utils \
 && apt-get install -y locales sudo \
 && locale-gen en_US.UTF-8 && update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8 \
 && apt-get upgrade -y

RUN apt-get update \
 && apt-get upgrade -y \
 && apt-get install -y xvfb fluxbox x11vnc xinit iputils-ping net-tools ssh terminator vim

RUN apt-get clean -y && rm -rf /var/lib/apt/lists/*

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
