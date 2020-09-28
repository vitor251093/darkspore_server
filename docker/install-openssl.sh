wget https://www.openssl.org/source/openssl-1.1.1b.tar.gz
mkdir /opt/openssl
tar xfvz openssl-1.1.1b.tar.gz --directory /opt/openssl
#export LD_LIBRARY_PATH=/opt/openssl/lib
cd /opt/openssl/openssl-1.1.1b
./config --prefix=/opt/openssl --openssldir=/opt/openssl/ssl
make
make install