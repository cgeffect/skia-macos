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
#include "include/core/SkTextBlob.h"

#include <iostream>
#include <vector>
#include <string>

// 富文本段落结构
struct RichTextSegment {
    std::string text;
    SkColor color;
    
    RichTextSegment(const std::string& t, SkColor c) : text(t), color(c) {}
};

// 文本换行函数（支持富文本）
std::vector<std::vector<RichTextSegment>> wrapRichText(const std::vector<RichTextSegment>& segments, float maxWidth, const SkFont& font) {
    std::vector<std::vector<RichTextSegment>> lines;
    std::vector<RichTextSegment> currentLine;
    std::string currentText;
    
    for (const auto& segment : segments) {
        std::string remainingText = segment.text;
        
        while (!remainingText.empty()) {
            // 按字符分割文本（支持中文）
            std::vector<std::string> characters;
            for (size_t i = 0; i < remainingText.length();) {
                if ((remainingText[i] & 0x80) == 0) {
                    // ASCII字符
                    characters.push_back(remainingText.substr(i, 1));
                    i++;
                } else if ((remainingText[i] & 0xE0) == 0xC0) {
                    // 2字节UTF-8字符
                    characters.push_back(remainingText.substr(i, 2));
                    i += 2;
                } else if ((remainingText[i] & 0xF0) == 0xE0) {
                    // 3字节UTF-8字符（中文）
                    characters.push_back(remainingText.substr(i, 3));
                    i += 3;
                } else {
                    // 其他情况，按单字节处理
                    characters.push_back(remainingText.substr(i, 1));
                    i++;
                }
            }
            
            std::string testText = currentText;
            for (const auto& ch : characters) {
                std::string newTestText = testText + ch;
                SkRect bounds;
                font.measureText(newTestText.c_str(), newTestText.length(), SkTextEncoding::kUTF8, &bounds);
                
                if (bounds.width() > maxWidth && !testText.empty()) {
                    // 当前行已满，开始新行
                    if (!testText.empty()) {
                        currentLine.push_back(RichTextSegment(testText, segment.color));
                        lines.push_back(currentLine);
                        currentLine.clear();
                    }
                    testText = ch;
                } else {
                    testText = newTestText;
                }
            }
            
            // 更新当前文本和剩余文本
            currentText = testText;
            remainingText.clear();
        }
        
        // 将当前段落的文本添加到当前行
        if (!currentText.empty()) {
            currentLine.push_back(RichTextSegment(currentText, segment.color));
        }
    }
    
    // 添加最后一行
    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }
    
    return lines;
}

void drawText(SkCanvas *canvas) {
    // 正确的字体加载代码
    sk_sp<SkFontMgr> fontMgr = SkFontMgr_New_CoreText(nullptr);
    if (!fontMgr) {
        fprintf(stderr, "无法创建字体管理器\n");
        return;
    }

    // 使用正确的方法名 makeFromFile
    sk_sp<SkTypeface> typeface = fontMgr->makeFromFile("res/字制区喜脉体.ttf", 0);
    if (!typeface) {
        fprintf(stderr, "无法加载字体文件: res/字制区喜脉体.ttf\n");
        return;
    }

    SkFont font(typeface, 32); // 设置字体和字号
    
    // 定义富文本段落
    std::vector<RichTextSegment> segments = {
        RichTextSegment("主导云端", SK_ColorBLUE),
        RichTextSegment("视频渲染", SK_ColorYELLOW),  // 黄色高亮
        RichTextSegment("系统架构设计，构建分布式异步消息处理架构", SK_ColorBLUE)
    };
    
    // 设置文本区域参数
    float maxWidth = 450.0f; // 画布宽度500减去左右边距
    float lineHeight = font.getSpacing() * 1.2f; // 行高
    float startX = 25.0f; // 起始X坐标
    float startY = 50.0f; // 起始Y坐标
    float padding = 20.0f; // 内边距
    
    // 文本换行
    std::vector<std::vector<RichTextSegment>> lines = wrapRichText(segments, maxWidth, font);
    
    // 计算文本区域的总高度和宽度
    float totalHeight = lines.size() * lineHeight + 2 * padding;
    float totalWidth = maxWidth + 2 * padding;
    
    // 绘制每一行文本
    for (size_t i = 0; i < lines.size(); i++) {
        float y = startY + padding + i * lineHeight + font.getSize(); // 调整Y坐标以对齐文本基线
        float x = startX + padding;
        
        for (const auto& segment : lines[i]) {
            // 绘制文字阴影
            SkPaint shadowPaint;
            shadowPaint.setColor(SkColorSetARGB(100, 0, 0, 0)); // 半透明黑色阴影
            shadowPaint.setStyle(SkPaint::kFill_Style);
            canvas->drawString(segment.text.c_str(), x + 2, y + 2, font, shadowPaint);
            
            // 绘制文字描边（蓝色）
            SkPaint strokePaint;
            strokePaint.setColor(segment.color);
            strokePaint.setStyle(SkPaint::kStroke_Style);
            strokePaint.setStrokeWidth(2.0f);
            canvas->drawString(segment.text.c_str(), x, y, font, strokePaint);
            
            // 绘制文字填充（白色）
            SkPaint fillPaint;
            fillPaint.setColor(SK_ColorWHITE);
            fillPaint.setStyle(SkPaint::kFill_Style);
            canvas->drawString(segment.text.c_str(), x, y, font, fillPaint);
            
            // 计算下一个段落的X位置
            SkRect bounds;
            font.measureText(segment.text.c_str(), segment.text.length(), SkTextEncoding::kUTF8, &bounds);
            x += bounds.width();
        }
    }
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
