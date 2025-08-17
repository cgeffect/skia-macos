# Skia 海报渲染引擎

基于 Skia 图形库的高性能海报渲染引擎，支持 JSON 协议驱动的动态图形渲染。专为营销海报、社交媒体图片、电商主图等场景设计。

## 🚀 核心功能

### 📄 **JSON 协议驱动**
- 使用简洁的 JSON 文件定义渲染内容
- 支持复杂的图形布局和样式配置
- 无需编程即可创建精美的海报

### 🎨 **丰富的渲染功能**
- **图片渲染**: 支持 PNG、JPEG、WebP 等格式
- **文本渲染**: 多种字体、颜色、效果支持
- **变换支持**: 位置、缩放、旋转、透明度
- **特效支持**: 阴影、描边、渐变等

### 🔤 **智能文本系统**
- **多种显示模式**: 单行、多行、自动换行、自适应缩放
- **省略号支持**: 智能截断和省略号显示
- **字体管理**: 系统字体和自定义字体支持
- **富文本效果**: 填充、描边、阴影、渐变

### 🖼️ **高质量输出**
- **多格式支持**: PNG、JPEG 输出
- **质量控制**: 可调节输出质量
- **批量处理**: 支持批量渲染多个协议

### 🛠️ **开发友好**
- **模块化设计**: 清晰的架构，易于扩展
- **错误处理**: 完善的错误提示和调试信息
- **调试模式**: 可视化文本区域边界

## 📁 项目结构

```
skia-macos/
├── src/                     # 核心源代码
│   ├── core/               # 核心类型定义
│   │   └── types.h         # 协议数据结构
│   ├── parsers/            # 协议解析器
│   │   ├── protocol_parser.h/cpp    # JSON协议解析
│   │   └── color_parser.h/cpp       # 颜色解析
│   ├── resources/          # 资源管理
│   │   └── font_manager.h/cpp       # 字体管理
│   ├── renderers/          # 渲染器模块
│   │   ├── canvas_renderer.h/cpp    # 画布渲染
│   │   ├── image_renderer.h/cpp     # 图片渲染
│   │   └── text_renderer.h/cpp      # 文本渲染
│   ├── output/             # 输出模块
│   │   └── image_writer.h/cpp       # 图片输出
│   └── engine/             # 主引擎
│       └── render_engine.h/cpp      # 渲染引擎
├── examples/               # 使用示例
│   └── simple_example.cpp  # 主程序示例
├── projects/               # 项目示例
│   ├── trip/              # 青海湖旅游攻略
│   ├── sunscreen/         # 防晒海报
│   ├── food/              # 美食海报
│   ├── spring/            # 春天海报
│   ├── dessert/           # 甜品海报
│   ├── cup/               # 杯子海报
│   ├── horizontal/        # 冬至海报
│   ├── long/              # 蛋黄心长图
│   ├── clothes/           # 服装海报
│   └── text_wrap_test/    # 文本换行测试
├── res/                   # 资源文件
│   └── Fonts/            # 字体文件
├── output/                # 输出目录
└── 3rdparty/             # 第三方库
    └── skia/             # Skia图形库
```

## 🎯 快速开始

### 环境要求
- macOS (已测试)
- CMake 3.10+
- C++17 编译器
- Python 3.6+ (用于翻译脚本)

### 编译安装

```bash
# 克隆项目
git clone <repository-url>
cd skia-macos

# 编译项目
./build.sh

# 运行示例
./build/skia
```

### 基本使用

```bash
# 渲染单个协议文件
./build/skia projects/trip/trip_protocol.json

# 渲染所有示例
./build/skia

# 使用命令行参数
./build/skia <protocol-file-path>
```

## 📋 协议格式详解

### 画布配置
```json
{
  "canvas": {
    "width": 1242,           // 画布宽度
    "height": 1660,          // 画布高度
    "background": "#FFFFFF", // 背景颜色
    "debug": false           // 调试模式
  }
}
```

### 图片元素
```json
{
  "images": [
    {
      "id": "background",           // 图片ID
      "path": "images/bg.png",      // 图片路径
      "x": 0, "y": 0,              // 位置坐标
      "width": 1242, "height": 1660, // 尺寸
      "scaleX": 1.0, "scaleY": 1.0,  // 缩放
      "rotation": 0,                // 旋转角度
      "opacity": 1.0                // 透明度
    }
  ]
}
```

### 文本元素
```json
{
  "texts": [
    {
      "id": "title",                    // 文本ID
      "content": "标题文本",            // 文本内容
      "x": 100, "y": 100,              // 位置坐标
      "fontFamily": "SourceHanSansCN-Normal", // 字体
      "fontSize": 48,                  // 字体大小
      "fillColor": "#000000",          // 填充颜色
      "strokeColor": "#FFFFFF",        // 描边颜色
      "strokeWidth": 2.0,              // 描边宽度
      "hasShadow": true,               // 是否启用阴影
      "shadowDx": 2, "shadowDy": 2,    // 阴影偏移
      "shadowColor": "#666666",        // 阴影颜色
      "width": 300, "height": 100,     // 文本区域尺寸
      "displayMode": "WordWrap",       // 显示模式
      "maxLines": 0,                   // 最大行数
      "ellipsis": false,               // 是否显示省略号
      "opacity": 1.0,                  // 透明度
      "scaleX": 1.0, "scaleY": 1.0,   // 缩放
      "rotation": 0                    // 旋转角度
    }
  ]
}
```

