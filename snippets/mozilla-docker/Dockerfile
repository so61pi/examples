FROM ubuntu:18.04

# Build time env
ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y --fix-missing gawk wget curl build-essential socat cpio python debianutils xterm locales vim tmux tree fish man-db less gdb iproute2 clang llvm

RUN dpkg-reconfigure locales && locale-gen en_US.UTF-8 && update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8

RUN wget -q https://static.rust-lang.org/rustup.sh -O /tmp/rustup.sh
RUN sh /tmp/rustup.sh --disable-sudo

RUN wget -q https://hg.mozilla.org/mozilla-central/raw-file/default/python/mozboot/bin/bootstrap.py -O /tmp/bootstrap.py
RUN python /tmp/bootstrap.py --application-choice=browser --no-interactive

COPY gdbinit /etc/gdb/gdbinit

ARG groupid
ARG userid
RUN [ $(getent group ${groupid}) ] || addgroup --quiet --gid ${groupid} builder
RUN [ $(getent passwd ${userid}) ] || adduser --quiet --uid ${userid} --gid ${groupid} --disabled-password --gecos '' builder
RUN usermod -a --groups ${groupid} $(id -un ${userid}) || true

RUN chsh --shell /usr/bin/fish builder

# Clean up
RUN apt-get clean -y
RUN rm -rf /var/lib/apt/lists/*

USER ${userid}:${groupid}
WORKDIR /mozilla-central
#CMD ./mach build
