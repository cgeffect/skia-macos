#!/bin/bash

echo "=== 测试所有文本显示模式 ==="

# 编译项目
echo "1. 编译项目..."
cd /Users/jason/Jason/opengl/skia-macos
./build.sh

if [ $? -ne 0 ]; then
    echo "❌ 编译失败"
    exit 1
fi

echo "✅ 编译成功"

# 测试单行模式
echo ""
echo "2. 测试单行模式..."
./build/skia projects/text_wrap_test/single_line_protocol.json
if [ $? -eq 0 ]; then
    echo "✅ 单行模式测试成功"
else
    echo "❌ 单行模式测试失败"
fi

# 测试多行模式
echo ""
echo "3. 测试多行模式..."
./build/skia projects/text_wrap_test/multi_line_protocol.json
if [ $? -eq 0 ]; then
    echo "✅ 多行模式测试成功"
else
    echo "❌ 多行模式测试失败"
fi

# 测试自动换行模式
echo ""
echo "4. 测试自动换行模式..."
./build/skia projects/text_wrap_test/word_wrap_protocol.json
if [ $? -eq 0 ]; then
    echo "✅ 自动换行模式测试成功"
else
    echo "❌ 自动换行模式测试失败"
fi

# 测试自适应模式
echo ""
echo "5. 测试自适应模式..."
./build/skia projects/text_wrap_test/auto_fit_protocol.json
if [ $? -eq 0 ]; then
    echo "✅ 自适应模式测试成功"
else
    echo "❌ 自适应模式测试失败"
fi

echo ""
echo "=== 所有测试完成 ==="
echo "生成的图片文件："
ls -la output/*_poster.png

echo ""
echo "📁 图片保存在 output/ 目录下："
echo "  - single_line_poster.png (单行模式)"
echo "  - multi_line_poster.png (多行模式)"
echo "  - word_wrap_poster.png (自动换行模式)"
echo "  - auto_fit_poster.png (自适应模式)"
echo ""
echo "🔍 Debug模式已启用，红色框框显示文本区域边界" 