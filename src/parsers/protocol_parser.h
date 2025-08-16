#pragma once

#include "core/types.h"
#include "3rdparty/json/include/nlohmann/json.hpp"
#include <string>

using json = nlohmann::json;

namespace skia_renderer {

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
    bool parseProtocol(const json& j);
    bool parseCanvas(const json& j);
    bool parseImages(const json& j);
    bool parseTexts(const json& j);
    bool parseOutput(const json& j);
    bool parseTransform(const json& j, Transform& transform);
    bool parseTextStyle(const json& j, TextStyle& style);
    
    // 辅助方法
    float parseFloat(const json& j, const std::string& key, float defaultValue = 0.0f);
    std::string parseString(const json& j, const std::string& key, const std::string& defaultValue = "");
    int parseInt(const json& j, const std::string& key, int defaultValue = 0);
    bool parseBool(const json& j, const std::string& key, bool defaultValue = false);
};

} // namespace skia_renderer 