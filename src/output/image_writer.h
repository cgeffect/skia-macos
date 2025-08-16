#pragma once

#include "include/core/SkImage.h"
#include "include/encode/SkPngEncoder.h"
#include "include/core/SkStream.h"
#include <string>

namespace skia_renderer {

class ImageWriter {
public:
    ImageWriter();
    ~ImageWriter();
    
    // 保存图像为PNG
    bool saveAsPng(sk_sp<SkImage> image, const std::string& filename);
    
    // 保存图像为JPEG
    bool saveAsJpeg(sk_sp<SkImage> image, const std::string& filename, int quality = 100);
    
    // 保存图像（根据扩展名自动选择格式）
    bool saveImage(sk_sp<SkImage> image, const std::string& filename, int quality = 100);
    
    // 获取错误信息
    const std::string& getErrorMessage() const { return errorMessage; }

private:
    std::string errorMessage;
    
    // 获取文件扩展名
    std::string getFileExtension(const std::string& filename);
    
    // 检查文件是否可写
    bool isFileWritable(const std::string& filename);
};

} // namespace skia_renderer 