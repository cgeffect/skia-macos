#include "include/core/SkCanvas.h"
#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkPath.h"
#include "include/core/SkStream.h"
#include "include/core/SkSurface.h"
#include "include/core/SkRRect.h"
#include "include/core/SkTextBlob.h"
#include "include/encode/SkPngEncoder.h"
#include "include/core/SkColor.h"
#include "include/core/SkFontMgr.h"
#include "include/core/SkPaint.h"
#include "include/ports/SkFontMgr_mac_ct.h"

#include <iostream>

void drawText(SkCanvas *canvas) {
    // 正确的字体加载代码
    sk_sp<SkFontMgr> fontMgr = SkFontMgr_New_CoreText(nullptr);
    // sk_sp<SkFontMgr> fontMgr = SkFontMgr::RefEmpty(); // 注意：这只是空实现，实际使用需获取系统字体管理器
    if (!fontMgr) {
        fprintf(stderr, "无法创建字体管理器\n");
        return;
    }

    // 使用正确的方法名 makeFromFile
    sk_sp<SkTypeface> typeface = fontMgr->makeFromFile("../res/字制区喜脉体.ttf", 0);
    if (!typeface) {
        fprintf(stderr, "无法加载字体文件: ../res/字制区喜脉体.ttf\n");
        return;
    }

    SkFont font(typeface, 56); // 设置字体和字号
    SkPaint paint;
    paint.setColor(SK_ColorBLUE);                             // 设置颜色
    canvas->drawString("SKia", 20, 120, font, paint); // 绘制中文
}

int main() {
    SkImageInfo info = SkImageInfo::Make(
        500, 500,
        SkColorType::kRGBA_8888_SkColorType,
        SkAlphaType::kOpaque_SkAlphaType);
    sk_sp<SkSurface> surface(SkSurfaces::Raster(info));
    if (!surface) {
        std::cerr << "无法创建Surface" << std::endl;
        return 1;
    }
    SkCanvas *canvas = surface->getCanvas();

    drawText(canvas);

    // 获取Surface的图像快照
    sk_sp<SkImage> image = surface->makeImageSnapshot();
    if (!image) {
        std::cerr << "无法创建图像快照" << std::endl;
        return 1;
    }

    // Fixed: Get SkPixmap from image for encoding
    SkPixmap pixmap;
    if (!image->peekPixels(&pixmap)) {
        std::cerr << "无法获取像素数据" << std::endl;
        return 1;
    }

    // 将图像编码为PNG并保存到文件
    SkFILEWStream out("output.png");
    if (!out.isValid()) {
        std::cerr << "无法打开输出文件" << std::endl;
        return 1;
    }

    SkPngEncoder::Options options;
    // Fixed: Use SkPixmap with the encoder
    if (!SkPngEncoder::Encode(&out, pixmap, options)) {
        std::cerr << "PNG编码失败" << std::endl;
        return 1;
    }

    std::cout << "图像已保存为output.png" << std::endl;
    return 0;
}
