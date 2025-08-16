# Horizontal 项目

这是一个冬至主题的横向海报项目，以"今日冬至"为主题，询问"今天你吃饺子了吗？"。

## 项目结构

```
projects/horizontal/
├── original/           # 原始设计文件
│   ├── horizontal.json # Fabric.js设计数据
│   └── horizontal.svg  # SVG参考文件
├── resources/          # 下载的图片资源
│   ├── horizontal_bg_1.png   # 背景图片
│   └── horizontal_decor_*.png # 装饰图片
├── output/            # 渲染输出
│   └── horizontal_poster.png # 生成的海报
├── horizontal_protocol.json  # Skia渲染协议
├── download_horizontal_images.sh # 图片下载脚本
├── extract_positions.py   # 位置信息提取脚本
├── extract_text.py        # 文本信息提取脚本
└── README.md          # 项目说明
```

## 设计元素

### 图片元素
- **背景图片**: 1张背景图片，尺寸900x383
- **装饰图片**: 5张装饰图片，包括饺子、冬至元素等

### 文本元素
- **主标题**: "今日冬至" - 大号字体(95px)，白色
- **副标题**: "今天你吃饺子了吗？" - 中等字体(34px)，白色

## 技术特点

- **画布尺寸**: 900x383像素（横向设计）
- **字体**: SourceHanSansCN-Normal
- **颜色方案**: 白色文字，温暖背景色调
- **主题**: 冬至节日，饺子文化
- **布局**: 横向横幅设计，适合社交媒体分享

## 使用方法

1. 下载图片资源：
   ```bash
   ./download_horizontal_images.sh
   ```

2. 渲染海报：
   ```bash
   ./build/skia
   ```

3. 查看结果：
   ```bash
   open projects/horizontal/output/horizontal_poster.png
   ```

## 设计亮点

- **节日主题**: 突出冬至这一重要节气
- **文化元素**: 融入饺子这一传统美食文化
- **横向设计**: 适合各种社交媒体平台分享
- **温馨文案**: "今天你吃饺子了吗？"增加互动性
- **视觉层次**: 大标题突出主题，小标题增加亲和力

## 应用场景

- 冬至节日营销
- 社交媒体分享
- 餐饮行业推广
- 传统文化宣传
- 节日问候卡片 