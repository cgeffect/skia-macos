#pragma once

#include "include/core/SkColor.h"
#include <string>

namespace skia_renderer {

class ColorParser {
public:
    // 解析颜色字符串为SkColor
    static SkColor parseColor(const std::string& colorString);
    
    // 解析十六进制颜色
    static SkColor parseHexColor(const std::string& hex);
    
    // 解析RGB颜色
    static SkColor parseRgbColor(const std::string& rgb);
    
    // 解析RGBA颜色
    static SkColor parseRgbaColor(const std::string& rgba);
    
    // 检查是否为有效的颜色格式
    static bool isValidColor(const std::string& colorString);

private:
    // 辅助方法
    static int hexToInt(char c);
    static bool isHexChar(char c);
};

} // namespace skia_renderer 