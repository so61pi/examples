FROM ubuntu:18.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
 && apt-get install -y apt-utils \
 && apt-get install -y locales sudo fish \
 && dpkg-reconfigure locales && locale-gen en_US.UTF-8 && update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8 \
 && apt-get upgrade -y

RUN apt-get update \
 && apt-get upgrade -y \
 && apt-get install -y build-essential debianutils gawk wget curl socat \
                       cpio python vim xterm tmux tree man-db less strace \
                       gdb iproute2 clang llvm libnotify-bin dbus dbus-x11 \
                       mercurial git php php-curl shellcheck firefox graphviz

# Add user.
ARG groupid
ARG userid
ARG username=builder
RUN [ $(getent group ${groupid}) ] || addgroup --quiet --gid ${groupid} ${username}
RUN [ $(getent passwd ${userid}) ] || adduser --quiet --uid ${userid} --gid ${groupid} --disabled-password --gecos '' ${username}
RUN usermod -a --groups ${groupid} $(id -un ${userid}) || true
RUN chsh --shell /usr/bin/fish ${username}

# Add user to sudo group.
RUN adduser ${username} sudo \
 && echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers \
 && echo 'Defaults logfile=/dev/null' >> /etc/sudoers

COPY gdbinit /etc/gdb/gdbinit

# Install node.
RUN curl -sL https://deb.nodesource.com/setup_8.x | bash - \
 && apt-get install -y nodejs \
 && npm install -g npm \
 && npm install -g jsdoc

# Install arcanist.
RUN git clone https://github.com/phacility/arcanist.git /opt/arcanist \
 && git clone https://github.com/phacility/libphutil.git /opt/libphutil
ENV PATH="/opt/arcanist/bin:${PATH}"

# Switch user.
USER ${userid}:${groupid}

RUN wget -q https://static.rust-lang.org/rustup.sh -O /tmp/rustup.sh \
 && sh /tmp/rustup.sh
ENV PATH="/home/${username}/.cargo/bin:${PATH}"

RUN wget -q https://hg.mozilla.org/mozilla-central/raw-file/default/python/mozboot/bin/bootstrap.py -O /tmp/bootstrap.py \
 && python /tmp/bootstrap.py --application-choice=browser --no-interactive

RUN cargo install cbindgen

RUN touch ~/.arcrc
RUN mkdir -p ~/.config/fish && echo 'alias mach /mozilla-central/mach' >> ~/.config/fish/config.fish
RUN mkdir -p ~/.local/share/fish && touch ~/.local/share/fish/fish_history

# Clean apt cache to reduce image size.
RUN sudo apt-get clean -y \
 && sudo rm -rf /var/lib/apt/lists/*

WORKDIR /mozilla-central
CMD /mozilla-central/mach build
