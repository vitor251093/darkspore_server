# Dockerfile
# https://hub.docker.com/r/rwgrim/msys2-cross/dockerfile

FROM rwgrim/msys2-cross

WORKDIR /recap
COPY install-openssl.sh install-openssl.sh

RUN pacman --root /windows -Syu --noconfirm cmake

RUN bash ./install-openssl.sh

ENTRYPOINT ./cmake_run.sh