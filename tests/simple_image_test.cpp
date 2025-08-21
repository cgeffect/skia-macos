#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cstdlib>

// Skia headers for image comparison
#include "include/core/SkBitmap.h"
#include "include/core/SkImage.h"
#include "include/core/SkData.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/core/SkImageInfo.h"
#include "include/encode/SkPngEncoder.h"

namespace fs = std::filesystem;

class SimpleImageTest {
private:
    std::string baselineDir = "tests/baseline/";
    std::string outputDir = "output/";
    std::string diffDir = "tests/diff/";
    double tolerance = 0.01; // 1% 容差

public:
    SimpleImageTest() {
        // 创建必要的目录
        fs::create_directories(baselineDir);
        fs::create_directories(diffDir);
    }

    // 计算图片的哈希值（用于快速比较）
    std::string calculateImageHash(const std::string& imagePath) {
        auto data = SkData::MakeFromFileName(imagePath.c_str());
        if (!data) {
            std::cerr << "无法读取图片: " << imagePath << std::endl;
            return "";
        }

        // 简单的哈希计算
        size_t hash = 0;
        const uint8_t* bytes = static_cast<const uint8_t*>(data->data());
        size_t size = data->size();
        
        for (size_t i = 0; i < size; ++i) {
            hash = hash * 31 + bytes[i];
        }
        
        std::stringstream ss;
        ss << std::hex << hash;
        return ss.str();
    }

