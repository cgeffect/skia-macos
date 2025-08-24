cmake_minimum_required(VERSION 3.26.1)

project(PosterMaker)
set(LIBRARY_NAME renderer)

# 设置默认构建类型为 Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# 设置构建类型选项
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "构建类型 (Debug/Release)" FORCE)
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")

# 根据构建类型设置编译选项和宏定义
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Release 模式：优化 + 禁用调试
    set(CMAKE_C_FLAGS_RELEASE   "-O3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    add_definitions(-DNDEBUG)
    message(STATUS "构建类型: Release (优化模式，禁用调试)")
else()
    # Debug 模式：调试信息 + 启用调试
    set(CMAKE_C_FLAGS_DEBUG   "-O0 -g")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
    add_definitions(-DDEBUG)
    message(STATUS "构建类型: Debug (调试模式，启用调试)")
endif()

# 关闭所有编译警告
# add_compile_options(-w)
# 为 C++ 源文件添加编译选项，抑制 -Wdeprecated-builtins 警告
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-builtins -Wno-deprecated-declarations")

set(CMAKE_SYSTEM_PROCESSOR "arm64")
set(CMAKE_OSX_ARCHITECTURES "arm64")
set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0" CACHE STRING "Minimum OS X deployment version")
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(libSRV_TRD_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
list(APPEND libSRV_INCLUDES_DIR ./)
list(APPEND libSRV_INCLUDES_DIR ${libSRV_TRD_ROOT_PATH}/skia/Skia-macOS-Release-arm64)
list(APPEND libSRV_INCLUDES_DIR ${libSRV_TRD_ROOT_PATH}/json/include)
list(APPEND libSRV_INCLUDES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE ABSL_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/skia/Skia-macOS-Release-arm64/out/Release-arm64/*.a)
# message(ABSL_LIBS ${ABSL_LIBS})

# 定义公共源文件变量（所有可执行文件都需要的核心源码）
# 避免在多个可执行文件中重复列出相同的源文件
file(GLOB_RECURSE COMMON_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/types.h                    # 核心数据结构
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/color_parser.cpp          # 颜色解析工具
        ${CMAKE_CURRENT_SOURCE_DIR}/src/parsers/protocol_parser.cpp     # JSON协议解析
        ${CMAKE_CURRENT_SOURCE_DIR}/src/resources/font_manager.cpp      # 字体管理
        ${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/canvas_renderer.cpp   # 画布渲染
        ${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/image_renderer.cpp    # 图片渲染
        ${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/text_layout.cpp       # 文本布局
        ${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/text_renderer.cpp     # 文本渲染
        ${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/rich_text_renderer.cpp # 富文本渲染
        ${CMAKE_CURRENT_SOURCE_DIR}/src/output/image_writer.cpp         # 图片输出
        ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/render_engine.cpp        # 渲染引擎
        )
# 在Xcode里面按照文件实际目录显示, 不要平铺
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${COMMON_SOURCE_FILES})

# finds all required platform libraries.
find_library(LZMA_FRAMEWORK lzma)
find_library(LZ_FRAMEWORK z)
find_library(LBZ2_FRAMEWORK bz2)
find_library(LICONV_FRAMEWORK iconv)
set(SYS_LIBS ${LZMA_FRAMEWORK} ${LZ_FRAMEWORK} ${LBZ2_FRAMEWORK} ${LICONV_FRAMEWORK})
find_library(COREFOUNDATION_FRAMEWORK CoreFoundation REQUIRED)
list(APPEND SYS_LIBS ${COREFOUNDATION_FRAMEWORK})
# libavfilter使用
find_library(COREGRAPHICS_FRAMEWORK CoreGraphics)
list(APPEND SYS_LIBS ${COREGRAPHICS_FRAMEWORK})
find_library(COREIMAGE_FRAMEWORK CoreImage)
list(APPEND SYS_LIBS ${COREIMAGE_FRAMEWORK})
find_library(Metal_FRAMEWORK Metal)
list(APPEND SYS_LIBS ${Metal_FRAMEWORK})
find_library(APP_FRAMEWORK AppKit)
list(APPEND SYS_LIBS ${APP_FRAMEWORK})
find_library(OpenGL_FRAMEWORK OpenGL)
list(APPEND SYS_LIBS ${OpenGL_FRAMEWORK})

# 在 add_library 里添加 allHEADER 是为了让xcode工程显示头文件
add_executable(${LIBRARY_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/examples/simple_example.cpp ${COMMON_SOURCE_FILES})
target_include_directories(${LIBRARY_NAME} PRIVATE ${libSRV_INCLUDES_DIR})
target_link_libraries(${LIBRARY_NAME} PRIVATE ${libs_trd_srv} ${ABSL_LIBS} ${SYS_LIBS})

# 简单测试可执行文件（独立运行，不修改现有源码）
add_executable(simple_test ${CMAKE_CURRENT_SOURCE_DIR}/tests/simple_image_test.cpp)
target_include_directories(simple_test PRIVATE ${libSRV_INCLUDES_DIR})
target_link_libraries(simple_test PRIVATE ${ABSL_LIBS} ${SYS_LIBS})

# 智能文本渲染器测试可执行文件
add_executable(smart_text_test ${CMAKE_CURRENT_SOURCE_DIR}/examples/simple_example.cpp ${COMMON_SOURCE_FILES})
target_include_directories(smart_text_test PRIVATE ${libSRV_INCLUDES_DIR})
target_link_libraries(smart_text_test PRIVATE ${ABSL_LIBS} ${SYS_LIBS})

# 文本样式演示可执行文件（使用公共源文件）
set(textStylesTestCPP ${CMAKE_CURRENT_SOURCE_DIR}/examples/text_styles_demo.cpp ${COMMON_SOURCE_FILES})
add_executable(text_styles_demo ${textStylesTestCPP})
target_include_directories(text_styles_demo PRIVATE ${libSRV_INCLUDES_DIR})
target_link_libraries(text_styles_demo PRIVATE ${ABSL_LIBS} ${SYS_LIBS})



