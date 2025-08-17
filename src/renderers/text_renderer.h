#pragma once

#include "core/types.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"
#include "include/core/SkTypeface.h"
#include "resources/font_manager.h"
#include <memory>
#include <vector>
#include <string>

namespace skia_renderer {

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();
    
    // 渲染文本元素
    bool renderText(SkCanvas* canvas, const TextElement& textElement, bool debugMode = false);
    
    // 设置字体管理器
    void setFontManager(std::shared_ptr<FontManager> fontManager);
    
    // 获取字体管理器
    std::shared_ptr<FontManager> getFontManager() const;

private:
    std::shared_ptr<FontManager> fontManager;
    
    // 应用变换
    void applyTransform(SkCanvas* canvas, const Transform& transform);
    
    // 渲染阴影
    void renderShadow(SkCanvas* canvas, const TextElement& textElement, const SkFont& font);
    
    // 渲染描边
    void renderStroke(SkCanvas* canvas, const TextElement& textElement, const SkFont& font);
    
    // 渲染填充
    void renderFill(SkCanvas* canvas, const TextElement& textElement, const SkFont& font);
    
    // 渲染文本（支持自动换行）
    void renderTextWithWrapping(SkCanvas* canvas, const TextElement& textElement, 
                               const SkFont& font, const SkPaint& paint, 
                               float offsetX, float offsetY);
    
    // 单行文本渲染（支持省略号）
    void renderSingleLineText(SkCanvas* canvas, const TextElement& textElement, 
                             const SkFont& font, const SkPaint& paint, 
                             float offsetX, float offsetY);
    
    // 多行文本渲染（固定行数）
    void renderMultiLineText(SkCanvas* canvas, const TextElement& textElement, 
                            const SkFont& font, const SkPaint& paint, 
                            float offsetX, float offsetY);
    
    // 自动换行文本渲染
    void renderWordWrapText(SkCanvas* canvas, const TextElement& textElement, 
                           const SkFont& font, const SkPaint& paint, 
                           float offsetX, float offsetY);
    
    // 自适应文本渲染（字体缩放）
    void renderAutoFitText(SkCanvas* canvas, const TextElement& textElement, 
                          const SkFont& font, const SkPaint& paint, 
                          float offsetX, float offsetY);
    
    // 分割文本为多行
    std::vector<std::string> splitText(const std::string& text);
    
    // 计算文本宽度
    float calculateTextWidth(const std::string& text, const SkFont& font);
    
    // 智能换行算法
    std::vector<std::string> smartWrapText(const std::string& text, float maxWidth, const SkFont& font);
    
    // 截断文本并添加省略号
    std::string truncateTextWithEllipsis(const std::string& text, float maxWidth, const SkFont& font);
    
    // 绘制调试框框
    void drawDebugRect(SkCanvas* canvas, const TextElement& textElement, float offsetX, float offsetY);
};

} // namespace skia_renderer 