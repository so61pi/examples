FROM ubuntu:16.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
 && apt-get install -y apt-utils \
 && apt-get install -y locales sudo fish \
 && dpkg-reconfigure locales && locale-gen en_US.UTF-8 && update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8 \
 && apt-get upgrade -y

#RUN apt-get update \
# && apt-get upgrade -y \
# && apt-get install -y g++ gdb

#RUN sudo apt-get clean -y
#RUN sudo rm -rf /var/lib/apt/lists/*

ARG groupid
ARG userid
ARG username=worker
RUN [ $(getent group ${groupid}) ] || addgroup --quiet --gid ${groupid} ${username}
RUN [ $(getent passwd ${userid}) ] || adduser --quiet --uid ${userid} --gid ${groupid} --disabled-password --gecos '' ${username}
RUN usermod -a --groups ${groupid} $(id -un ${userid}) || true
RUN chsh --shell /usr/bin/fish ${username}

RUN adduser ${username} sudo
RUN echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
RUN echo 'Defaults logfile=/dev/null' >> /etc/sudoers

USER ${userid}:${groupid}
RUN mkdir -p ~/.local/share/fish && touch ~/.local/share/fish/fish_history

WORKDIR /code
