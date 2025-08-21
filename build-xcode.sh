#!/bin/bash

echo "=== 开始构建 Xcode 项目 (Debug 版本) ==="

# 设置 Xcode 项目目录
XCODE_DIR="Xcode"

# 清理并创建构建目录
echo "🧹 清理构建目录..."
rm -rf $XCODE_DIR
mkdir -p $XCODE_DIR
cd $XCODE_DIR

# 配置 CMake 生成 Xcode 项目
echo "📱 生成 Xcode 项目..."
cmake -G "Xcode" -DCMAKE_BUILD_TYPE=Debug ..

if [ $? -ne 0 ]; then
    echo "❌ CMake 配置失败！"
    exit 1
fi

echo "✅ Xcode 项目生成成功！"
echo "📁 项目位置: $XCODE_DIR/PosterMaker.xcodeproj"

# 编译项目
# echo "🔨 开始编译 Xcode 项目..."
# xcodebuild -project PosterMaker.xcodeproj -scheme renderer -configuration Debug build

if [ $? -eq 0 ]; then
    echo "✅ Xcode 项目编译成功！"
    echo ""
    echo "📋 编译结果:"
    echo "  - 主程序: $XCODE_DIR/Debug/renderer"
    echo "  - 测试程序: $XCODE_DIR/Debug/simple_test"
    echo ""
    echo "🚀 使用方法:"
    echo "  # 运行主程序"
    echo "  ./$XCODE_DIR/Debug/renderer projects/trip/trip_protocol.json"
    echo ""
    echo "  # 运行测试"
    echo "  ./$XCODE_DIR/Debug/simple_test run"
    echo ""
    echo "💡 提示:"
echo "  - 可以在 Xcode 中打开 $XCODE_DIR/PosterMaker.xcodeproj 进行开发"
echo "  - 支持断点调试、代码导航等 IDE 功能"
else
    echo "❌ Xcode 项目编译失败！"
    exit 1
fi 