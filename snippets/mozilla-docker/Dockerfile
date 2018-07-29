FROM ubuntu:18.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get install -y apt-utils
RUN apt-get install -y locales
RUN dpkg-reconfigure locales && locale-gen en_US.UTF-8 && update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
RUN apt-get upgrade -y

RUN apt-get install -y gawk wget curl build-essential socat cpio python debianutils xterm vim tmux tree fish man-db less gdb iproute2 clang llvm

RUN wget -q https://static.rust-lang.org/rustup.sh -O /tmp/rustup.sh
RUN sh /tmp/rustup.sh --disable-sudo

RUN wget -q https://hg.mozilla.org/mozilla-central/raw-file/default/python/mozboot/bin/bootstrap.py -O /tmp/bootstrap.py
RUN python /tmp/bootstrap.py --application-choice=browser --no-interactive

ARG groupid
ARG userid
RUN [ $(getent group ${groupid}) ] || addgroup --quiet --gid ${groupid} builder
RUN [ $(getent passwd ${userid}) ] || adduser --quiet --uid ${userid} --gid ${groupid} --disabled-password --gecos '' builder
RUN usermod -a --groups ${groupid} $(id -un ${userid}) || true

RUN chsh --shell /usr/bin/fish builder

RUN apt-get clean -y
RUN rm -rf /var/lib/apt/lists/*

COPY gdbinit /etc/gdb/gdbinit

USER ${userid}:${groupid}
WORKDIR /mozilla-central
#CMD ./mach build