    // 比较两个图片的像素差异
    double compareImages(const std::string& image1Path, const std::string& image2Path) {
        auto image1 = SkImages::DeferredFromEncodedData(SkData::MakeFromFileName(image1Path.c_str()));
        auto image2 = SkImages::DeferredFromEncodedData(SkData::MakeFromFileName(image2Path.c_str()));
        
        if (!image1 || !image2) {
            std::cerr << "无法加载图片进行比较" << std::endl;
            return 1.0; // 返回最大差异
        }

        SkBitmap bitmap1, bitmap2;
        if (!image1->asLegacyBitmap(&bitmap1) || !image2->asLegacyBitmap(&bitmap2)) {
            std::cerr << "无法转换为位图" << std::endl;
            return 1.0;
        }

        // 检查尺寸是否相同
        if (bitmap1.width() != bitmap2.width() || bitmap1.height() != bitmap2.height()) {
            std::cerr << "图片尺寸不同: " << bitmap1.width() << "x" << bitmap1.height() 
                      << " vs " << bitmap2.width() << "x" << bitmap2.height() << std::endl;
            return 1.0;
        }

        int width = bitmap1.width();
        int height = bitmap1.height();
        int totalPixels = width * height;
        int differentPixels = 0;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                SkColor color1 = bitmap1.getColor(x, y);
                SkColor color2 = bitmap2.getColor(x, y);
                
                if (color1 != color2) {
                    differentPixels++;
                }
            }
        }

        return static_cast<double>(differentPixels) / totalPixels;
    }

    // 生成差异图片
    void generateDiffImage(const std::string& image1Path, const std::string& image2Path, 
                          const std::string& diffPath) {
        auto image1 = SkImages::DeferredFromEncodedData(SkData::MakeFromFileName(image1Path.c_str()));
        auto image2 = SkImages::DeferredFromEncodedData(SkData::MakeFromFileName(image2Path.c_str()));
        
        if (!image1 || !image2) {
            std::cerr << "无法加载图片生成差异图" << std::endl;
            return;
        }

        SkBitmap bitmap1, bitmap2;
        if (!image1->asLegacyBitmap(&bitmap1) || !image2->asLegacyBitmap(&bitmap2)) {
            return;
        }

        int width = bitmap1.width();
        int height = bitmap1.height();

        // 创建差异图片
        SkImageInfo info = SkImageInfo::Make(width, height, kRGBA_8888_SkColorType, kPremul_SkAlphaType);
        auto surface = SkSurfaces::Raster(info);
        SkCanvas* canvas = surface->getCanvas();

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                SkColor color1 = bitmap1.getColor(x, y);
                SkColor color2 = bitmap2.getColor(x, y);
                
                if (color1 != color2) {
                    // 差异像素显示为红色
                    SkPaint paint;
                    paint.setColor(SK_ColorRED);
                    canvas->drawPoint(x, y, paint);
                } else {
                    // 相同像素显示为原色
                    SkPaint paint;
                    paint.setColor(color1);
                    canvas->drawPoint(x, y, paint);
                }
            }
        }

        // 保存差异图片
        auto diffImage = surface->makeImageSnapshot();
        auto data = SkPngEncoder::Encode(nullptr, diffImage.get(), {});
        if (data) {
            std::ofstream file(diffPath, std::ios::binary);
            file.write(static_cast<const char*>(data->data()), data->size());
        }
    }

    // 运行单个测试
    bool runTest(const std::string& testName, const std::string& protocolFile) {
        std::cout << "运行测试: " << testName << std::endl;
        
        // 基线图片路径
        std::string baselineImagePath = baselineDir + testName + "_baseline.png";
        std::string diffImagePath = diffDir + testName + "_diff.png";

        // 1. 运行渲染程序生成当前图片
        std::cout << "  渲染协议文件: " << protocolFile << std::endl;
        
        std::string command = "./build/renderer " + protocolFile;
        int result = system(command.c_str());
        
        if (result != 0) {
            std::cerr << "  ❌ 渲染失败，退出码: " << result << std::endl;
            return false;
        }

        // 2. 查找生成的图片文件
        std::string currentImagePath = "";
        for (const auto& entry : fs::directory_iterator(outputDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".png") {
                std::string filename = entry.path().filename().string();
                if (filename.find(testName) != std::string::npos) {
                    currentImagePath = entry.path().string();
                    break;
                }
            }
        }
        
        if (currentImagePath.empty()) {
            std::cerr << "  ❌ 未找到生成的图片文件" << std::endl;
            return false;
        }

        std::cout << "  找到生成图片: " << currentImagePath << std::endl;
        
        // 3. 检查基线图片是否存在
        if (!fs::exists(baselineImagePath)) {
            std::cout << "  基线图片不存在，创建基线..." << std::endl;
            fs::copy_file(currentImagePath, baselineImagePath);
            return true;
        }

        // 4. 计算哈希值进行快速比较
        std::string currentHash = calculateImageHash(currentImagePath);
        std::string baselineHash = calculateImageHash(baselineImagePath);
        
        if (currentHash == baselineHash) {
            std::cout << "  ✅ 图片完全一致 (哈希值相同)" << std::endl;
            return true;
        }

        // 5. 进行像素级比较
        double difference = compareImages(currentImagePath, baselineImagePath);
        
        if (difference <= tolerance) {
            std::cout << "  ✅ 图片基本一致 (差异: " << std::fixed << std::setprecision(4) 
                      << (difference * 100) << "%)" << std::endl;
            return true;
        } else {
            std::cout << "  ❌ 图片差异过大 (差异: " << std::fixed << std::setprecision(4) 
                      << (difference * 100) << "%)" << std::endl;
            
            // 生成差异图片
            generateDiffImage(currentImagePath, baselineImagePath, diffImagePath);
            std::cout << "  差异图片已保存: " << diffImagePath << std::endl;
            
            return false;
        }
    }

    // 运行所有测试
    void runAllTests() {
        std::cout << "=== 简单图片对比测试开始 ===" << std::endl;
        std::cout << "容差设置: " << (tolerance * 100) << "%" << std::endl;
        std::cout << "基线目录: " << baselineDir << std::endl;
        std::cout << "输出目录: " << outputDir << std::endl;
        std::cout << "差异目录: " << diffDir << std::endl;
        std::cout << std::endl;

        auto startTime = std::chrono::high_resolution_clock::now();
        
        std::vector<std::pair<std::string, std::string>> tests = {
            {"trip", "projects/trip/trip_protocol.json"},
            {"sunscreen", "projects/sunscreen/sunscreen_protocol.json"},
            {"food", "projects/food/food_protocol.json"},
            {"spring", "projects/spring/spring_protocol.json"},
            {"dessert", "projects/dessert/dessert_protocol.json"},
            {"cup", "projects/cup/cup_protocol.json"},
            {"horizontal", "projects/horizontal/horizontal_protocol.json"},
            {"long", "projects/long/long_protocol.json"},
            {"clothes", "projects/clothes/clothes_protocol.json"},
            {"tshirt", "projects/tshirt/tshirt_protocol.json"},
            {"single_line", "projects/text_wrap_test/single_line_protocol.json"},
            {"multi_line", "projects/text_wrap_test/multi_line_protocol.json"},
            {"word_wrap", "projects/text_wrap_test/word_wrap_protocol.json"},
            {"auto_fit", "projects/text_wrap_test/auto_fit_protocol.json"}
        };

        int passedTests = 0;
        int totalTests = tests.size();

        for (const auto& test : tests) {
            bool result = runTest(test.first, test.second);
            if (result) {
                passedTests++;
            }
            std::cout << std::endl;
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        std::cout << "=== 测试结果 ===" << std::endl;
        std::cout << "总测试数: " << totalTests << std::endl;
        std::cout << "通过测试: " << passedTests << std::endl;
        std::cout << "失败测试: " << (totalTests - passedTests) << std::endl;
        std::cout << "测试耗时: " << duration.count() << "ms" << std::endl;
        
        if (passedTests == totalTests) {
            std::cout << "🎉 所有测试通过！" << std::endl;
        } else {
            std::cout << "⚠️  部分测试失败，请检查差异图片" << std::endl;
        }
    }

    // 更新基线图片
    void updateBaseline(const std::string& testName) {
        // 查找当前输出图片
        std::string currentImagePath = "";
        for (const auto& entry : fs::directory_iterator(outputDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".png") {
                std::string filename = entry.path().filename().string();
                if (filename.find(testName) != std::string::npos) {
                    currentImagePath = entry.path().string();
                    break;
                }
            }
        }
        
        std::string baselineImagePath = baselineDir + testName + "_baseline.png";
        
        if (!currentImagePath.empty() && fs::exists(currentImagePath)) {
            fs::copy_file(currentImagePath, baselineImagePath, fs::copy_options::overwrite_existing);
            std::cout << "已更新基线图片: " << baselineImagePath << std::endl;
        } else {
            std::cerr << "当前图片不存在，请先运行渲染" << std::endl;
        }
    }

    // 设置容差
    void setTolerance(double newTolerance) {
        tolerance = newTolerance;
        std::cout << "容差已设置为: " << (tolerance * 100) << "%" << std::endl;
    }

    // 一致性测试 - 多次渲染同一协议，检查是否一致
    void consistencyTest(const std::string& testName, const std::string& protocolFile, int iterations = 5) {
        std::cout << "=== 一致性测试: " << testName << " (迭代 " << iterations << " 次) ===" << std::endl;
        
        std::vector<std::string> hashes;
        
        for (int i = 0; i < iterations; ++i) {
            std::cout << "  第 " << (i+1) << " 次渲染..." << std::endl;
            
            // 运行渲染程序
            std::string command = "./build/renderer " + protocolFile;
            int result = system(command.c_str());
            
            if (result != 0) {
                std::cerr << "  ❌ 第 " << (i+1) << " 次渲染失败" << std::endl;
                return;
            }
            
            // 查找生成的图片文件
            std::string imagePath = "";
            for (const auto& entry : fs::directory_iterator(outputDir)) {
                if (entry.is_regular_file() && entry.path().extension() == ".png") {
                    std::string filename = entry.path().filename().string();
                    if (filename.find(testName) != std::string::npos) {
                        imagePath = entry.path().string();
                        break;
                    }
                }
            }
            
            if (imagePath.empty()) {
                std::cerr << "  ❌ 第 " << (i+1) << " 次渲染未找到图片文件" << std::endl;
                return;
            }
            
            std::string hash = calculateImageHash(imagePath);
            hashes.push_back(hash);
            
            std::cout << "  第 " << (i+1) << " 次渲染完成，哈希值: " << hash << std::endl;
        }
        
        // 检查所有哈希值是否相同
        bool allSame = true;
        std::string firstHash = hashes[0];
        
        for (size_t i = 1; i < hashes.size(); ++i) {
            if (hashes[i] != firstHash) {
                allSame = false;
                std::cout << "  ❌ 第 " << (i+1) << " 次渲染结果不同" << std::endl;
            }
        }
        
        if (allSame) {
            std::cout << "  ✅ 所有渲染结果完全一致！" << std::endl;
        } else {
            std::cout << "  ❌ 渲染结果不一致，可能存在随机性" << std::endl;
        }
    }
};

