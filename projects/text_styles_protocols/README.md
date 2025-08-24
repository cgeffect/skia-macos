# 🎨 文本样式演示项目

本项目包含了6个不同的文本样式演示协议，全面展示Skia文本渲染引擎的功能。

## 📋 项目文件

### 1. `basic_font_styles.json` - 基础字体样式
- **输出**: `basic_font_styles_demo.png`
- **内容**: 不同字体大小、中英文混合、数字符号渲染

### 2. `text_effects.json` - 文本效果演示
- **输出**: `text_effects_demo.png`
- **内容**: 阴影、描边、填充效果的多层绘制机制

### 3. `display_modes.json` - 四种显示模式
- **输出**: `display_modes_demo.png`
- **内容**: SingleLine、MultiLine、WordWrap、AutoFit模式对比

### 4. `colors_and_transforms.json` - 颜色和变换
- **输出**: `colors_and_transforms_demo.png`
- **内容**: 颜色设置、缩放、旋转、透明度效果

### 5. `text_alignment.json` - 对齐和布局
- **输出**: `text_alignment_demo.png`
- **内容**: 容器约束、位置定位、间距控制（debug模式）

### 6. `mixed_languages.json` - 多语言支持
- **输出**: `mixed_languages_demo.png`
- **内容**: 中英文混合、Unicode字符、特殊符号渲染

### 7. `rich_text_demo.json` - 富文本效果演示
- **输出**: `rich_text_demo.png`
- **内容**: 字体样式组合、颜色渐变、文字特效、多层效果、创意排版

## 🚀 快速运行

### 运行所有演示
```bash
# 从项目根目录
./build/text_styles_demo

# 或使用便捷脚本
./run_text_styles_demo.sh
```

### 运行单个演示
```bash
# 基础字体样式
./build/renderer projects/text_styles_protocols/basic_font_styles.json

# 文本效果
./build/renderer projects/text_styles_protocols/text_effects.json

# 显示模式对比
./build/renderer projects/text_styles_protocols/display_modes.json

# 颜色和变换
./build/renderer projects/text_styles_protocols/colors_and_transforms.json

# 对齐和布局
./build/renderer projects/text_styles_protocols/text_alignment.json

# 多语言支持
./build/renderer projects/text_styles_protocols/mixed_languages.json

# 富文本效果
./build/renderer projects/text_styles_protocols/rich_text_demo.json
```

## 🎯 技术特点

### 多层绘制机制
文本效果通过多次绘制实现：
1. **阴影绘制**：在偏移位置绘制文本副本
2. **描边绘制**：绘制文本轮廓线（kStroke_Style）
3. **填充绘制**：填充文本内部（kFill_Style）

### 四种显示模式
- **SingleLine**：单行显示，省略号截断
- **MultiLine**：固定行数，支持行数限制
- **WordWrap**：智能换行，中英文混合支持
- **AutoFit**：自适应字体缩放，二分查找算法

### 坐标变换系统
- **围绕指定点变换**：文本的缩放和旋转围绕文本位置进行
- **分离控制机制**：位置通过drawString参数，变换通过Canvas矩阵

## 📊 支持的文本属性

| 属性类别 | 属性名 | 说明 | 示例 |
|----------|--------|------|------|
| **字体** | fontFamily | 字体族 | "SourceHanSansCN-Normal" |
| **字体** | fontSize | 字体大小(px) | 18 |
| **颜色** | fillColor | 填充颜色 | "#FF0000" |
| **颜色** | strokeColor | 描边颜色 | "#000000" |
| **效果** | strokeWidth | 描边宽度(px) | 2.0 |
| **效果** | hasShadow | 启用阴影 | true |
| **效果** | shadowDx/Dy | 阴影偏移(px) | 3, 3 |
| **布局** | displayMode | 显示模式 | "WordWrap" |
| **布局** | width/height | 容器尺寸(px) | 300, 100 |
| **变换** | scaleX/scaleY | 缩放比例 | 1.5, 0.8 |
| **变换** | rotation | 旋转角度(度) | 15 |
| **变换** | opacity | 透明度(0-1) | 0.8 |

## 🔍 调试功能

在协议中设置 `"debug": true` 可以：
- 显示文本容器的红色边界框
- 验证容器约束是否正确
- 调试文本布局问题

## 📚 学习价值

通过这些演示，你可以学习到：
1. **Skia文本渲染的核心概念**
2. **多层绘制机制的实现原理**
3. **四种显示模式的适用场景**
4. **坐标变换系统的工作方式**
5. **中英文混合排版的处理方法**
6. **文本效果的实现技巧**

这些演示覆盖了Skia文本渲染的所有重要功能，是学习和理解文本渲染技术的绝佳资源！
