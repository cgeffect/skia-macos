#pragma once

#include "core/types.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"
#include "include/core/SkTypeface.h"
#include "renderers/text_layout.h"
#include "resources/font_manager.h"
#include <memory>
#include <vector>
#include <string>

namespace skia_renderer {

// 主文本渲染器
class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();
    
    // 渲染文本元素
    bool renderText(SkCanvas* canvas, const TextElement& textElement, bool debugMode = false);
    
    // 设置字体管理器
    void setFontManager(std::shared_ptr<FontManager> fontManager);
    
    // 获取字体管理器
    std::shared_ptr<FontManager> getFontManager() const;
    
    // 设置布局策略
    void setLayoutStrategy(LayoutStrategy strategy) { layoutStrategy = strategy; }
    
    // 获取布局策略
    LayoutStrategy getLayoutStrategy() const { return layoutStrategy; }
    
    // 获取渲染统计信息
    struct RenderStats {
        int simpleLayoutCount = 0;
        int paragraphLayoutCount = 0;
        int totalRenderCount = 0;
    };
    
    RenderStats getRenderStats() const { return renderStats; }
    void resetRenderStats() { renderStats = RenderStats(); }

private:
    std::shared_ptr<FontManager> fontManager;
    std::unique_ptr<SimpleTextLayoutEngine> simpleLayoutEngine;
    std::unique_ptr<ParagraphTextLayoutEngine> paragraphLayoutEngine;
    LayoutStrategy layoutStrategy;
    RenderStats renderStats;
    
    // 应用变换
    void applyTransform(SkCanvas* canvas, const Transform& transform);
    
    // 绘制调试框框
    void drawDebugRect(SkCanvas* canvas, const TextElement& textElement, float offsetX, float offsetY);
    
    // 智能选择布局引擎
    TextLayoutEngine* selectLayoutEngine(const TextElement& textElement);
};

} // namespace skia_renderer 