#pragma once

#include "protocol_parser.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/core/SkImage.h"
#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"
#include "include/core/SkFontMgr.h"
#include "include/core/SkColor.h"
#include "include/core/SkRect.h"
#include "include/core/SkMatrix.h"
#include "include/core/SkPath.h"
#include "include/effects/SkGradientShader.h"
#include "include/effects/SkBlurMaskFilter.h"
#include "include/ports/SkFontMgr_mac_ct.h"
#include "include/encode/SkPngEncoder.h"
#include "include/core/SkStream.h"
#include "include/core/SkData.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

class UniversalRenderer {
public:
    UniversalRenderer();
    ~UniversalRenderer();
    
    // 从协议文件渲染
    bool renderFromProtocol(const std::string& protocolFile);
    
    // 从协议对象渲染
    bool renderFromProtocol(const RenderProtocol& protocol);
    
    // 获取错误信息
    const std::string& getErrorMessage() const { return errorMessage; }

private:
    sk_sp<SkFontMgr> fontMgr;
    std::string errorMessage;
    
    // 渲染方法
    bool renderCanvas(SkCanvas* canvas, const CanvasConfig& canvasConfig);
    void renderImages(SkCanvas* canvas, const std::vector<ImageElement>& images);
    void renderTexts(SkCanvas* canvas, const std::vector<TextElement>& texts);
    
    // 辅助方法
    void renderImageElement(SkCanvas* canvas, const ImageElement& img);
    void renderTextElement(SkCanvas* canvas, const TextElement& text, sk_sp<SkTypeface> typeface);
    sk_sp<SkTypeface> loadFont(const std::string& fontFamily);
    bool saveImage(sk_sp<SkImage> image, const std::string& filename);
}; 