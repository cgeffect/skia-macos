git clone https://skia.googlesource.com/skia.git
cd skia
python3 tools/git-sync-deps
python3 bin/fetch-ninja


bin/gn gen out/Static --args='is_official_build=false'
ninja -C out/Static

skia 编译
https://zhuanlan.zhihu.com/p/666265475?share_code=CUsib6lMA77I&utm_psn=1935997978302214561