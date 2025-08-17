#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import json
import re
import os
import urllib.request
from urllib.parse import urlparse

def rgb_to_hex(rgb_str):
    """将RGB字符串转换为十六进制颜色"""
    if not rgb_str.startswith('rgb'):
        return rgb_str
    
    # 提取RGB值（支持小数点）
    match = re.search(r'rgb\(([\d.]+),\s*([\d.]+),\s*([\d.]+)\)', rgb_str)
    if match:
        r, g, b = float(match.group(1)), float(match.group(2)), float(match.group(3))
        return f"#{int(r):02x}{int(g):02x}{int(b):02x}"
    
    # 提取RGBA值
    match = re.search(r'rgba\(([\d.]+),\s*([\d.]+),\s*([\d.]+),\s*([\d.]+)\)', rgb_str)
    if match:
        r, g, b, a = float(match.group(1)), float(match.group(2)), float(match.group(3)), float(match.group(4))
        return f"#{int(r):02x}{int(g):02x}{int(b):02x}"
    
    return "#000000"

def download_image(url, local_path):
    """下载图片到本地"""
    try:
        # 确保目录存在
        os.makedirs(os.path.dirname(local_path), exist_ok=True)
        
        # 下载图片
        urllib.request.urlretrieve(url, local_path)
        
        print(f"✅ 下载成功: {local_path}")
        return True
    except Exception as e:
        print(f"❌ 下载失败 {url}: {e}")
        return False

def extract_filename_from_url(url):
    """从URL中提取文件名"""
    parsed = urlparse(url)
    filename = os.path.basename(parsed.path)
    if not filename:
        filename = "image.png"
    return filename

def translate_clothes():
    """翻译服装项目"""
    print("=== 开始翻译服装项目 ===")
    
    # 读取原始JSON文件
    with open('projects/clothes/original/clothes.json', 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    # 创建输出目录
    os.makedirs('projects/clothes/resources', exist_ok=True)
    os.makedirs('projects/clothes/output', exist_ok=True)
    
    # 提取画布信息
    canvas_width = 1000
    canvas_height = 1000
    canvas_background = "#FFFFFF"
    
    # 查找背景矩形
    for obj in data['objects']:
        if obj.get('type') == 'rect' and obj.get('id') == 'workspace':
            canvas_width = obj.get('width', 1000)
            canvas_height = obj.get('height', 1000)
            canvas_background = rgb_to_hex(obj.get('fill', 'rgba(255,255,255,1)'))
            break
    
    # 提取图片元素
    images = []
    image_index = 1
    
    for obj in data['objects']:
        if obj.get('type') == 'image':
            # 提取图片信息
            image_url = obj.get('src', '')
            if not image_url:
                continue
            
            # 生成本地文件名
            filename = extract_filename_from_url(image_url)
            local_path = f"projects/clothes/resources/{filename}"
            
            # 下载图片
            if download_image(image_url, local_path):
                # 创建图片元素
                image_element = {
                    "id": f"image_{image_index}",
                    "path": local_path,
                    "x": obj.get('left', 0),
                    "y": obj.get('top', 0),
                    "width": obj.get('width', 0),
                    "height": obj.get('height', 0),
                    "scaleX": obj.get('scaleX', 1.0),
                    "scaleY": obj.get('scaleY', 1.0),
                    "rotation": obj.get('angle', 0),
                    "opacity": obj.get('opacity', 1.0)
                }
                images.append(image_element)
                image_index += 1
                
                print(f"图片 {image_index-1}: {filename}")
                print(f"  位置: ({obj.get('left', 0)}, {obj.get('top', 0)})")
                print(f"  尺寸: {obj.get('width', 0)} x {obj.get('height', 0)}")
                print(f"  缩放: {obj.get('scaleX', 1.0)} x {obj.get('scaleY', 1.0)}")
                print(f"  旋转: {obj.get('angle', 0)}°")
                print(f"  透明度: {obj.get('opacity', 1.0)}")
    
    # 创建协议
    protocol = {
        "canvas": {
            "width": canvas_width,
            "height": canvas_height,
            "background": canvas_background,
            "debug": False
        },
        "images": images,
        "texts": [],
        "output": {
            "format": "png",
            "filename": "clothes_poster.png",
            "quality": 100
        }
    }
    
    # 保存协议文件
    protocol_path = "projects/clothes/clothes_protocol.json"
    with open(protocol_path, 'w', encoding='utf-8') as f:
        json.dump(protocol, f, ensure_ascii=False, indent=2)
    
    print(f"\n✅ 协议文件已保存: {protocol_path}")
    print(f"📊 统计信息:")
    print(f"  画布尺寸: {canvas_width} x {canvas_height}")
    print(f"  背景颜色: {canvas_background}")
    print(f"  图片数量: {len(images)}")
    print(f"  文本数量: {len(protocol['texts'])}")
    
    return protocol

if __name__ == "__main__":
    translate_clothes() 