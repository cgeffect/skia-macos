#include "include/core/SkCanvas.h"
#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkPath.h"
#include "include/core/SkStream.h"
#include "include/core/SkSurface.h"
#include "include/core/SkRRect.h"
#include "include/core/SkTextBlob.h"
#include "include/encode/SkPngEncoder.h"
#include <iostream>
void draw(SkCanvas *canvas) {
  const SkScalar scale = 256.0f;
  const SkScalar R = 0.45f * scale;
  const SkScalar TAU = 6.2831853f;
  SkPath path;
  path.moveTo(R, 0.0f);
  for (int i = 1; i < 7; ++i) {
    SkScalar theta = 3 * i * TAU / 7;
    path.lineTo(R * cos(theta), R * sin(theta));
  }
  path.close();
  SkPaint p;
  p.setAntiAlias(true);
  canvas->clear(SK_ColorWHITE);
  canvas->translate(0.5f * scale, 0.5f * scale);
  canvas->drawPath(path, p);
}

void draw1(SkCanvas* canvas) {
    canvas->drawColor(SK_ColorWHITE);

    SkPaint paint;
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(4);
    paint.setColor(SK_ColorRED);

    SkRect rect = SkRect::MakeXYWH(50, 50, 40, 60);
    canvas->drawRect(rect, paint);

    SkRRect oval;
    oval.setOval(rect);
    oval.offset(40, 60);
    paint.setColor(SK_ColorBLUE);
    canvas->drawRRect(oval, paint);

    paint.setColor(SK_ColorCYAN);
    canvas->drawCircle(180, 50, 25, paint);

    rect.offset(80, 0);
    paint.setColor(SK_ColorYELLOW);
    canvas->drawRoundRect(rect, 10, 10, paint);

    SkPath path;
    path.cubicTo(768, 0, -512, 256, 256, 256);
    paint.setColor(SK_ColorGREEN);
    canvas->drawPath(path, paint);

    SkPaint paint2;
    auto text = SkTextBlob::MakeFromString("Hello, Skia!", SkFont(nullptr, 18));
    canvas->drawTextBlob(text.get(), 50, 25, paint2);
}

int main() {

  auto surface(SkSurfaces::Raster(SkImageInfo::MakeN32Premul(500, 500)));

  if (!surface) {
    std::cerr << "无法创建Surface" << std::endl;
    return 1;
  }
  SkCanvas *canvas = surface->getCanvas();

  draw1(canvas);

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
