#include "../src/engine/render_engine.h"
#include <iostream>
#include <sys/stat.h>

int main() {
    // 创建output目录
    struct stat st = {0};
    if (stat("output", &st) == -1) {
        mkdir("output", 0700);
    }

    std::cout << "=== Skia文本样式功能演示 ===" << std::endl;

    // 创建渲染引擎
    skia_renderer::RenderEngine engine;

    // 演示1: 基础字体样式
    std::cout << "\n1. 基础字体样式演示..." << std::endl;
    if (engine.renderFromProtocol("projects/text_styles_protocols/basic_font_styles.json")) {
        std::cout << "✅ 基础字体样式渲染成功！" << std::endl;
    } else {
        std::cerr << "❌ 基础字体样式渲染失败: " << engine.getErrorMessage() << std::endl;
    }

    // 演示2: 文本效果（阴影、描边、填充）
    std::cout << "\n2. 文本效果演示..." << std::endl;
    if (engine.renderFromProtocol("projects/text_styles_protocols/text_effects.json")) {
        std::cout << "✅ 文本效果渲染成功！" << std::endl;
    } else {
        std::cerr << "❌ 文本效果渲染失败: " << engine.getErrorMessage() << std::endl;
    }

    // 演示3: 文本对齐和布局
    std::cout << "\n3. 文本对齐和布局演示..." << std::endl;
    if (engine.renderFromProtocol("projects/text_styles_protocols/text_alignment.json")) {
        std::cout << "✅ 文本对齐和布局渲染成功！" << std::endl;
    } else {
        std::cerr << "❌ 文本对齐和布局渲染失败: " << engine.getErrorMessage() << std::endl;
    }

    // 演示4: 四种显示模式对比
    std::cout << "\n4. 显示模式对比演示..." << std::endl;
    if (engine.renderFromProtocol("projects/text_styles_protocols/display_modes.json")) {
        std::cout << "✅ 显示模式对比渲染成功！" << std::endl;
    } else {
        std::cerr << "❌ 显示模式对比渲染失败: " << engine.getErrorMessage() << std::endl;
    }

    // 演示5: 颜色和变换效果
    std::cout << "\n5. 颜色和变换效果演示..." << std::endl;
    if (engine.renderFromProtocol("projects/text_styles_protocols/colors_and_transforms.json")) {
        std::cout << "✅ 颜色和变换效果渲染成功！" << std::endl;
    } else {
        std::cerr << "❌ 颜色和变换效果渲染失败: " << engine.getErrorMessage() << std::endl;
    }

    // 演示6: 中英文混合排版
    std::cout << "\n6. 中英文混合排版演示..." << std::endl;
    if (engine.renderFromProtocol("projects/text_styles_protocols/mixed_languages.json")) {
        std::cout << "✅ 中英文混合排版渲染成功！" << std::endl;
    } else {
        std::cerr << "❌ 中英文混合排版渲染失败: " << engine.getErrorMessage() << std::endl;
    }

    // 演示7: 富文本效果
    std::cout << "\n7. 富文本效果演示..." << std::endl;
    if (engine.renderFromProtocol("projects/text_styles_protocols/rich_text_demo.json")) {
        std::cout << "✅ 富文本效果渲染成功！" << std::endl;
    } else {
        std::cerr << "❌ 富文本效果渲染失败: " << engine.getErrorMessage() << std::endl;
    }

    std::cout << "\n🎨 所有文本样式演示完成！请查看output目录中的输出图片。" << std::endl;
    
    return 0;
}
