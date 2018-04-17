FROM ubuntu:16.04

RUN DEBIAN_FRONTEND=noninteractive; apt-get update
RUN DEBIAN_FRONTEND=noninteractive; apt-get upgrade -y
RUN DEBIAN_FRONTEND=noninteractive; apt-get install -y g++ gdb git cmake zsh curl build-essential python python3 vim
RUN DEBIAN_FRONTEND=noninteractive; apt-get clean -y

RUN TERM=xterm-256color sh -c "$(curl -fsSL https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh)" || true
RUN sed -i -e 's/ZSH_THEME="robbyrussell"/ZSH_THEME="ys"/' /root/.zshrc
