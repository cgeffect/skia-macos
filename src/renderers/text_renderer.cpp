#include "renderers/text_renderer.h"
#include "renderers/rich_text_renderer.h"
#include <iostream>

namespace skia_renderer {

// ==================== TextRenderer 主类实现 ====================

TextRenderer::TextRenderer() :
    layoutStrategy(LayoutStrategy::Auto) {
    fontManager = std::make_shared<FontManager>();
    simpleLayoutEngine = std::make_unique<SimpleTextLayoutEngine>();
    paragraphLayoutEngine = std::make_unique<ParagraphTextLayoutEngine>();
}

TextRenderer::~TextRenderer() {
}

bool TextRenderer::renderText(SkCanvas *canvas, const TextElement &textElement, bool debugMode) {
    if (!canvas) {
        std::cerr << "画布为空" << std::endl;
        return false;
    }

    if (!fontManager) {
        std::cerr << "字体管理器未初始化" << std::endl;
        return false;
    }

    // 【新增】检查是否为富文本模式
    if (textElement.isRichText()) {
        #ifndef NDEBUG
        std::cout << "调试: 检测到富文本模式，片段数量: " << textElement.richTextSegments.size() << std::endl;
        #endif
        
        // 创建富文本渲染器
        auto richTextRenderer = RichTextRendererFactory::create(textElement.richTextStrategy);
        
        #ifndef NDEBUG
        std::cout << "调试: 使用富文本渲染策略: " << richTextRenderer->getStrategyName() << std::endl;
        #endif
        
        // 应用透明度
        if (textElement.transform.opacity < 1.0f) {
            canvas->save();
            SkPaint opacityPaint;
            opacityPaint.setAlphaf(textElement.transform.opacity);
            canvas->saveLayer(nullptr, &opacityPaint);
        }
        
        // 渲染富文本
        bool success = richTextRenderer->renderRichText(canvas, textElement, fontManager.get());
        
        // 恢复透明度状态
        if (textElement.transform.opacity < 1.0f) {
            canvas->restore(); // 恢复saveLayer
            canvas->restore(); // 恢复save
        }
        
        return success;
    }

    // 【原有逻辑】普通文本渲染
    // 加载字体
    sk_sp<SkTypeface> typeface = fontManager->loadFont(textElement.style.fontFamily);
    if (!typeface) {
        std::cerr << "无法加载字体: " << textElement.style.fontFamily << std::endl;
        return false;
    }

// 调试信息
#ifndef NDEBUG
    std::cout << "调试: 加载字体 '" << textElement.style.fontFamily << "' 成功" << std::endl;
    std::cout << "调试: 文本内容: '" << textElement.content << "'" << std::endl;
    std::cout << "调试: 位置: (" << textElement.transform.x << ", " << textElement.transform.y << ")" << std::endl;
#endif

    // 创建字体
    SkFont font(typeface, textElement.style.fontSize);

    // 保存画布状态
    canvas->save();

    // 应用变换
    applyTransform(canvas, textElement.transform);

    // 选择布局引擎
    TextLayoutEngine *selectedEngine = selectLayoutEngine(textElement);

    // 【关键理解】文本效果通过多次绘制实现，每次使用不同的Paint设置
    // 绘制顺序很重要：阴影 → 描边 → 填充（从底层到顶层）
    
    // 第一次绘制：阴影（如果有）
    // 实际上是在偏移位置绘制一次文本，使用阴影颜色
    if (textElement.style.hasShadow) {
        TextEffectRenderer::renderShadow(canvas, textElement, selectedEngine, font);
    }

    // 第二次绘制：描边（如果有）  
    // 实际上是绘制文本的轮廓线，使用描边颜色和宽度
    if (textElement.style.strokeWidth > 0) {
        TextEffectRenderer::renderStroke(canvas, textElement, selectedEngine, font);
    }

    // 第三次绘制：填充
    // 在描边基础上填充文本内部，使用填充颜色
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

void TextRenderer::applyTransform(SkCanvas *canvas, const Transform &transform) {
    // 【关键理解】这是"围绕指定点进行变换"的标准算法
    // 
    // 数学原理：要围绕点P进行变换，需要：
    // 1. 将坐标系移动到P点（让P成为原点）
    // 2. 在新坐标系中进行缩放和旋转（围绕原点=围绕P点）
    // 3. 将坐标系移回原来的位置
    // 
    // 虽然最后移回了原点，但变换矩阵已经"记住"了所有操作！
    
    canvas->translate(transform.x, transform.y);       // 1. 移动到变换中心点P
    canvas->scale(transform.scaleX, transform.scaleY); // 2. 围绕P点缩放
    canvas->rotate(transform.rotation);                // 3. 围绕P点旋转
    canvas->translate(-transform.x, -transform.y);     // 4. 移回原始坐标系
    
    // 此时变换矩阵 = T(-P) * R * S * T(P)
    // 结果：任何绘制的内容都会围绕点P进行缩放和旋转！
}

void TextRenderer::drawDebugRect(SkCanvas *canvas, const TextElement &textElement, float offsetX, float offsetY) {
    if (textElement.width > 0 && textElement.height > 0) {
        SkPaint debugPaint;
        debugPaint.setColor(SK_ColorRED);
        debugPaint.setStyle(SkPaint::kStroke_Style);
        debugPaint.setStrokeWidth(2.0f);

        // 修正：红色边框应该与文本基线对齐
        // 文本渲染从 transform.y + fontSize 开始，所以边框也应该从这里开始
        SkRect debugRect = SkRect::MakeXYWH(
            textElement.transform.x + offsetX,
            textElement.transform.y + textElement.style.fontSize + offsetY,
            textElement.width,
            textElement.height);

        canvas->drawRect(debugRect, debugPaint);
    }
}

TextLayoutEngine *TextRenderer::selectLayoutEngine(const TextElement &textElement) {
    // 【性能统计】记录总的文本渲染次数，用于性能分析和调试
    // 每次调用selectLayoutEngine都表示要渲染一个文本元素
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
        // 【性能统计】记录使用ParagraphTextLayoutEngine的次数
        renderStats.paragraphLayoutCount++;
        return paragraphLayoutEngine.get();
    } else {
        // 【性能统计】记录使用SimpleTextLayoutEngine的次数
        renderStats.simpleLayoutCount++;
        return simpleLayoutEngine.get();
    }
}

} // namespace skia_renderer