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

// 简化的协议结构
struct TextElement {
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
};

struct ImageElement {
    std::string path;
    float x, y;
    float scaleX, scaleY;
    float rotation;
    float opacity;
    int width, height;
};

class UniversalRenderer {
public:
    UniversalRenderer() {
        // 初始化字体管理器
        fontMgr = SkFontMgr_New_CoreText(nullptr);
    }
    
    // 渲染青海湖旅游攻略
    bool renderTrip() {
        // 创建画布
        SkImageInfo info = SkImageInfo::Make(
            1242, 1660,
            SkColorType::kRGBA_8888_SkColorType,
            SkAlphaType::kOpaque_SkAlphaType);
        sk_sp<SkSurface> surface(SkSurfaces::Raster(info));
        if (!surface) {
            std::cerr << "无法创建Surface" << std::endl;
            return false;
        }
        SkCanvas* canvas = surface->getCanvas();
        
        // 设置白色背景
        canvas->clear(SK_ColorWHITE);
        
        // 渲染图片元素
        renderImageElements(canvas);
        
        // 渲染文本元素
        renderTextElements(canvas);
        
        // 保存图像
        sk_sp<SkImage> image = surface->makeImageSnapshot();
        SkPixmap pixmap;
        if (!image->peekPixels(&pixmap)) {
            std::cerr << "无法获取像素数据" << std::endl;
            return false;
        }
        
        SkFILEWStream out("trip_rendered.png");
        if (!out.isValid()) {
            std::cerr << "无法打开输出文件" << std::endl;
            return false;
        }
        
        SkPngEncoder::Options options;
        if (!SkPngEncoder::Encode(&out, pixmap, options)) {
            std::cerr << "PNG编码失败" << std::endl;
            return false;
        }
        
        std::cout << "青海湖旅游攻略图已保存为trip_rendered.png" << std::endl;
        return true;
    }
    
    // 渲染防晒海报
    bool renderSunscreen() {
        // 创建画布
        SkImageInfo info = SkImageInfo::Make(
            1242, 1660,
            SkColorType::kRGBA_8888_SkColorType,
            SkAlphaType::kOpaque_SkAlphaType);
        sk_sp<SkSurface> surface(SkSurfaces::Raster(info));
        if (!surface) {
            std::cerr << "无法创建Surface" << std::endl;
            return false;
        }
        SkCanvas* canvas = surface->getCanvas();
        
        // 设置白色背景
        canvas->clear(SK_ColorWHITE);
        
        // 渲染图片元素
        renderSunscreenImageElements(canvas);
        
        // 渲染文本元素
        renderSunscreenTextElements(canvas);
        
        // 保存图像
        sk_sp<SkImage> image = surface->makeImageSnapshot();
        SkPixmap pixmap;
        if (!image->peekPixels(&pixmap)) {
            std::cerr << "无法获取像素数据" << std::endl;
            return false;
        }
        
        SkFILEWStream out("sunscreen_poster.png");
        if (!out.isValid()) {
            std::cerr << "无法打开输出文件" << std::endl;
            return false;
        }
        
        SkPngEncoder::Options options;
        if (!SkPngEncoder::Encode(&out, pixmap, options)) {
            std::cerr << "PNG编码失败" << std::endl;
            return false;
        }
        
        std::cout << "防晒海报已保存为sunscreen_poster.png" << std::endl;
        return true;
    }
    
    // 渲染美食海报
    bool renderFood() {
        // 创建画布
        SkImageInfo info = SkImageInfo::Make(
            1242, 1660,
            SkColorType::kRGBA_8888_SkColorType,
            SkAlphaType::kOpaque_SkAlphaType);
        sk_sp<SkSurface> surface(SkSurfaces::Raster(info));
        if (!surface) {
            std::cerr << "无法创建Surface" << std::endl;
            return false;
        }
        SkCanvas* canvas = surface->getCanvas();
        
        // 设置白色背景
        canvas->clear(SK_ColorWHITE);
        
        // 渲染图片元素
        renderFoodImageElements(canvas);
        
        // 保存图像
        sk_sp<SkImage> image = surface->makeImageSnapshot();
        SkPixmap pixmap;
        if (!image->peekPixels(&pixmap)) {
            std::cerr << "无法获取像素数据" << std::endl;
            return false;
        }
        
        SkFILEWStream out("food_poster.png");
        if (!out.isValid()) {
            std::cerr << "无法打开输出文件" << std::endl;
            return false;
        }
        
        SkPngEncoder::Options options;
        if (!SkPngEncoder::Encode(&out, pixmap, options)) {
            std::cerr << "PNG编码失败" << std::endl;
            return false;
        }
        
        std::cout << "美食海报已保存为food_poster.png" << std::endl;
        return true;
    }

private:
    sk_sp<SkFontMgr> fontMgr;
    
