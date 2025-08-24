#!/bin/bash

# Skia 文本样式演示运行脚本

echo "🎨 Skia 文本样式功能演示"
echo "=========================="

# 检查是否已编译
if [ ! -f "build/text_styles_demo" ]; then
    echo "📦 检测到项目未编译，开始编译..."
    ./build.sh
    if [ $? -ne 0 ]; then
        echo "❌ 编译失败！"
        exit 1
    fi
fi

# 创建输出目录
mkdir -p output

echo ""
echo "🚀 开始运行文本样式演示..."
echo ""

# 运行演示
./build/text_styles_demo

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ 所有演示完成！"
    echo ""
    echo "📁 输出文件位置："
    echo "   - output/basic_font_styles_demo.png      (基础字体样式)"
    echo "   - output/text_effects_demo.png           (文本效果演示)"
    echo "   - output/display_modes_demo.png          (四种显示模式)"
    echo "   - output/colors_and_transforms_demo.png  (颜色和变换)"
    echo "   - output/text_alignment_demo.png         (对齐和布局)"
    echo "   - output/mixed_languages_demo.png        (多语言支持)"
    echo "   - output/rich_text_demo.png              (富文本效果)"
    echo ""
    echo "🎯 你也可以单独运行某个演示："
    echo "   ./build/simple_example projects/text_styles_protocols/basic_font_styles.json"
    echo ""
    echo "📚 查看详细说明："
    echo "   cat examples/README_TEXT_STYLES.md"
else
    echo ""
    echo "❌ 演示运行失败！"
    echo "请检查错误信息并重试。"
    exit 1
fi
