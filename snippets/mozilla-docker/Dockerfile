FROM ubuntu:18.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get install -y apt-utils
RUN apt-get install -y locales
RUN dpkg-reconfigure locales && locale-gen en_US.UTF-8 && update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
RUN apt-get upgrade -y

RUN apt-get install -y build-essential debianutils gawk wget curl socat \
                       cpio python vim xterm tmux tree fish man-db less \
                       gdb iproute2 clang llvm libnotify-bin dbus dbus-x11 \
                       mercurial git php php-curl shellcheck nodejs npm
RUN apt-get install -y firefox

RUN wget -q https://static.rust-lang.org/rustup.sh -O /tmp/rustup.sh
RUN sh /tmp/rustup.sh --disable-sudo

RUN wget -q https://hg.mozilla.org/mozilla-central/raw-file/default/python/mozboot/bin/bootstrap.py -O /tmp/bootstrap.py
RUN python /tmp/bootstrap.py --application-choice=browser --no-interactive

RUN npm install -g npm
RUN npm install -g jsdoc
RUN git clone https://github.com/phacility/arcanist.git /opt/arcanist
RUN git clone https://github.com/phacility/libphutil.git /opt/libphutil
ENV PATH="/opt/arcanist/bin:${PATH}"

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
RUN touch /home/builder/.arcrc
RUN mkdir -p ~/.config/fish && echo 'alias mach /mozilla-central/mach' >> ~/.config/fish/config.fish

WORKDIR /mozilla-central
CMD /mozilla-central/mach build
