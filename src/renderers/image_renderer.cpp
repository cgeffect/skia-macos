#include "renderers/image_renderer.h"
#include <iostream>

namespace skia_renderer {

ImageRenderer::ImageRenderer() {
}

ImageRenderer::~ImageRenderer() {
}

bool ImageRenderer::renderImage(SkCanvas *canvas, const ImageElement &imageElement) {
    if (!canvas) {
        std::cerr << "画布为空" << std::endl;
        return false;
    }

    // 加载图片
    sk_sp<SkImage> image = loadImage(imageElement.path);
    if (!image) {
        std::cerr << "无法加载图片: " << imageElement.path << std::endl;
        return false;
    }

    // 【状态保存】将调用时刻的Canvas当前状态压入栈中
    // 保存内容：当前的变换矩阵、裁剪区域、绘制参数等完整状态
    // 注意：保存的是当前状态，不一定是初始状态！
    canvas->save();
    
    // 【关键】三层渲染架构的协同工作：
    // 1. 先设置Canvas变换（坐标系统）
    applyTransform(canvas, imageElement.transform);
    
    // 2. 然后在变换后的坐标系中绘制图片（矩形+绘制参数）
    drawImage(canvas, image, imageElement);
    
    // 【状态恢复】弹出栈顶状态，恢复到save()时刻的状态
    // 效果：完全清除applyTransform()造成的变换，回到进入函数时的状态
    // 确保后续元素不受当前元素变换的影响，实现元素间的独立性
    canvas->restore();

    return true;
}

sk_sp<SkImage> ImageRenderer::loadImage(const std::string &imagePath) {
    sk_sp<SkData> data = SkData::MakeFromFileName(imagePath.c_str());
    if (!data) {
        return nullptr;
    }

    return SkImages::DeferredFromEncodedData(data);
}

bool ImageRenderer::isValidImage(const std::string &imagePath) {
    sk_sp<SkImage> image = loadImage(imagePath);
    return image != nullptr;
}

void ImageRenderer::applyTransform(SkCanvas *canvas, const Transform &transform) {
    // 【第一层：Canvas变换】- 影响所有后续绘制操作的坐标系统
    canvas->translate(transform.x, transform.y);       // 平移：移动坐标原点到指定位置
    canvas->scale(transform.scaleX, transform.scaleY); // 缩放：改变坐标系的比例
    canvas->rotate(transform.rotation);                // 旋转：围绕当前原点旋转坐标系
}

void ImageRenderer::drawImage(SkCanvas *canvas, sk_sp<SkImage> image, const ImageElement &imageElement) {
    // 【第二层：目标矩形】- 定义图片在当前坐标系中的绘制区域
    // 
    // 【精确理解】(0,0) = 经过变换之后的起始位置！
    // 
    // 变换过程：
    // 1. 原始坐标系：原点在画布(0,0)
    // 2. translate(transform.x, transform.y)：原点移动到目标位置
    // 3. scale/rotate：在新原点基础上进行缩放旋转
    // 4. 此时的(0,0) = 变换后坐标系的起始位置
    // 
    // 结果：SkRect(0,0,w,h) 从变换后的起始位置开始绘制
    SkRect dstRect = SkRect::MakeXYWH(0, 0, imageElement.width, imageElement.height);

    // 【第三层：绘制参数】- 控制绘制的视觉效果（颜色、透明度等）
    SkPaint paint;
    // 将0.0-1.0的透明度转换为0-255的Alpha值
    paint.setAlpha(static_cast<U8CPU>(imageElement.transform.opacity * 255));

    // 最终绘制：将源图片绘制到目标矩形，应用Paint效果
    // image: 源图片数据
    // dstRect: 目标矩形（已经在变换后的坐标系中）
    // SkSamplingOptions(): 采样选项（图片缩放时的插值方法）
    // &paint: 绘制参数（透明度等视觉效果）
    canvas->drawImageRect(image, dstRect, SkSamplingOptions(), &paint);
}

} // namespace skia_renderer
