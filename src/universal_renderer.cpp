#include "universal_renderer.h"

UniversalRenderer::UniversalRenderer() {
    // 初始化字体管理器
    fontMgr = SkFontMgr_New_CoreText(nullptr);
}

UniversalRenderer::~UniversalRenderer() {
}

bool UniversalRenderer::renderFromProtocol(const std::string& protocolFile) {
    ProtocolParser parser;
    if (!parser.loadFromFile(protocolFile)) {
        errorMessage = "协议解析失败: " + parser.getErrorMessage();
        return false;
    }
    
    return renderFromProtocol(parser.getProtocol());
}

bool UniversalRenderer::renderFromProtocol(const RenderProtocol& protocol) {
    // 创建画布
    SkImageInfo info = SkImageInfo::Make(
        protocol.canvas.width, protocol.canvas.height,
        SkColorType::kRGBA_8888_SkColorType,
        SkAlphaType::kOpaque_SkAlphaType);
    sk_sp<SkSurface> surface(SkSurfaces::Raster(info));
    if (!surface) {
        errorMessage = "无法创建Surface";
        return false;
    }
    SkCanvas* canvas = surface->getCanvas();
    
    // 渲染画布背景
    if (!renderCanvas(canvas, protocol.canvas)) {
        return false;
    }
    
    // 渲染图片元素
    renderImages(canvas, protocol.images);
    
    // 渲染文本元素
    renderTexts(canvas, protocol.texts);
    
    // 保存图像
    sk_sp<SkImage> image = surface->makeImageSnapshot();
    if (!saveImage(image, protocol.output.filename)) {
        return false;
    }
    
    std::cout << "海报已保存为" << protocol.output.filename << std::endl;
    return true;
}

bool UniversalRenderer::renderCanvas(SkCanvas* canvas, const CanvasConfig& canvasConfig) {
    // 设置背景色
    SkColor backgroundColor = SK_ColorWHITE;
    if (!canvasConfig.background.empty()) {
        // 简单的颜色解析，可以扩展
        if (canvasConfig.background == "#FFFFFF" || canvasConfig.background == "white") {
            backgroundColor = SK_ColorWHITE;
        } else if (canvasConfig.background == "#000000" || canvasConfig.background == "black") {
            backgroundColor = SK_ColorBLACK;
        } else {
            // 这里可以添加更复杂的颜色解析
            backgroundColor = SK_ColorWHITE;
        }
    }
    
    canvas->clear(backgroundColor);
    return true;
}

void UniversalRenderer::renderImages(SkCanvas* canvas, const std::vector<ImageElement>& images) {
    for (const auto& img : images) {
        renderImageElement(canvas, img);
    }
}

void UniversalRenderer::renderImageElement(SkCanvas* canvas, const ImageElement& img) {
    // 加载图片
    sk_sp<SkData> data = SkData::MakeFromFileName(img.path.c_str());
    if (!data) {
        std::cerr << "无法加载图片: " << img.path << std::endl;
        return;
    }
    
    sk_sp<SkImage> image = SkImages::DeferredFromEncodedData(data);
    if (!image) {
        std::cerr << "无法解码图片: " << img.path << std::endl;
        return;
    }
    
    // 保存画布状态
    canvas->save();
    
    // 应用变换
    canvas->translate(img.x, img.y);
    canvas->scale(img.scaleX, img.scaleY);
    canvas->rotate(img.rotation);
    
    // 创建绘制矩形
    SkRect dstRect = SkRect::MakeXYWH(0, 0, img.width, img.height);
    
    // 设置绘制参数
    SkPaint paint;
    paint.setAlpha(static_cast<U8CPU>(img.opacity * 255));
    
    // 绘制图片
    canvas->drawImageRect(image, dstRect, SkSamplingOptions(), &paint);
    
    // 恢复画布状态
    canvas->restore();
}

void UniversalRenderer::renderTexts(SkCanvas* canvas, const std::vector<TextElement>& texts) {
    for (const auto& text : texts) {
        sk_sp<SkTypeface> typeface = loadFont(text.fontFamily);
        renderTextElement(canvas, text, typeface);
    }
}

void UniversalRenderer::renderTextElement(SkCanvas* canvas, const TextElement& text, sk_sp<SkTypeface> typeface) {
    if (!typeface) {
        std::cerr << "无法加载字体: " << text.fontFamily << std::endl;
        typeface = fontMgr->legacyMakeTypeface("Arial", SkFontStyle::Normal());
    }
    
    // 保存画布状态
    canvas->save();
    
    // 应用变换
    canvas->translate(text.x, text.y);
    canvas->scale(text.scaleX, text.scaleY);
    canvas->rotate(text.rotation);
    canvas->translate(-text.x, -text.y);
    
    // 创建字体
    SkFont font(typeface, text.fontSize);
    
    // 渲染阴影（如果有）
    if (text.hasShadow) {
        SkPaint shadowPaint;
        shadowPaint.setColor(text.shadowColor);
        shadowPaint.setStyle(SkPaint::kFill_Style);
        
        // 应用阴影偏移
        canvas->drawString(text.content.c_str(), 
                          text.x + text.shadowDx, 
                          text.y + text.fontSize + text.shadowDy, 
                          font, shadowPaint);
    }
    
    // 渲染描边（如果有）
    if (text.strokeWidth > 0) {
        SkPaint strokePaint;
        strokePaint.setColor(text.strokeColor);
        strokePaint.setStyle(SkPaint::kStroke_Style);
        strokePaint.setStrokeWidth(text.strokeWidth);
        canvas->drawString(text.content.c_str(), text.x, text.y + text.fontSize, font, strokePaint);
    }
    
    // 渲染填充
    SkPaint fillPaint;
    fillPaint.setColor(text.fillColor);
    fillPaint.setStyle(SkPaint::kFill_Style);
    canvas->drawString(text.content.c_str(), text.x, text.y + text.fontSize, font, fillPaint);
    
    // 恢复画布状态
    canvas->restore();
}

sk_sp<SkTypeface> UniversalRenderer::loadFont(const std::string& fontFamily) {
    // 尝试加载字体文件
    if (fontFamily == "站酷快乐体") {
        return fontMgr->makeFromFile("res/字制区喜脉体.ttf", 0);
    }
    
    // 尝试系统字体
    sk_sp<SkTypeface> typeface = fontMgr->legacyMakeTypeface(fontFamily.c_str(), SkFontStyle::Normal());
    if (typeface) {
        return typeface;
    }
    
    // 回退到默认字体
    return fontMgr->legacyMakeTypeface("Arial", SkFontStyle::Normal());
}

bool UniversalRenderer::saveImage(sk_sp<SkImage> image, const std::string& filename) {
    SkPixmap pixmap;
    if (!image->peekPixels(&pixmap)) {
        errorMessage = "无法获取像素数据";
        return false;
    }
    
    SkFILEWStream out(filename.c_str());
    if (!out.isValid()) {
        errorMessage = "无法打开输出文件: " + filename;
        return false;
    }
    
    SkPngEncoder::Options options;
    if (!SkPngEncoder::Encode(&out, pixmap, options)) {
        errorMessage = "PNG编码失败";
        return false;
    }
    
    return true;
} 