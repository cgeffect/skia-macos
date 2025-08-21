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
#include "include/ports/SkFontMgr_mac_ct.h"

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
    
    // 新策略：基于displayMode属性选择渲染引擎
    // 只有明确设置了新的displayMode属性才使用Paragraph
    // 对于旧的协议（没有displayMode属性），使用Simple布局
    if (textElement.style.displayMode == TextDisplayMode::SingleLine ||
        textElement.style.displayMode == TextDisplayMode::MultiLine ||
        textElement.style.displayMode == TextDisplayMode::AutoFit) {
        return LayoutStrategy::Paragraph;
    }
    
    // 对于WordWrap模式，需要检查是否有其他新属性
    if (textElement.style.displayMode == TextDisplayMode::WordWrap) {
        // 如果有maxLines或ellipsis属性，说明是新的WordWrap模式
        if (textElement.style.maxLines > 0 || textElement.style.ellipsis) {
            return LayoutStrategy::Paragraph;
        }
    }
    
    // 旧的渲染方式使用Simple（默认的WordWrap或没有新属性）
    return LayoutStrategy::Simple;
}

// ==================== SimpleTextLayoutEngine 实现 ====================

bool SimpleTextLayoutEngine::layoutText(SkCanvas* canvas, const TextElement& textElement, 
                                       const SkFont& font, const SkPaint& paint, 
                                       float offsetX, float offsetY) {
    // SimpleTextLayoutEngine只处理无宽高限制的文本（旧的渲染逻辑）
    // 支持手动换行（\r\n），不支持自动换行
    
    // 分割文本（按\r\n换行）
    std::vector<std::string> lines = splitText(textElement.content);
    
    float lineHeight = textElement.style.fontSize * 1.2f;
    for (size_t i = 0; i < lines.size(); ++i) {
        float y = textElement.transform.y + textElement.style.fontSize + offsetY + i * lineHeight;
        canvas->drawString(lines[i].c_str(), 
                          textElement.transform.x + offsetX, 
                          y, 
                          font, paint);
    }
    
    return true;
}

// SimpleTextLayoutEngine不再需要复杂的渲染方法，只保留基本的文本分割功能

// 这些复杂的渲染方法现在由ParagraphTextLayoutEngine处理

// 这些复杂的渲染方法现在由ParagraphTextLayoutEngine处理

// 这些复杂的渲染方法现在由ParagraphTextLayoutEngine处理

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

// 这些方法现在由ParagraphTextLayoutEngine处理，不再需要

// ==================== ParagraphTextLayoutEngine 实现 ====================

bool ParagraphTextLayoutEngine::layoutText(SkCanvas* canvas, const TextElement& textElement, 
                                          const SkFont& font, const SkPaint& paint, 
                                          float offsetX, float offsetY) {
    renderParagraph(canvas, textElement, paint, offsetX, offsetY);
    return true;
}

void ParagraphTextLayoutEngine::renderParagraph(SkCanvas* canvas, const TextElement& textElement, 
                                               const SkPaint& paint, float offsetX, float offsetY) {
    try {
        // 创建字体集合
        auto fontCollection = sk_make_sp<skia::textlayout::FontCollection>();
        
        // 使用系统默认字体管理器
        auto fontMgr = SkFontMgr_New_CoreText(nullptr);
        if (fontMgr) {
            fontCollection->setDefaultFontManager(fontMgr);
        }
        
        // 创建段落样式
        skia::textlayout::ParagraphStyle paragraphStyle;
        
        // 根据显示模式设置样式
        switch (textElement.style.displayMode) {
            case TextDisplayMode::SingleLine:
                paragraphStyle.setMaxLines(1);
                if (textElement.style.ellipsis) {
                    paragraphStyle.setEllipsis(SkString("..."));
                }
                break;
                
            case TextDisplayMode::MultiLine:
                if (textElement.style.maxLines > 0) {
                    paragraphStyle.setMaxLines(textElement.style.maxLines);
                }
                if (textElement.style.ellipsis) {
                    paragraphStyle.setEllipsis(SkString("..."));
                }
                break;
                
            case TextDisplayMode::WordWrap:
                // 自动换行，不需要特殊设置
                break;
                
            case TextDisplayMode::AutoFit:
                // 自适应模式，通过调整字体大小实现
                break;
        }
        
        // 设置文本对齐
        paragraphStyle.setTextAlign(skia::textlayout::TextAlign::kLeft);
        
        // 创建段落构建器
        auto paragraphBuilder = skia::textlayout::ParagraphBuilder::make(paragraphStyle, fontCollection);
        
        // 创建文本样式
        skia::textlayout::TextStyle textStyle;
        textStyle.setFontSize(textElement.style.fontSize);
        textStyle.setColor(textElement.style.fillColor);
        
        // 设置字体
        if (!textElement.style.fontFamily.empty()) {
            std::vector<SkString> fontFamilies;
            fontFamilies.push_back(SkString(textElement.style.fontFamily.c_str()));
            textStyle.setFontFamilies(fontFamilies);
        }
        
        // 添加文本
        paragraphBuilder->pushStyle(textStyle);
        paragraphBuilder->addText(textElement.content.c_str());
        paragraphBuilder->pop();
        
        // 构建段落
        auto paragraph = paragraphBuilder->Build();
        
        // 布局段落
        float layoutWidth = textElement.width > 0 ? textElement.width : 1000.0f;
        paragraph->layout(layoutWidth);
        
        // 渲染段落
        paragraph->paint(canvas, 
                        textElement.transform.x + offsetX, 
                        textElement.transform.y + textElement.style.fontSize + offsetY);
        
    } catch (const std::exception& e) {
        std::cerr << "ParagraphTextLayoutEngine 错误: " << e.what() << std::endl;
        // 回退到简单渲染
        canvas->drawString(textElement.content.c_str(), 
                          textElement.transform.x + offsetX, 
                          textElement.transform.y + textElement.style.fontSize + offsetY, 
                          SkFont(), paint);
    }
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