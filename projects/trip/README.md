# 青海湖旅游攻略海报项目

## 项目结构

```
trip/
├── original/          # 原始设计文件
│   ├── trip.json     # Fabric.js 设计数据
│   └── trip.svg      # SVG 参考文件
├── resources/         # 图片资源
│   ├── image_0.png   # 背景图片1
│   ├── image_1.png   # 背景图片2
│   ├── image_2.png   # 装饰元素1
│   ├── image_3.png   # 装饰元素2
│   ├── image_4.png   # 主图片
│   ├── image_5.png   # 文字背景1
│   ├── image_6.png   # 文字背景2
│   └── image_7.png   # 装饰元素3
├── output/           # 渲染输出
│   └── trip_rendered.png
├── trip_protocol.json # 我们的渲染协议
└── README.md         # 项目说明
```

## 设计元素

### 文字内容
- **主标题**: "青海湖旅游攻略"
- **副标题**: "4天人均花费不到2000"
- **描述**: "规划路线 住宿安排、美食分享"

### 图片资源
- 8个图片元素，包括背景、装饰和主图
- 所有图片从 `res/trip.json` 中的URL下载

### 渲染参数
- **画布尺寸**: 1242x1660
- **背景色**: 白色
- **字体**: 站酷快乐体
- **输出格式**: PNG

## 使用方法

1. 下载图片资源：
   ```bash
   ./download_images.sh
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

`trip_protocol.json` 包含了所有渲染元素的定义，包括：
- 图片位置、尺寸、透明度
- 文字内容、字体、颜色、描边
- 画布设置和输出参数 