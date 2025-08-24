# 🎨 Skia 文本样式功能演示

本目录包含了全面的Skia文本样式功能演示，展示了文本渲染引擎支持的各种文本样式和效果。

## 🚀 快速开始

### 编译和运行

```bash
# 编译项目（如果还没编译）
./build.sh

# 运行文本样式演示
./build/text_styles_demo

# 或者单独运行某个演示
./build/renderer projects/text_styles_protocols/basic_font_styles.json
```

## 📋 演示内容

### 1. 基础字体样式演示 (`basic_font_styles.json`)
- **文件输出**: `basic_font_styles_demo.png`
- **演示内容**:
  - 不同字体大小 (12px - 36px)
  - 中文、英文、中英混合文本
  - 数字和特殊符号渲染
  - 字体渲染质量对比

### 2. 文本效果演示 (`text_effects.json`)
- **文件输出**: `text_effects_demo.png`
- **演示内容**:
  - 纯填充文本
  - 纯描边文本
  - 填充+描边组合
  - 阴影效果
  - 复合效果 (阴影+描边+填充)
  - 不同描边宽度和阴影偏移

### 3. 四种显示模式对比 (`display_modes.json`)
- **文件输出**: `display_modes_demo.png`
- **演示内容**:
  - **SingleLine**: 单行显示，省略号截断
  - **MultiLine**: 固定行数，支持省略号
  - **WordWrap**: 智能换行，中英文混合
  - **AutoFit**: 自适应字体缩放
  - 相同文本在不同模式下的对比

### 4. 颜色和变换效果 (`colors_and_transforms.json`)
- **文件输出**: `colors_and_transforms_demo.png`
- **演示内容**:
  - 多种颜色文本
  - 缩放变换 (scaleX, scaleY)
  - 旋转变换 (rotation)
  - 透明度效果 (opacity)
  - 复合变换效果

### 5. 文本对齐和布局 (`text_alignment.json`)
- **文件输出**: `text_alignment_demo.png`
- **演示内容**:
  - 容器约束演示 (width, height)
  - Debug模式边界显示
  - 不同的间距效果
  - 精确位置定位
  - 行高模拟

### 6. 多语言文本渲染 (`mixed_languages.json`)
- **文件输出**: `mixed_languages_demo.png`
- **演示内容**:
  - 简体中文渲染
  - 繁体中文渲染
  - 英文文本渲染
  - 中英文混合排版
  - Unicode字符支持
  - 特殊符号和emoji

## 🎯 支持的文本样式属性

### 基础属性
| 属性 | 类型 | 说明 | 示例值 |
|------|------|------|--------|
| `fontFamily` | string | 字体族 | "SourceHanSansCN-Normal" |
| `fontSize` | number | 字体大小(像素) | 18 |
| `fillColor` | string | 填充颜色 | "#FF0000", "red" |
| `opacity` | number | 透明度(0.0-1.0) | 0.8 |

### 效果属性
| 属性 | 类型 | 说明 | 示例值 |
|------|------|------|--------|
| `strokeColor` | string | 描边颜色 | "#000000" |
| `strokeWidth` | number | 描边宽度(像素) | 2.0 |
| `hasShadow` | boolean | 是否启用阴影 | true |
| `shadowDx` | number | 阴影X偏移(像素) | 3 |
| `shadowDy` | number | 阴影Y偏移(像素) | 3 |
| `shadowColor` | string | 阴影颜色 | "#666666" |

### 布局属性
| 属性 | 类型 | 说明 | 示例值 |
|------|------|------|--------|
| `displayMode` | string | 显示模式 | "WordWrap", "AutoFit" |
| `width` | number | 容器宽度(像素) | 300 |
| `height` | number | 容器高度(像素) | 100 |
| `maxLines` | number | 最大行数 | 3 |
| `ellipsis` | boolean | 省略号 | true |

### 变换属性
| 属性 | 类型 | 说明 | 示例值 |
|------|------|------|--------|
| `x`, `y` | number | 位置坐标(像素) | 100, 200 |
| `scaleX`, `scaleY` | number | 缩放比例 | 1.5, 0.8 |
| `rotation` | number | 旋转角度(度) | 15 |

## 🔧 技术特点

### 多层绘制机制
文本效果通过多次绘制实现：
1. **阴影绘制**: 在偏移位置绘制文本副本
2. **描边绘制**: 绘制文本轮廓线
3. **填充绘制**: 填充文本内部

### 智能布局策略
- **自动引擎选择**: 根据文本特征选择最佳渲染引擎
- **双引擎架构**: SimpleTextLayoutEngine + ParagraphTextLayoutEngine
- **智能换行**: 支持中英文混合文本的智能断行

### 坐标变换系统
- **围绕指定点变换**: 文本的缩放和旋转围绕文本位置进行
- **分离控制**: 位置通过drawString参数控制，变换通过Canvas矩阵控制
- **状态管理**: save/restore确保变换的独立性

## 🎨 颜色格式支持

支持多种颜色格式：
- **十六进制**: `#RRGGBB`, `#RRGGBBAA`
- **RGB**: `rgb(255, 0, 0)`
- **RGBA**: `rgba(255, 0, 0, 0.5)`
- **预定义**: `red`, `blue`, `green`, `black`, `white`

## 📊 性能特点

- **GPU加速**: 利用Skia的硬件加速能力
- **智能缓存**: 字体和文本布局缓存
- **按需渲染**: 只绘制启用的效果
- **内存优化**: 高效的内存管理

## 🧪 调试功能

### Debug模式
在协议中设置 `"debug": true` 可以：
- 显示文本容器边界（红色框）
- 验证文本布局的正确性
- 调试容器约束效果

### 性能统计
渲染引擎提供详细的性能统计：
- 总渲染次数
- 不同引擎使用次数
- 渲染时间统计

## 📚 扩展开发

### 添加新的文本效果
1. 在`TextEffectRenderer`中添加新的渲染方法
2. 在`TextStyle`结构体中添加相应属性
3. 在协议解析器中添加属性解析

### 自定义显示模式
1. 在`TextDisplayMode`枚举中添加新模式
2. 在布局引擎中实现相应逻辑
3. 更新策略选择器

## 🎯 最佳实践

1. **选择合适的显示模式**: 根据使用场景选择最佳的displayMode
2. **合理使用效果**: 避免过度使用阴影和描边影响性能
3. **字体回退**: 确保字体可用性，设置合理的字体回退
4. **容器约束**: 合理设置width和height避免文本溢出
5. **颜色对比**: 确保文本与背景有足够的对比度

## 🔍 故障排除

### 常见问题
1. **字体加载失败**: 检查字体路径和字体文件
2. **文本不显示**: 检查颜色设置和透明度
3. **布局异常**: 检查容器尺寸和displayMode设置
4. **效果不生效**: 检查相关属性的设置和取值范围

通过这些演示，你可以全面了解Skia文本渲染引擎的强大功能，并在实际项目中灵活运用这些文本样式和效果。
