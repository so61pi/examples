FROM ubuntu:16.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
 && apt-get install -y apt-utils \
 && apt-get install -y locales sudo \
 && locale-gen en_US.UTF-8 && update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8 \
 && apt-get upgrade -y

RUN apt-get update \
 && apt-get upgrade -y \
 && apt-get install -y g++ gdb git cmake zsh curl build-essential python python3 vim

RUN apt-get clean -y && rm -rf /var/lib/apt/lists/*

RUN TERM=xterm-256color sh -c "$(curl -fsSL https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh)" || true
RUN sed -i -e 's/ZSH_THEME="robbyrussell"/ZSH_THEME="ys"/' /root/.zshrc
