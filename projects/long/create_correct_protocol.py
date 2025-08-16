#!/usr/bin/env python3
import json
import os

def create_correct_protocol():
    """创建正确的协议文件，只包含实际存在的图片"""
    
    # 获取实际存在的图片文件
    resources_dir = 'projects/long/resources'
    existing_files = []
    
    for filename in os.listdir(resources_dir):
        if filename.endswith('.png'):
            existing_files.append(filename)
    
    # 按文件名排序
    existing_files.sort()
    
    print(f"找到 {len(existing_files)} 个图片文件")
    
    # 读取原始JSON文件
    with open('projects/long/original/long.json', 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    # 提取图片信息
    image_objects = [obj for obj in data['objects'] if obj.get('type') == 'image']
    
    # 创建图片元素
    image_elements = []
    for i, (filename, obj) in enumerate(zip(existing_files, image_objects)):
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
            "x": obj.get('left', 0),
            "y": obj.get('top', 0),
            "width": obj.get('width', 0),
            "height": obj.get('height', 0),
            "scaleX": obj.get('scaleX', 1.0),
            "scaleY": obj.get('scaleY', 1.0),
            "rotation": obj.get('angle', 0),
            "opacity": obj.get('opacity', 1.0)
        }
        image_elements.append(image_element)
    
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
    
    print(f"协议文件已更新，包含 {len(image_elements)} 个图片元素")

if __name__ == "__main__":
    create_correct_protocol() 