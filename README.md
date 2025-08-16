# Skia 图文营销图渲染项目

基于Skia图形库的图文营销图渲染系统，支持多轨道图片和文字渲染，具备完整的变换和属性配置功能。

## 项目结构

```
skia-macos/
├── src/                    # 源代码目录
│   ├── main.cpp           # 主程序入口
│   ├── render_engine.h    # 渲染引擎头文件
│   └── protocol_renderer.cpp # 协议渲染器实现
├── res/                   # 资源文件目录
│   └── 字制区喜脉体.ttf   # 中文字体文件
├── 3rdparty/             # 第三方库
│   └── skia/             # Skia图形库
├── build/                # 构建输出目录
├── render_protocol.json  # 渲染协议规范
├── README_PROTOCOL.md    # 协议详细文档
├── CMakeLists.txt        # CMake构建配置
└── build.sh              # 构建脚本
```

## 功能特性

- **多轨道渲染**：支持图片、文字、形状、渐变等多种元素
- **富文本效果**：字体、颜色、描边、阴影等完整文字属性
- **变换系统**：位置、缩放、旋转、透明度、混合模式
- **协议驱动**：JSON格式的渲染协议，易于配置和扩展
- **高性能**：基于Skia的高性能图形渲染

## 快速开始

### 编译项目
```bash
./build.sh
```

### 运行程序
```bash
./build/skia
```

程序将生成 `marketing_image.png` 营销图文件。

## 协议文档

详细的渲染协议说明请参考 [README_PROTOCOL.md](README_PROTOCOL.md)

## 技术栈

- **C++17**：核心编程语言
- **Skia**：Google高性能2D图形库
- **CMake**：跨平台构建系统
- **CoreText**：macOS字体渲染