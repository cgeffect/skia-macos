#!/usr/bin/env python3
import re

def generate_download_script():
    """生成完整的下载脚本"""
    
    # 读取所有URL
    with open('projects/long/all_urls.txt', 'r') as f:
        urls = f.read().strip().split('\n')
    
    # 生成下载脚本
    script_content = """#!/bin/bash

# 创建资源目录
mkdir -p projects/long/resources

# 下载图片资源
cd projects/long/resources

"""
    
    # 添加每个URL的下载命令
    for i, url in enumerate(urls):
        if i == 0:
            filename = "long_bg_1.png"
        elif i == 1:
            filename = "long_bg_2.png"
        elif i == 2:
            filename = "long_bg_3.png"
        else:
            filename = f"long_decor_{i-2}.png"
        
        script_content += f'curl -o {filename} "{url}"\n'
    
    script_content += '\necho "long项目所有图片下载完成！"\n'
    
    # 写入文件
    with open('projects/long/download_all_images.sh', 'w') as f:
        f.write(script_content)
    
    print(f"生成了下载脚本，包含 {len(urls)} 张图片")

if __name__ == "__main__":
    generate_download_script() 