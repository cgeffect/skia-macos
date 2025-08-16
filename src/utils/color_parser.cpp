#include "utils/color_parser.h"
#include <regex>
#include <cctype>

namespace skia_renderer {

SkColor ColorParser::parseColor(const std::string& colorString) {
    if (colorString.empty()) {
        return SK_ColorBLACK;
    }
    
    // 处理 #RRGGBB 格式
    if (colorString[0] == '#') {
        return parseHexColor(colorString);
    }
    
    // 处理 rgb(r,g,b) 格式
    if (colorString.find("rgb(") == 0) {
        return parseRgbColor(colorString);
    }
    
    // 处理 rgba(r,g,b,a) 格式
    if (colorString.find("rgba(") == 0) {
        return parseRgbaColor(colorString);
    }
    
    // 处理预定义颜色名称
    if (colorString == "white" || colorString == "WHITE") {
        return SK_ColorWHITE;
    }
    if (colorString == "black" || colorString == "BLACK") {
        return SK_ColorBLACK;
    }
    if (colorString == "red" || colorString == "RED") {
        return SK_ColorRED;
    }
    if (colorString == "green" || colorString == "GREEN") {
        return SK_ColorGREEN;
    }
    if (colorString == "blue" || colorString == "BLUE") {
        return SK_ColorBLUE;
    }
    if (colorString == "transparent" || colorString == "TRANSPARENT") {
        return SK_ColorTRANSPARENT;
    }
    
    // 默认返回黑色
    return SK_ColorBLACK;
}

SkColor ColorParser::parseHexColor(const std::string& hex) {
    std::string cleanHex = hex.substr(1); // 移除 #
    
    if (cleanHex.length() == 6) {
        unsigned int r, g, b;
        sscanf(cleanHex.c_str(), "%02x%02x%02x", &r, &g, &b);
        return SkColorSetRGB(r, g, b);
    } else if (cleanHex.length() == 8) {
        unsigned int r, g, b, a;
        sscanf(cleanHex.c_str(), "%02x%02x%02x%02x", &r, &g, &b, &a);
        return SkColorSetARGB(a, r, g, b);
    }
    
    return SK_ColorBLACK;
}

SkColor ColorParser::parseRgbColor(const std::string& rgb) {
    std::regex rgbPattern(R"(rgb\((\d+),\s*(\d+),\s*(\d+)\))");
    std::smatch match;
    
    if (std::regex_match(rgb, match, rgbPattern)) {
        int r = std::stoi(match[1]);
        int g = std::stoi(match[2]);
        int b = std::stoi(match[3]);
        return SkColorSetRGB(r, g, b);
    }
    
    return SK_ColorBLACK;
}

SkColor ColorParser::parseRgbaColor(const std::string& rgba) {
    std::regex rgbaPattern(R"(rgba\((\d+),\s*(\d+),\s*(\d+),\s*([\d.]+)\))");
    std::smatch match;
    
    if (std::regex_match(rgba, match, rgbaPattern)) {
        int r = std::stoi(match[1]);
        int g = std::stoi(match[2]);
        int b = std::stoi(match[3]);
        float a = std::stof(match[4]);
        return SkColorSetARGB(static_cast<U8CPU>(a * 255), r, g, b);
    }
    
    return SK_ColorBLACK;
}

bool ColorParser::isValidColor(const std::string& colorString) {
    if (colorString.empty()) {
        return false;
    }
    
    // 检查十六进制格式
    if (colorString[0] == '#') {
        std::string hex = colorString.substr(1);
        if (hex.length() == 6 || hex.length() == 8) {
            return std::all_of(hex.begin(), hex.end(), isHexChar);
        }
        return false;
    }
    
    // 检查RGB格式
    if (colorString.find("rgb(") == 0) {
        std::regex rgbPattern(R"(rgb\(\d+,\s*\d+,\s*\d+\))");
        return std::regex_match(colorString, rgbPattern);
    }
    
    // 检查RGBA格式
    if (colorString.find("rgba(") == 0) {
        std::regex rgbaPattern(R"(rgba\(\d+,\s*\d+,\s*\d+,\s*[\d.]+\))");
        return std::regex_match(colorString, rgbaPattern);
    }
    
    // 检查预定义颜色名称
    std::vector<std::string> predefinedColors = {
        "white", "WHITE", "black", "BLACK", "red", "RED",
        "green", "GREEN", "blue", "BLUE", "transparent", "TRANSPARENT"
    };
    
    return std::find(predefinedColors.begin(), predefinedColors.end(), colorString) != predefinedColors.end();
}

int ColorParser::hexToInt(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
}

bool ColorParser::isHexChar(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

} // namespace skia_renderer 