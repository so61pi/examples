FROM ubuntu:16.04

ARG DEBIAN_FRONTEND=noninteractive
#ARG HTTP_PROXY=
#ARG http_proxy=
#ARG HTTPS_PROXY=
#ARG https_proxy=

RUN apt-get update \
 && apt-get install -y apt-utils \
 && apt-get install -y locales sudo \
 && locale-gen en_US.UTF-8 && update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8 \
 && apt-get upgrade -y

#RUN apt-get update \
# && apt-get upgrade -y \
# && apt-get install -y g++ gdb git

#RUN apt-get clean -y && rm -rf /var/lib/apt/lists/*

ARG groupid
ARG userid
ARG username=worker
RUN [ $(getent group ${groupid}) ] || addgroup --quiet --gid ${groupid} ${username}
RUN [ $(getent passwd ${userid}) ] || adduser --quiet --uid ${userid} --gid ${groupid} --disabled-password --gecos '' ${username}
RUN usermod -a --groups ${groupid} $(id -un ${userid}) || true

RUN adduser ${username} sudo
RUN echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
RUN echo 'Defaults logfile=/dev/null' >> /etc/sudoers

#ARG gituser
#ARG gitemail
#RUN git config --system user.email "${gitemail}"
#RUN git config --system user.name "${gituser}"

USER ${userid}:${groupid}

WORKDIR /code
