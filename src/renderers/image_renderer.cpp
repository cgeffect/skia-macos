#include "renderers/image_renderer.h"
#include <iostream>

namespace skia_renderer {

ImageRenderer::ImageRenderer() {
}

ImageRenderer::~ImageRenderer() {
}

bool ImageRenderer::renderImage(SkCanvas* canvas, const ImageElement& imageElement) {
    if (!canvas) {
        std::cerr << "画布为空" << std::endl;
        return false;
    }
    
    // 加载图片
    sk_sp<SkImage> image = loadImage(imageElement.path);
    if (!image) {
        std::cerr << "无法加载图片: " << imageElement.path << std::endl;
        return false;
    }
    
    // 保存画布状态
    canvas->save();
    
    // 应用变换
    applyTransform(canvas, imageElement.transform);
    
    // 绘制图片
    drawImage(canvas, image, imageElement);
    
    // 恢复画布状态
    canvas->restore();
    
    return true;
}

sk_sp<SkImage> ImageRenderer::loadImage(const std::string& imagePath) {
    sk_sp<SkData> data = SkData::MakeFromFileName(imagePath.c_str());
    if (!data) {
        return nullptr;
    }
    
    return SkImages::DeferredFromEncodedData(data);
}

bool ImageRenderer::isValidImage(const std::string& imagePath) {
    sk_sp<SkImage> image = loadImage(imagePath);
    return image != nullptr;
}

void ImageRenderer::applyTransform(SkCanvas* canvas, const Transform& transform) {
    canvas->translate(transform.x, transform.y);
    canvas->scale(transform.scaleX, transform.scaleY);
    canvas->rotate(transform.rotation);
}

void ImageRenderer::drawImage(SkCanvas* canvas, sk_sp<SkImage> image, const ImageElement& imageElement) {
    // 创建绘制矩形
    SkRect dstRect = SkRect::MakeXYWH(0, 0, imageElement.width, imageElement.height);
    
    // 设置绘制参数
    SkPaint paint;
    paint.setAlpha(static_cast<U8CPU>(imageElement.transform.opacity * 255));
    
    // 绘制图片
    canvas->drawImageRect(image, dstRect, SkSamplingOptions(), &paint);
}

} // namespace skia_renderer 