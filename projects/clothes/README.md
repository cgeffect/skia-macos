# 服装项目

这是一个服装相关的海报项目，包含两个图片元素。

## 项目结构

```
projects/clothes/
├── original/                    # 原始文件
│   ├── clothes.json            # Fabric.js JSON文件
│   └── clothes.svg             # SVG文件
├── resources/                   # 资源文件
│   ├── 1750068131805_d01a88a738.png
│   └── 1750069348246_fea9c796b8.webp
├── output/                      # 输出目录
├── clothes_protocol.json        # 生成的协议文件
├── translate_clothes.py         # 翻译脚本
└── README.md                    # 说明文档
```

## 原始设计分析

### 画布信息
- **尺寸**: 1000 x 1000 像素
- **背景**: 白色 (#ffffff)

### 图片元素

#### 图片1 (主图片)
- **文件名**: 1750068131805_d01a88a738.png
- **原始尺寸**: 2000 x 2000 像素
- **位置**: (0, 0)
- **缩放**: 0.5 x 0.5 (缩小到50%)
- **旋转**: 0°
- **透明度**: 1.0

#### 图片2 (裁剪图片)
- **文件名**: 1750069348246_fea9c796b8.webp
- **原始尺寸**: 1024 x 1024 像素
- **位置**: (0, 0)
- **缩放**: 0.9766 x 0.9766 (轻微缩小)
- **旋转**: 0°
- **透明度**: 1.0

## 使用方法

### 1. 翻译原始文件
```bash
python3 projects/clothes/translate_clothes.py
```

这个脚本会：
- 解析原始的Fabric.js JSON文件
- 下载图片资源到本地
- 生成Skia渲染协议文件

### 2. 渲染海报
```bash
# 单独渲染
./build/skia projects/clothes/clothes_protocol.json

# 或者运行所有示例
./build/skia
```

### 3. 查看结果
生成的图片保存在 `output/clothes_poster.png`

## 协议文件说明

生成的协议文件包含以下内容：

```json
{
  "canvas": {
    "width": 1000,
    "height": 1000,
    "background": "#ffffff",
    "debug": false
  },
  "images": [
    {
      "id": "image_1",
      "path": "projects/clothes/resources/1750068131805_d01a88a738.png",
      "x": 0,
      "y": 0,
      "width": 2000,
      "height": 2000,
      "scaleX": 0.5,
      "scaleY": 0.5,
      "rotation": 0,
      "opacity": 1
    },
    {
      "id": "image_2",
      "path": "projects/clothes/resources/1750069348246_fea9c796b8.webp",
      "x": 0,
      "y": 0,
      "width": 1024,
      "height": 1024,
      "scaleX": 0.9766,
      "scaleY": 0.9766,
      "rotation": 0,
      "opacity": 1
    }
  ],
  "texts": [],
  "output": {
    "format": "png",
    "filename": "clothes_poster.png",
    "quality": 100
  }
}
```

## 特点

1. **纯图片设计**: 这个项目只包含图片元素，没有文本
2. **双层图片**: 使用两个图片叠加，可能用于创建特殊效果
3. **精确缩放**: 每个图片都有精确的缩放比例
4. **高质量输出**: 支持PNG格式，质量设置为100%

## 注意事项

- 确保网络连接正常，以便下载图片资源
- 图片文件较大，下载可能需要一些时间
- 生成的协议文件可以直接用于Skia渲染引擎 