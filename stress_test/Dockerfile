FROM ubuntu:jammy

ENV TZ=Asia/Tokyo

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && \
    echo $TZ > /etc/timezone

RUN apt update && \
	apt install -y \
		siege \
		curl \
		build-essential \
		php-cgi && \
	apt clean

WORKDIR /home/webserv

EXPOSE 8081

# CMD [ "siege", "http://localhost:8080" ]
