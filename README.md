# Skia 海报渲染引擎

基于 Skia 图形库的高性能海报渲染引擎，支持 JSON 协议驱动的动态图形渲染。专为营销海报、社交媒体图片、电商主图等场景设计。

## 🚀 核心功能

- **📄 JSON 协议驱动** - 使用简洁的 JSON 文件定义渲染内容
- **🎨 丰富的渲染功能** - 支持图片、文本、变换、特效等
- **🔤 智能文本系统** - 四种文本显示模式，支持省略号、自动换行、字体缩放
- **🖼️ 高质量输出** - 支持 PNG、JPEG 格式，可调节输出质量
- **🛠️ 开发友好** - 模块化设计，完善的错误处理和调试功能

## 📁 项目结构

```
skia-macos/
├── src/                     # 核心源代码
├── examples/               # 使用示例
├── projects/               # 项目示例
├── tests/                 # 测试套件
├── res/                   # 资源文件
├── output/                # 输出目录
└── 3rdparty/             # 第三方库
```

## 🎯 快速开始

### 环境要求
- macOS (已测试)
- CMake 3.10+
- C++17 编译器

### 编译安装

```bash
# 克隆项目
git clone <repository-url>
cd skia-macos

# 编译项目 (默认 Debug 模式)
./build.sh

# 或指定构建模式
./build.sh --debug    # Debug 模式 (包含调试信息)
./build.sh --release  # Release 模式 (优化性能)

# 查看构建选项
./build.sh --help

# 运行示例
./build/renderer
```

### 基本使用

```bash
# 渲染单个协议文件
./build/renderer projects/trip/trip_protocol.json

# 运行所有测试
./build/simple_test run
```

## 📋 项目示例

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
- **文本渲染模式测试**: `projects/text_wrap_test/` (四种文本显示模式完整测试)
  - SingleLine (单行模式) - 支持省略号截断
  - MultiLine (多行模式) - 支持行数限制和省略号
  - WordWrap (自动换行模式) - 智能换行算法
  - AutoFit (自适应模式) - 智能字体缩放

## 📚 详细文档

- **[协议格式详解](docs/PROTOCOL.md)** - JSON 协议格式、属性说明、配置示例
- **[文本渲染功能](docs/TEXT_RENDERING.md)** - 四种文本显示模式、智能布局策略、技术实现

## ⚙️ 构建模式

项目支持两种构建模式：

### Debug 模式 (默认)
```bash
./build.sh --debug
```
- 包含调试信息 (`-g`)
- 无优化 (`-O0`)
- 启用调试输出 (控制台显示详细信息)
- 适合开发和调试

### Release 模式
```bash
./build.sh --release
```
- 最高优化 (`-O3`)
- 禁用调试输出 (静默运行)
- 更小的可执行文件
- 适合生产环境

## 🧪 测试系统

项目包含完整的自动化测试系统，覆盖14个测试项目：

```bash
# 运行所有测试
./build/simple_test run

# 更新特定测试的基线
./build/simple_test update trip

# 设置容差
./build/simple_test tolerance 0.02
```

## 📈 性能特点

- **高性能渲染** - 基于 Skia 图形库，渲染速度快
- **智能布局** - 基于 SkParagraph 的智能文本布局策略
- **精确控制** - 像素级精确的文本布局和字体缩放
- **多语言支持** - 完整支持中英文混合文本和国际化
- **协议兼容** - 支持基础协议和高级协议，自动选择最佳渲染引擎

## 📝 使用场景

- **营销海报** - 产品宣传、活动推广
- **社交媒体** - 朋友圈、微博配图
- **电商主图** - 商品展示、详情页
- **内容创作** - 文章配图、教程插图
- **品牌设计** - Logo设计、VI系统

**让设计更简单，让创意更自由！** 🎨✨ 