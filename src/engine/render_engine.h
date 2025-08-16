#pragma once

#include "core/types.h"
#include "parsers/protocol_parser.h"
#include "renderers/canvas_renderer.h"
#include "renderers/image_renderer.h"
#include "renderers/text_renderer.h"
#include "output/image_writer.h"
#include <memory>
#include <string>

namespace skia_renderer {

class RenderEngine {
public:
    RenderEngine();
    ~RenderEngine();
    
    // 从协议文件渲染
    bool renderFromProtocol(const std::string& protocolFile);
    
    // 从协议对象渲染
    bool renderFromProtocol(const RenderProtocol& protocol);
    
    // 获取错误信息
    const std::string& getErrorMessage() const { return errorMessage; }
    
    // 设置字体管理器
    void setFontManager(std::shared_ptr<FontManager> fontManager);
    
    // 获取字体管理器
    std::shared_ptr<FontManager> getFontManager() const;

private:
    std::string errorMessage;
    
    // 组件
    std::unique_ptr<ProtocolParser> protocolParser;
    std::unique_ptr<CanvasRenderer> canvasRenderer;
    std::unique_ptr<ImageRenderer> imageRenderer;
    std::unique_ptr<TextRenderer> textRenderer;
    std::unique_ptr<ImageWriter> imageWriter;
    
    // 渲染方法
    bool renderCanvas(const CanvasConfig& canvasConfig);
    bool renderImages(const std::vector<ImageElement>& images);
    bool renderTexts(const std::vector<TextElement>& texts);
    bool saveOutput(sk_sp<SkImage> image, const OutputConfig& outputConfig);
};

} // namespace skia_renderer 