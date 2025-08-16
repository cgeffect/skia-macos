#pragma once

#include "core/types.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"
#include "include/core/SkTypeface.h"
#include "resources/font_manager.h"
#include <memory>

namespace skia_renderer {

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();
    
    // 渲染文本元素
    bool renderText(SkCanvas* canvas, const TextElement& textElement);
    
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
};

} // namespace skia_renderer 