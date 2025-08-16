# 图文营销图渲染协议

## 概述

这是一个基于Skia图形库的图文营销图渲染协议，支持多轨道图片和文字渲染，具备完整的变换和属性配置功能。

## 协议特性

### 1. 多轨道支持
- **图片轨道**：支持多个图片元素，每个都有独立的变换和滤镜
- **文字轨道**：支持富文本渲染，包括字体、颜色、阴影等属性
- **形状轨道**：支持基础几何形状的渲染
- **渐变轨道**：支持线性渐变和径向渐变

### 2. 完整的变换系统
- **位置变换**：x, y坐标定位
- **缩放变换**：x, y方向独立缩放
- **旋转变换**：以锚点为中心的旋转
- **透明度**：0-1之间的透明度控制
- **混合模式**：支持多种Skia混合模式

### 3. 丰富的文字属性
- **字体属性**：字体族、大小、粗细、斜体等
- **颜色属性**：填充色、描边色、描边宽度
- **阴影效果**：偏移、模糊半径、颜色
- **段落属性**：对齐方式、方向、最大行数等

### 4. 图片处理能力
- **滤镜效果**：阴影、模糊、颜色调整等
- **裁剪功能**：矩形裁剪区域
- **变换支持**：位置、缩放、旋转、透明度

## 协议结构

### 顶层结构
```json
{
  "version": "1.0",
  "metadata": { ... },
  "tracks": {
    "images": [ ... ],
    "texts": [ ... ],
    "shapes": [ ... ],
    "gradients": [ ... ]
  },
  "animations": [ ... ],
  "effects": { ... },
  "output": { ... }
}
```

### 画布配置
```json
{
  "canvas": {
    "width": 1920,
    "height": 1080,
    "background": {
      "type": "color",
      "value": "#FFFFFF"
    }
  }
}
```

### 图片轨道项
```json
{
  "id": "img_001",
  "type": "image",
  "source": {
    "type": "file",
    "path": "assets/product_main.png"
  },
  "transform": {
    "position": { "x": 100, "y": 150 },
    "scale": { "x": 1.0, "y": 1.0 },
    "rotation": 0.0,
    "anchor": { "x": 0.5, "y": 0.5 },
    "opacity": 1.0,
    "blendMode": "srcOver"
  },
  "filters": [ ... ],
  "clip": { ... }
}
```

### 文字轨道项
```json
{
  "id": "text_001",
  "type": "text",
  "content": "主导云端音视频渲染系统架构设计",
  "transform": { ... },
  "style": {
    "font": {
      "family": "字制区喜脉体",
      "size": 48,
      "weight": "normal",
      "slant": "upright",
      "embolden": false,
      "linearMetrics": false,
      "baselineSnap": false,
      "forceAutoHinting": false,
      "embeddedBitmaps": true,
      "subpixel": true,
      "edging": "antialias"
    },
    "color": {
      "fill": "#FFFFFF",
      "stroke": "#0000FF",
      "strokeWidth": 2.0
    },
    "shadow": {
      "enabled": true,
      "dx": 3,
      "dy": 3,
      "sigma": 2,
      "color": "#00000060"
    },
    "paragraph": {
      "textAlign": "left",
      "textDirection": "ltr",
      "maxLines": 0,
      "ellipsis": "",
      "height": 0,
      "minHeight": 0,
      "maxHeight": 0,
      "textHeightBehavior": "all",
      "effectiveAlign": "left",
      "disableHinting": false
    }
  }
}
```

## 支持的Skia属性

### 字体属性 (SkFont)
- `family`: 字体族名称
- `size`: 字体大小
- `weight`: 字体粗细 (normal, bold, 100-900)
- `slant`: 字体斜体 (upright, italic)
- `embolden`: 是否加粗
- `linearMetrics`: 线性度量
- `baselineSnap`: 基线对齐
- `forceAutoHinting`: 强制自动提示
- `embeddedBitmaps`: 嵌入位图
- `subpixel`: 子像素渲染
- `edging`: 边缘处理 (antialias, aliased, subpixelAntialias)

