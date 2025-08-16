#include "engine/render_engine.h"
#include <iostream>

namespace skia_renderer {

RenderEngine::RenderEngine() {
    // 初始化组件
    protocolParser = std::make_unique<ProtocolParser>();
    canvasRenderer = std::make_unique<CanvasRenderer>();
    imageRenderer = std::make_unique<ImageRenderer>();
    textRenderer = std::make_unique<TextRenderer>();
    imageWriter = std::make_unique<ImageWriter>();
}

RenderEngine::~RenderEngine() {
}

bool RenderEngine::renderFromProtocol(const std::string& protocolFile) {
    if (!protocolParser->loadFromFile(protocolFile)) {
        errorMessage = "协议解析失败: " + protocolParser->getErrorMessage();
        return false;
    }
    
    return renderFromProtocol(protocolParser->getProtocol());
}

bool RenderEngine::renderFromProtocol(const RenderProtocol& protocol) {
    // 创建画布
    if (!canvasRenderer->createCanvas(protocol.canvas.width, protocol.canvas.height)) {
        errorMessage = "无法创建画布";
        return false;
    }
    
    // 渲染画布背景
    if (!renderCanvas(protocol.canvas)) {
        return false;
    }
    
    // 渲染图片元素
    if (!renderImages(protocol.images)) {
        return false;
    }
    
    // 渲染文本元素
    if (!renderTexts(protocol.texts)) {
        return false;
    }
    
    // 保存输出
    sk_sp<SkImage> image = canvasRenderer->makeImageSnapshot();
    if (!saveOutput(image, protocol.output)) {
        return false;
    }
    
    std::cout << "海报已保存为" << protocol.output.filename << std::endl;
    return true;
}

void RenderEngine::setFontManager(std::shared_ptr<FontManager> fontManager) {
    textRenderer->setFontManager(fontManager);
}

std::shared_ptr<FontManager> RenderEngine::getFontManager() const {
    return textRenderer->getFontManager();
}

bool RenderEngine::renderCanvas(const CanvasConfig& canvasConfig) {
    return canvasRenderer->setBackground(canvasConfig.background);
}

bool RenderEngine::renderImages(const std::vector<ImageElement>& images) {
    SkCanvas* canvas = canvasRenderer->getCanvas();
    if (!canvas) {
        errorMessage = "画布未初始化";
        return false;
    }
    
    for (const auto& img : images) {
        if (!imageRenderer->renderImage(canvas, img)) {
            errorMessage = "图片渲染失败: " + img.path;
            return false;
        }
    }
    
    return true;
}

bool RenderEngine::renderTexts(const std::vector<TextElement>& texts) {
    SkCanvas* canvas = canvasRenderer->getCanvas();
    if (!canvas) {
        errorMessage = "画布未初始化";
        return false;
    }
    
    for (const auto& text : texts) {
        if (!textRenderer->renderText(canvas, text)) {
            errorMessage = "文本渲染失败: " + text.content;
            return false;
        }
    }
    
    return true;
}

bool RenderEngine::saveOutput(sk_sp<SkImage> image, const OutputConfig& outputConfig) {
    if (!image) {
        errorMessage = "图像为空";
        return false;
    }
    
    return imageWriter->saveImage(image, outputConfig.filename, outputConfig.quality);
}

} // namespace skia_renderer 