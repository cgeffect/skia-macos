#!/bin/bash

# 创建资源目录
mkdir -p projects/cup/resources

# 下载图片资源
cd projects/cup/resources

curl -o cup_bg_1.png "https://newapi.kuaitu.cc/uploads/1750059202923_1c961f65f4.png"
curl -o cup_bg_2.png "https://newapi.kuaitu.cc/uploads/1750059203408_145fe6d361.png"
curl -o cup_decor_1.png "https://newapi.kuaitu.cc/uploads/1750059204548_ae79a6fbaa.png"
curl -o cup_decor_2.png "https://newapi.kuaitu.cc/uploads/1750059205782_67c6df0f70.png"
curl -o cup_decor_3.png "https://newapi.kuaitu.cc/uploads/1750059206237_42773cba40.png"
curl -o cup_decor_4.png "https://newapi.kuaitu.cc/uploads/1750059206863_ea8a67c4d6.png"
curl -o cup_decor_5.png "https://newapi.kuaitu.cc/uploads/1750059207337_b53eb59517.png"
curl -o cup_decor_6.png "https://newapi.kuaitu.cc/uploads/1750059208381_6cd6ffbf00.png"
curl -o cup_decor_7.png "https://newapi.kuaitu.cc/uploads/1750059208869_a2ed0c32b3.png"
curl -o cup_decor_8.png "https://newapi.kuaitu.cc/uploads/1750059209227_01f0c1b957.png"
curl -o cup_decor_9.png "https://newapi.kuaitu.cc/uploads/1750059209414_f8c1c4a498.png"
curl -o cup_decor_10.png "https://newapi.kuaitu.cc/uploads/1750059209730_4eacfeeac3.png"
curl -o cup_decor_11.png "https://newapi.kuaitu.cc/uploads/1750059210135_8bf9a35f9d.png"
curl -o cup_decor_12.png "https://newapi.kuaitu.cc/uploads/1750059210335_cefd431194.png"
curl -o cup_decor_13.png "https://newapi.kuaitu.cc/uploads/1750059210683_65ab556625.png"
curl -o cup_decor_14.png "https://newapi.kuaitu.cc/uploads/1750059211138_8b7d8c4379.png"

echo "图片下载完成！" 