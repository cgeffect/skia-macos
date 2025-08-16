#!/bin/bash

# 创建图片目录
mkdir -p res/images

echo "开始下载防晒海报所需的图片资源..."

# 下载背景图片
echo "下载背景图片..."
curl -L "https://newapi.kuaitu.cc/uploads/1750055896004_5873ae6dee.png" -o "res/images/sunscreen_bg_1.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750055897166_9044386c55.png" -o "res/images/sunscreen_bg_2.png"

# 下载产品图片
echo "下载产品图片..."
curl -L "https://newapi.kuaitu.cc/uploads/1750055909963_c45b313606.png" -o "res/images/sunscreen_product_1.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750055910288_b393d5958b.png" -o "res/images/sunscreen_product_2.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750055910784_4a60647266.png" -o "res/images/sunscreen_product_3.png"

# 下载装饰图片
echo "下载装饰图片..."
curl -L "https://newapi.kuaitu.cc/uploads/1750055911396_ee850dcd4b.png" -o "res/images/sunscreen_decor.png"

echo "图片资源下载完成！" 