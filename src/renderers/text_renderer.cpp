#include "renderers/text_renderer.h"
#include <iostream>

namespace skia_renderer {

// ==================== TextRenderer 主类实现 ====================

TextRenderer::TextRenderer() 
    : layoutStrategy(LayoutStrategy::Auto) {
    fontManager = std::make_shared<FontManager>();
    simpleLayoutEngine = std::make_unique<SimpleTextLayoutEngine>();
    paragraphLayoutEngine = std::make_unique<ParagraphTextLayoutEngine>();
}

TextRenderer::~TextRenderer() {
}

bool TextRenderer::renderText(SkCanvas* canvas, const TextElement& textElement, bool debugMode) {
    if (!canvas) {
        std::cerr << "画布为空" << std::endl;
        return false;
    }
    
    if (!fontManager) {
        std::cerr << "字体管理器未初始化" << std::endl;
        return false;
    }
    
    // 加载字体
    sk_sp<SkTypeface> typeface = fontManager->loadFont(textElement.style.fontFamily);
    if (!typeface) {
        std::cerr << "无法加载字体: " << textElement.style.fontFamily << std::endl;
        return false;
    }
    
    // 创建字体
    SkFont font(typeface, textElement.style.fontSize);
    
    // 保存画布状态
    canvas->save();
    
    // 应用变换
    applyTransform(canvas, textElement.transform);
    
    // 选择布局引擎
    TextLayoutEngine* selectedEngine = selectLayoutEngine(textElement);
    
    // 渲染阴影（如果有）
    if (textElement.style.hasShadow) {
        TextEffectRenderer::renderShadow(canvas, textElement, selectedEngine, font);
    }
    
    // 渲染描边（如果有）
    if (textElement.style.strokeWidth > 0) {
        TextEffectRenderer::renderStroke(canvas, textElement, selectedEngine, font);
    }
    
    // 渲染填充
    TextEffectRenderer::renderFill(canvas, textElement, selectedEngine, font);
    
    // 在debug模式下绘制框框
    if (debugMode) {
        drawDebugRect(canvas, textElement, 0, 0);
    }
    
    // 恢复画布状态
    canvas->restore();
    
    return true;
}

void TextRenderer::setFontManager(std::shared_ptr<FontManager> fontManager) {
    this->fontManager = fontManager;
}

std::shared_ptr<FontManager> TextRenderer::getFontManager() const {
    return fontManager;
}

void TextRenderer::applyTransform(SkCanvas* canvas, const Transform& transform) {
    canvas->translate(transform.x, transform.y);
    canvas->scale(transform.scaleX, transform.scaleY);
    canvas->rotate(transform.rotation);
    canvas->translate(-transform.x, -transform.y);
}

void TextRenderer::drawDebugRect(SkCanvas* canvas, const TextElement& textElement, float offsetX, float offsetY) {
    if (textElement.width > 0 && textElement.height > 0) {
        SkPaint debugPaint;
        debugPaint.setColor(SK_ColorRED);
        debugPaint.setStyle(SkPaint::kStroke_Style);
        debugPaint.setStrokeWidth(2.0f);
        
        SkRect debugRect = SkRect::MakeXYWH(
            textElement.transform.x + offsetX,
            textElement.transform.y + offsetY,
            textElement.width,
            textElement.height
        );
        
        canvas->drawRect(debugRect, debugPaint);
    }
}

TextLayoutEngine* TextRenderer::selectLayoutEngine(const TextElement& textElement) {
    renderStats.totalRenderCount++;
    
    bool useParagraph = false;
    
    switch (layoutStrategy) {
        case LayoutStrategy::Auto:
            useParagraph = (TextFeatureAnalyzer::suggestLayoutStrategy(textElement) == LayoutStrategy::Paragraph);
            break;
        case LayoutStrategy::Simple:
            useParagraph = false;
            break;
        case LayoutStrategy::Paragraph:
            useParagraph = true;
            break;
    }
    
    if (useParagraph) {
        renderStats.paragraphLayoutCount++;
        return paragraphLayoutEngine.get();
    } else {
        renderStats.simpleLayoutCount++;
        return simpleLayoutEngine.get();
    }
}

} // namespace skia_renderer 