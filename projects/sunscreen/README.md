# 夏日防晒攻略海报项目

## 项目结构

```
sunscreen/
├── original/              # 原始设计文件
│   ├── 防晒.json         # Fabric.js 设计数据
│   └── 防晒.svg          # SVG 参考文件
├── resources/             # 图片资源
│   ├── sunscreen_bg_1.png    # 背景图片1
│   ├── sunscreen_bg_2.png    # 背景图片2
│   ├── sunscreen_product_1.png # 防晒产品1
│   ├── sunscreen_product_2.png # 防晒产品2
│   ├── sunscreen_product_3.png # 防晒产品3
│   └── sunscreen_decor.png    # 装饰图片
├── output/               # 渲染输出
│   └── sunscreen_poster.png
├── sunscreen_protocol.json # 我们的渲染协议
└── README.md             # 项目说明
```

## 设计元素

### 文字内容
- **主标题**: "夏日防晒攻略" (蓝色，白色描边，有阴影)
- **副标题**: "好物推荐 值得收藏" (红色)

### 图片资源
- 6个图片元素：
  - 2个背景图片
  - 3个防晒产品图片
  - 1个装饰图片
- 所有图片从 `res/防晒.json` 中的URL下载

### 渲染参数
- **画布尺寸**: 1242x1660
- **背景色**: 白色
- **字体**: 站酷快乐体
- **输出格式**: PNG

## 特殊效果

### 主标题样式
- **填充色**: #1F87E8 (蓝色)
- **描边色**: #FFFFFF (白色)
- **描边宽度**: 6px
- **阴影**: 4px偏移，3px偏移，颜色#044A95

### 副标题样式
- **填充色**: #C74D4C (红色)
- **无描边和阴影**

## 使用方法

1. 下载图片资源：
   ```bash
   ./download_sunscreen_images.sh
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

`sunscreen_protocol.json` 包含了所有渲染元素的定义，包括：
- 图片位置、尺寸、透明度
- 文字内容、字体、颜色、描边、阴影
- 画布设置和输出参数

## 设计特点

这是一个典型的营销海报设计，具有：
- 清晰的产品展示
- 醒目的标题设计
- 专业的色彩搭配
- 良好的视觉层次 