FROM rwgrim/msys2-cross

WORKDIR /recap
COPY install-openssl.sh install-openssl.sh

RUN bash ./install-openssl.sh

ENTRYPOINT cd /recap && ./run.sh