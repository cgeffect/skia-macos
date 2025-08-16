#!/usr/bin/env python3
import json

def fix_protocol():
    """修复协议文件中的图片路径"""
    
    # 读取协议文件
    with open('projects/long/long_protocol.json', 'r', encoding='utf-8') as f:
        protocol = json.load(f)
    
    # 修复图片路径
    for image in protocol['images']:
        if image['id'] == 'background_1':
            image['path'] = 'projects/long/resources/long_bg_1.png'
        elif image['id'] == 'background_2':
            image['path'] = 'projects/long/resources/long_bg_2.png'
        elif image['id'] == 'background_3':
            image['path'] = 'projects/long/resources/long_bg_3.png'
        else:
            # 对于装饰图片，保持原来的路径格式
            pass
    
    # 保存修复后的协议文件
    with open('projects/long/long_protocol.json', 'w', encoding='utf-8') as f:
        json.dump(protocol, f, ensure_ascii=False, indent=4)
    
    print("协议文件中的图片路径已修复")

if __name__ == "__main__":
    fix_protocol() 