FROM ubuntu:16.04

#ENV http_proxy=http://10.10.10.10:8080/
#ENV https_proxy=https://10.10.10.10:8080/
#ENV ftp_proxy=ftp://10.10.10.10:8080/
#ENV socks_proxy=socks://10.10.10.10:8080/

RUN DEBIAN_FRONTEND=noninteractive; apt-get update && apt-get install -y g++ python curl
RUN DEBIAN_FRONTEND=noninteractive; curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py && python get-pip.py && pip install Django && rm get-pip.py
