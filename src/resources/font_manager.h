#pragma once

#include "include/core/SkFontMgr.h"
#include "include/core/SkTypeface.h"
#include "include/ports/SkFontMgr_mac_ct.h"
#include <string>
#include <map>
#include <memory>

namespace skia_renderer {

class FontManager {
public:
    FontManager();
    ~FontManager();
    
    // 加载字体
    sk_sp<SkTypeface> loadFont(const std::string& fontFamily);
    
    // 注册字体文件
    bool registerFontFile(const std::string& fontName, const std::string& filePath);
    
    // 获取默认字体
    sk_sp<SkTypeface> getDefaultFont();
    
    // 检查字体是否可用
    bool isFontAvailable(const std::string& fontFamily);

private:
    sk_sp<SkFontMgr> fontMgr;
    std::map<std::string, std::string> fontFileMap;
    
    // 辅助方法
    sk_sp<SkTypeface> loadSystemFont(const std::string& fontFamily);
    sk_sp<SkTypeface> loadFileFont(const std::string& filePath);
};

} // namespace skia_renderer 