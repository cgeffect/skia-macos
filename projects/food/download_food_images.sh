#!/bin/bash
mkdir -p projects/food/resources
echo "开始下载美食海报所需的图片资源..."

echo "下载背景图片..."
curl -L "https://newapi.kuaitu.cc/uploads/1750054541163_9c98648f5d.png" -o "projects/food/resources/food_bg_1.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054548496_df21267399.png" -o "projects/food/resources/food_bg_2.png"

echo "下载主图..."
curl -L "https://newapi.kuaitu.cc/uploads/1750054549044_fead84a2b6.png" -o "projects/food/resources/food_main.png"

echo "下载装饰图片..."
curl -L "https://newapi.kuaitu.cc/uploads/1750054549722_9b1365a3ea.png" -o "projects/food/resources/food_decor_1.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054550023_2673645e18.png" -o "projects/food/resources/food_decor_2.png"

echo "下载文字背景..."
curl -L "https://newapi.kuaitu.cc/uploads/1750054550299_e0772b809e.png" -o "projects/food/resources/food_text_bg_1.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054550744_e7156f0a23.png" -o "projects/food/resources/food_text_bg_2.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054550965_05593d9b14.png" -o "projects/food/resources/food_text_bg_3.png"

echo "图片资源下载完成！" 