#include "parsers/protocol_parser.h"
#include "utils/color_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace skia_renderer {

ProtocolParser::ProtocolParser() : valid(false) {
}

ProtocolParser::~ProtocolParser() {
}

bool ProtocolParser::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        errorMessage = "无法打开文件: " + filename;
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return loadFromString(buffer.str());
}

bool ProtocolParser::loadFromString(const std::string& jsonString) {
    try {
        json j = json::parse(jsonString);
        return parseProtocol(j);
    } catch (const json::parse_error& e) {
        errorMessage = "JSON解析错误: " + std::string(e.what());
        return false;
    } catch (const std::exception& e) {
        errorMessage = "解析错误: " + std::string(e.what());
        return false;
    }
}

bool ProtocolParser::parseProtocol(const json& j) {
    valid = false;
    
    // 解析画布配置
    if (j.contains("canvas")) {
        if (!parseCanvas(j["canvas"])) {
            return false;
        }
    } else {
        errorMessage = "缺少canvas配置";
        return false;
    }
    
    // 解析图片元素
    if (j.contains("images")) {
        if (!parseImages(j["images"])) {
            return false;
        }
    }
    
    // 解析文本元素
    if (j.contains("texts")) {
        if (!parseTexts(j["texts"])) {
            return false;
        }
    }
    
    // 解析输出配置
    if (j.contains("output")) {
        if (!parseOutput(j["output"])) {
            return false;
        }
    }
    
    valid = true;
    return true;
}

bool ProtocolParser::parseCanvas(const json& j) {
    protocol.canvas.width = parseInt(j, "width", 1242);
    protocol.canvas.height = parseInt(j, "height", 1660);
    protocol.canvas.background = parseString(j, "background", "#FFFFFF");
    return true;
}

bool ProtocolParser::parseImages(const json& j) {
    if (!j.is_array()) {
        errorMessage = "images必须是数组";
        return false;
    }
    
    protocol.images.clear();
    for (const auto& imgJson : j) {
        ImageElement img;
        img.id = parseString(imgJson, "id", "");
        img.path = parseString(imgJson, "path", "");
        img.width = parseInt(imgJson, "width", 0);
        img.height = parseInt(imgJson, "height", 0);
        
        if (!parseTransform(imgJson, img.transform)) {
            return false;
        }
        
        protocol.images.push_back(img);
    }
    return true;
}

bool ProtocolParser::parseTexts(const json& j) {
    if (!j.is_array()) {
        errorMessage = "texts必须是数组";
        return false;
    }
    
    protocol.texts.clear();
    for (const auto& textJson : j) {
        TextElement text;
        text.id = parseString(textJson, "id", "");
        text.content = parseString(textJson, "content", "");
        text.width = parseFloat(textJson, "width", 0.0f);
        text.height = parseFloat(textJson, "height", 0.0f);
        
        if (!parseTransform(textJson, text.transform)) {
            return false;
        }
        
        if (!parseTextStyle(textJson, text.style)) {
            return false;
        }
        
        protocol.texts.push_back(text);
    }
    return true;
}

bool ProtocolParser::parseOutput(const json& j) {
    protocol.output.format = parseString(j, "format", "png");
    protocol.output.filename = parseString(j, "filename", "output.png");
    protocol.output.quality = parseInt(j, "quality", 100);
    return true;
}

bool ProtocolParser::parseTransform(const json& j, Transform& transform) {
    transform.x = parseFloat(j, "x", 0.0f);
    transform.y = parseFloat(j, "y", 0.0f);
    transform.scaleX = parseFloat(j, "scaleX", 1.0f);
    transform.scaleY = parseFloat(j, "scaleY", 1.0f);
    transform.rotation = parseFloat(j, "rotation", 0.0f);
    transform.opacity = parseFloat(j, "opacity", 1.0f);
    return true;
}

bool ProtocolParser::parseTextStyle(const json& j, TextStyle& style) {
    style.fontFamily = parseString(j, "fontFamily", "Arial");
    style.fontSize = parseFloat(j, "fontSize", 12.0f);
    style.fillColor = ColorParser::parseColor(parseString(j, "fillColor", "#000000"));
    style.strokeColor = ColorParser::parseColor(parseString(j, "strokeColor", "#000000"));
    style.strokeWidth = parseFloat(j, "strokeWidth", 0.0f);
    style.hasShadow = parseBool(j, "hasShadow", false);
    style.shadowDx = parseFloat(j, "shadowDx", 0.0f);
    style.shadowDy = parseFloat(j, "shadowDy", 0.0f);
    style.shadowSigma = parseFloat(j, "shadowSigma", 0.0f);
    style.shadowColor = ColorParser::parseColor(parseString(j, "shadowColor", "#000000"));
    return true;
}

float ProtocolParser::parseFloat(const json& j, const std::string& key, float defaultValue) {
    if (j.contains(key) && j[key].is_number()) {
        return j[key].get<float>();
    }
    return defaultValue;
}

std::string ProtocolParser::parseString(const json& j, const std::string& key, const std::string& defaultValue) {
    if (j.contains(key) && j[key].is_string()) {
        return j[key].get<std::string>();
    }
    return defaultValue;
}

int ProtocolParser::parseInt(const json& j, const std::string& key, int defaultValue) {
    if (j.contains(key) && j[key].is_number()) {
        return j[key].get<int>();
    }
    return defaultValue;
}

bool ProtocolParser::parseBool(const json& j, const std::string& key, bool defaultValue) {
    if (j.contains(key) && j[key].is_boolean()) {
        return j[key].get<bool>();
    }
    return defaultValue;
}

} // namespace skia_renderer 