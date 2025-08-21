# 文本渲染模式测试项目

本项目用于测试和验证文字渲染引擎的四种不同文本显示模式，基于 SkParagraph 和智能布局策略实现。

## 🎯 支持的文本渲染模式

### 1. **SingleLine (单行模式)** ✅ 完全支持
- **文件**: `single_line_protocol.json`
- **渲染引擎**: `ParagraphTextLayoutEngine` (SkParagraph)
- **特点**: 
  - 文本强制在一行内显示
  - 支持省略号截断 (`ellipsis: true`)
  - 支持宽度限制 (`width` 属性)
  - 超出部分自动截断并显示 `...`

**协议配置示例**:
```json
{
  "displayMode": "SingleLine",
  "ellipsis": true,
  "width": 300,
  "content": "这是一行很长的文本，应该被截断并显示省略号"
}
```

**渲染效果**:
```
这是一行很长的文本，应该被截断并...
```

### 2. **MultiLine (多行模式)** ✅ 完全支持
- **文件**: `multi_line_protocol.json`
- **渲染引擎**: `ParagraphTextLayoutEngine` (SkParagraph)
- **特点**:
  - 支持行数限制 (`maxLines` 属性)
  - 支持省略号截断 (`ellipsis: true`)
  - 智能换行算法
  - 超出行数限制时显示省略号

**协议配置示例**:
```json
{
  "displayMode": "MultiLine",
  "maxLines": 2,
  "ellipsis": true,
  "content": "这是一段很长的文本，限制为2行显示..."
}
```

**渲染效果**:
```
这是第一行文本内容...
这是第二行文本内容...
```

### 3. **WordWrap (自动换行模式)** ✅ 完全支持
- **文件**: `word_wrap_protocol.json`
- **渲染引擎**: `ParagraphTextLayoutEngine` (SkParagraph)
- **特点**:
  - 根据容器宽度自动换行
  - 支持中英文混合文本
  - 智能换行算法，避免单词被截断
  - 支持宽度限制 (`width` 属性)

**协议配置示例**:
```json
{
  "displayMode": "WordWrap",
  "width": 300,
  "content": "这是一段需要自动换行的文本，包含中英文混合内容"
}
```

**渲染效果**:
```
这是一段需要自动换行的文本，
包含中英文混合内容
```

### 4. **AutoFit (自适应模式)** ✅ 完全支持
- **文件**: `auto_fit_protocol.json`
- **渲染引擎**: `ParagraphTextLayoutEngine` (SkParagraph)
- **特点**:
  - 自动调整字体大小以适应容器
  - 智能两步策略：先尝试换行，超出时才缩小字体
  - 支持宽度和高度限制 (`width` 和 `height` 属性)
  - 最小字体大小限制 (8.0px)
  - 充分利用容器空间

**协议配置示例**:
```json
{
  "displayMode": "AutoFit",
  "width": 200,
  "height": 100,
  "content": "这是一段很长的文本，会自动缩小字体以适应容器"
}
```

**渲染效果**:
```
字体自动缩小，文本完全适应容器
```

## 🏗️ 技术实现架构

### 智能策略选择
```cpp
LayoutStrategy TextFeatureAnalyzer::suggestLayoutStrategy(const TextElement& textElement) {
    // 基于 displayMode 和 width/height 属性智能选择渲染引擎
    if (textElement.style.displayMode == TextDisplayMode::SingleLine ||
        textElement.style.displayMode == TextDisplayMode::MultiLine ||
        textElement.style.displayMode == TextDisplayMode::AutoFit) {
        return LayoutStrategy::Paragraph;  // 使用 SkParagraph
    }
    
    if (textElement.style.displayMode == TextDisplayMode::WordWrap) {
        if (textElement.style.maxLines > 0 || textElement.style.ellipsis || textElement.width > 0) {
            return LayoutStrategy::Paragraph;  // 使用 SkParagraph
        }
    }
    
    return LayoutStrategy::Simple;  // 使用简单渲染（旧协议兼容）
}
```

### 渲染引擎对比

| 渲染引擎 | 适用场景 | 特点 |
|----------|----------|------|
| **ParagraphTextLayoutEngine** | 新协议、复杂文本 | 基于 SkParagraph，支持自动换行、省略号、字体缩放 |
| **SimpleTextLayoutEngine** | 旧协议、简单文本 | 支持手动换行 (`\r\n`)，向后兼容 |

## 🚀 运行测试

### 一键测试所有模式
```bash
cd projects/text_wrap_test
./test_all_modes.sh
```

### 单独测试各模式
```bash
# 测试单行模式
./build/renderer projects/text_wrap_test/single_line_protocol.json

# 测试多行模式
./build/renderer projects/text_wrap_test/multi_line_protocol.json

# 测试自动换行模式
./build/renderer projects/text_wrap_test/word_wrap_protocol.json

# 测试自适应模式
./build/renderer projects/text_wrap_test/auto_fit_protocol.json
```

## 📊 输出结果

测试完成后，在 `output/` 目录下生成：
- `single_line_poster.png` - 单行模式测试结果
- `multi_line_poster.png` - 多行模式测试结果
- `word_wrap_poster.png` - 自动换行模式测试结果
- `auto_fit_poster.png` - 自适应模式测试结果

## 🔍 验证要点

### ✅ 单行模式验证
- 文本强制在一行内显示
- 省略号在文本截断时正确显示
- 宽度限制生效，超出部分被截断

### ✅ 多行模式验证
- 行数限制 (`maxLines`) 生效
- 省略号在最后一行正确显示
- 文本布局美观，行间距合理

### ✅ 自动换行模式验证
- 换行位置合理，避免单词被截断
- 中英文混合文本正确处理
- 窄宽度容器正常换行

### ✅ 自适应模式验证
- 字体大小自动调整以适应容器
- 文本完全在容器范围内显示
- 最小字体大小限制生效 (8.0px)
- 充分利用容器空间，避免不必要的缩小

## 🎨 协议属性说明

### 核心属性
- `displayMode`: 文本显示模式 (`SingleLine`, `MultiLine`, `WordWrap`, `AutoFit`)
- `width`: 容器宽度 (像素)
- `height`: 容器高度 (像素)
- `maxLines`: 最大行数限制
- `ellipsis`: 是否显示省略号 (`true`/`false`)

### 样式属性
- `fontFamily`: 字体族
- `fontSize`: 字体大小
- `fillColor`: 文字颜色
- `opacity`: 透明度

## 🔧 向后兼容性

- **旧协议**: 没有 `displayMode` 属性的协议使用 `SimpleTextLayoutEngine`
- **新协议**: 有 `displayMode` 属性的协议使用 `ParagraphTextLayoutEngine`
- **混合协议**: 智能策略选择确保最佳渲染效果

## 📈 性能特点

- **高性能**: 基于 Skia 图形库的硬件加速渲染
- **智能布局**: 自动选择最佳文本布局策略
- **精确控制**: 像素级精确的文本布局和字体缩放
- **多语言支持**: 完整支持中英文混合文本
- **内存优化**: 高效的字体管理和文本缓存 