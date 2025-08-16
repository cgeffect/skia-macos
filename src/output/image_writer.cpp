#include "output/image_writer.h"
#include "include/encode/SkJpegEncoder.h"
#include <iostream>
#include <algorithm>
#include <fstream>

namespace skia_renderer {

ImageWriter::ImageWriter() {
}

ImageWriter::~ImageWriter() {
}

bool ImageWriter::saveAsPng(sk_sp<SkImage> image, const std::string& filename) {
    if (!image) {
        errorMessage = "图像为空";
        return false;
    }
    
    SkPixmap pixmap;
    if (!image->peekPixels(&pixmap)) {
        errorMessage = "无法获取像素数据";
        return false;
    }
    
    SkFILEWStream out(filename.c_str());
    if (!out.isValid()) {
        errorMessage = "无法打开输出文件: " + filename;
        return false;
    }
    
    SkPngEncoder::Options options;
    if (!SkPngEncoder::Encode(&out, pixmap, options)) {
        errorMessage = "PNG编码失败";
        return false;
    }
    
    return true;
}

bool ImageWriter::saveAsJpeg(sk_sp<SkImage> image, const std::string& filename, int quality) {
    if (!image) {
        errorMessage = "图像为空";
        return false;
    }
    
    SkPixmap pixmap;
    if (!image->peekPixels(&pixmap)) {
        errorMessage = "无法获取像素数据";
        return false;
    }
    
    SkFILEWStream out(filename.c_str());
    if (!out.isValid()) {
        errorMessage = "无法打开输出文件: " + filename;
        return false;
    }
    
    SkJpegEncoder::Options options;
    options.fQuality = quality;
    if (!SkJpegEncoder::Encode(&out, pixmap, options)) {
        errorMessage = "JPEG编码失败";
        return false;
    }
    
    return true;
}

bool ImageWriter::saveImage(sk_sp<SkImage> image, const std::string& filename, int quality) {
    std::string extension = getFileExtension(filename);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    
    if (extension == "png") {
        return saveAsPng(image, filename);
    } else if (extension == "jpg" || extension == "jpeg") {
        return saveAsJpeg(image, filename, quality);
    } else {
        // 默认保存为PNG
        return saveAsPng(image, filename);
    }
}

std::string ImageWriter::getFileExtension(const std::string& filename) {
    size_t pos = filename.find_last_of('.');
    if (pos != std::string::npos) {
        return filename.substr(pos + 1);
    }
    return "";
}

bool ImageWriter::isFileWritable(const std::string& filename) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file.close();
        return true;
    }
    return false;
}

} // namespace skia_renderer 