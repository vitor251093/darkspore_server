# Reference:
# https://askubuntu.com/questions/1126893/how-to-install-openssl-1-1-1-and-libssl-package

wget https://www.openssl.org/source/openssl-1.1.1b.tar.gz
sudo mkdir /opt/openssl
sudo tar xfvz openssl-1.1.1b.tar.gz --directory /opt/openssl
export LD_LIBRARY_PATH=/opt/openssl/lib
cd /opt/openssl/openssl-1.1.1b
sudo ./config --prefix=/opt/openssl --openssldir=/opt/openssl/ssl
sudo make
sudo make install

# cd libs/openssl-OpenSSL_1_1_1b
# ./config
# make
# make test
# make install
