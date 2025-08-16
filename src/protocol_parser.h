#pragma once

#include "include/core/SkColor.h"
#include "3rdparty/json/include/nlohmann/json.hpp"
#include <string>
#include <vector>
#include <memory>

using json = nlohmann::json;

// 画布配置结构
struct CanvasConfig {
    int width;
    int height;
    std::string background;
};

// 图片元素结构
struct ImageElement {
    std::string id;
    std::string path;
    float x, y;
    float width, height;
    float scaleX, scaleY;
    float rotation;
    float opacity;
};

// 文本元素结构
struct TextElement {
    std::string id;
    std::string content;
    float x, y;
    float fontSize;
    std::string fontFamily;
    SkColor fillColor;
    SkColor strokeColor;
    float strokeWidth;
    bool hasShadow;
    float shadowDx, shadowDy, shadowSigma;
    SkColor shadowColor;
    float scaleX, scaleY;
    float rotation;
};

// 输出配置结构
struct OutputConfig {
    std::string format;
    std::string filename;
    int quality;
};

// 完整协议结构
struct RenderProtocol {
    CanvasConfig canvas;
    std::vector<ImageElement> images;
    std::vector<TextElement> texts;
    OutputConfig output;
};

// 协议解析类
class ProtocolParser {
public:
    ProtocolParser();
    ~ProtocolParser();
    
    // 从JSON文件加载协议
    bool loadFromFile(const std::string& filename);
    
    // 从JSON字符串加载协议
    bool loadFromString(const std::string& jsonString);
    
    // 获取解析后的协议
    const RenderProtocol& getProtocol() const { return protocol; }
    
    // 检查协议是否有效
    bool isValid() const { return valid; }
    
    // 获取错误信息
    const std::string& getErrorMessage() const { return errorMessage; }

private:
    RenderProtocol protocol;
    bool valid;
    std::string errorMessage;
    
    // 解析各个部分
    bool parseProtocol(const json& json);
    bool parseCanvas(const json& json);
    bool parseImages(const json& json);
    bool parseTexts(const json& json);
    bool parseOutput(const json& json);
    
    // 辅助方法
    SkColor parseColor(const std::string& colorString);
    float parseFloat(const json& json, const std::string& key, float defaultValue = 0.0f);
    std::string parseString(const json& json, const std::string& key, const std::string& defaultValue = "");
    int parseInt(const json& json, const std::string& key, int defaultValue = 0);
    bool parseBool(const json& json, const std::string& key, bool defaultValue = false);
}; 