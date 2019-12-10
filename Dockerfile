#Version 0.1
FROM ubuntu:latest

MAINTAINER helight "helight@qq.com"

#设置root用户为后续命令的执行者
USER root

RUN rm -rf /etc/apt/sources.list
RUN echo 'deb http://mirrors.163.com/ubuntu/ bionic main restricted universe multiverse' >> /etc/apt/sources.list
RUN echo 'deb http://mirrors.163.com/ubuntu/ bionic-security main restricted universe multiverse' >> /etc/apt/sources.list
RUN echo 'deb http://mirrors.163.com/ubuntu/ bionic-updates main restricted universe multiverse' >> /etc/apt/sources.list
RUN echo 'deb http://mirrors.163.com/ubuntu/ bionic-proposed main restricted universe multiverse' >> /etc/apt/sources.list
RUN echo 'deb http://mirrors.163.com/ubuntu/ bionic-backports main restricted universe multiverse' >> /etc/apt/sources.list

#执行操作
RUN \
        apt-get update && \
        apt-get install -y build-essential && \
        apt-get install -y git net-tools iputils-ping curl && \
        apt-get install -y golang-1.10 golang && \
        apt-get install -y nginx && \
        apt-get install -y sqlite3 && \
        apt-get install -y g++ make autoconf && \
        apt-get install -y openssh-server && \
        apt-get install -y python3 python3-pip && \
        apt-get install -y vim
#        apt-get install -y mariadb-server mariadb-common && \
#        apt-get install -y php7.2  php7.2-common php7.2-json php7.2-readline php7.2-cli php-common
#对外暴露端口
EXPOSE 80 8080 3306

CMD ["/bin/bash"]
