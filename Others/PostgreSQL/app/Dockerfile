FROM ubuntu:16.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get install -y apt-utils
RUN apt-get install -y locales
RUN dpkg-reconfigure locales && locale-gen en_US.UTF-8 && update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
RUN apt-get upgrade -y

RUN apt-get install -y g++ gdb cmake qtbase5-dev libqt5sql5-psql

RUN apt-get clean -y
RUN rm -rf /var/lib/apt/lists/*

WORKDIR /code
COPY . /code

CMD ["/code/run.sh"]
