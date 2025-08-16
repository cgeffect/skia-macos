#!/usr/bin/env python3
import json
import re

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

def should_add_vertical_line_breaks(width, height, text):
    """判断是否需要添加竖版换行符"""
    # 宽度很窄且高度明显大于宽度，且文本长度大于1
    if width > 0 and height > 0 and len(text) > 1:
        if width < 80 and height > width * 1.5:
            return True
    return False

def add_vertical_line_breaks(text):
    """为文本添加竖版换行符"""
    # 在每个字符之间添加\r换行符
    return '\r'.join(text)

def add_fixed_width_line_breaks(text, width, fontSize):
    """根据固定宽度添加换行符（每4个字符换行）"""
    if not text:
        return text
    
    # 计算每行大约能容纳的字符数
    # 假设每个字符大约占用 fontSize 的宽度
    chars_per_line = max(1, int(width / fontSize * 1.2))  # 1.2是调整系数
    
    # 如果宽度很窄，强制每4个字符换行
    if width < 80:
        chars_per_line = 4
    
    result = []
    for i in range(0, len(text), chars_per_line):
        result.append(text[i:i + chars_per_line])
    
    return '\r'.join(result)

def is_shadow_text(obj, all_objects):
    """判断是否为阴影文字"""
    # 检查是否有相同文字但位置稍微偏移且颜色不同的对象
    text = obj.get('text', '')
    left = obj.get('left', 0)
    top = obj.get('top', 0)
    fill = obj.get('fill', '')
    
    for other_obj in all_objects:
        if (other_obj.get('type') == 'textbox' and 
            other_obj.get('text') == text and
            other_obj.get('id') != obj.get('id')):
            
            other_left = other_obj.get('left', 0)
            other_top = other_obj.get('top', 0)
            other_fill = other_obj.get('fill', '')
            
            # 检查位置是否接近（偏移1-5像素）
            if (abs(left - other_left) <= 5 and 
                abs(top - other_top) <= 5 and
                fill != other_fill):
                
                # 判断哪个是阴影（深色的是阴影）
                if (fill == "rgb(103.00215, 53.0043, 43.0032)" or 
                    fill == "rgb(103, 53, 43)"):
                    return True
    
    return False

def find_main_text(obj, all_objects):
    """找到对应的主文字对象"""
    text = obj.get('text', '')
    left = obj.get('left', 0)
    top = obj.get('top', 0)
    
    for other_obj in all_objects:
        if (other_obj.get('type') == 'textbox' and 
            other_obj.get('text') == text and
            other_obj.get('id') != obj.get('id')):
            
            other_left = other_obj.get('left', 0)
            other_top = other_obj.get('top', 0)
            other_fill = other_obj.get('fill', '')
            
            # 检查位置是否接近且颜色不同
            if (abs(left - other_left) <= 5 and 
                abs(top - other_top) <= 5 and
                other_fill != obj.get('fill', '')):
                
                # 返回主文字对象（金黄色的）
                if other_fill == "rgb(251.99609999999998, 216.9999, 53.0043)":
                    return other_obj
    
    return None

def is_main_text(obj, all_objects):
    """判断是否为主文字（金黄色的）"""
    fill = obj.get('fill', '')
    return fill == "rgb(251.99609999999998, 216.9999, 53.0043)"