int main(int argc, char* argv[]) {
    SimpleImageTest test;
    
    if (argc > 1) {
        std::string command = argv[1];
        
        if (command == "run") {
            test.runAllTests();
        } else if (command == "update" && argc > 2) {
            test.updateBaseline(argv[2]);
        } else if (command == "tolerance" && argc > 2) {
            double tolerance = std::stod(argv[2]);
            test.setTolerance(tolerance);
        } else if (command == "consistency" && argc > 2) {
            std::string testName = argv[2];
            int iterations = (argc > 3) ? std::stoi(argv[3]) : 5;
            
            // 特殊处理文本模式测试的路径
            std::string protocolFile;
            if (testName == "single_line" || testName == "multi_line" || 
                testName == "word_wrap" || testName == "auto_fit") {
                protocolFile = "projects/text_wrap_test/" + testName + "_protocol.json";
            } else {
                protocolFile = "projects/" + testName + "/" + testName + "_protocol.json";
            }
            
            test.consistencyTest(testName, protocolFile, iterations);
        } else {
            std::cout << "用法:" << std::endl;
            std::cout << "  " << argv[0] << " run                    # 运行所有测试" << std::endl;
            std::cout << "  " << argv[0] << " update <test_name>     # 更新指定测试的基线" << std::endl;
            std::cout << "  " << argv[0] << " tolerance <value>      # 设置容差 (0.0-1.0)" << std::endl;
            std::cout << "  " << argv[0] << " consistency <test_name> [iterations] # 一致性测试" << std::endl;
        }
    } else {
        test.runAllTests();
    }
    
    return 0;
} 