    void renderImageElements(SkCanvas* canvas) {
        // 定义图片元素
        std::vector<ImageElement> images = {
            // 背景图片
            {"projects/trip/resources/image_0.png", 0, 0, 1.0, 1.0, 0.0, 1.0, 1242, 1660},
            {"projects/trip/resources/image_1.png", 0, 0, 1.0, 1.0, 0.0, 1.0, 1242, 1660},
            
            // 装饰元素
            {"projects/trip/resources/image_2.png", 1175, 1033, 1.0, 1.0, 0.0, 1.0, 22, 224},
            {"projects/trip/resources/image_3.png", 45, 452, 1.0, 1.0, 0.0, 1.0, 22, 224},
            
            // 主图片
            {"projects/trip/resources/image_4.png", 119, 538, 1.0, 1.0, 0.0, 1.0, 1005, 749},
            
            // 文字背景
            {"projects/trip/resources/image_5.png", 124, 329, 1.0, 1.0, 0.0, 1.0, 906, 165},
            {"projects/trip/resources/image_6.png", 255, 1329, 1.0, 1.0, 0.0, 1.0, 906, 165},
            {"projects/trip/resources/image_7.png", 103, 1534, 1.0, 1.0, 0.0, 1.0, 492, 47}
        };
        
        // 渲染每个图片元素
        for (const auto& img : images) {
            renderImageElement(canvas, img);
        }
    }
    
    void renderImageElement(SkCanvas* canvas, const ImageElement& img) {
        // 加载图片
        sk_sp<SkData> data = SkData::MakeFromFileName(img.path.c_str());
        if (!data) {
            std::cerr << "无法加载图片: " << img.path << std::endl;
            return;
        }
        
        sk_sp<SkImage> image = SkImages::DeferredFromEncodedData(data);
        if (!image) {
            std::cerr << "无法解码图片: " << img.path << std::endl;
            return;
        }
        
        // 创建绘制矩形
        SkRect dstRect = SkRect::MakeXYWH(img.x, img.y, img.width, img.height);
        
        // 设置绘制参数
        SkPaint paint;
        paint.setAlpha(static_cast<U8CPU>(img.opacity * 255));
        
        // 绘制图片
        canvas->drawImageRect(image, dstRect, SkSamplingOptions(), &paint);
    }
    
    void renderTextElements(SkCanvas* canvas) {
        // 加载字体（使用系统字体作为替代）
        sk_sp<SkTypeface> typeface = fontMgr->makeFromFile("res/字制区喜脉体.ttf", 0);
        if (!typeface) {
            std::cerr << "无法加载字体文件，使用系统字体" << std::endl;
            typeface = fontMgr->legacyMakeTypeface("Arial", SkFontStyle::Normal());
        }
        
        // 定义文本元素
        std::vector<TextElement> texts = {
            {
                "青海湖旅游攻略", 111.7632, 96.096, 80, "站酷快乐体",
                SkColorSetRGB(31, 135, 232), SK_ColorWHITE, 10.0f,
                false, 0, 0, 0, SK_ColorBLACK,
                1.7652, 1.7652
            },
            {
                "4天人均花费不到2000", 199, 387, 75, "站酷快乐体",
                SkColorSetRGB(89, 67, 204), SK_ColorTRANSPARENT, 0.0f,
                false, 0, 0, 0, SK_ColorBLACK,
                1.0, 1.0
            },
            {
                "规划路线 住宿安排、美食分享", 308, 1396, 57, "站酷快乐体",
                SkColorSetRGB(89, 67, 204), SK_ColorTRANSPARENT, 0.0f,
                false, 0, 0, 0, SK_ColorBLACK,
                1.0, 1.0
            }
        };
        
        // 渲染每个文本元素
        for (const auto& text : texts) {
            renderTextElement(canvas, text, typeface);
        }
    }
    
    void renderTextElement(SkCanvas* canvas, const TextElement& text, sk_sp<SkTypeface> typeface) {
        // 保存画布状态
        canvas->save();
        
        // 应用缩放变换
        canvas->translate(text.x, text.y);
        canvas->scale(text.scaleX, text.scaleY);
        canvas->translate(-text.x, -text.y);
        
        // 创建字体
        SkFont font(typeface, text.fontSize);
        
        // 渲染描边（如果有）
        if (text.strokeWidth > 0) {
            SkPaint strokePaint;
            strokePaint.setColor(text.strokeColor);
            strokePaint.setStyle(SkPaint::kStroke_Style);
            strokePaint.setStrokeWidth(text.strokeWidth);
            canvas->drawString(text.content.c_str(), text.x, text.y + text.fontSize, font, strokePaint);
        }
        
        // 渲染填充
        SkPaint fillPaint;
        fillPaint.setColor(text.fillColor);
        fillPaint.setStyle(SkPaint::kFill_Style);
        canvas->drawString(text.content.c_str(), text.x, text.y + text.fontSize, font, fillPaint);
        
        // 恢复画布状态
        canvas->restore();
    }
    
