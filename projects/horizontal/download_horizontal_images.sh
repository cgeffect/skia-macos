#!/bin/bash

# 创建资源目录
mkdir -p projects/horizontal/resources

# 下载图片资源
cd projects/horizontal/resources

curl -o horizontal_bg_1.png "https://newapi.kuaitu.cc/uploads/1750056463159_72bcaa2267.png"
curl -o horizontal_decor_1.png "https://newapi.kuaitu.cc/uploads/1750056464465_c7d448bf28.png"
curl -o horizontal_decor_2.png "https://newapi.kuaitu.cc/uploads/1750056465029_2da30ff2e4.png"
curl -o horizontal_decor_3.png "https://newapi.kuaitu.cc/uploads/1750056465428_a077c119f5.png"
curl -o horizontal_decor_4.png "https://newapi.kuaitu.cc/uploads/1750056466607_b1efce5a33.png"
curl -o horizontal_decor_5.png "https://newapi.kuaitu.cc/uploads/1750056467228_d8651d654e.png"

echo "图片下载完成！" 