#pragma once

#include "include/core/SkColor.h"
#include <string>
#include <vector>

namespace skia_renderer {

// 基础变换结构 - 用于图片和文本元素的位置、缩放、旋转和透明度控制
struct Transform {
    float x = 0.0f;         // X坐标位置 (像素)，相对于画布左上角，向右为正
    float y = 0.0f;         // Y坐标位置 (像素)，相对于画布左上角，向下为正
    float scaleX = 1.0f;    // 水平缩放比例，1.0=原始大小，0.5=缩小50%，2.0=放大100%
    float scaleY = 1.0f;    // 垂直缩放比例，1.0=原始大小，0.5=缩小50%，2.0=放大100%
    float rotation = 0.0f;  // 旋转角度 (度)，顺时针为正，逆时针为负，围绕元素中心旋转
    float opacity = 1.0f;   // 透明度 (0.0-1.0)，1.0=完全不透明，0.5=半透明，0.0=完全透明
};

// 画布配置 - 定义渲染画布的尺寸、背景色和调试选项
struct CanvasConfig {
    int width = 1242;                    // 画布宽度 (像素)，最终图片的宽度
    int height = 1660;                   // 画布高度 (像素)，最终图片的高度
    std::string background = "#FFFFFF";  // 背景颜色，支持#RRGGBB、rgb()、rgba()等格式
    bool debug = false;                  // 调试模式，true时显示文本区域的红色边框
};

// 图片元素 - 定义要渲染的图片及其属性
struct ImageElement {
    std::string id;         // 图片唯一标识符，用于调试和引用
    std::string path;       // 图片文件路径，支持PNG、JPEG、WebP格式
    Transform transform;    // 变换属性（位置、缩放、旋转、透明度）
    int width = 0;          // 显示宽度 (像素)，0表示使用图片原始宽度
    int height = 0;         // 显示高度 (像素)，0表示使用图片原始高度
};

// 文本显示模式 - 定义文本的布局和显示策略
enum class TextDisplayMode {
    SingleLine,     // 单行模式：强制单行显示，超出部分截断并可显示省略号
    MultiLine,      // 多行模式：固定行数显示，支持行数限制和省略号
    WordWrap,       // 自动换行模式：根据容器宽度智能换行，支持中英文混合
    AutoFit         // 自适应模式：自动调整字体大小以适应容器尺寸
};

// 文本样式 - 定义文本的字体、颜色、效果和布局属性
struct TextStyle {
    std::string fontFamily = "Arial";                        // 字体族名称，如"SourceHanSansCN-Bold"直接指定Weight
    float fontSize = 12.0f;                                  // 字体大小 (像素)
    SkColor fillColor = SK_ColorBLACK;                       // 文字填充颜色
    SkColor strokeColor = SK_ColorBLACK;                     // 文字描边颜色
    float strokeWidth = 0.0f;                                // 描边宽度 (像素)，0表示无描边
    bool hasShadow = false;                                  // 是否启用阴影效果
    float shadowDx = 0.0f;                                   // 阴影X轴偏移 (像素)
    float shadowDy = 0.0f;                                   // 阴影Y轴偏移 (像素)
    float shadowSigma = 0.0f;                                // 阴影模糊半径 (像素)
    SkColor shadowColor = SK_ColorBLACK;                     // 阴影颜色
    TextDisplayMode displayMode = TextDisplayMode::WordWrap; // 文本显示模式
    bool hasExplicitDisplayMode = false;                     // 标记是否明确设置了displayMode（用于引擎选择）
    int maxLines = 0;                                        // 最大行数限制，0表示无限制
    bool ellipsis = false;                                   // 是否在截断时显示省略号"..."
};

// 富文本片段 - 定义同行多色文本中的单个文本段
struct RichTextSegment {
    std::string content;                    // 文本内容
    std::string fontFamily = "";            // 字体族名称，空字符串表示继承父级
    float fontSize = 0.0f;                  // 字体大小，0表示继承父级
    SkColor fillColor = SK_ColorTRANSPARENT; // 填充颜色，透明表示继承父级
    SkColor strokeColor = SK_ColorTRANSPARENT; // 描边颜色，透明表示继承父级  
    float strokeWidth = -1.0f;              // 描边宽度，-1表示继承父级
    bool hasShadow = false;                 // 是否启用阴影效果
    float shadowDx = 0.0f;                  // 阴影X轴偏移
    float shadowDy = 0.0f;                  // 阴影Y轴偏移
    float shadowSigma = 0.0f;               // 阴影模糊半径
    SkColor shadowColor = SK_ColorTRANSPARENT; // 阴影颜色，透明表示继承父级
};

// 富文本渲染策略枚举
enum class RichTextRenderStrategy {
    MeasureText,    // 使用measureText API进行精确测量和定位（默认）
    Paragraph       // 使用SkParagraph API进行高级富文本渲染
};

// 文本元素 - 定义要渲染的文本及其所有属性
struct TextElement {
    std::string id;         // 文本唯一标识符，用于调试和引用
    std::string content;    // 文本内容，支持中英文混合和Unicode字符
    Transform transform;    // 变换属性（位置、缩放、旋转、透明度）
    TextStyle style;        // 文本样式（字体、颜色、效果、布局）
    float width = 0.0f;     // 文本容器宽度 (像素)，0表示无宽度限制
    float height = 0.0f;    // 文本容器高度 (像素)，0表示无高度限制，AutoFit模式需要
    
    // 富文本支持
    std::vector<RichTextSegment> richTextSegments;  // 富文本片段数组，空数组表示使用普通文本
    RichTextRenderStrategy richTextStrategy = RichTextRenderStrategy::MeasureText; // 富文本渲染策略
    float letterSpacing = 0.0f;                     // 字间距 (像素)，仅在富文本模式下生效
    
    // 判断是否为富文本模式
    bool isRichText() const { return !richTextSegments.empty(); }
};

// 输出配置 - 定义渲染结果的保存格式和参数
struct OutputConfig {
    std::string format = "png";           // 输出格式："png"(无损)或"jpeg"(有损)
    std::string filename = "output.png";  // 输出文件名，包含扩展名
    int quality = 100;                    // 输出质量 (1-100)，仅对JPEG格式有效
};

// 完整渲染协议 - 包含一次完整渲染所需的所有配置和元素
struct RenderProtocol {
    CanvasConfig canvas;                  // 画布配置（尺寸、背景、调试）
    std::vector<ImageElement> images;     // 图片元素列表，按顺序渲染（后渲染的在上层）
    std::vector<TextElement> texts;       // 文本元素列表，按顺序渲染（后渲染的在上层）
    OutputConfig output;                  // 输出配置（格式、文件名、质量）
};

} // namespace skia_renderer 