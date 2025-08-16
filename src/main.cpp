#include "universal_renderer.h"
#include <iostream>

int main() {
    UniversalRenderer renderer;
    
    // 渲染青海湖旅游攻略
    std::cout << "正在渲染青海湖旅游攻略..." << std::endl;
    if (renderer.renderFromProtocol("projects/trip/trip_protocol.json")) {
        std::cout << "青海湖旅游攻略图渲染成功！" << std::endl;
    } else {
        std::cerr << "青海湖旅游攻略图渲染失败！" << std::endl;
        std::cerr << "错误信息: " << renderer.getErrorMessage() << std::endl;
        return 1;
    }
    
    // 渲染防晒海报
    std::cout << "正在渲染防晒海报..." << std::endl;
    if (renderer.renderFromProtocol("projects/sunscreen/sunscreen_protocol.json")) {
        std::cout << "防晒海报渲染成功！" << std::endl;
    } else {
        std::cerr << "防晒海报渲染失败！" << std::endl;
        std::cerr << "错误信息: " << renderer.getErrorMessage() << std::endl;
        return 1;
    }
    
    // 渲染美食海报
    std::cout << "正在渲染美食海报..." << std::endl;
    if (renderer.renderFromProtocol("projects/food/food_protocol.json")) {
        std::cout << "美食海报渲染成功！" << std::endl;
    } else {
        std::cerr << "美食海报渲染失败！" << std::endl;
        std::cerr << "错误信息: " << renderer.getErrorMessage() << std::endl;
        return 1;
    }
    
    return 0;
}
