cmake_minimum_required(VERSION 3.26.1)
message("--begin avmedia complie---")

project(skia)
set(LIBRARY_NAME skia)
# 关闭所有编译警告
# add_compile_options(-w)
# 为 C++ 源文件添加编译选项，抑制 -Wdeprecated-builtins 警告
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-builtins -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS_DEBUG   "-O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
message(STATUS "Debug build avmedia")

set(CMAKE_SYSTEM_PROCESSOR "arm64")
set(CMAKE_OSX_ARCHITECTURES "arm64")
set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0" CACHE STRING "Minimum OS X deployment version")
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(libSRV_TRD_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
list(APPEND libSRV_INCLUDES_DIR ./)
list(APPEND libSRV_INCLUDES_DIR ${libSRV_TRD_ROOT_PATH}/skia/Skia-macOS-Release-arm64)

# list(APPEND libs_trd_srv ${libSRV_TRD_ROOT_PATH}/skia/deploy/lib/libskia.a)

file(GLOB_RECURSE ABSL_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/skia/Skia-macOS-Release-arm64/out/Release-arm64/*.a)
message(ABSL_LIBS ${ABSL_LIBS})
file(GLOB_RECURSE allCPP 
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp        
        )
# 在Xcode里面按照文件实际目录显示, 不要平铺
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${allCPP})

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
add_executable(${LIBRARY_NAME} ${allCPP})
target_include_directories(${LIBRARY_NAME} PRIVATE ${libSRV_INCLUDES_DIR})
target_link_libraries(${LIBRARY_NAME} PRIVATE ${libs_trd_srv} ${ABSL_LIBS} ${SYS_LIBS})

message("--end avmedia complie---")