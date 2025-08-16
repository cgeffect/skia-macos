#!/usr/bin/env python3
import json
import re

def extract_image_positions():
    """从horizontal.json中提取所有图片的位置信息"""
    
    with open('projects/horizontal/original/horizontal.json', 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    image_positions = []
    
    for obj in data['objects']:
        if obj.get('type') == 'image':
            position = {
                'left': obj.get('left', 0),
                'top': obj.get('top', 0),
                'width': obj.get('width', 0),
                'height': obj.get('height', 0),
                'opacity': obj.get('opacity', 1.0),
                'src': obj.get('src', '')
            }
            image_positions.append(position)
    
    print("图片位置信息：")
    for i, pos in enumerate(image_positions):
        print(f"图片 {i+1}: left={pos['left']}, top={pos['top']}, width={pos['width']}, height={pos['height']}, opacity={pos['opacity']}")
        if pos['src']:
            print(f"  src: {pos['src']}")
        print()

if __name__ == "__main__":
    extract_image_positions() 