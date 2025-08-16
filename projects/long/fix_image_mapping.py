#!/usr/bin/env python3
import json
import os

def fix_image_mapping():
    """修复图片映射关系，确保每个图片元素对应正确的图片文件"""
    
    # 读取原始JSON文件
    with open('projects/long/original/long.json', 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    # 提取所有图片对象及其URL
    image_objects = []
    for obj in data['objects']:
        if obj.get('type') == 'image':
            image_objects.append({
                'left': obj.get('left', 0),
                'top': obj.get('top', 0),
                'width': obj.get('width', 0),
                'height': obj.get('height', 0),
                'scaleX': obj.get('scaleX', 1.0),
                'scaleY': obj.get('scaleY', 1.0),
                'angle': obj.get('angle', 0),
                'opacity': obj.get('opacity', 1.0),
                'src': obj.get('src', '')
            })
    
    print(f"原始JSON中有 {len(image_objects)} 个图片对象")
    
    # 获取下载的图片文件列表
    resources_dir = 'projects/long/resources'
    downloaded_files = []
    for filename in os.listdir(resources_dir):
        if filename.endswith('.png'):
            downloaded_files.append(filename)
    
    downloaded_files.sort()
    print(f"下载了 {len(downloaded_files)} 个图片文件")
    
    # 创建正确的图片元素映射
    image_elements = []
    for i, (obj, filename) in enumerate(zip(image_objects, downloaded_files)):
        if i == 0:
            image_id = "background_1"
        elif i == 1:
            image_id = "background_2"
        elif i == 2:
            image_id = "background_3"
        else:
            image_id = f"decor_{i-2}"
        
        image_element = {
            "id": image_id,
            "path": f"projects/long/resources/{filename}",
            "x": obj['left'],
            "y": obj['top'],
            "width": obj['width'],
            "height": obj['height'],
            "scaleX": obj['scaleX'],
            "scaleY": obj['scaleY'],
            "rotation": obj['angle'],
            "opacity": obj['opacity']
        }
        image_elements.append(image_element)
        
        print(f"映射: {image_id} -> {filename}")
    
    # 读取现有的协议文件获取文本信息
    with open('projects/long/long_protocol.json', 'r', encoding='utf-8') as f:
        old_protocol = json.load(f)
    
    # 创建新的协议文件
    new_protocol = {
        "canvas": old_protocol["canvas"],
        "images": image_elements,
        "texts": old_protocol["texts"],
        "output": old_protocol["output"]
    }
    
    # 保存新的协议文件
    with open('projects/long/long_protocol.json', 'w', encoding='utf-8') as f:
        json.dump(new_protocol, f, ensure_ascii=False, indent=4)
    
    print(f"协议文件已更新，包含 {len(image_elements)} 个正确映射的图片元素")

if __name__ == "__main__":
    fix_image_mapping() 