FROM ubuntu:latest
WORKDIR /lexington

ARG USER_ID
ARG GROUP_ID

RUN apt-get update && \
	apt-get install -y bcc gcc make nasm fish

RUN addgroup --gid $GROUP_ID user
RUN adduser --disabled-password --gecos '' --uid $USER_ID --gid $GROUP_ID user
USER user
