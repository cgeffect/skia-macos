#pragma once

#include "include/core/SkColor.h"
#include <string>
#include <vector>

namespace skia_renderer {

// 基础变换结构
struct Transform {
    float x = 0.0f;
    float y = 0.0f;
    float scaleX = 1.0f;
    float scaleY = 1.0f;
    float rotation = 0.0f;
    float opacity = 1.0f;
};

// 画布配置
struct CanvasConfig {
    int width = 1242;
    int height = 1660;
    std::string background = "#FFFFFF";
};

// 图片元素
struct ImageElement {
    std::string id;
    std::string path;
    Transform transform;
    int width = 0;
    int height = 0;
};

// 文本样式
struct TextStyle {
    std::string fontFamily = "Arial";
    float fontSize = 12.0f;
    SkColor fillColor = SK_ColorBLACK;
    SkColor strokeColor = SK_ColorBLACK;
    float strokeWidth = 0.0f;
    bool hasShadow = false;
    float shadowDx = 0.0f;
    float shadowDy = 0.0f;
    float shadowSigma = 0.0f;
    SkColor shadowColor = SK_ColorBLACK;
};

// 文本元素
struct TextElement {
    std::string id;
    std::string content;
    Transform transform;
    TextStyle style;
    float width = 0.0f;  // 文本区域宽度
    float height = 0.0f; // 文本区域高度
};

// 输出配置
struct OutputConfig {
    std::string format = "png";
    std::string filename = "output.png";
    int quality = 100;
};

// 完整渲染协议
struct RenderProtocol {
    CanvasConfig canvas;
    std::vector<ImageElement> images;
    std::vector<TextElement> texts;
    OutputConfig output;
};

} // namespace skia_renderer 