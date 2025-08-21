# 文本换行测试项目

本项目用于测试和验证文字渲染引擎的四种不同文本显示模式。

## 📋 测试内容

### 1. **SingleLine (单行模式)**
- **文件**: `single_line_protocol.json`
- **特点**: 文本在一行内显示，支持省略号截断
- **测试场景**:
  - 有省略号的单行文本
  - 无省略号的单行文本
  - 无宽度限制的文本

### 2. **MultiLine (多行模式)**
- **文件**: `multi_line_protocol.json`
- **特点**: 固定行数显示，支持行数限制和省略号
- **测试场景**:
  - 3行限制的文本
  - 2行限制的文本
  - 无行数限制的文本

### 3. **WordWrap (自动换行模式)**
- **文件**: `word_wrap_protocol.json`
- **特点**: 根据宽度自动换行，智能换行算法
- **测试场景**:
  - 正常宽度的文本换行
  - 窄宽度的频繁换行
  - 英文文本换行
  - 中英文混合文本换行

### 4. **AutoFit (自适应模式)**
- **文件**: `auto_fit_protocol.json`
- **特点**: 自动调整字体大小，确保文本完全适应容器
- **测试场景**:
  - 大文本自适应
  - 小容器大幅缩小字体
  - 数字文本自适应
  - 英文长文本自适应
  - 无约束的原始显示

## 🚀 运行测试

### 方法一：使用测试脚本
```bash
cd projects/text_wrap_test
chmod +x test_all_modes.sh
./test_all_modes.sh
```

### 方法二：手动运行
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

测试完成后，会在 `output/` 目录下生成以下文件：
- `single_line_poster.png` - 单行模式测试结果
- `multi_line_poster.png` - 多行模式测试结果
- `word_wrap_poster.png` - 自动换行模式测试结果
- `auto_fit_poster.png` - 自适应模式测试结果

## 🔍 Debug模式

所有测试都启用了Debug模式，红色框框会显示文本区域的边界，便于验证文本布局的正确性。

## 🎯 验证要点

### 单行模式验证
- ✅ 文本是否在一行内显示
- ✅ 省略号是否正确显示
- ✅ 文本截断是否准确

### 多行模式验证
- ✅ 行数限制是否生效
- ✅ 省略号是否正确显示
- ✅ 文本布局是否美观

### 自动换行模式验证
- ✅ 换行是否在合适位置
- ✅ 中英文混合是否正确处理
- ✅ 窄宽度是否正常换行

### 自适应模式验证
- ✅ 字体大小是否自动调整
- ✅ 文本是否完全适应容器
- ✅ 最小字体限制是否生效

## 🏗️ 技术实现

这些测试基于重构后的文字布局模块：
- **TextFeatureAnalyzer**: 分析文本特征
- **SimpleTextLayoutEngine**: 实现四种显示模式
- **TextEffectRenderer**: 处理视觉效果
- **智能策略选择**: 自动选择最佳布局引擎

## 📈 性能特点

- **快速渲染**: 基于Skia图形库的高性能渲染
- **智能布局**: 自动选择最佳文本布局策略
- **精确控制**: 像素级精确的文本布局
- **兼容性好**: 支持各种字体和字符集 