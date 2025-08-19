#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
T恤设计翻译脚本
将Fabric.js格式的设计文件转换为Skia渲染协议
参考clothes项目的实现方式
"""

import json
import os
import requests
import re
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

def download_image(url, save_path):
    """下载图片并保存到本地"""
    try:
        response = requests.get(url, timeout=30)
        response.raise_for_status()
        
        # 确保目录存在
        os.makedirs(os.path.dirname(save_path), exist_ok=True)
        
        # 保存图片
        with open(save_path, 'wb') as f:
            f.write(response.content)
        
        print(f"✅ 下载成功: {url} -> {save_path}")
        return True
    except Exception as e:
        print(f"❌ 下载失败: {url} - {e}")
        return False

def extract_filename_from_url(url):
    """从URL中提取文件名"""
    parsed = urlparse(url)
    path = parsed.path
    filename = os.path.basename(path)
    
    # 如果没有扩展名，根据URL判断
    if not os.path.splitext(filename)[1]:
        if 'png' in url.lower():
            filename += '.png'
        elif 'webp' in url.lower():
            filename += '.webp'
        elif 'jpg' in url.lower() or 'jpeg' in url.lower():
            filename += '.jpg'
    
    return filename

def parse_fabric_json(json_file):
    """解析Fabric.js JSON文件"""
    with open(json_file, 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    canvas_width = 1000
    canvas_height = 1000
    canvas_background = "#FFFFFF"
    
    # 提取背景
    for obj in data.get('objects', []):
        if obj['type'] == 'rect' and obj.get('id') == 'workspace':
            canvas_width = obj.get('width', 1000)
            canvas_height = obj.get('height', 1000)
            canvas_background = rgb_to_hex(obj.get('fill', 'rgba(255,255,255,1)'))
            break
    
    # 提取图片元素
    images = []
    image_index = 1
    
    for obj in data.get('objects', []):
        if obj['type'] == 'image':
            # 提取图片信息
            image_url = obj.get('src', '')
            if not image_url:
                continue
            
            # 生成本地文件名
            filename = extract_filename_from_url(image_url)
            local_path = f"projects/tshirt/resources/{filename}"
            
            # 下载图片
            if download_image(image_url, local_path):
                # 创建图片元素 - 参考clothes项目的格式
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
    
    return {
        'canvas_width': canvas_width,
        'canvas_height': canvas_height,
        'canvas_background': canvas_background,
        'images': images
    }

def create_skia_protocol(parsed_data):
    """创建Skia渲染协议 - 参考clothes项目格式"""
    protocol = {
        "canvas": {
            "width": parsed_data['canvas_width'],
            "height": parsed_data['canvas_height'],
            "background": parsed_data['canvas_background'],
            "debug": False
        },
        "images": parsed_data['images'],
        "texts": [],
        "output": {
            "format": "png",
            "filename": "tshirt_poster.png",
            "quality": 100
        }
    }
    
    return protocol

def main():
    # 项目路径
    project_dir = os.path.dirname(os.path.abspath(__file__))
    original_dir = os.path.join(project_dir, 'original')
    resources_dir = os.path.join(project_dir, 'resources')
    output_dir = os.path.join(project_dir, 'output')
    
    # 创建目录
    os.makedirs(resources_dir, exist_ok=True)
    os.makedirs(output_dir, exist_ok=True)
    
    # 解析JSON文件
    json_file = os.path.join(original_dir, 'tshirt.json')
    print(f"📄 解析文件: {json_file}")
    
    parsed_data = parse_fabric_json(json_file)
    print(f"✅ 解析完成:")
    print(f"   - 画布尺寸: {parsed_data['canvas_width']}x{parsed_data['canvas_height']}")
    print(f"   - 背景颜色: {parsed_data['canvas_background']}")
    print(f"   - 图片数量: {len(parsed_data['images'])}")
    
    # 创建Skia协议
    print("\n🔄 创建Skia渲染协议...")
    protocol = create_skia_protocol(parsed_data)
    
    # 保存协议文件
    protocol_file = os.path.join(project_dir, 'tshirt_protocol.json')
    with open(protocol_file, 'w', encoding='utf-8') as f:
        json.dump(protocol, f, ensure_ascii=False, indent=2)
    
    print(f"✅ 协议文件已保存: {protocol_file}")
    
    # 统计信息
    print(f"\n📊 翻译统计:")
    print(f"   - 画布尺寸: {protocol['canvas']['width']} x {protocol['canvas']['height']}")
    print(f"   - 背景颜色: {protocol['canvas']['background']}")
    print(f"   - 图片数量: {len(protocol['images'])}")
    print(f"   - 文本数量: {len(protocol['texts'])}")
    print(f"   - 资源文件: {len(os.listdir(resources_dir)) if os.path.exists(resources_dir) else 0}")
    
    print(f"\n🎯 下一步:")
    print(f"   1. 检查协议文件: {protocol_file}")
    print(f"   2. 检查资源文件: {resources_dir}")
    print(f"   3. 运行渲染: ./build/skia {protocol_file}")

if __name__ == "__main__":
    main() 