### 颜色属性 (SkPaint)
- `fill`: 填充颜色
- `stroke`: 描边颜色
- `strokeWidth`: 描边宽度
- `opacity`: 透明度
- `blendMode`: 混合模式

### 阴影效果
- `enabled`: 是否启用阴影
- `dx`, `dy`: 阴影偏移
- `sigma`: 模糊半径
- `color`: 阴影颜色

### 变换属性
- `position`: 位置 (x, y)
- `scale`: 缩放 (x, y)
- `rotation`: 旋转角度
- `anchor`: 锚点 (x, y)
- `opacity`: 透明度
- `blendMode`: 混合模式

### 滤镜效果
- `dropShadow`: 投影阴影
- `blur`: 模糊效果
- `colorFilter`: 颜色滤镜
- `imageFilter`: 图像滤镜

## 使用示例

### 1. 基础文字渲染
```json
{
  "texts": [
    {
      "id": "title",
      "content": "产品标题",
      "transform": {
        "position": { "x": 100, "y": 100 }
      },
      "style": {
        "font": {
          "family": "Arial",
          "size": 36
        },
        "color": {
          "fill": "#000000"
        }
      }
    }
  ]
}
```

### 2. 富文本渲染
```json
{
  "texts": [
    {
      "id": "rich_text",
      "content": "重要信息",
      "transform": {
        "position": { "x": 200, "y": 200 }
      },
      "style": {
        "font": {
          "family": "字制区喜脉体",
          "size": 48,
          "weight": "bold"
        },
        "color": {
          "fill": "#FFFFFF",
          "stroke": "#FF0000",
          "strokeWidth": 2.0
        },
        "shadow": {
          "enabled": true,
          "dx": 3,
          "dy": 3,
          "sigma": 2,
          "color": "#00000060"
        }
      }
    }
  ]
}
```

### 3. 图片渲染
```json
{
  "images": [
    {
      "id": "product",
      "source": {
        "type": "file",
        "path": "assets/product.png"
      },
      "transform": {
        "position": { "x": 100, "y": 300 },
        "scale": { "x": 0.8, "y": 0.8 },
        "rotation": 15.0
      },
      "filters": [
        {
          "type": "dropShadow",
          "dx": 5,
          "dy": 5,
          "sigma": 3,
          "color": "#00000040"
        }
      ]
    }
  ]
}
```

## 渲染引擎

### 核心类
- `RenderEngine`: 主渲染引擎
- `ProtocolRenderer`: 协议解析器
- `Transform`: 变换结构体
- `TextStyle`: 文字样式结构体
- `FontStyle`: 字体样式结构体

### 主要功能
1. **协议解析**: 解析JSON格式的渲染协议
2. **元素渲染**: 渲染图片、文字、形状等元素
3. **变换应用**: 应用位置、缩放、旋转等变换
4. **效果处理**: 处理阴影、滤镜等效果
5. **输出生成**: 生成最终的图像文件

## 编译和运行

### 编译
```bash
./build.sh
```

### 运行
```bash
./build/skia  # 运行基础文字渲染示例
```

### 生成营销图
```bash
# 使用协议渲染器生成营销图
./build/skia  # 会生成 marketing_image.png
```

## 扩展性

### 自定义属性
协议支持扩展自定义属性，可以在各个元素中添加额外的配置项。

### 插件系统
可以设计插件系统来支持更多的滤镜效果和渲染功能。

### 动画支持
协议预留了动画配置，可以扩展支持关键帧动画和过渡效果。

## 性能优化

### 渲染优化
- 使用Skia的硬件加速
- 批量渲染相同类型的元素
- 缓存常用的字体和图像资源

### 内存管理
- 及时释放不需要的资源
- 使用智能指针管理内存
- 优化大图像的加载和处理

## 总结

这个渲染协议提供了一个完整的图文营销图生成解决方案，具备以下优势：

1. **功能完整**: 支持Skia的所有主要功能
2. **易于使用**: JSON格式，结构清晰
3. **高度可配置**: 支持复杂的变换和效果
4. **性能优秀**: 基于Skia的高性能渲染
5. **扩展性强**: 支持自定义属性和插件

通过这个协议，可以轻松生成各种复杂的图文营销图，满足不同的设计需求。 