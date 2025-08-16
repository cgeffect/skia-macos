#!/usr/bin/env python3
import json
import re

def update_protocol():
    """更新协议文件，添加更多图片元素"""
    
    # 读取原始JSON文件
    with open('projects/long/original/long.json', 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    # 提取所有图片信息
    image_elements = []
    for i, obj in enumerate(data['objects']):
        if obj.get('type') == 'image':
            image_id = f"image_{i+1}"
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
                "path": f"projects/long/resources/long_{image_id}.png",
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
    
    # 读取现有的协议文件
    with open('projects/long/long_protocol.json', 'r', encoding='utf-8') as f:
        protocol = json.load(f)
    
    # 更新图片元素
    protocol['images'] = image_elements
    
    # 保存更新后的协议文件
    with open('projects/long/long_protocol.json', 'w', encoding='utf-8') as f:
        json.dump(protocol, f, ensure_ascii=False, indent=4)
    
    print(f"协议文件已更新，包含 {len(image_elements)} 个图片元素")

if __name__ == "__main__":
    update_protocol() 