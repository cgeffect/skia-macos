#!/bin/bash

# 创建资源目录
mkdir -p projects/dessert/resources

# 下载图片资源
cd projects/dessert/resources

curl -o dessert_bg_1.png "https://newapi.kuaitu.cc/uploads/1750064593476_7b8a7d34b3.png"
curl -o dessert_bg_2.png "https://newapi.kuaitu.cc/uploads/1750064601238_a02e4e2fce.png"
curl -o dessert_decor_1.png "https://newapi.kuaitu.cc/uploads/1750064614513_0097508463.png"
curl -o dessert_decor_2.png "https://newapi.kuaitu.cc/uploads/1750064614937_d926d49d85.png"
curl -o dessert_decor_3.png "https://newapi.kuaitu.cc/uploads/1750064616678_5c0be19c12.png"

echo "立春主题图片下载完成！" 