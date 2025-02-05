export LDFLAGS="-Wl,--copy-dt-needed-entries"
cmake . -B build -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-Wno-deprecated-declarations -Wno-narrowing" || exit 1
cmake --build build || exit 1

rm build/config.xml || true

rm -r build/data || true
cp -r res/data build/

mkdir -p build/storage/user
touch build/storage/user/PLACEHOLDER

cd build
./recap_server