# JSON 协议格式详解

本文档详细说明了 Skia 海报渲染引擎的 JSON 协议格式，包括所有支持的属性、配置选项和使用示例。

## 📋 协议结构

一个完整的渲染协议包含以下主要部分：

```json
{
  "canvas": { ... },      // 画布配置
  "images": [ ... ],      // 图片元素列表
  "texts": [ ... ],       // 文本元素列表
  "output": { ... }       // 输出配置
}
```

## 🎨 画布配置

### 基本属性

```json
{
  "canvas": {
    "width": 1242,           // 画布宽度 (像素)
    "height": 1660,          // 画布高度 (像素)
    "background": "#FFFFFF", // 背景颜色
    "debug": false           // 调试模式 (显示文本区域边界)
  }
}
```

### 背景颜色支持

支持多种颜色格式：

- **十六进制**: `#RRGGBB`, `#RRGGBBAA`
- **RGB**: `rgb(r,g,b)`
- **RGBA**: `rgba(r,g,b,a)`
- **预定义**: `white`, `black`, `red`, `green`, `blue`, `transparent`

## 🖼️ 图片元素

### 基本配置

```json
{
  "images": [
    {
      "id": "background",           // 图片ID (唯一标识)
      "path": "images/bg.png",      // 图片文件路径
      "x": 0, "y": 0,              // 位置坐标 (像素)
      "width": 1242, "height": 1660, // 显示尺寸 (像素)
      "scaleX": 1.0, "scaleY": 1.0,  // 缩放比例
      "rotation": 0,                // 旋转角度 (度)
      "opacity": 1.0                // 透明度 (0.0-1.0)
    }
  ]
}
```

### 支持的图片格式

- **PNG** - 支持透明通道
- **JPEG** - 压缩格式
- **WebP** - 现代压缩格式

### 变换属性详解

| 属性 | 类型 | 说明 | 默认值 |
|------|------|------|--------|
| `x`, `y` | number | 位置坐标 (像素) | 0 |
| `width`, `height` | number | 显示尺寸 (像素) | 图片原始尺寸 |
| `scaleX`, `scaleY` | number | 缩放比例 | 1.0 |
| `rotation` | number | 旋转角度 (度) | 0 |
| `opacity` | number | 透明度 (0.0-1.0) | 1.0 |

## 🔤 文本元素

### 基本配置

```json
{
  "texts": [
    {
      "id": "title",                    // 文本ID (唯一标识)
      "content": "标题文本",            // 文本内容
      "x": 100, "y": 100,              // 位置坐标 (像素)
      "fontFamily": "SourceHanSansCN-Normal", // 字体族
      "fontSize": 48,                  // 字体大小 (像素)
      "fillColor": "#000000",          // 填充颜色
      "strokeColor": "#FFFFFF",        // 描边颜色
      "strokeWidth": 2.0,              // 描边宽度 (像素)
      "hasShadow": true,               // 是否启用阴影
      "shadowDx": 2, "shadowDy": 2,    // 阴影偏移 (像素)
      "shadowColor": "#666666",        // 阴影颜色
      "width": 300, "height": 100,     // 文本区域尺寸 (像素)
      "displayMode": "WordWrap",       // 显示模式
      "maxLines": 0,                   // 最大行数
      "ellipsis": false,               // 是否显示省略号
      "opacity": 1.0,                  // 透明度 (0.0-1.0)
      "scaleX": 1.0, "scaleY": 1.0,   // 缩放比例
      "rotation": 0                    // 旋转角度 (度)
    }
  ]
}
```

### 文本属性详解

#### 基础属性

| 属性 | 类型 | 说明 | 默认值 |
|------|------|------|--------|
| `id` | string | 文本ID (唯一标识) | 必需 |
| `content` | string | 文本内容 | 必需 |
| `x`, `y` | number | 位置坐标 (像素) | 0 |
| `fontFamily` | string | 字体族 | "SourceHanSansCN-Normal" |
| `fontSize` | number | 字体大小 (像素) | 16 |

#### 颜色属性

| 属性 | 类型 | 说明 | 默认值 |
|------|------|------|--------|
| `fillColor` | string | 填充颜色 | "#000000" |
| `strokeColor` | string | 描边颜色 | 无 |
| `strokeWidth` | number | 描边宽度 (像素) | 0 |

#### 阴影属性

| 属性 | 类型 | 说明 | 默认值 |
|------|------|------|--------|
| `hasShadow` | boolean | 是否启用阴影 | false |
| `shadowDx` | number | 阴影X偏移 (像素) | 0 |
| `shadowDy` | number | 阴影Y偏移 (像素) | 0 |
| `shadowColor` | string | 阴影颜色 | "#666666" |

#### 布局属性

| 属性 | 类型 | 说明 | 默认值 |
|------|------|------|--------|
| `width` | number | 文本区域宽度 (像素) | 0 (无限制) |
| `height` | number | 文本区域高度 (像素) | 0 (无限制) |
| `displayMode` | string | 显示模式 | "WordWrap" |
| `maxLines` | number | 最大行数 | 0 (无限制) |
| `ellipsis` | boolean | 是否显示省略号 | false |

#### 变换属性

