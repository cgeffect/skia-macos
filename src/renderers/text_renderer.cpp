#include "renderers/text_renderer.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include "include/core/SkTextBlob.h"
#include "include/utils/SkTextUtils.h"

namespace skia_renderer {

TextRenderer::TextRenderer() {
    fontManager = std::make_shared<FontManager>();
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
    // 如果没有设置宽度和高度，直接按单行模式渲染
    if (textElement.width <= 0 && textElement.height <= 0) {
        canvas->drawString(textElement.content.c_str(), 
                          textElement.transform.x + offsetX, 
                          textElement.transform.y + textElement.style.fontSize + offsetY, 
                          font, paint);
        return;
    }
    
    // 根据显示模式选择渲染方法
    switch (textElement.style.displayMode) {
        case TextDisplayMode::SingleLine:
            renderSingleLineText(canvas, textElement, font, paint, offsetX, offsetY);
            break;
        case TextDisplayMode::MultiLine:
            renderMultiLineText(canvas, textElement, font, paint, offsetX, offsetY);
            break;
        case TextDisplayMode::AutoFit:
            renderAutoFitText(canvas, textElement, font, paint, offsetX, offsetY);
            break;
        case TextDisplayMode::WordWrap:
        default:
            renderWordWrapText(canvas, textElement, font, paint, offsetX, offsetY);
            break;
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

// 计算文本宽度
float TextRenderer::calculateTextWidth(const std::string& text, const SkFont& font) {
    SkRect bounds;
    font.measureText(text.c_str(), text.length(), SkTextEncoding::kUTF8, &bounds);
    return bounds.width();
}

// 智能换行算法
std::vector<std::string> TextRenderer::smartWrapText(const std::string& text, float maxWidth, const SkFont& font) {
    std::vector<std::string> lines;
    std::string currentLine;
    
    // 按空格分割文本（适用于英文）
    std::istringstream iss(text);
    std::string word;
    
    while (iss >> word) {
        std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
        float testWidth = calculateTextWidth(testLine, font);
        
        if (testWidth <= maxWidth) {
            currentLine = testLine;
        } else {
            if (!currentLine.empty()) {
                lines.push_back(currentLine);
                currentLine = word;
            } else {
                // 单个单词就超出宽度，强制换行
                lines.push_back(word);
            }
        }
    }
    
    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }
    
    return lines;
}

// 截断文本并添加省略号
std::string TextRenderer::truncateTextWithEllipsis(const std::string& text, float maxWidth, const SkFont& font) {
    const std::string ellipsis = "...";
    float ellipsisWidth = calculateTextWidth(ellipsis, font);
    
    if (calculateTextWidth(text, font) <= maxWidth) {
        return text;
    }
    
    // 二分查找合适的截断位置
    int left = 0;
    int right = text.length();
    
    while (left < right) {
        int mid = (left + right + 1) / 2;
        std::string testText = text.substr(0, mid) + ellipsis;
        float testWidth = calculateTextWidth(testText, font);
        
        if (testWidth <= maxWidth) {
            left = mid;
        } else {
            right = mid - 1;
        }
    }
    
    return text.substr(0, left) + ellipsis;
}

// 单行文本渲染（支持省略号）
void TextRenderer::renderSingleLineText(SkCanvas* canvas, const TextElement& textElement, 
                                       const SkFont& font, const SkPaint& paint, 
                                       float offsetX, float offsetY) {
    std::string displayText = textElement.content;
    
    // 如果有宽度限制且需要显示省略号
    if (textElement.width > 0 && textElement.style.ellipsis) {
        displayText = truncateTextWithEllipsis(textElement.content, textElement.width, font);
    }
    
    canvas->drawString(displayText.c_str(), 
                      textElement.transform.x + offsetX, 
                      textElement.transform.y + textElement.style.fontSize + offsetY, 
                      font, paint);
}

// 多行文本渲染（固定行数）
void TextRenderer::renderMultiLineText(SkCanvas* canvas, const TextElement& textElement, 
                                      const SkFont& font, const SkPaint& paint, 
                                      float offsetX, float offsetY) {
    // 如果没有宽度限制，按单行处理
    if (textElement.width <= 0) {
        canvas->drawString(textElement.content.c_str(), 
                          textElement.transform.x + offsetX, 
                          textElement.transform.y + textElement.style.fontSize + offsetY, 
                          font, paint);
        return;
    }
    
    std::vector<std::string> lines = smartWrapText(textElement.content, textElement.width, font);
    
    // 限制行数
    if (textElement.style.maxLines > 0 && lines.size() > textElement.style.maxLines) {
        lines.resize(textElement.style.maxLines);
        if (textElement.style.ellipsis && !lines.empty()) {
            // 在最后一行添加省略号
            std::string lastLine = lines.back();
            std::string ellipsisText = truncateTextWithEllipsis(lastLine, textElement.width, font);
            if (ellipsisText != lastLine) {
                lines.back() = ellipsisText;
            }
        }
    }
    
    // 渲染每一行
    float lineHeight = textElement.style.fontSize * 1.2f;
    for (size_t i = 0; i < lines.size(); ++i) {
        float y = textElement.transform.y + textElement.style.fontSize + offsetY + i * lineHeight;
        canvas->drawString(lines[i].c_str(), 
                          textElement.transform.x + offsetX, 
                          y, 
                          font, paint);
    }
}

// 自动换行文本渲染
void TextRenderer::renderWordWrapText(SkCanvas* canvas, const TextElement& textElement, 
                                     const SkFont& font, const SkPaint& paint, 
                                     float offsetX, float offsetY) {
    // 如果没有宽度限制，直接渲染
    if (textElement.width <= 0) {
        canvas->drawString(textElement.content.c_str(), 
                          textElement.transform.x + offsetX, 
                          textElement.transform.y + textElement.style.fontSize + offsetY, 
                          font, paint);
        return;
    }
    
    std::vector<std::string> lines = smartWrapText(textElement.content, textElement.width, font);
    
    // 渲染每一行
    float lineHeight = textElement.style.fontSize * 1.2f;
    for (size_t i = 0; i < lines.size(); ++i) {
        float y = textElement.transform.y + textElement.style.fontSize + offsetY + i * lineHeight;
        canvas->drawString(lines[i].c_str(), 
                          textElement.transform.x + offsetX, 
                          y, 
                          font, paint);
    }
}

// 自适应文本渲染（字体缩放）
void TextRenderer::renderAutoFitText(SkCanvas* canvas, const TextElement& textElement, 
                                    const SkFont& font, const SkPaint& paint, 
                                    float offsetX, float offsetY) {
    // 如果没有宽度和高度限制，直接渲染
    if (textElement.width <= 0 || textElement.height <= 0) {
        canvas->drawString(textElement.content.c_str(), 
                          textElement.transform.x + offsetX, 
                          textElement.transform.y + textElement.style.fontSize + offsetY, 
                          font, paint);
        return;
    }
    
    // 尝试不同的字体大小，找到最适合的
    float originalFontSize = textElement.style.fontSize;
    float currentFontSize = originalFontSize;
    float minFontSize = 8.0f; // 最小字体大小
    
    SkFont currentFont = font;
    
    while (currentFontSize >= minFontSize) {
        currentFont.setSize(currentFontSize);
        std::vector<std::string> lines = smartWrapText(textElement.content, textElement.width, currentFont);
        
        float totalHeight = lines.size() * currentFontSize * 1.2f;
        
        if (totalHeight <= textElement.height) {
            // 找到了合适的字体大小
            break;
        }
        
        currentFontSize -= 1.0f;
    }
    
    // 使用找到的字体大小渲染
    currentFont.setSize(currentFontSize);
    std::vector<std::string> lines = smartWrapText(textElement.content, textElement.width, currentFont);
    
    float lineHeight = currentFontSize * 1.2f;
    for (size_t i = 0; i < lines.size(); ++i) {
        float y = textElement.transform.y + currentFontSize + offsetY + i * lineHeight;
        canvas->drawString(lines[i].c_str(), 
                          textElement.transform.x + offsetX, 
                          y, 
                          currentFont, paint);
    }
}

// 绘制调试框框
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

} // namespace skia_renderer 