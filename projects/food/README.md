# 美食海报项目

## 项目结构

```
food/
├── original/              # 原始设计文件
│   ├── food.json         # Fabric.js 设计数据
│   └── food.svg          # SVG 参考文件
├── resources/             # 图片资源
│   ├── food_bg_1.png        # 背景图片1
│   ├── food_bg_2.png        # 背景图片2
│   ├── food_main.png        # 主图
│   ├── food_decor_1.png     # 装饰图片1
│   ├── food_decor_2.png     # 装饰图片2
│   ├── food_text_bg_1.png   # 文字背景1
│   ├── food_text_bg_2.png   # 文字背景2
│   └── food_text_bg_3.png   # 文字背景3
├── output/               # 渲染输出
│   └── food_poster.png
├── food_protocol.json    # 我们的渲染协议
└── README.md             # 项目说明
```

## 设计元素

### 图片资源
- 8个图片元素：
  - 2个背景图片
  - 1个主图（美食图片）
  - 2个装饰图片
  - 3个文字背景图片
- 所有图片从 `food.json` 中的URL下载

### 渲染参数
- **画布尺寸**: 1242x1660
- **背景色**: 白色
- **输出格式**: PNG

## 图片元素详情

### 背景层
1. **food_bg_1.png** - 主背景图片 (1285x1927)
2. **food_bg_2.png** - 辅助背景图片 (1194x1586)

### 主图
3. **food_main.png** - 美食主图 (1050x750)

### 装饰元素
4. **food_decor_1.png** - 右上角装饰 (226x223)
5. **food_decor_2.png** - 左下角装饰 (396x175)

### 文字背景
6. **food_text_bg_1.png** - 顶部文字背景 (1081x351)
7. **food_text_bg_2.png** - 底部文字背景1 (1097x147)
8. **food_text_bg_3.png** - 底部文字背景2 (978x111)

## 使用方法

1. 下载图片资源：
   ```bash
   ./projects/food/download_food_images.sh
   ```

2. 编译渲染器：
   ```bash
   ./build.sh
   ```

3. 运行渲染：
   ```bash
   ./build/skia
   ```

## 协议说明

`food_protocol.json` 包含了所有渲染元素的定义，包括：
- 图片位置、尺寸、透明度
- 画布设置和输出参数

## 设计特点

这是一个美食主题的海报设计，具有：
- 丰富的层次结构
- 精美的美食图片展示
- 专业的装饰元素
- 清晰的文字背景设计 