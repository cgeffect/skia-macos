#include "renderers/text_renderer.h"
#include <iostream>
#include <sstream>
#include "include/core/SkTextBlob.h"
#include "include/utils/SkTextUtils.h"

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
    
    // 渲染文本（支持自动换行）
    renderTextWithWrapping(canvas, textElement, font, shadowPaint, 
                          textElement.style.shadowDx, textElement.style.shadowDy);
}

void TextRenderer::renderStroke(SkCanvas* canvas, const TextElement& textElement, const SkFont& font) {
    SkPaint strokePaint;
    strokePaint.setColor(textElement.style.strokeColor);
    strokePaint.setStyle(SkPaint::kStroke_Style);
    strokePaint.setStrokeWidth(textElement.style.strokeWidth);
    
    // 渲染文本（支持自动换行）
    renderTextWithWrapping(canvas, textElement, font, strokePaint, 0, 0);
}

void TextRenderer::renderFill(SkCanvas* canvas, const TextElement& textElement, const SkFont& font) {
    SkPaint fillPaint;
    fillPaint.setColor(textElement.style.fillColor);
    fillPaint.setStyle(SkPaint::kFill_Style);
    
    // 渲染文本（支持自动换行）
    renderTextWithWrapping(canvas, textElement, font, fillPaint, 0, 0);
}

void TextRenderer::renderTextWithWrapping(SkCanvas* canvas, const TextElement& textElement, 
                                        const SkFont& font, const SkPaint& paint, 
                                        float offsetX, float offsetY) {
    // 如果有宽度限制，使用Skia的文本换行功能
    if (textElement.width > 0) {
        // 先处理换行符，分割文本
        std::vector<std::string> lines = splitText(textElement.content);
        
        if (lines.size() > 1) {
            // 有多行，逐行渲染
            float lineHeight = textElement.style.fontSize * 1.2f;
            for (size_t i = 0; i < lines.size(); ++i) {
                float y = textElement.transform.y + textElement.style.fontSize + offsetY + i * lineHeight;
                canvas->drawString(lines[i].c_str(), 
                                  textElement.transform.x + offsetX, 
                                  y, 
                                  font, paint);
            }
        } else {
            // 单行文本，直接渲染（不使用SkTextUtils，因为它可能影响位置）
            canvas->drawString(textElement.content.c_str(), 
                              textElement.transform.x + offsetX, 
                              textElement.transform.y + textElement.style.fontSize + offsetY, 
                              font, paint);
        }
    } else {
        // 没有宽度限制，直接渲染
        canvas->drawString(textElement.content.c_str(), 
                          textElement.transform.x + offsetX, 
                          textElement.transform.y + textElement.style.fontSize + offsetY, 
                          font, paint);
    }
}

std::vector<std::string> TextRenderer::splitText(const std::string& text) {
    std::vector<std::string> lines;
    std::stringstream ss(text);
    std::string line;
    
    while (std::getline(ss, line, '\r')) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    
    // 如果没有找到\r分隔符，尝试\n
    if (lines.empty()) {
        std::stringstream ss2(text);
        while (std::getline(ss2, line, '\n')) {
            if (!line.empty()) {
                lines.push_back(line);
            }
        }
    }
    
    // 如果还是没有分割，返回原文本
    if (lines.empty()) {
        lines.push_back(text);
    }
    
    return lines;
}

} // namespace skia_renderer 