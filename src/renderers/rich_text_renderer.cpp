#include "renderers/rich_text_renderer.h"
#include "resources/font_manager.h"
#include "renderers/text_layout.h"
#include <iostream>
#include <algorithm>

// SkParagraph相关头文件
#include "modules/skparagraph/include/Paragraph.h"
#include "modules/skparagraph/include/ParagraphBuilder.h"
#include "modules/skparagraph/include/ParagraphStyle.h"
#include "modules/skparagraph/include/TextStyle.h"
#include "include/core/SkFontMgr.h"
#include "include/ports/SkFontMgr_mac_ct.h"
#include "include/core/SkMaskFilter.h"

namespace skia_renderer {

// ==================== IRichTextRenderer 基类实现 ====================

TextStyle IRichTextRenderer::mergeStyles(const TextStyle& parentStyle, const RichTextSegment& segment) const {
    TextStyle merged = parentStyle;
    
    // 合并字体相关属性
    if (!segment.fontFamily.empty()) {
        merged.fontFamily = segment.fontFamily;
    }
    if (segment.fontSize > 0.0f) {
        merged.fontSize = segment.fontSize;
    }
    
    // 合并颜色属性（使用透明色表示继承）
    if (segment.fillColor != SK_ColorTRANSPARENT) {
        merged.fillColor = segment.fillColor;
    }
    if (segment.strokeColor != SK_ColorTRANSPARENT) {
        merged.strokeColor = segment.strokeColor;
    }
    if (segment.strokeWidth >= 0.0f) {
        merged.strokeWidth = segment.strokeWidth;
    }
    
    // 合并阴影属性
    if (segment.hasShadow) {
        merged.hasShadow = true;
        merged.shadowDx = segment.shadowDx;
        merged.shadowDy = segment.shadowDy;
        merged.shadowSigma = segment.shadowSigma;
        if (segment.shadowColor != SK_ColorTRANSPARENT) {
            merged.shadowColor = segment.shadowColor;
        }
    }
    
    return merged;
}

// ==================== MeasureTextRichTextRenderer 实现 ====================

bool MeasureTextRichTextRenderer::renderRichText(SkCanvas* canvas, 
                                                 const TextElement& textElement,
                                                 FontManager* fontManager) {
    if (!canvas || !fontManager || textElement.richTextSegments.empty()) {
        return false;
    }
    
    // 保存画布状态
    canvas->save();
    
    // 应用基础变换
    canvas->translate(textElement.transform.x, textElement.transform.y);
    canvas->scale(textElement.transform.scaleX, textElement.transform.scaleY);
    canvas->rotate(textElement.transform.rotation);
    
    float currentX = 0.0f;
    float baseY = textElement.style.fontSize; // 基线位置
    
    #ifndef NDEBUG
    std::cout << "调试: MeasureText富文本渲染 - 片段数量: " << textElement.richTextSegments.size() << std::endl;
    #endif
    
    // 逐个渲染每个富文本片段
    for (const auto& segment : textElement.richTextSegments) {
        // 合并样式
        TextStyle mergedStyle = mergeStyles(textElement.style, segment);
        
        // 渲染片段
        renderSegment(canvas, segment, mergedStyle, currentX, baseY, fontManager);
        
        // 计算下一个片段的X位置
        /**
         segmentWidth：文本紧贴的边界框宽度
         letterSpacing：片段之间的字间距, 如果为0则为字体本身的字间距, 如果设置为正值, 则为本身的字间距加上增量的字间距
         */
        float segmentWidth = calculateSegmentWidth(segment, mergedStyle, fontManager);
        currentX += segmentWidth + textElement.letterSpacing;
        
        #ifndef NDEBUG
        std::cout << "调试: 片段\"" << segment.content << "\" - 宽度: " << segmentWidth 
                  << "px, 位置: " << currentX << "px" << std::endl;
        #endif
    }
    
    // 恢复画布状态
    canvas->restore();
    
    return true;
}

void MeasureTextRichTextRenderer::renderSegment(SkCanvas* canvas,
                                               const RichTextSegment& segment,
                                               const TextStyle& mergedStyle,
                                               float x, float y,
                                               FontManager* fontManager) {
    // 加载字体
    auto typeface = fontManager->loadFont(mergedStyle.fontFamily);
    if (!typeface) {
        std::cerr << "警告: 无法加载字体 " << mergedStyle.fontFamily << "，使用默认字体" << std::endl;
        typeface = fontManager->getDefaultFont();
    }
    
    SkFont font(typeface, mergedStyle.fontSize);
    
    // 【直接渲染文本效果】避免使用layoutEngine
    
    // 渲染阴影
    if (mergedStyle.hasShadow) {
        SkPaint shadowPaint;
        shadowPaint.setColor(mergedStyle.shadowColor);
        shadowPaint.setStyle(SkPaint::kFill_Style);
        
        // TODO: 添加阴影模糊效果支持
        // 目前简化实现，不支持模糊
        
        // 在偏移位置绘制阴影
        canvas->drawString(segment.content.c_str(), 
                          x + mergedStyle.shadowDx, 
                          y + mergedStyle.shadowDy, 
                          font, shadowPaint);
    }
    
    // 渲染描边
    if (mergedStyle.strokeWidth > 0.0f) {
        SkPaint strokePaint;
        strokePaint.setColor(mergedStyle.strokeColor);
        strokePaint.setStyle(SkPaint::kStroke_Style);
        strokePaint.setStrokeWidth(mergedStyle.strokeWidth);
        
        canvas->drawString(segment.content.c_str(), x, y, font, strokePaint);
    }
    
    // 渲染填充
    SkPaint fillPaint;
    fillPaint.setColor(mergedStyle.fillColor);
    fillPaint.setStyle(SkPaint::kFill_Style);
    
    canvas->drawString(segment.content.c_str(), x, y, font, fillPaint);
}

float MeasureTextRichTextRenderer::calculateSegmentWidth(const RichTextSegment& segment,
                                                        const TextStyle& mergedStyle,
                                                        FontManager* fontManager) {
    // 加载字体
    auto typeface = fontManager->loadFont(mergedStyle.fontFamily);
    if (!typeface) {
        typeface = fontManager->getDefaultFont();
    }
    
    SkFont font(typeface, mergedStyle.fontSize);
    
    // 使用measureText精确测量
    return font.measureText(segment.content.c_str(), segment.content.size(), SkTextEncoding::kUTF8);
}

float MeasureTextRichTextRenderer::calculateTotalWidth(const TextElement& textElement,
                                                       FontManager* fontManager) {
    float totalWidth = 0.0f;
    
    for (const auto& segment : textElement.richTextSegments) {
        TextStyle mergedStyle = mergeStyles(textElement.style, segment);
        float segmentWidth = calculateSegmentWidth(segment, mergedStyle, fontManager);
        totalWidth += segmentWidth;
        
        // 添加字间距（除了最后一个片段）
        if (&segment != &textElement.richTextSegments.back()) {
            totalWidth += textElement.letterSpacing;
        }
    }
    
    return totalWidth;
}

// ==================== ParagraphRichTextRenderer 实现 ====================

bool ParagraphRichTextRenderer::renderRichText(SkCanvas* canvas, 
                                               const TextElement& textElement,
                                               FontManager* fontManager) {
    if (!canvas || !fontManager || textElement.richTextSegments.empty()) {
        return false;
    }
    
    try {
        // 创建字体集合
        auto fontCollection = sk_make_sp<skia::textlayout::FontCollection>();
        fontCollection->setDefaultFontManager(SkFontMgr_New_CoreText(nullptr));
        
        // 创建段落样式
        skia::textlayout::ParagraphStyle paragraphStyle;
        paragraphStyle.setTextAlign(skia::textlayout::TextAlign::kLeft);
        
        // 创建段落构建器
        auto paragraphBuilder = skia::textlayout::ParagraphBuilder::make(paragraphStyle, fontCollection);
        
        #ifndef NDEBUG
        std::cout << "调试: Paragraph富文本渲染 - 片段数量: " << textElement.richTextSegments.size() << std::endl;
        #endif
        
        // 逐个添加富文本片段
        for (const auto& segment : textElement.richTextSegments) {
            // 合并样式
            TextStyle mergedStyle = mergeStyles(textElement.style, segment);
            
            // 创建SkParagraph文本样式
            skia::textlayout::TextStyle textStyle;
            textStyle.setFontSize(mergedStyle.fontSize);
            textStyle.setColor(mergedStyle.fillColor);
            
            // 设置字体
            if (!mergedStyle.fontFamily.empty()) {
                std::vector<SkString> fontFamilies;
                fontFamilies.push_back(SkString(mergedStyle.fontFamily.c_str()));
                textStyle.setFontFamilies(fontFamilies);
            }
            
            // 设置描边（如果有）
            if (mergedStyle.strokeWidth > 0.0f) {
                // SkParagraph的描边支持有限，这里简化处理
                // 实际项目中可能需要使用多次渲染来模拟描边效果
            }
            
            // 设置字间距
            if (textElement.letterSpacing > 0.0f) {
                textStyle.setLetterSpacing(textElement.letterSpacing);
            }
            
            // 应用样式并添加文本
            paragraphBuilder->pushStyle(textStyle);
            paragraphBuilder->addText(segment.content.c_str());
            paragraphBuilder->pop();
        }
        
        // 构建段落
        auto paragraph = paragraphBuilder->Build();
        
        // 布局段落
        float layoutWidth = textElement.width > 0 ? textElement.width : 1000.0f;
        paragraph->layout(layoutWidth);
        
        // 保存画布状态并应用变换
        canvas->save();
        canvas->translate(textElement.transform.x, textElement.transform.y);
        canvas->scale(textElement.transform.scaleX, textElement.transform.scaleY);
        canvas->rotate(textElement.transform.rotation);
        
        // 渲染段落
        paragraph->paint(canvas, 0, 0);
        
        // 恢复画布状态
        canvas->restore();
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "ParagraphRichTextRenderer 错误: " << e.what() << std::endl;
        return false;
    }
}

float ParagraphRichTextRenderer::calculateTotalWidth(const TextElement& textElement,
                                                     FontManager* fontManager) {
    try {
        // 创建字体集合
        auto fontCollection = sk_make_sp<skia::textlayout::FontCollection>();
        fontCollection->setDefaultFontManager(SkFontMgr_New_CoreText(nullptr));
        
        // 创建段落样式
        skia::textlayout::ParagraphStyle paragraphStyle;
        paragraphStyle.setTextAlign(skia::textlayout::TextAlign::kLeft);
        
        // 创建段落构建器
        auto paragraphBuilder = skia::textlayout::ParagraphBuilder::make(paragraphStyle, fontCollection);
        
        // 添加所有片段
        for (const auto& segment : textElement.richTextSegments) {
            TextStyle mergedStyle = mergeStyles(textElement.style, segment);
            
            skia::textlayout::TextStyle textStyle;
            textStyle.setFontSize(mergedStyle.fontSize);
            
            if (!mergedStyle.fontFamily.empty()) {
                std::vector<SkString> fontFamilies;
                fontFamilies.push_back(SkString(mergedStyle.fontFamily.c_str()));
                textStyle.setFontFamilies(fontFamilies);
            }
            
            if (textElement.letterSpacing > 0.0f) {
                textStyle.setLetterSpacing(textElement.letterSpacing);
            }
            
            paragraphBuilder->pushStyle(textStyle);
            paragraphBuilder->addText(segment.content.c_str());
            paragraphBuilder->pop();
        }
        
        // 构建并布局段落
        auto paragraph = paragraphBuilder->Build();
        paragraph->layout(10000.0f); // 使用很大的宽度来获取单行宽度
        
        return paragraph->getMaxWidth();
        
    } catch (const std::exception& e) {
        std::cerr << "ParagraphRichTextRenderer calculateTotalWidth 错误: " << e.what() << std::endl;
        return 0.0f;
    }
}

// ==================== RichTextRendererFactory 实现 ====================

std::unique_ptr<IRichTextRenderer> RichTextRendererFactory::create(RichTextRenderStrategy strategy) {
    switch (strategy) {
        case RichTextRenderStrategy::MeasureText:
            return std::make_unique<MeasureTextRichTextRenderer>();
        case RichTextRenderStrategy::Paragraph:
            return std::make_unique<ParagraphRichTextRenderer>();
        default:
            return std::make_unique<MeasureTextRichTextRenderer>(); // 默认策略
    }
}

} // namespace skia_renderer
