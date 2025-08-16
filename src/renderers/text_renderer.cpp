#include "renderers/text_renderer.h"
#include <iostream>

namespace skia_renderer {

TextRenderer::TextRenderer() {
    fontManager = std::make_shared<FontManager>();
}

TextRenderer::~TextRenderer() {
}

bool TextRenderer::renderText(SkCanvas* canvas, const TextElement& textElement) {
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
    
    // 渲染阴影（如果有）
    if (textElement.style.hasShadow) {
        renderShadow(canvas, textElement, font);
    }
    
    // 渲染描边（如果有）
    if (textElement.style.strokeWidth > 0) {
        renderStroke(canvas, textElement, font);
    }
    
    // 渲染填充
    renderFill(canvas, textElement, font);
    
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

void TextRenderer::renderShadow(SkCanvas* canvas, const TextElement& textElement, const SkFont& font) {
    SkPaint shadowPaint;
    shadowPaint.setColor(textElement.style.shadowColor);
    shadowPaint.setStyle(SkPaint::kFill_Style);
    
    // 应用阴影偏移
    canvas->drawString(textElement.content.c_str(), 
                      textElement.transform.x + textElement.style.shadowDx, 
                      textElement.transform.y + textElement.style.fontSize + textElement.style.shadowDy, 
                      font, shadowPaint);
}

void TextRenderer::renderStroke(SkCanvas* canvas, const TextElement& textElement, const SkFont& font) {
    SkPaint strokePaint;
    strokePaint.setColor(textElement.style.strokeColor);
    strokePaint.setStyle(SkPaint::kStroke_Style);
    strokePaint.setStrokeWidth(textElement.style.strokeWidth);
    
    canvas->drawString(textElement.content.c_str(), 
                      textElement.transform.x, 
                      textElement.transform.y + textElement.style.fontSize, 
                      font, strokePaint);
}

void TextRenderer::renderFill(SkCanvas* canvas, const TextElement& textElement, const SkFont& font) {
    SkPaint fillPaint;
    fillPaint.setColor(textElement.style.fillColor);
    fillPaint.setStyle(SkPaint::kFill_Style);
    
    canvas->drawString(textElement.content.c_str(), 
                      textElement.transform.x, 
                      textElement.transform.y + textElement.style.fontSize, 
                      font, fillPaint);
}

} // namespace skia_renderer 