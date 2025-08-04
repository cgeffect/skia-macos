git clone https://skia.googlesource.com/skia.git
cd skia
python3 tools/git-sync-deps
python3 bin/fetch-ninja


bin/gn gen out/Static --args='is_official_build=false'
ninja -C out/Static