def simple_translate():
    """简单翻译：直接按照原始JSON的顺序"""
    
    # 读取原始JSON文件
    with open('projects/long/original/long.json', 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    # 获取所有图片对象
    image_objects = []
    for obj in data['objects']:
        if obj.get('type') == 'image':
            image_objects.append(obj)
    
    print(f"找到 {len(image_objects)} 个图片对象")
    
    # 获取所有文本对象
    text_objects = []
    for obj in data['objects']:
        if obj.get('type') == 'textbox':
            text_objects.append(obj)
    
    print(f"找到 {len(text_objects)} 个文本对象")
    
    # 创建图片元素
    images = []
    for i, obj in enumerate(image_objects):
        if i == 0:
            image_id = "background_1"
            filename = "long_bg_1.png"
        elif i == 1:
            image_id = "background_2"
            filename = "long_bg_2.png"
        elif i == 2:
            image_id = "background_3"
            filename = "long_bg_3.png"
        else:
            image_id = f"decor_{i-2}"
            filename = f"long_decor_{i-2}.png"
        
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
        images.append(image_element)
        print(f"图片 {i+1}: {image_id} -> {filename}")
    
    # 创建文本元素
    texts = []
    processed_shadows = set()  # 记录已处理的阴影文字ID
    
    for i, obj in enumerate(text_objects):
        # 跳过已处理的阴影文字
        if obj.get('id') in processed_shadows:
            continue
            
        # 提取原始字体信息
        font_family = obj.get('fontFamily', 'SourceHanSansCN-Normal')
        font_size = obj.get('fontSize', 12)
        fill_color = obj.get('fill', '#000000')
        width = obj.get('width', 0)
        height = obj.get('height', 0)
        text_content = obj.get('text', '')
        
        # 转换颜色格式
        fill_color = rgb_to_hex(fill_color)
        
        # 检查是否为阴影文字
        if is_shadow_text(obj, data['objects']):
            # 找到对应的主文字
            main_text_obj = find_main_text(obj, data['objects'])
            if main_text_obj:
                # 使用主文字的信息，但添加阴影效果
                main_fill_color = rgb_to_hex(main_text_obj.get('fill', '#000000'))
                shadow_offset_x = obj.get('left', 0) - main_text_obj.get('left', 0)
                shadow_offset_y = obj.get('top', 0) - main_text_obj.get('top', 0)
                
                text_element = {
                    "id": f"text_{i+1}",
                    "content": text_content,
                    "x": main_text_obj.get('left', 0),
                    "y": main_text_obj.get('top', 0),
                    "fontFamily": font_family,
                    "fontSize": font_size,
                    "fillColor": main_fill_color,
                    "hasShadow": True,
                    "shadowColor": fill_color,
                    "shadowDx": shadow_offset_x,
                    "shadowDy": shadow_offset_y,
                    "shadowSigma": 2.0,
                    "opacity": obj.get('opacity', 1.0),
                    "scaleX": obj.get('scaleX', 1.0),
                    "scaleY": obj.get('scaleY', 1.0),
                    "width": width,
                    "height": height
                }
                texts.append(text_element)
                processed_shadows.add(main_text_obj.get('id'))
                print(f"文本 {i+1}: {obj.get('text', '')[:20]}... (带阴影效果, 主色: {main_fill_color}, 阴影色: {fill_color})")
                continue
        
        # 检查是否需要根据固定宽度添加换行符
        # 对于宽度较窄的文本，按照每4个字符换行
        if width > 0 and width < 80 and len(text_content) > 4:
            # 移除原始文本中的\r换行符，重新计算
            text_content = text_content.replace('\r', '')
            text_content = add_fixed_width_line_breaks(text_content, width, font_size)
            print(f"文本 {i+1}: 添加固定宽度换行 - {obj.get('text', '')[:20]}... (宽度: {width}, 字体大小: {font_size})")
        
        # 特殊处理：这三个文本需要竖版显示
        vertical_texts = ["亮丽金黄表皮", "蛋香浓郁", "精选原材"]
        if text_content in vertical_texts:
            # 为竖版文本添加换行符，每个字符一行
            text_content = '\r'.join(text_content)
            print(f"文本 {i+1}: 添加竖版换行 - {text_content}")
        
        text_element = {
            "id": f"text_{i+1}",
            "content": text_content,
            "x": obj.get('left', 0),
            "y": obj.get('top', 0),
            "fontFamily": font_family,
            "fontSize": font_size,
            "fillColor": fill_color,
            "opacity": obj.get('opacity', 1.0),
            "scaleX": obj.get('scaleX', 1.0),
            "scaleY": obj.get('scaleY', 1.0),
            "width": width,
            "height": height
        }
        texts.append(text_element)
        print(f"文本 {i+1}: {obj.get('text', '')[:20]}... (字体: {font_family}, 大小: {font_size}, 颜色: {fill_color}, 宽度: {width})")
    
    # 创建协议
    protocol = {
        "canvas": {
            "width": 732,
            "height": 5080,
            "backgroundColor": "#FFFFFF"
        },
        "images": images,
        "texts": texts,
        "output": {
            "format": "png",
            "quality": 100,
            "filename": "long_poster.png"
        }
    }
    
    # 保存协议文件
    with open('projects/long/long_protocol.json', 'w', encoding='utf-8') as f:
        json.dump(protocol, f, ensure_ascii=False, indent=4)
    
    print(f"协议文件已创建：{len(images)} 个图片，{len(texts)} 个文本")

if __name__ == "__main__":
    simple_translate() 