| 属性 | 类型 | 说明 | 默认值 |
|------|------|------|--------|
| `opacity` | number | 透明度 (0.0-1.0) | 1.0 |
| `scaleX`, `scaleY` | number | 缩放比例 | 1.0 |
| `rotation` | number | 旋转角度 (度) | 0 |

### 显示模式详解

#### 1. SingleLine (单行模式)

```json
{
  "displayMode": "SingleLine",
  "ellipsis": true,
  "width": 300,
  "content": "这是一行很长的文本，应该被截断并显示省略号"
}
```

**特点**:
- 文本强制在一行内显示
- 超出部分自动截断
- 支持省略号显示

#### 2. MultiLine (多行模式)

```json
{
  "displayMode": "MultiLine",
  "maxLines": 2,
  "ellipsis": true,
  "content": "这是一段很长的文本，限制为2行显示..."
}
```

**特点**:
- 固定行数显示
- 支持行数限制
- 支持省略号显示

#### 3. WordWrap (自动换行模式)

```json
{
  "displayMode": "WordWrap",
  "width": 300,
  "content": "这是一段需要自动换行的文本，包含中英文混合内容"
}
```

**特点**:
- 根据宽度自动换行
- 智能换行算法
- 支持中英文混合

#### 4. AutoFit (自适应模式)

```json
{
  "displayMode": "AutoFit",
  "width": 200,
  "height": 100,
  "content": "这是一段很长的文本，会自动缩小字体以适应容器"
}
```

**特点**:
- 自动调整字体大小
- 确保文本完全适应容器
- 智能两步策略

## 📤 输出配置

### 基本配置

```json
{
  "output": {
    "format": "png",        // 输出格式 (png/jpeg)
    "filename": "output.png", // 输出文件名
    "quality": 100          // 输出质量 (1-100)
  }
}
```

### 输出属性详解

| 属性 | 类型 | 说明 | 默认值 |
|------|------|------|--------|
| `format` | string | 输出格式 | "png" |
| `filename` | string | 输出文件名 | "output.png" |
| `quality` | number | 输出质量 (1-100) | 100 |

### 支持的输出格式

- **PNG** - 无损压缩，支持透明通道
- **JPEG** - 有损压缩，文件较小

## 🔤 字体管理

### 默认字体

项目内置以下字体：

- `SourceHanSansCN-Normal` - 思源黑体 (默认)
- `站酷快乐体2016修订版.ttf` - 站酷快乐体

### 字体回退机制

当指定的字体不可用时，系统会自动回退到默认字体。

## 🎨 颜色格式详解

### 十六进制格式

```json
{
  "fillColor": "#FF0000",      // 红色
  "fillColor": "#00FF00",      // 绿色
  "fillColor": "#0000FF",      // 蓝色
  "fillColor": "#FF000080"     // 半透明红色
}
```

### RGB/RGBA 格式

```json
{
  "fillColor": "rgb(255, 0, 0)",      // 红色
  "fillColor": "rgba(255, 0, 0, 0.5)" // 半透明红色
}
```

### 预定义颜色

```json
{
  "fillColor": "white",
  "fillColor": "black",
  "fillColor": "red",
  "fillColor": "green",
  "fillColor": "blue",
  "fillColor": "transparent"
}
```

## 📝 完整示例

### 简单海报示例

```json
{
  "canvas": {
    "width": 800,
    "height": 600,
    "background": "#FFFFFF",
    "debug": true
  },
  "images": [
    {
      "id": "background",
      "path": "images/bg.jpg",
      "x": 0,
      "y": 0,
      "width": 800,
      "height": 600,
      "opacity": 1.0
    }
  ],
  "texts": [
    {
      "id": "title",
      "content": "欢迎使用 Skia 海报渲染引擎",
      "x": 100,
      "y": 100,
      "fontFamily": "SourceHanSansCN-Normal",
      "fontSize": 36,
      "fillColor": "#000000",
      "displayMode": "SingleLine",
      "ellipsis": false
    },
    {
      "id": "description",
      "content": "这是一个功能强大的海报渲染引擎，支持多种文本显示模式和丰富的视觉效果。",
      "x": 100,
      "y": 200,
      "fontFamily": "SourceHanSansCN-Normal",
      "fontSize": 18,
      "fillColor": "#666666",
      "width": 600,
      "displayMode": "WordWrap",
      "ellipsis": false
    }
  ],
  "output": {
    "format": "png",
    "filename": "poster.png",
    "quality": 100
  }
}
```

## 🔧 调试功能

### Debug 模式

在画布配置中设置 `"debug": true` 可以启用调试模式：

```json
{
  "canvas": {
    "debug": true
  }
}
```

**调试功能**:
- 显示文本区域边界 (红色框框)
- 便于验证文本布局的正确性
- 帮助调试文本位置和尺寸

### 错误处理

系统提供详细的错误信息：

- 文件路径错误
- 字体加载失败
- 协议格式错误
- 渲染过程中的异常

## 📚 最佳实践

### 1. 文件路径

- 使用相对路径
- 确保文件存在
- 支持常见图片格式

### 2. 字体选择

- 优先使用内置字体
- 测试字体可用性
- 考虑字体回退

### 3. 性能优化

- 合理设置图片尺寸
- 避免过大的画布
- 优化文本内容长度

### 4. 协议设计

- 使用有意义的ID
- 保持协议结构清晰
- 添加适当的注释 