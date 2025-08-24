#include "resources/font_manager.h"
#include <iostream>

namespace skia_renderer {

FontManager::FontManager() {
    // 初始化字体管理器
    fontMgr = SkFontMgr_New_CoreText(nullptr);
    
    // 注册常用字体文件
    registerFontFile("站酷快乐体", "res/Fonts/站酷快乐体2016修订版.ttf");
    
    // 注册 Source Han Sans CN 字体家族的所有Weight变体
    registerFontFile("SourceHanSansCN-ExtraLight", "res/Fonts/SourceHanSansCN-ExtraLight.otf");
    registerFontFile("SourceHanSansCN-Light", "res/Fonts/SourceHanSansCN-Light.otf");
    registerFontFile("SourceHanSansCN-Normal", "res/Fonts/SourceHanSansCN-Normal.otf");
    registerFontFile("SourceHanSansCN-Regular", "res/Fonts/SourceHanSansCN-Regular.otf");
    registerFontFile("SourceHanSansCN-Medium", "res/Fonts/SourceHanSansCN-Medium.otf");
    registerFontFile("SourceHanSansCN-Bold", "res/Fonts/SourceHanSansCN-Bold.otf");
    registerFontFile("SourceHanSansCN-Heavy", "res/Fonts/SourceHanSansCN-Heavy.otf");
}

FontManager::~FontManager() {
}

sk_sp<SkTypeface> FontManager::loadFont(const std::string& fontFamily) {
    // 首先检查是否有注册的字体文件
    auto it = fontFileMap.find(fontFamily);
    if (it != fontFileMap.end()) {
        sk_sp<SkTypeface> typeface = loadFileFont(it->second);
        if (typeface) {
            return typeface;
        }
    }
    
    // 尝试加载系统字体
    sk_sp<SkTypeface> typeface = loadSystemFont(fontFamily);
    if (typeface) {
        return typeface;
    }
    
    // 回退到默认字体
    return getDefaultFont();
}

bool FontManager::registerFontFile(const std::string& fontName, const std::string& filePath) {
    fontFileMap[fontName] = filePath;
    return true;
}

sk_sp<SkTypeface> FontManager::getDefaultFont() {
    return fontMgr->legacyMakeTypeface("Arial", SkFontStyle::Normal());
}

bool FontManager::isFontAvailable(const std::string& fontFamily) {
    sk_sp<SkTypeface> typeface = loadFont(fontFamily);
    return typeface != nullptr;
}

sk_sp<SkTypeface> FontManager::loadSystemFont(const std::string& fontFamily) {
    return fontMgr->legacyMakeTypeface(fontFamily.c_str(), SkFontStyle::Normal());
}

sk_sp<SkTypeface> FontManager::loadFileFont(const std::string& filePath) {
    return fontMgr->makeFromFile(filePath.c_str(), 0);
}

} // namespace skia_renderer 
