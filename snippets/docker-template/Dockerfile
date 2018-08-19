FROM ubuntu:16.04

#ENV http_proxy=http://<proxy>:<port>/
#ENV https_proxy=https://<proxy>:<port>/
#ENV ftp_proxy=ftp://<proxy>:<port>/
#ENV socks_proxy=socks://<proxy>:<port>/

#ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
 && apt-get install -y apt-utils \
 && apt-get install -y locales sudo \
 && dpkg-reconfigure locales && locale-gen en_US.UTF-8 && update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8 \
 && apt-get upgrade -y

#RUN apt-get update \
# && apt-get upgrade -y \
# && apt-get install -y g++ gdb

#RUN apt-get clean -y
#RUN rm -rf /var/lib/apt/lists/*

ARG groupid
ARG userid
RUN [ $(getent group ${groupid}) ] || addgroup --quiet --gid ${groupid} worker
RUN [ $(getent passwd ${userid}) ] || adduser --quiet --uid ${userid} --gid ${groupid} --disabled-password --gecos '' worker
RUN usermod -a --groups ${groupid} $(id -un ${userid}) || true

RUN adduser worker sudo
RUN echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
RUN echo 'Defaults logfile=/dev/null' >> /etc/sudoers

USER worker
WORKDIR /code
