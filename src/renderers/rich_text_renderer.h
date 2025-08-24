#pragma once

#include "core/types.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"
#include <memory>

namespace skia_renderer {

// 前向声明
class FontManager;

/**
 * 富文本渲染器接口 - 支持多态渲染策略
 * 
 * 设计理念：
 * - 策略模式：支持不同的渲染实现（measureText vs SkParagraph）
 * - 继承样式：子片段可以继承父级TextElement的样式
 * - 自动布局：自动计算每个片段的位置，支持字间距控制
 */
class IRichTextRenderer {
public:
    virtual ~IRichTextRenderer() = default;
    
    /**
     * 渲染富文本
     * @param canvas 画布
     * @param textElement 文本元素（包含富文本片段数组）
     * @param fontManager 字体管理器
     * @return 渲染是否成功
     */
    virtual bool renderRichText(SkCanvas* canvas, 
                               const TextElement& textElement,
                               FontManager* fontManager) = 0;
    
    /**
     * 计算富文本的总宽度
     * @param textElement 文本元素
     * @param fontManager 字体管理器
     * @return 总宽度（像素）
     */
    virtual float calculateTotalWidth(const TextElement& textElement,
                                     FontManager* fontManager) = 0;
    
    /**
     * 获取渲染策略名称（用于调试）
     */
    virtual std::string getStrategyName() const = 0;

protected:
    /**
     * 合并样式 - 将片段样式与父级样式合并
     * @param parentStyle 父级TextStyle
     * @param segment 富文本片段
     * @return 合并后的TextStyle
     */
    TextStyle mergeStyles(const TextStyle& parentStyle, const RichTextSegment& segment) const;
};

/**
 * 基于measureText API的富文本渲染器（默认实现）
 * 
 * 特点：
 * - 精确控制：使用SkFont::measureText精确测量每个片段宽度
 * - 高性能：直接使用Skia基础API，开销小
 * - 灵活性高：可以精确控制每个片段的位置和样式
 */
class MeasureTextRichTextRenderer : public IRichTextRenderer {
public:
    bool renderRichText(SkCanvas* canvas, 
                       const TextElement& textElement,
                       FontManager* fontManager) override;
    
    float calculateTotalWidth(const TextElement& textElement,
                             FontManager* fontManager) override;
    
    std::string getStrategyName() const override { return "MeasureText"; }

private:
    /**
     * 渲染单个富文本片段
     */
    void renderSegment(SkCanvas* canvas,
                      const RichTextSegment& segment,
                      const TextStyle& mergedStyle,
                      float x, float y,
                      FontManager* fontManager);
    
    /**
     * 计算单个片段的宽度
     */
    float calculateSegmentWidth(const RichTextSegment& segment,
                               const TextStyle& mergedStyle,
                               FontManager* fontManager);
};

/**
 * 基于SkParagraph API的富文本渲染器
 * 
 * 特点：
 * - 功能强大：支持复杂的文本布局和排版
 * - 自动处理：自动处理字体回退、连字等高级特性
 * - 标准兼容：遵循Web标准的文本渲染
 */
class ParagraphRichTextRenderer : public IRichTextRenderer {
public:
    bool renderRichText(SkCanvas* canvas, 
                       const TextElement& textElement,
                       FontManager* fontManager) override;
    
    float calculateTotalWidth(const TextElement& textElement,
                             FontManager* fontManager) override;
    
    std::string getStrategyName() const override { return "Paragraph"; }
};

/**
 * 富文本渲染器工厂
 */
class RichTextRendererFactory {
public:
    /**
     * 创建富文本渲染器
     * @param strategy 渲染策略
     * @return 渲染器实例
     */
    static std::unique_ptr<IRichTextRenderer> create(RichTextRenderStrategy strategy);
};

} // namespace skia_renderer