### 输出配置
```json
{
  "output": {
    "format": "png",        // 输出格式 (png/jpeg)
    "filename": "output.png", // 输出文件名
    "quality": 100          // 输出质量 (1-100)
  }
}
```

## 🔤 文本显示模式

### 1. **SingleLine (单行模式)**
- 文本在一行内显示
- 支持省略号截断
- 适用于标题、标签

### 2. **MultiLine (多行模式)**
- 固定行数显示
- 支持行数限制和省略号
- 适用于简介、描述

### 3. **WordWrap (自动换行模式)**
- 根据宽度自动换行
- 智能换行算法
- 适用于正文内容

### 4. **AutoFit (自适应模式)**
- 自动调整字体大小
- 确保文本完全适应容器
- 适用于需要精确控制的场景

## 🎨 颜色支持

支持多种颜色格式：
- **十六进制**: `#RRGGBB`, `#RRGGBBAA`
- **RGB**: `rgb(r,g,b)`
- **RGBA**: `rgba(r,g,b,a)`
- **预定义**: `white`, `black`, `red`, `green`, `blue`, `transparent`

## 🔤 字体管理

### 默认字体
- `SourceHanSansCN-Normal`: 思源黑体
- `站酷快乐体2016修订版.ttf`: 站酷快乐体

### 自定义字体
```cpp
auto fontManager = std::make_shared<skia_renderer::FontManager>();
fontManager->registerFontFile("CustomFont", "fonts/custom.ttf");
engine.setFontManager(fontManager);
```

## 📊 项目示例

### 营销海报
- **青海湖旅游攻略**: `projects/trip/`
- **防晒海报**: `projects/sunscreen/`
- **美食海报**: `projects/food/`
- **春天海报**: `projects/spring/`
- **甜品海报**: `projects/dessert/`
- **杯子海报**: `projects/cup/`
- **冬至海报**: `projects/horizontal/`
- **服装海报**: `projects/clothes/`

### 长图设计
- **蛋黄心长图**: `projects/long/` (94张图片的复杂长图)

### 功能测试
- **文本换行测试**: `projects/text_wrap_test/` (4种文本模式测试)

## 🛠️ 开发工具

### 翻译脚本
每个项目都包含翻译脚本，用于将原始设计文件转换为渲染协议：

```bash
# 翻译服装项目
python3 projects/clothes/translate_clothes.py

# 翻译长图项目
python3 projects/long/simple_translate.py
```

### 调试功能
- **Debug模式**: 在协议中设置 `"debug": true` 可显示文本区域边界
- **错误提示**: 详细的错误信息和调试输出
- **日志记录**: 渲染过程的详细日志

## 📈 性能特点

- **高性能渲染**: 基于 Skia 图形库，渲染速度快
- **内存优化**: 智能内存管理，支持大尺寸图片
- **批量处理**: 支持批量渲染多个协议文件
- **并行处理**: 可扩展为多线程渲染

## 🔧 扩展开发

### 添加新的渲染器
```cpp
class CustomRenderer {
public:
    bool render(SkCanvas* canvas, const CustomElement& element);
};
```

### 添加新的协议字段
```cpp
struct CustomElement {
    std::string id;
    // 自定义字段
};
```

## 📝 使用场景

- **营销海报**: 产品宣传、活动推广
- **社交媒体**: 朋友圈、微博配图
- **电商主图**: 商品展示、详情页
- **内容创作**: 文章配图、教程插图
- **品牌设计**: Logo设计、VI系统

## 🤝 贡献指南

欢迎提交 Issue 和 Pull Request！

### 开发流程
1. Fork 项目
2. 创建功能分支
3. 提交代码
4. 创建 Pull Request

### 代码规范
- 使用 C++17 标准
- 遵循 Google C++ 代码规范
- 添加适当的注释和文档

## 📄 许可证

本项目基于 Skia 图形库开发，遵循相应的开源许可证。

## 🔄 更新日志

### v2.0.0 (当前版本)
- ✅ 智能文本系统 (4种显示模式)
- ✅ Debug模式支持
- ✅ 批量项目示例 (10+个项目)
- ✅ 翻译脚本工具
- ✅ 完善的文档和示例

### v1.0.0
- ✅ 基础渲染功能
- ✅ JSON协议支持
- ✅ 模块化架构
- ✅ 字体和图片支持

---

**让设计更简单，让创意更自由！** 🎨✨ 