#include "renderers/text_layout.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <limits>
#include "include/core/SkTextBlob.h"
#include "include/utils/SkTextUtils.h"
#include "modules/skparagraph/include/Paragraph.h"
#include "modules/skparagraph/include/ParagraphBuilder.h"
#include "modules/skparagraph/include/ParagraphStyle.h"
#include "modules/skparagraph/include/TextStyle.h"
#include "include/core/SkFontMgr.h"

namespace skia_renderer {

// ==================== TextFeatureAnalyzer 实现 ====================

TextFeatureAnalyzer::TextFeatures TextFeatureAnalyzer::analyze(const TextElement& textElement) {
    TextFeatures features;
    
    features.textLength = textElement.content.length();
    features.lineCount = countLines(textElement.content);
    features.hasMultipleLines = features.lineCount > 1;
    features.hasLongText = features.textLength > 50;
    features.hasComplexCharacters = containsComplexCharacters(textElement.content);
    features.needsWordWrap = needsWordWrapping(textElement);
    
    // 检查特殊格式要求
    features.hasSpecialFormatting = 
        textElement.style.maxLines > 0 ||
        textElement.style.ellipsis ||
        textElement.style.displayMode == TextDisplayMode::WordWrap ||
        textElement.style.displayMode == TextDisplayMode::MultiLine ||
        textElement.style.displayMode == TextDisplayMode::AutoFit;
    
    return features;
}

bool TextFeatureAnalyzer::containsComplexCharacters(const std::string& text) {
    // 检查是否包含非ASCII字符（中文、日文、韩文等）
    for (unsigned char c : text) {
        if (c > 127) {
            return true;
        }
    }
    
    // 检查是否包含特殊Unicode字符
    std::regex unicodePattern(R"([\u4e00-\u9fff\u3040-\u309f\u30a0-\u30ff\uac00-\ud7af])");
    return std::regex_search(text, unicodePattern);
}

bool TextFeatureAnalyzer::needsWordWrapping(const TextElement& textElement) {
    if (textElement.width <= 0) {
        return false;
    }
    
    if (textElement.style.displayMode == TextDisplayMode::SingleLine) {
        return false;
    }
    
    float estimatedCharWidth = textElement.style.fontSize * 0.6f;
    float estimatedTextWidth = textElement.content.length() * estimatedCharWidth;
    
    return estimatedTextWidth > textElement.width;
}

int TextFeatureAnalyzer::countLines(const std::string& text) {
    int lines = 1;
    for (char c : text) {
        if (c == '\n' || c == '\r') {
            lines++;
        }
    }
    return lines;
}

LayoutStrategy TextFeatureAnalyzer::suggestLayoutStrategy(const TextElement& textElement) {
    TextFeatures features = analyze(textElement);
    
    // 暂时强制使用简单布局，确保基本功能稳定
    // TODO: 在 SkParagraph 字体管理问题解决后，再启用智能选择
    return LayoutStrategy::Simple;
    
    if (features.hasComplexCharacters && features.hasLongText && features.needsWordWrap) {
        return LayoutStrategy::Paragraph;
    }
    
    return LayoutStrategy::Simple;
}

// ==================== SimpleTextLayoutEngine 实现 ====================

bool SimpleTextLayoutEngine::layoutText(SkCanvas* canvas, const TextElement& textElement, 
                                       const SkFont& font, const SkPaint& paint, 
                                       float offsetX, float offsetY) {
    // 如果没有设置宽度和高度，直接按单行模式渲染
    if (textElement.width <= 0 && textElement.height <= 0) {
        canvas->drawString(textElement.content.c_str(), 
                          textElement.transform.x + offsetX, 
                          textElement.transform.y + textElement.style.fontSize + offsetY, 
                          font, paint);
        return true;
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
    
    return true;
}

void SimpleTextLayoutEngine::renderSingleLineText(SkCanvas* canvas, const TextElement& textElement, 
                                                 const SkFont& font, const SkPaint& paint, 
                                                 float offsetX, float offsetY) {
    std::string displayText = textElement.content;
    
    if (textElement.width > 0 && textElement.style.ellipsis) {
        displayText = truncateTextWithEllipsis(textElement.content, textElement.width, font);
    }
    
    canvas->drawString(displayText.c_str(), 
                      textElement.transform.x + offsetX, 
                      textElement.transform.y + textElement.style.fontSize + offsetY, 
                      font, paint);
}

void SimpleTextLayoutEngine::renderMultiLineText(SkCanvas* canvas, const TextElement& textElement, 
                                                const SkFont& font, const SkPaint& paint, 
                                                float offsetX, float offsetY) {
    if (textElement.width <= 0) {
        canvas->drawString(textElement.content.c_str(), 
                          textElement.transform.x + offsetX, 
                          textElement.transform.y + textElement.style.fontSize + offsetY, 
                          font, paint);
        return;
    }
    
    std::vector<std::string> lines = smartWrapText(textElement.content, textElement.width, font);
    
    if (textElement.style.maxLines > 0 && lines.size() > textElement.style.maxLines) {
        lines.resize(textElement.style.maxLines);
        if (textElement.style.ellipsis && !lines.empty()) {
            std::string lastLine = lines.back();
            std::string ellipsisText = truncateTextWithEllipsis(lastLine, textElement.width, font);
            if (ellipsisText != lastLine) {
                lines.back() = ellipsisText;
            }
        }
    }
    
    float lineHeight = textElement.style.fontSize * 1.2f;
    for (size_t i = 0; i < lines.size(); ++i) {
        float y = textElement.transform.y + textElement.style.fontSize + offsetY + i * lineHeight;
        canvas->drawString(lines[i].c_str(), 
                          textElement.transform.x + offsetX, 
                          y, 
                          font, paint);
    }
}

void SimpleTextLayoutEngine::renderWordWrapText(SkCanvas* canvas, const TextElement& textElement, 
                                               const SkFont& font, const SkPaint& paint, 
                                               float offsetX, float offsetY) {
    if (textElement.width <= 0) {
        canvas->drawString(textElement.content.c_str(), 
                          textElement.transform.x + offsetX, 
                          textElement.transform.y + textElement.style.fontSize + offsetY, 
                          font, paint);
        return;
    }
    
    std::vector<std::string> lines = smartWrapText(textElement.content, textElement.width, font);
    
    float lineHeight = textElement.style.fontSize * 1.2f;
    for (size_t i = 0; i < lines.size(); ++i) {
        float y = textElement.transform.y + textElement.style.fontSize + offsetY + i * lineHeight;
        canvas->drawString(lines[i].c_str(), 
                          textElement.transform.x + offsetX, 
                          y, 
                          font, paint);
    }
}

void SimpleTextLayoutEngine::renderAutoFitText(SkCanvas* canvas, const TextElement& textElement, 
                                              const SkFont& font, const SkPaint& paint, 
                                              float offsetX, float offsetY) {
    if (textElement.width <= 0 || textElement.height <= 0) {
        canvas->drawString(textElement.content.c_str(), 
                          textElement.transform.x + offsetX, 
                          textElement.transform.y + textElement.style.fontSize + offsetY, 
                          font, paint);
        return;
    }
    
    float currentFontSize = textElement.style.fontSize;
    float minFontSize = 8.0f;
    
    SkFont currentFont = font;
    
    while (currentFontSize >= minFontSize) {
        currentFont.setSize(currentFontSize);
        std::vector<std::string> lines = smartWrapText(textElement.content, textElement.width, currentFont);
        
        float totalHeight = lines.size() * currentFontSize * 1.2f;
        
        if (totalHeight <= textElement.height) {
            break;
        }
        
        currentFontSize -= 1.0f;
    }
    
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

std::vector<std::string> SimpleTextLayoutEngine::splitText(const std::string& text) {
    std::vector<std::string> lines;
    std::stringstream ss(text);
    std::string line;
    
    while (std::getline(ss, line, '\r')) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    
    if (lines.empty()) {
        std::stringstream ss2(text);
        while (std::getline(ss2, line, '\n')) {
            if (!line.empty()) {
                lines.push_back(line);
            }
        }
    }
    
    if (lines.empty()) {
        lines.push_back(text);
    }
    
    return lines;
}

float SimpleTextLayoutEngine::calculateTextWidth(const std::string& text, const SkFont& font) {
    SkRect bounds;
    font.measureText(text.c_str(), text.length(), SkTextEncoding::kUTF8, &bounds);
    return bounds.width();
}

std::vector<std::string> SimpleTextLayoutEngine::smartWrapText(const std::string& text, float maxWidth, const SkFont& font) {
    std::vector<std::string> lines;
    std::string currentLine;
    
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
                lines.push_back(word);
            }
        }
    }
    
    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }
    
    return lines;
}

