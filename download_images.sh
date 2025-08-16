#!/bin/bash

# 创建图片目录
mkdir -p res/images

# 下载图片列表
declare -a images=(
    "https://newapi.kuaitu.cc/uploads/1750054231235_a219c50f9e.png"
    "https://newapi.kuaitu.cc/uploads/1750054232045_e1e929641b.png"
    "https://newapi.kuaitu.cc/uploads/1750054232655_409634990a.png"
    "https://newapi.kuaitu.cc/uploads/1750054233008_2da5bca274.png"
    "https://newapi.kuaitu.cc/uploads/1750054233279_b0f8246d9f.png"
    "https://newapi.kuaitu.cc/uploads/1750054239489_6936f05849.png"
    "https://newapi.kuaitu.cc/uploads/1750054240003_e4162c9011.png"
    "https://newapi.kuaitu.cc/uploads/1750054240544_e65341983c.png"
)

# 下载每个图片
for i in "${!images[@]}"; do
    url="${images[$i]}"
    filename="res/images/image_${i}.png"
    echo "下载图片 $((i+1))/${#images[@]}: $filename"
    curl -L "$url" -o "$filename"
done

echo "所有图片下载完成！" 