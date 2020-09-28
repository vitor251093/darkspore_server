# cd libs/openssl-OpenSSL_1_1_1b
# ./config
# make
# make test
# make install

/usr/src/mxe/usr/bin/x86_64-w64-mingw32.shared-cmake . \
     -DCMAKE_BUILD_TYPE=Release \
     -DPROGRAMFILES=C:\\Program\ Files \
     -DPROGRAMFILESX86=C:\\Program\ Files\ \(x86\)

make
