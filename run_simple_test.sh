#!/bin/bash

# 简单图片对比测试脚本
# 不修改任何现有源码，独立运行

set -e  # 遇到错误立即退出

echo "=== 简单图片对比测试 ==="
echo "时间: $(date)"
echo ""

# 检查是否已编译
if [ ! -f "build/simple_test" ]; then
    echo "❌ 简单测试程序未编译，正在编译..."
    ./build.sh
fi

# 创建必要的目录
mkdir -p tests/baseline
mkdir -p tests/diff
mkdir -p output

echo "📁 目录结构:"
echo "  - tests/baseline/  : 基线图片"
echo "  - tests/diff/      : 差异图片"
echo "  - output/          : 当前输出"
echo ""

# 运行一致性测试（检查渲染是否确定）
echo "🔍 运行一致性测试..."
echo ""

# 测试几个关键项目的一致性
for project in trip food clothes; do
    echo "测试项目: $project"
    ./build/simple_test consistency $project 3
    echo ""
done

# 运行完整的图片对比测试
echo "🔄 运行完整图片对比测试..."
echo ""

./build/simple_test run

echo ""
echo "=== 测试完成 ==="
echo "时间: $(date)"

# 显示测试结果摘要
echo ""
echo "📊 测试结果摘要:"
echo ""

if [ -d "tests/diff" ] && [ "$(ls -A tests/diff)" ]; then
    echo "⚠️  发现差异图片:"
    ls -la tests/diff/
    echo ""
    echo "请检查差异图片以了解具体变化"
else
    echo "✅ 所有测试通过，未发现差异"
fi

echo ""
echo "💡 使用说明:"
echo "  - 运行测试: ./run_simple_test.sh"
echo "  - 更新基线: ./build/simple_test update <project_name>"
echo "  - 设置容差: ./build/simple_test tolerance <value>"
echo "  - 一致性测试: ./build/simple_test consistency <project_name> [iterations]"
echo ""
echo "🎯 特点:"
echo "  - 不修改任何现有源码"
echo "  - 独立运行渲染程序"
echo "  - 纯单元测试设计" 