# 图片对比单元测试使用指南

## 📋 概述

这是一个简单的图片对比单元测试系统，用于验证渲染引擎的一致性和正确性。**不修改任何现有源码**，通过独立运行渲染程序来对比图片差异。

## 🚀 快速开始

### 1. 编译测试程序
```bash
./build.sh
```

### 2. 运行完整测试
```bash
./run_simple_test.sh
```

### 3. 查看测试结果
```bash
ls -la tests/diff/  # 查看差异图片
ls -la tests/baseline/  # 查看基线图片
```

## 🛠️ 详细使用方法

### 运行所有测试
```bash
# 使用脚本（推荐）
./run_simple_test.sh

# 直接运行程序
./build/simple_test run
```

### 一致性测试
验证同一协议多次渲染是否产生相同结果：
```bash
# 测试trip项目，迭代3次
./build/simple_test consistency trip 3

# 测试food项目，迭代5次（默认）
./build/simple_test consistency food
```

### 更新基线图片
当确认新结果正确时，更新基线：
```bash
# 更新单个项目
./build/simple_test update trip
./build/simple_test update food

# 更新前确保已运行渲染
./build/skia projects/trip/trip_protocol.json
./build/simple_test update trip
```

### 设置容差
调整图片差异的容忍度：
```bash
# 设置0.5%容差
./build/simple_test tolerance 0.005

# 设置2%容差
./build/simple_test tolerance 0.02
```

## 📊 测试项目

| 项目 | 协议文件 | 描述 |
|------|----------|------|
| trip | `projects/trip/trip_protocol.json` | 青海湖旅游攻略 |
| sunscreen | `projects/sunscreen/sunscreen_protocol.json` | 防晒海报 |
| food | `projects/food/food_protocol.json` | 美食海报 |
| spring | `projects/spring/spring_protocol.json` | 春天海报 |
| dessert | `projects/dessert/dessert_protocol.json` | 甜品海报 |
| cup | `projects/cup/cup_protocol.json` | 杯子海报 |
| horizontal | `projects/horizontal/horizontal_protocol.json` | 冬至海报 |
| long | `projects/long/long_protocol.json` | 蛋黄心长图 |
| clothes | `projects/clothes/clothes_protocol.json` | 服装海报 |

## 🔍 测试流程

### 首次运行
1. 系统自动创建基线图片
2. 所有测试标记为通过
3. 基线图片保存在 `tests/baseline/`

### 后续运行
1. 运行渲染程序生成当前图片
2. 与基线图片对比
3. 计算差异百分比
4. 生成差异图片（如有差异）

### 差异处理
- **差异 ≤ 容差**: 测试通过 ✅
- **差异 > 容差**: 测试失败 ❌，生成差异图片
- **哈希值相同**: 完全一致，跳过像素对比 ✅

## 📁 目录结构

```
tests/
├── baseline/           # 基线图片目录
│   ├── trip_baseline.png
│   ├── food_baseline.png
│   └── ...
├── diff/              # 差异图片目录
│   ├── trip_diff.png
│   ├── food_diff.png
│   └── ...
├── simple_image_test.cpp # 测试程序源码
└── README.md          # 详细文档

run_simple_test.sh     # 测试运行脚本
UNIT_TEST_GUIDE.md     # 本文档
```

## 💡 使用场景

### 1. 代码修改后验证
```bash
# 修改代码后运行测试
./run_simple_test.sh

# 检查是否有差异
ls -la tests/diff/
```

### 2. 验证渲染一致性
```bash
# 确保渲染结果是确定性的
./build/simple_test consistency trip 5
```

### 3. 更新基线
```bash
# 确认新结果正确后更新基线
./build/simple_test update trip
```

### 4. 调整容差
```bash
# 根据修改类型调整容差
./build/simple_test tolerance 0.005  # 小改动
./build/simple_test tolerance 0.02   # 大改动
```

## ⚠️ 常见问题

### 1. 测试程序编译失败
```bash
# 重新编译
./build.sh
```

### 2. 基线图片不存在
```bash
# 系统会自动创建基线
./build/simple_test run
```

### 3. 差异过大
- 检查代码修改
- 确认资源文件变化
- 调整容差设置

### 4. 一致性测试失败
- 检查随机数生成
- 确认时间相关代码
- 验证外部依赖

## 🎯 最佳实践

### 1. 开发流程
```bash
# 1. 修改代码
# 2. 运行测试
./run_simple_test.sh

# 3. 检查结果
ls -la tests/diff/

# 4. 确认无误后更新基线
./build/simple_test update <project_name>
```

### 2. 容差设置建议
- **小改动**（bug修复）: 0.1% - 0.5%
- **中等改动**（功能优化）: 0.5% - 1%
- **大改动**（算法变更）: 1% - 2%

### 3. 定期维护
- 定期运行一致性测试
- 及时更新基线图片
- 监控测试性能

## 📈 性能指标

- **测试耗时**: 约1.7秒（9个项目）
- **哈希值比较**: 毫秒级
- **像素级比较**: 秒级
- **内存使用**: 根据图片大小而定

## 🔧 扩展使用

### 添加新测试项目
1. 在 `tests/simple_image_test.cpp` 中添加项目：
```cpp
std::vector<std::pair<std::string, std::string>> tests = {
    // ... 现有项目
    {"new_project", "projects/new_project/new_project_protocol.json"}
};
```

2. 确保协议文件存在且可渲染
3. 运行测试验证

### 自定义测试逻辑
可以修改 `SimpleImageTest` 类：
- 调整容差计算方式
- 修改差异图片生成逻辑
- 添加新的比较算法

## 🎉 总结

这个单元测试系统提供了：

1. **✅ 简单易用**: 一键运行所有测试
2. **✅ 独立运行**: 不修改任何现有源码
3. **✅ 精确对比**: 哈希值 + 像素级双重验证
4. **✅ 可视化差异**: 生成差异图片
5. **✅ 灵活配置**: 可调节容差和测试参数

**让代码修改更安全，让渲染质量更可靠！** 🎨✨ 