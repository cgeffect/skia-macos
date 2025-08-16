# Skia 渲染库

基于 Skia 图形库的模块化海报渲染库，支持 JSON 协议驱动的图形渲染。

## 特性

- 🎨 **模块化设计**: 清晰的模块分离，易于维护和扩展
- 📄 **JSON 协议**: 使用 JSON 文件定义渲染内容
- 🖼️ **多格式支持**: 支持 PNG、JPEG 输出格式
- 🔤 **字体管理**: 支持系统字体和自定义字体
- 🎯 **变换支持**: 位置、缩放、旋转、透明度变换
- ✨ **文本效果**: 填充、描边、阴影效果
- 🚀 **高性能**: 基于 Skia 图形库，渲染性能优异

## 目录结构

```
├── src/                     # 源代码
│   ├── core/               # 核心类型定义
│   ├── utils/              # 工具类
│   ├── parsers/            # 解析器模块
│   ├── resources/          # 资源管理模块
│   ├── renderers/          # 渲染器模块
│   ├── output/             # 输出模块
│   └── engine/             # 主引擎
├── examples/               # 使用示例
├── projects/               # 项目示例
│   ├── trip/              # 青海湖旅游攻略
│   ├── sunscreen/         # 防晒海报
│   └── food/              # 美食海报
├── res/                   # 资源文件
│   └── Fonts/            # 字体文件
└── 3rdparty/             # 第三方库
```

## 快速开始

### 编译

```bash
./build.sh
```

### 运行示例

```bash
./build/skia
```

### 基本使用

```cpp
#include "src/engine/render_engine.h"

int main() {
    skia_renderer::RenderEngine engine;
    
    // 从协议文件渲染
    if (engine.renderFromProtocol("protocol.json")) {
        std::cout << "渲染成功！" << std::endl;
    } else {
        std::cerr << "渲染失败: " << engine.getErrorMessage() << std::endl;
    }
    
    return 0;
}
```

## 协议格式

### 基本结构

```json
{
  "canvas": {
    "width": 1242,
    "height": 1660,
    "background": "#FFFFFF"
  },
  "images": [
    {
      "id": "background",
      "path": "images/bg.png",
      "x": 0, "y": 0,
      "width": 1242, "height": 1660,
      "scaleX": 1.0, "scaleY": 1.0,
      "rotation": 0, "opacity": 1.0
    }
  ],
  "texts": [
    {
      "id": "title",
      "content": "标题文本",
      "x": 100, "y": 100,
      "fontSize": 48,
      "fontFamily": "SourceHanSansCN-Normal",
      "fillColor": "#000000",
      "strokeColor": "#FFFFFF",
      "strokeWidth": 2.0,
      "hasShadow": true,
      "shadowDx": 2, "shadowDy": 2,
      "shadowColor": "#666666"
    }
  ],
  "output": {
    "format": "png",
    "filename": "output.png",
    "quality": 100
  }
}
```

## 字体配置

### 默认字体

库默认支持以下字体：
- `站酷快乐体`: 中文字体
- `SourceHanSansCN-Normal`: 思源黑体

### 自定义字体

```cpp
auto fontManager = std::make_shared<skia_renderer::FontManager>();
fontManager->registerFontFile("CustomFont", "fonts/custom.ttf");
engine.setFontManager(fontManager);
```

## 颜色支持

支持多种颜色格式：
- 十六进制: `#RRGGBB`, `#RRGGBBAA`
- RGB: `rgb(r,g,b)`
- RGBA: `rgba(r,g,b,a)`
- 预定义: `white`, `black`, `red`, `green`, `blue`, `transparent`

## 项目示例

### 青海湖旅游攻略
- 协议文件: `projects/trip/trip_protocol.json`
- 输出文件: `projects/trip/output/trip_rendered.png`

### 防晒海报
- 协议文件: `projects/sunscreen/sunscreen_protocol.json`
- 输出文件: `projects/sunscreen/output/sunscreen_poster.png`

### 美食海报
- 协议文件: `projects/food/food_protocol.json`
- 输出文件: `projects/food/output/food_poster.png`

## 依赖

- **Skia**: 图形渲染库
- **nlohmann/json**: JSON 解析库
- **CMake**: 构建系统

## 许可证

本项目基于 Skia 图形库开发，遵循相应的开源许可证。

## 贡献

欢迎提交 Issue 和 Pull Request！

## 更新日志

### v1.0.0
- ✅ 模块化架构设计
- ✅ JSON 协议支持
- ✅ 多格式输出
- ✅ 字体管理
- ✅ 变换支持
- ✅ 文本效果 