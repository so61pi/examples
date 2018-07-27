FROM ubuntu:18.04

RUN DEBIAN_FRONTEND=noninteractive; apt-get update && apt-get upgrade -y && apt-get install -y --fix-missing gawk wget curl build-essential socat cpio python debianutils xterm locales vim tmux tree fish man-db less gdb iproute2
RUN DEBIAN_FRONTEND=noninteractive; apt-get clean -y

RUN dpkg-reconfigure locales && locale-gen en_US.UTF-8 && update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8

RUN wget -q https://hg.mozilla.org/mozilla-central/raw-file/default/python/mozboot/bin/bootstrap.py -O /tmp/bootstrap.py
RUN python /tmp/bootstrap.py --application-choice=browser --no-interactive

COPY gdbinit /etc/gdb/gdbinit

ARG groupid
ARG userid
RUN [ $(getent group ${groupid}) ] || addgroup --quiet --gid ${groupid} builder
RUN [ $(getent passwd ${userid}) ] || adduser --quiet --uid ${userid} --gid ${groupid} --disabled-password --gecos '' builder
RUN usermod -a --groups ${groupid} $(id -un ${userid}) || true

RUN chsh --shell /usr/bin/fish builder

USER ${userid}:${groupid}
WORKDIR /mozilla-central/js/src