    // 防晒海报相关方法
    void renderSunscreenImageElements(SkCanvas* canvas) {
        // 定义图片元素
        std::vector<ImageElement> images = {
            {"projects/sunscreen/resources/sunscreen_bg_1.png", 0, 0, 1.0, 1.0, 0.0, 1.0, 1242, 1660},
            {"projects/sunscreen/resources/sunscreen_bg_2.png", 0, 0, 1.0, 1.0, 0.0, 1.0, 1242, 1660},
            {"projects/sunscreen/resources/sunscreen_product_1.png", 69, 754, 1.0, 1.0, 0.0, 1.0, 254, 280},
            {"projects/sunscreen/resources/sunscreen_product_2.png", 916, 443, 1.0, 1.0, 0.0, 1.0, 318, 319},
            {"projects/sunscreen/resources/sunscreen_product_3.png", 664, 1326, 1.0, 1.0, 0.0, 1.0, 521, 259},
            {"projects/sunscreen/resources/sunscreen_decor.png", 252, 352, 1.0, 1.0, 0.0, 1.0, 737, 91}
        };
        
        // 渲染每个图片元素
        for (const auto& img : images) {
            renderImageElement(canvas, img);
        }
    }
    
    void renderSunscreenTextElements(SkCanvas* canvas) {
        // 加载字体
        sk_sp<SkTypeface> typeface = fontMgr->makeFromFile("res/字制区喜脉体.ttf", 0);
        if (!typeface) {
            std::cerr << "无法加载字体文件，使用系统字体" << std::endl;
            typeface = fontMgr->legacyMakeTypeface("Arial", SkFontStyle::Normal());
        }
        
        // 定义文本元素
        std::vector<TextElement> texts = {
            {
                "夏日防晒攻略", 135.7453, 109.081, 160, "站酷快乐体",
                SkColorSetRGB(31, 135, 232), SK_ColorWHITE, 6.0f,
                true, 4, 3, 0, SkColorSetRGB(4, 74, 149),
                1.0, 1.0
            },
            {
                "好物推荐 值得收藏", 322, 365, 67, "站酷快乐体",
                SkColorSetRGB(199, 77, 76), SkColorSetRGB(199, 77, 76), 0.0f,
                false, 0, 0, 0, SK_ColorBLACK,
                1.0, 1.0
            }
        };
        
        // 渲染每个文本元素
        for (const auto& text : texts) {
            renderTextElement(canvas, text, typeface);
        }
    }
    
    // 美食海报相关方法
    void renderFoodImageElements(SkCanvas* canvas) {
        // 定义图片元素
        std::vector<ImageElement> images = {
            {"projects/food/resources/food_bg_1.png", -29, -228, 1.0, 1.0, 0.0, 1.0, 1285, 1927},
            {"projects/food/resources/food_bg_2.png", 24, 37, 1.0, 1.0, 0.0, 1.0, 1194, 1586},
            {"projects/food/resources/food_main.png", 95, 105, 1.0, 1.0, 0.0, 1.0, 1050, 750},
            {"projects/food/resources/food_decor_1.png", 921, 589, 1.0, 1.0, 0.0, 1.0, 226, 223},
            {"projects/food/resources/food_decor_2.png", 64, 1079, 1.0, 1.0, 0.0, 1.0, 396, 175},
            {"projects/food/resources/food_text_bg_1.png", 83, 125, 1.0, 1.0, 0.0, 1.0, 1081, 351},
            {"projects/food/resources/food_text_bg_2.png", 84, 1343, 1.0, 1.0, 0.0, 1.0, 1097, 147},
            {"projects/food/resources/food_text_bg_3.png", 143, 1362, 1.0, 1.0, 0.0, 1.0, 978, 111}
        };
        
        // 渲染每个图片元素
        for (const auto& img : images) {
            renderImageElement(canvas, img);
        }
    }
};

int main() {
    // 使用通用渲染器
    UniversalRenderer renderer;
    
    // 渲染青海湖旅游攻略
    std::cout << "正在渲染青海湖旅游攻略..." << std::endl;
    if (renderer.renderTrip()) {
        std::cout << "青海湖旅游攻略图渲染成功！" << std::endl;
    } else {
        std::cerr << "青海湖旅游攻略图渲染失败！" << std::endl;
        return 1;
    }
    
    // 渲染防晒海报
    std::cout << "正在渲染防晒海报..." << std::endl;
    if (renderer.renderSunscreen()) {
        std::cout << "防晒海报渲染成功！" << std::endl;
    } else {
        std::cerr << "防晒海报渲染失败！" << std::endl;
        return 1;
    }
    
    // 渲染美食海报
    std::cout << "正在渲染美食海报..." << std::endl;
    if (renderer.renderFood()) {
        std::cout << "美食海报渲染成功！" << std::endl;
    } else {
        std::cerr << "美食海报渲染失败！" << std::endl;
        return 1;
    }
    
    return 0;
}
