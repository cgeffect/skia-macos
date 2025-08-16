#include "renderers/canvas_renderer.h"
#include <iostream>

namespace skia_renderer {

CanvasRenderer::CanvasRenderer() : canvas(nullptr) {
}

CanvasRenderer::~CanvasRenderer() {
}

bool CanvasRenderer::createCanvas(int width, int height) {
    SkImageInfo info = SkImageInfo::Make(
        width, height,
        SkColorType::kRGBA_8888_SkColorType,
        SkAlphaType::kOpaque_SkAlphaType);
    
    surface = SkSurfaces::Raster(info);
    if (!surface) {
        std::cerr << "无法创建Surface" << std::endl;
        return false;
    }
    
    canvas = surface->getCanvas();
    return true;
}

bool CanvasRenderer::setBackground(const std::string& backgroundColor) {
    if (!canvas) {
        std::cerr << "画布未初始化" << std::endl;
        return false;
    }
    
    SkColor color = parseBackgroundColor(backgroundColor);
    canvas->clear(color);
    return true;
}

SkCanvas* CanvasRenderer::getCanvas() const {
    return canvas;
}

sk_sp<SkSurface> CanvasRenderer::getSurface() const {
    return surface;
}

sk_sp<SkImage> CanvasRenderer::makeImageSnapshot() {
    if (!surface) {
        return nullptr;
    }
    return surface->makeImageSnapshot();
}

SkColor CanvasRenderer::parseBackgroundColor(const std::string& colorString) {
    return ColorParser::parseColor(colorString);
}

} // namespace skia_renderer 