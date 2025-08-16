#include "protocol_parser.h"
#include "3rdparty/json/include/nlohmann/json.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

using json = nlohmann::json;

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
        img.x = parseFloat(imgJson, "x", 0.0f);
        img.y = parseFloat(imgJson, "y", 0.0f);
        img.width = parseFloat(imgJson, "width", 0.0f);
        img.height = parseFloat(imgJson, "height", 0.0f);
        img.scaleX = parseFloat(imgJson, "scaleX", 1.0f);
        img.scaleY = parseFloat(imgJson, "scaleY", 1.0f);
        img.rotation = parseFloat(imgJson, "rotation", 0.0f);
        img.opacity = parseFloat(imgJson, "opacity", 1.0f);
        
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
        text.x = parseFloat(textJson, "x", 0.0f);
        text.y = parseFloat(textJson, "y", 0.0f);
        text.fontSize = parseFloat(textJson, "fontSize", 12.0f);
        text.fontFamily = parseString(textJson, "fontFamily", "Arial");
        text.fillColor = parseColor(parseString(textJson, "fillColor", "#000000"));
        text.strokeColor = parseColor(parseString(textJson, "strokeColor", "#000000"));
        text.strokeWidth = parseFloat(textJson, "strokeWidth", 0.0f);
        text.hasShadow = parseBool(textJson, "hasShadow", false);
        text.shadowDx = parseFloat(textJson, "shadowDx", 0.0f);
        text.shadowDy = parseFloat(textJson, "shadowDy", 0.0f);
        text.shadowSigma = parseFloat(textJson, "shadowSigma", 0.0f);
        text.shadowColor = parseColor(parseString(textJson, "shadowColor", "#000000"));
        text.scaleX = parseFloat(textJson, "scaleX", 1.0f);
        text.scaleY = parseFloat(textJson, "scaleY", 1.0f);
        text.rotation = parseFloat(textJson, "rotation", 0.0f);
        
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

SkColor ProtocolParser::parseColor(const std::string& colorString) {
    if (colorString.empty()) {
        return SK_ColorBLACK;
    }
    
    // 处理 #RRGGBB 格式
    if (colorString[0] == '#') {
        std::string hex = colorString.substr(1);
        if (hex.length() == 6) {
            unsigned int r, g, b;
            sscanf(hex.c_str(), "%02x%02x%02x", &r, &g, &b);
            return SkColorSetRGB(r, g, b);
        }
    }
    
    // 处理 rgb(r,g,b) 格式
    std::regex rgbPattern(R"(rgb\((\d+),\s*(\d+),\s*(\d+)\))");
    std::smatch match;
    if (std::regex_match(colorString, match, rgbPattern)) {
        int r = std::stoi(match[1]);
        int g = std::stoi(match[2]);
        int b = std::stoi(match[3]);
        return SkColorSetRGB(r, g, b);
    }
    
    // 处理 rgba(r,g,b,a) 格式
    std::regex rgbaPattern(R"(rgba\((\d+),\s*(\d+),\s*(\d+),\s*([\d.]+)\))");
    if (std::regex_match(colorString, match, rgbaPattern)) {
        int r = std::stoi(match[1]);
        int g = std::stoi(match[2]);
        int b = std::stoi(match[3]);
        float a = std::stof(match[4]);
        return SkColorSetARGB(static_cast<U8CPU>(a * 255), r, g, b);
    }
    
    // 默认返回黑色
    return SK_ColorBLACK;
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