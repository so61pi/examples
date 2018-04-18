FROM ubuntu:16.04

RUN DEBIAN_FRONTEND=noninteractive; apt-get update
RUN DEBIAN_FRONTEND=noninteractive; apt-get upgrade -y --fix-missing
RUN DEBIAN_FRONTEND=noninteractive; apt-get install -y --fix-missing g++ gdb cmake qtbase5-dev
RUN DEBIAN_FRONTEND=noninteractive; apt-get clean -y

RUN DEBIAN_FRONTEND=noninteractive; apt-get install -y --fix-missing libqt5sql5-psql

WORKDIR /code
COPY . /code

CMD ["/code/run.sh"]
