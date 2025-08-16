#include "../src/engine/render_engine.h"
#include <iostream>

int main() {
    // 创建渲染引擎
    skia_renderer::RenderEngine engine;
    
    // 示例1: 从协议文件渲染
    std::cout << "=== 示例1: 从协议文件渲染 ===" << std::endl;
    if (engine.renderFromProtocol("projects/trip/trip_protocol.json")) {
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
    
    return 0;
} 