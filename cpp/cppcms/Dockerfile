FROM ubuntu:16.04

RUN DEBIAN_FRONTEND=noninteractive; apt-get update
RUN DEBIAN_FRONTEND=noninteractive; apt-get upgrade -y
RUN DEBIAN_FRONTEND=noninteractive; apt-get install -y g++ git cmake libpcre3-dev zlib1g-dev libgcrypt11-dev libicu-dev python libboost-all-dev
RUN DEBIAN_FRONTEND=noninteractive; apt-get clean -y

WORKDIR /data
RUN git clone https://github.com/artyom-beilis/cppcms.git
RUN cd cppcms && mkdir build && cd build && cmake .. && make && make install
RUN ldconfig
