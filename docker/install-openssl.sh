# References
# https://www.howtoforge.com/tutorial/how-to-install-openssl-from-source-on-linux/
# https://wiki.openssl.org/index.php/Compilation_and_Installation#Configure_Options
# https://github.com/openssl/openssl/issues/7686
# https://hub.docker.com/r/rwgrim/msys2-cross

apt update
apt install build-essential checkinstall zlib1g-dev -y

cd /usr/local/src/
wget https://www.openssl.org/source/openssl-1.1.1b.tar.gz
tar -xf openssl-1.1.1b.tar.gz
cd openssl-1.1.1b
./config --prefix=/usr/local/ssl --openssldir=/usr/local/ssl '-Wl,-rpath,$(LIBRPATH)'
make
make test
make install

cd /etc/ld.so.conf.d/
cat "/usr/local/ssl/lib" >> ./openssl-1.0.2o.conf
ldconfig -v
