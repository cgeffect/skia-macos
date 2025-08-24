#include "../src/engine/render_engine.h"
#include <iostream>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    // 创建output目录
    struct stat st = {0};
    if (stat("output", &st) == -1) {
        mkdir("output", 0700);
    }

    // 如果提供了命令行参数，渲染指定的协议文件
    if (argc > 1) {
        std::string protocolFile = argv[1];
        std::cout << "渲染协议文件: " << protocolFile << std::endl;

        skia_renderer::RenderEngine engine;
        if (engine.renderFromProtocol(protocolFile)) {
            std::cout << "✅ 渲染成功！" << std::endl;
        } else {
            std::cerr << "❌ 渲染失败: " << engine.getErrorMessage() << std::endl;
            return 1;
        }
        return 0;
    }

    // 创建渲染引擎
    skia_renderer::RenderEngine engine;

    // 示例1: 从协议文件渲染
    std::cout << "=== 示例1: 从协议文件渲染 ===" << std::endl;
    if (engine.renderFromProtocol("/Users/jason/Desktop/skia-macos/projects/trip/trip_protocol.json")) {
        std::cout << "✅ 渲染成功！" << std::endl;
    } else {
        std::cerr << "❌ 渲染失败: " << engine.getErrorMessage() << std::endl;
    }

    // 示例2: 自定义字体管理器
    std::cout << "\n=== 示例2: 自定义字体管理器 ===" << std::endl;
    auto fontManager = std::make_shared<skia_renderer::FontManager>();
    fontManager->registerFontFile("SourceHanSansCN-Normal", "res/Fonts/SourceHanSansCN-Normal.otf");
    engine.setFontManager(fontManager);

    if (engine.renderFromProtocol("projects/sunscreen/sunscreen_protocol.json")) {
        std::cout << "✅ 使用自定义字体渲染成功！" << std::endl;
    } else {
        std::cerr << "❌ 渲染失败: " << engine.getErrorMessage() << std::endl;
    }

    // 示例3: 渲染美食海报
    std::cout << "\n=== 示例3: 渲染美食海报 ===" << std::endl;
    if (engine.renderFromProtocol("projects/food/food_protocol.json")) {
        std::cout << "✅ 美食海报渲染成功！" << std::endl;
    } else {
        std::cerr << "❌ 美食海报渲染失败: " << engine.getErrorMessage() << std::endl;
    }

    // 示例4: 渲染春天海报
    std::cout << "\n=== 示例4: 渲染春天海报 ===" << std::endl;
    if (engine.renderFromProtocol("projects/spring/spring_protocol.json")) {
        std::cout << "✅ 春天海报渲染成功！" << std::endl;
    } else {
        std::cerr << "❌ 春天海报渲染失败: " << engine.getErrorMessage() << std::endl;
    }

    // 示例5: 渲染甜品海报
    std::cout << "\n=== 示例5: 渲染甜品海报 ===" << std::endl;
    if (engine.renderFromProtocol("projects/dessert/dessert_protocol.json")) {
        std::cout << "✅ 甜品海报渲染成功！" << std::endl;
    } else {
        std::cerr << "❌ 甜品海报渲染失败: " << engine.getErrorMessage() << std::endl;
    }

    // 示例6: 渲染杯子海报
    std::cout << "\n=== 示例6: 渲染杯子海报 ===" << std::endl;
    if (engine.renderFromProtocol("projects/cup/cup_protocol.json")) {
        std::cout << "✅ 杯子海报渲染成功！" << std::endl;
    } else {
        std::cerr << "❌ 杯子海报渲染失败: " << engine.getErrorMessage() << std::endl;
    }

    // 示例7: 渲染冬至海报
    std::cout << "\n=== 示例7: 渲染冬至海报 ===" << std::endl;
    if (engine.renderFromProtocol("projects/horizontal/horizontal_protocol.json")) {
        std::cout << "✅ 冬至海报渲染成功！" << std::endl;
    } else {
        std::cerr << "❌ 冬至海报渲染失败: " << engine.getErrorMessage() << std::endl;
    }

    // 示例8: 渲染蛋黄心长图
    std::cout << "\n=== 示例8: 渲染蛋黄心长图 ===" << std::endl;
    if (engine.renderFromProtocol("projects/long/long_protocol.json")) {
        std::cout << "✅ 蛋黄心长图渲染成功！" << std::endl;
    } else {
        std::cerr << "❌ 蛋黄心长图渲染失败: " << engine.getErrorMessage() << std::endl;
    }

    // 示例9: 渲染文本换行测试
    std::cout << "\n=== 示例9: 渲染文本换行测试 ===" << std::endl;
    if (engine.renderFromProtocol("projects/text_wrap_test/text_wrap_test_protocol.json")) {
        std::cout << "✅ 文本换行测试渲染成功！" << std::endl;
    } else {
        std::cerr << "❌ 文本换行测试渲染失败: " << engine.getErrorMessage() << std::endl;
    }

    // 示例10: 渲染服装海报
    std::cout << "\n=== 示例10: 渲染服装海报 ===" << std::endl;
    if (engine.renderFromProtocol("projects/clothes/clothes_protocol.json")) {
        std::cout << "✅ 服装海报渲染成功！" << std::endl;
    } else {
        std::cerr << "❌ 服装海报渲染失败: " << engine.getErrorMessage() << std::endl;
    }

    return 0;
}