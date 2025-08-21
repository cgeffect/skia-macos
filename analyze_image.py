#!/usr/bin/env python3
"""
图片分析脚本 - 检测文本渲染问题
"""

import sys
import os
from PIL import Image
import numpy as np

def analyze_image(image_path):
    """分析图片内容，检测文本区域和红色边框"""
    try:
        # 打开图片
        img = Image.open(image_path)
        print(f"\n=== 分析图片: {os.path.basename(image_path)} ===")
        print(f"尺寸: {img.size}")
        print(f"模式: {img.mode}")
        
        # 转换为numpy数组
        img_array = np.array(img)
        
        # 检测红色边框 (RGB: 255, 0, 0)
        red_pixels = np.where((img_array[:, :, 0] == 255) & 
                             (img_array[:, :, 1] == 0) & 
                             (img_array[:, :, 2] == 0))
        
        if len(red_pixels[0]) > 0:
            print(f"✅ 检测到红色边框像素: {len(red_pixels[0])} 个")
            print(f"红色边框位置范围: X({red_pixels[1].min()}-{red_pixels[1].max()}), Y({red_pixels[0].min()}-{red_pixels[0].max()})")
        else:
            print("❌ 未检测到红色边框")
        
        # 检测非白色像素 (可能的文本)
        non_white_pixels = np.where((img_array[:, :, 0] != 255) | 
                                   (img_array[:, :, 1] != 255) | 
                                   (img_array[:, :, 2] != 255))
        
        if len(non_white_pixels[0]) > 0:
            print(f"✅ 检测到非白色像素: {len(non_white_pixels[0])} 个")
            print(f"非白色区域范围: X({non_white_pixels[1].min()}-{non_white_pixels[1].max()}), Y({non_white_pixels[0].min()}-{non_white_pixels[0].max()})")
            
            # 分析颜色分布
            unique_colors = np.unique(img_array.reshape(-1, 3), axis=0)
            print(f"颜色种类: {len(unique_colors)}")
            
            # 显示主要颜色
            print("主要颜色 (RGB):")
            for i, color in enumerate(unique_colors[:10]):  # 显示前10种颜色
                if not np.array_equal(color, [255, 255, 255]):  # 排除白色
                    print(f"  {i+1}. {color}")
        else:
            print("❌ 未检测到非白色像素 (可能全是白色背景)")
        
        # 检测文本区域问题
        if len(red_pixels[0]) > 0 and len(non_white_pixels[0]) > 0:
            # 检查文本是否在红色边框内
            red_x_min, red_x_max = red_pixels[1].min(), red_pixels[1].max()
            red_y_min, red_y_max = red_pixels[0].min(), red_pixels[0].max()
            
            text_x_min, text_x_max = non_white_pixels[1].min(), non_white_pixels[1].max()
            text_y_min, text_y_max = non_white_pixels[0].min(), non_white_pixels[0].max()
            
            # 检查文本是否超出红色边框
            if (text_x_min < red_x_min or text_x_max > red_x_max or 
                text_y_min < red_y_min or text_y_max > red_y_max):
                print("⚠️  检测到文本超出红色边框范围!")
                print(f"红色边框: X({red_x_min}-{red_x_max}), Y({red_y_min}-{red_y_max})")
                print(f"文本区域: X({text_x_min}-{text_x_max}), Y({text_y_min}-{text_y_max})")
            else:
                print("✅ 文本在红色边框范围内")
        
        return True
        
    except Exception as e:
        print(f"❌ 分析图片失败: {e}")
        return False

def main():
    """主函数"""
    if len(sys.argv) < 2:
        print("用法: python3 analyze_image.py <图片路径>")
        print("示例: python3 analyze_image.py output/single_line_poster.png")
        return
    
    image_path = sys.argv[1]
    
    if not os.path.exists(image_path):
        print(f"❌ 图片文件不存在: {image_path}")
        return
    
    analyze_image(image_path)

if __name__ == "__main__":
    main() 