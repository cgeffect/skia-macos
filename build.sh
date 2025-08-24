#!/bin/bash

# 默认构建模式
BUILD_TYPE="debug"

# 解析命令行参数
while [[ $# -gt 0 ]]; do
    case $1 in
        --debug)
            BUILD_TYPE="debug"
            shift
            ;;
        --release)
            BUILD_TYPE="release"
            shift
            ;;
        -h|--help)
            echo "用法: $0 [选项]"
            echo "选项:"
            echo "  --debug    构建 Debug 版本 (默认)"
            echo "  --release  构建 Release 版本"
            echo "  -h, --help 显示此帮助信息"
            exit 0
            ;;
        *)
            echo "未知选项: $1"
            echo "使用 -h 或 --help 查看帮助信息"
            exit 1
            ;;
    esac
done

echo "=== 开始构建 $BUILD_TYPE 版本 ==="

# 清理并创建构建目录
rm -rf build
mkdir -p build
cd build

# 配置 CMake 并构建
if [ "$BUILD_TYPE" = "release" ]; then
    cmake -DCMAKE_BUILD_TYPE=Release .. && make -j12
else
    cmake -DCMAKE_BUILD_TYPE=Debug .. && make -j12
fi

if [ $? -eq 0 ]; then
    echo "=== $BUILD_TYPE 版本构建成功！ ==="
    echo "主要可执行文件:"
    echo "  - 渲染器: ./build/simple_example"
    echo "  - 文本样式演示: ./build/text_styles_demo"
    echo "  - 简单测试: ./build/simple_image_test"
else
    echo "=== 构建失败！ ==="
    exit 1
fi