std::string SimpleTextLayoutEngine::truncateTextWithEllipsis(const std::string& text, float maxWidth, const SkFont& font) {
    const std::string ellipsis = "...";
    float ellipsisWidth = calculateTextWidth(ellipsis, font);
    
    if (calculateTextWidth(text, font) <= maxWidth) {
        return text;
    }
    
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

// ==================== ParagraphTextLayoutEngine 实现 ====================

bool ParagraphTextLayoutEngine::layoutText(SkCanvas* canvas, const TextElement& textElement, 
                                          const SkFont& font, const SkPaint& paint, 
                                          float offsetX, float offsetY) {
    renderParagraph(canvas, textElement, paint, offsetX, offsetY);
    return true;
}

void ParagraphTextLayoutEngine::renderParagraph(SkCanvas* canvas, const TextElement& textElement, 
                                               const SkPaint& paint, float offsetX, float offsetY) {
    // 由于 SkParagraph 的字体管理比较复杂，我们暂时回退到简单渲染
    // 这样可以确保文字能够正常显示
    std::cerr << "ParagraphTextLayoutEngine: 回退到简单渲染以确保兼容性" << std::endl;
    
    // 使用简单渲染
    canvas->drawString(textElement.content.c_str(), 
                      textElement.transform.x + offsetX, 
                      textElement.transform.y + textElement.style.fontSize + offsetY, 
                      SkFont(), paint);
}

// ==================== TextEffectRenderer 实现 ====================

void TextEffectRenderer::renderShadow(SkCanvas* canvas, const TextElement& textElement, 
                                     TextLayoutEngine* layoutEngine, const SkFont& font) {
    SkPaint shadowPaint;
    shadowPaint.setColor(textElement.style.shadowColor);
    shadowPaint.setStyle(SkPaint::kFill_Style);
    
    layoutEngine->layoutText(canvas, textElement, font, shadowPaint, 
                           textElement.style.shadowDx, textElement.style.shadowDy);
}

void TextEffectRenderer::renderStroke(SkCanvas* canvas, const TextElement& textElement, 
                                     TextLayoutEngine* layoutEngine, const SkFont& font) {
    SkPaint strokePaint;
    strokePaint.setColor(textElement.style.strokeColor);
    strokePaint.setStyle(SkPaint::kStroke_Style);
    strokePaint.setStrokeWidth(textElement.style.strokeWidth);
    
    layoutEngine->layoutText(canvas, textElement, font, strokePaint, 0, 0);
}

void TextEffectRenderer::renderFill(SkCanvas* canvas, const TextElement& textElement, 
                                   TextLayoutEngine* layoutEngine, const SkFont& font) {
    SkPaint fillPaint;
    fillPaint.setColor(textElement.style.fillColor);
    fillPaint.setStyle(SkPaint::kFill_Style);
    
    layoutEngine->layoutText(canvas, textElement, font, fillPaint, 0, 0);
}

} // namespace skia_renderer 