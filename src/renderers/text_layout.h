#pragma once

#include "core/types.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"
#include "include/core/SkTypeface.h"
#include <memory>
#include <vector>
#include <string>

namespace skia_renderer {

// 文本布局策略
enum class LayoutStrategy {
    Auto,           // 自动选择
    Simple,         // 简单布局（适合单行文本）
    Paragraph       // 段落布局（适合多行、复杂文本）
};

// 文本特征分析器
class TextFeatureAnalyzer {
public:
    struct TextFeatures {
        bool hasMultipleLines = false;
        bool hasComplexCharacters = false;
        bool needsWordWrap = false;
        bool hasLongText = false;
        bool hasSpecialFormatting = false;
        int textLength = 0;
        int lineCount = 0;
    };
    
    static TextFeatures analyze(const TextElement& textElement);
    static bool containsComplexCharacters(const std::string& text);
    static bool needsWordWrapping(const TextElement& textElement);
    static int countLines(const std::string& text);
    static LayoutStrategy suggestLayoutStrategy(const TextElement& textElement);
};

// 文本布局器基类
class TextLayoutEngine {
public:
    virtual ~TextLayoutEngine() = default;
    virtual bool layoutText(SkCanvas* canvas, const TextElement& textElement, 
                           const SkFont& font, const SkPaint& paint, 
                           float offsetX, float offsetY) = 0;
};

// 简单文本布局器（原有实现）
class SimpleTextLayoutEngine : public TextLayoutEngine {
public:
    bool layoutText(SkCanvas* canvas, const TextElement& textElement, 
                   const SkFont& font, const SkPaint& paint, 
                   float offsetX, float offsetY) override;
    
private:
    std::vector<std::string> splitText(const std::string& text);
};

// 段落文本布局器（基于SkParagraph）
class ParagraphTextLayoutEngine : public TextLayoutEngine {
public:
    bool layoutText(SkCanvas* canvas, const TextElement& textElement, 
                   const SkFont& font, const SkPaint& paint, 
                   float offsetX, float offsetY) override;
    
private:
    void renderParagraph(SkCanvas* canvas, const TextElement& textElement, 
                        const SkPaint& paint, float offsetX, float offsetY);
    
    float calculateAutoFitFontSize(const TextElement& textElement, 
                                  void* fontCollection);  // 使用void*避免头文件依赖
};

// 文本效果渲染器
class TextEffectRenderer {
public:
    static void renderShadow(SkCanvas* canvas, const TextElement& textElement, 
                           TextLayoutEngine* layoutEngine, const SkFont& font);
    static void renderStroke(SkCanvas* canvas, const TextElement& textElement, 
                           TextLayoutEngine* layoutEngine, const SkFont& font);
    static void renderFill(SkCanvas* canvas, const TextElement& textElement, 
                          TextLayoutEngine* layoutEngine, const SkFont& font);
};

} // namespace skia_renderer 