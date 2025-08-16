#!/usr/bin/env python3
import json
import re

def extract_text_info():
    """从cup.json中提取所有文本的信息"""
    
    with open('projects/cup/original/cup.json', 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    text_info = []
    
    for obj in data['objects']:
        if obj.get('type') == 'textbox':
            text_data = {
                'text': obj.get('text', ''),
                'left': obj.get('left', 0),
                'top': obj.get('top', 0),
                'fontSize': obj.get('fontSize', 12),
                'fontFamily': obj.get('fontFamily', 'Arial'),
                'fill': obj.get('fill', '#000000'),
                'opacity': obj.get('opacity', 1.0),
                'scaleX': obj.get('scaleX', 1.0),
                'scaleY': obj.get('scaleY', 1.0)
            }
            text_info.append(text_data)
    
    print("文本信息：")
    for i, text in enumerate(text_info):
        print(f"文本 {i+1}:")
        print(f"  内容: {text['text']}")
        print(f"  位置: left={text['left']}, top={text['top']}")
        print(f"  字体: {text['fontFamily']}, 大小: {text['fontSize']}")
        print(f"  颜色: {text['fill']}")
        print(f"  透明度: {text['opacity']}")
        print(f"  缩放: scaleX={text['scaleX']}, scaleY={text['scaleY']}")
        print()

if __name__ == "__main__":
    extract_text_info() 