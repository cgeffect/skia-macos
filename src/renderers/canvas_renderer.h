#pragma once

#include "core/types.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/core/SkColor.h"
#include "utils/color_parser.h"

namespace skia_renderer {

class CanvasRenderer {
public:
    CanvasRenderer();
    ~CanvasRenderer();
    
    // 创建画布
    bool createCanvas(int width, int height);
    
    // 设置背景
    bool setBackground(const std::string& backgroundColor);
    
    // 获取画布
    SkCanvas* getCanvas() const;
    
    // 获取Surface
    sk_sp<SkSurface> getSurface() const;
    
    // 获取图像快照
    sk_sp<SkImage> makeImageSnapshot();

private:
    sk_sp<SkSurface> surface;
    SkCanvas* canvas;
    
    // 辅助方法
    SkColor parseBackgroundColor(const std::string& colorString);
};

} // namespace skia_renderer 