#!/bin/bash
mkdir -p projects/spring/resources
echo "开始下载春天海报所需的图片资源..."

echo "下载背景图片..."
curl -L "https://newapi.kuaitu.cc/uploads/1750054598419_8a2b4e2d78.png" -o "projects/spring/resources/spring_bg_1.png"

echo "下载主图..."
curl -L "https://newapi.kuaitu.cc/uploads/1750054599336_5e111baccb.png" -o "projects/spring/resources/spring_main.png"

echo "下载装饰图片..."
curl -L "https://newapi.kuaitu.cc/uploads/1750054600006_17b6563a76.png" -o "projects/spring/resources/spring_decor_1.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054603917_490e56740f.png" -o "projects/spring/resources/spring_decor_2.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054604417_b5c0827813.png" -o "projects/spring/resources/spring_decor_3.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054604812_5ddd7c13a1.png" -o "projects/spring/resources/spring_decor_4.png"

echo "下载文字背景..."
curl -L "https://newapi.kuaitu.cc/uploads/1750054605178_cd51451a0a.png" -o "projects/spring/resources/spring_text_bg_1.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054605840_ee8397990e.png" -o "projects/spring/resources/spring_text_bg_2.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054606347_2bfb2477d4.png" -o "projects/spring/resources/spring_text_bg_3.png"

echo "下载装饰元素..."
curl -L "https://newapi.kuaitu.cc/uploads/1750054606654_ef901dbd2b.png" -o "projects/spring/resources/spring_element_1.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054606886_494023d540.png" -o "projects/spring/resources/spring_element_2.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054607228_07e56814f5.png" -o "projects/spring/resources/spring_element_3.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054607637_93e33b9615.png" -o "projects/spring/resources/spring_element_4.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054607872_dec5378bef.png" -o "projects/spring/resources/spring_element_5.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054608069_074f066678.png" -o "projects/spring/resources/spring_element_6.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054608443_772e0fb06b.png" -o "projects/spring/resources/spring_element_7.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054608646_c23537cb14.png" -o "projects/spring/resources/spring_element_8.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054608934_75ceb7793d.png" -o "projects/spring/resources/spring_element_9.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054609236_44a240c647.png" -o "projects/spring/resources/spring_element_10.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054609570_4ae01a2683.png" -o "projects/spring/resources/spring_element_11.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054609928_d63fae87fb.png" -o "projects/spring/resources/spring_element_12.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054610239_8caa9528e2.png" -o "projects/spring/resources/spring_element_13.png"
curl -L "https://newapi.kuaitu.cc/uploads/1750054610612_7935c487c8.png" -o "projects/spring/resources/spring_element_14.png"

echo "图片资源下载完成！" 