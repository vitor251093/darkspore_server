cd "${0%/*}"

cmake . \
     -DCMAKE_BUILD_TYPE=Release \
     -DPROGRAMFILES=C:\\Program\ Files \
     -DPROGRAMFILESX86=C:\\Program\ Files\ \(x86\)

make
