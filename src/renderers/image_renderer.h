#pragma once

#include "core/types.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkImage.h"
#include "include/core/SkData.h"
#include "include/core/SkRect.h"
#include "include/core/SkPaint.h"
#include <string>

namespace skia_renderer {

class ImageRenderer {
public:
    ImageRenderer();
    ~ImageRenderer();
    
    // 渲染图片元素
    bool renderImage(SkCanvas* canvas, const ImageElement& imageElement);
    
    // 加载图片
    sk_sp<SkImage> loadImage(const std::string& imagePath);
    
    // 检查图片是否有效
    bool isValidImage(const std::string& imagePath);

private:
    // 应用变换
    void applyTransform(SkCanvas* canvas, const Transform& transform);
    
    // 绘制图片
    void drawImage(SkCanvas* canvas, sk_sp<SkImage> image, const ImageElement& imageElement);
};

} // namespace skia_renderer 