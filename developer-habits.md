# CMX 开发习惯和最佳实践

基于对 WinuxCmd 和 AstraDB 项目的深度分析，总结的开发习惯、工具使用和工作流程。

## 📝 Git 工作流习惯

### 提交模式分析

#### 提交消息规范
```
提交类型分布:
├── feat:     新功能添加 (30%)
├── fix:      错误修复 (25%)  
├── refactor: 重构优化 (20%)
├── chore:    工具配置 (15%)
├── docs:     文档更新 (5%)
└── format:   代码格式化 (5%)
```

#### 具体提交示例
```bash
# 功能添加
feat: add FFI function to list all available commands

# 错误修复（带作用域）
fix(cat,console): stop early on broken pipe and stream passthrough

# 重构优化
refactor: remove daemon and IPC modules after FFI migration

# 工具配置
chore: add git protection to gitignore and disable warning 4129 in pch

# 代码格式化
format: format CMakePresets.json
```

#### 工作习惯特点
1. **小步提交**：每次提交解决一个具体问题
2. **规范消息**：使用 Conventional Commits 规范
3. **PR流程**：通过 Pull Request 进行代码审查
4. **版本管理**：明确的版本发布流程

## 🎨 代码风格配置

### Google C++ 风格实现

#### clang-format 配置
```yaml
# .clang-format 核心配置
BasedOnStyle: google
Language: Cpp
AccessModifierOffset: -1
AlignAfterOpenBracket: Align
ColumnLimit: 80
IndentWidth: 2
UseTab: Never
```

#### clang-tidy 配置
```yaml
Checks: |
  -*,  # 禁用所有，然后显式启用需要的
  clang-analyzer-core.*,
  clang-analyzer-cplusplus.*,
  modernize-redundant-void-arg,
  modernize-use-bool-literals,
  modernize-use-equals-default,
  modernize-use-nullptr,
  modernize-use-override,
  google-explicit-constructor,
  google-readability-casting,
  readability-braces-around-statements,
  readability-identifier-naming.ClassCase,
  readability-identifier-naming.StructCase
```

### 具体代码风格特点

#### 命名约定
```cpp
// 类名 - 大写开头
class Person {
    // 成员变量 - 小写，下划线分隔
    std::string first_name;
    std::string last_name;
    
    // 方法名 - 小写，下划线分隔
    void set_name(const std::string& name);
};

// 结构体 - 大写开头
struct Point {
    int x;
    int y;
};

// 枚举 - 大写，下划线分隔
enum class Color {
    RED,
    GREEN,
    BLUE
};
```

#### 头文件组织
```cpp
// 使用 #pragma once（现代方式）
#pragma once

// 包含顺序：系统头文件 -> 库头文件 -> 项目头文件
#include <vector>
#include <string>
#include <memory>

#include "base/common.h"
#include "utils/string_utils.h"

// 前向声明减少依赖
class SomeClass;
struct SomeStruct;
```

## 🔧 工具链集成

### 构建系统配置

#### CMake 最佳实践
```cmake
# 分层 CMakeLists.txt 结构
project(WinuxCmd VERSION 0.8.2 LANGUAGES CXX)

# 现代 CMake 特性
cmake_minimum_required(VERSION 3.20)

# 编译器特性检测
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 目标定义
add_library(commands STATIC ${SOURCES})
add_executable(winuxcmd ${MAIN_SOURCES})

# 依赖管理
target_link_libraries(winuxcmd PRIVATE commands)
```

#### 跨平台构建
```cmake
# Windows 特定配置
if(WIN32)
    target_compile_definitions(winuxcmd PRIVATE _WIN32_WINNT=0x0A00)
    target_link_libraries(winuxcmd PRIVATE Shell32.lib User32.lib)
endif()

# Linux 特定配置  
if(UNIX AND NOT APPLE)
    target_link_libraries(winuxcmd PRIVATE pthread)
endif()
```

### CI/CD 集成

#### GitHub Actions 配置
```yaml
name: CMake Windows Multi-Arch CI

on:
  push:
    branches: [ "main", "master" ]
    tags: [ "v*" ]
  pull_request:
    branches: [ "main", "master" ]

jobs:
  build-windows:
    runs-on: windows-latest
    strategy:
      matrix:
        include:
          - arch: x64
            vcvars_arg: x64
          - arch: arm64
            vcvars_arg: x64_arm64
    
    steps:
      - name: Checkout
        uses: actions/checkout@v4
      
      - name: Configure CMake
        run: cmake -B build -DCMAKE_BUILD_TYPE=Release
        
      - name: Build
        run: cmake --build build --config Release
```

### 开发工具集成

#### 预提交钩子
```powershell
#!/usr/bin/env pwsh
# scripts/hooks/pre-commit.ps1

Write-Host "=== Pre-commit hook started ===" -ForegroundColor Cyan

# 版本检查
$ver = (Get-Content PROJECT_VERSION -ErrorAction Stop).Trim()
Write-Host "Version from PROJECT_VERSION: $ver" -ForegroundColor Yellow

# 文件检查
$files = @(
    "scripts\create_links.ps1",
    "scripts\winuxcmd_init.cmd"
)

foreach ($f in $files) {
    if (-not (Test-Path $f)) {
        Write-Host "ERROR: $f not found!" -ForegroundColor Red
        exit 1
    }
}
```

#### 发布脚本
```powershell
<#
.SYNOPSIS
    WinuxCmd One-Click Release Script
.DESCRIPTION
    Updates version numbers in all files and pushes to GitHub
#>

param(
    [Parameter(Mandatory=$true)]
    [string]$Version,

    [Parameter(Mandatory=$false)]
    [string]$CommitMessage = "release: version $Version"
)

# 更新版本文件
$versionFiles = @("PROJECT_VERSION", "CMakeLists.txt")
foreach ($file in $versionFiles) {
    $content = Get-Content $file -Raw
    $content = $content -replace '\d+\.\d+\.\d+', $Version
    Set-Content $file $content
}

# 提交和推送
git add .
git commit -m $CommitMessage
git tag -a "v$Version" -m "Release version $Version"
git push origin main --tags
```

## 🚀 开发工作流程

### 日常开发流程

#### 1. 代码编写阶段
```bash
# 启用预提交钩子
git config core.hooksPath scripts/hooks

# 代码格式化（开发中随时运行）
clang-format -i src/**/*.cpp src/**/*.h

# 静态分析
clang-tidy src/**/*.cpp --checks=*
```

#### 2. 提交阶段
```bash
# 小步提交，每次解决一个问题
git add src/commands/new_feature.cpp
git commit -m "feat: add new feature implementation"

# 或者修复特定问题
git commit -m "fix(parser): handle edge case in input validation"
```

#### 3. 代码审查阶段
```bash
# 创建功能分支
git checkout -b feature/new-feature

# 开发完成后创建PR
git push origin feature/new-feature

# PR合并后清理
git checkout main
git pull origin main
git branch -d feature/new-feature
```

#### 4. 发布阶段
```powershell
# 使用发布脚本
.\scripts\release.ps1 -Version "1.0.0" -CommitMessage "release: version 1.0.0"
```

### 质量保证流程

#### 代码质量检查
```bash
# 1. 代码格式化检查
clang-format --dry-run --Werror src/**/*.cpp

# 2. 静态分析
clang-tidy src/**/*.cpp --checks=modernize-*,readability-*

# 3. 编译检查
cmake --build build --config Debug

# 4. 单元测试
ctest --test-dir build --output-on-failure
```

#### 性能优化流程
```bash
# 1. 基准测试建立
./build/benchmark --benchmark_out=results.json

# 2. 性能分析
perf record ./build/winuxcmd --some-command
perf report

# 3. 内存分析
valgrind --tool=massif ./build/winuxcmd
ms_print massif.out.*
```

## 📊 项目管理习惯

### 项目结构组织
```
项目标准结构:
├── src/                    # 源代码
│   ├── core/              # 核心模块
│   ├── commands/          # 功能模块
│   ├── utils/             # 工具函数
│   └── ffi/               # 外部接口
├── tests/                 # 测试代码
│   ├── unit/             # 单元测试
│   └── integration/      # 集成测试
├── examples/              # 使用示例
├── scripts/               # 构建和工具脚本
│   ├── hooks/            # Git钩子
│   └── release.ps1       # 发布脚本
├── .github/              # CI/CD配置
│   └── workflows/
├── CMakeLists.txt        # 构建配置
├── .clang-format         # 代码风格
├── .clang-tidy           # 静态分析
└── README.md             # 项目文档
```

### 文档编写习惯

#### README 结构
```markdown
# 项目名称

## 特性
- 跨平台支持
- 高性能设计
- 现代C++实现

## 快速开始
```bash
# 构建指令
cmake -B build
cmake --build build
```

## API文档
详细的使用示例和API说明

## 贡献指南
代码风格、提交规范、测试要求
```

#### 代码注释规范
```cpp
/**
 * @brief 类的简要描述
 * 
 * 详细的功能说明和使用示例
 * 
 * @tparam T 模板参数说明
 */
template<typename T>
class Container {
public:
    /**
     * @brief 方法的简要描述
     * 
     * @param value 参数说明
     * @return 返回值说明
     * @throws 异常说明
     */
    void add(const T& value);
};
```

## 🔄 持续改进习惯

### 技术债务管理
```bash
# 定期代码审查
# 1. 检查未使用的代码
clang-tidy --checks=clang-analyzer-deadcode.DeadStores

# 2. 检查性能问题
clang-tidy --checks=performance-*

# 3. 检查现代C++特性使用
clang-tidy --checks=modernize-*
```

### 依赖更新策略
```bash
# 定期检查依赖更新
# 1. vcpkg包管理
vcpkg update
vcpkg upgrade --no-dry-run

# 2. 编译器版本更新
# 定期升级到最新的稳定版编译器
```

### 知识分享习惯
```bash
# 1. 代码审查记录
# 记录重要的设计决策和问题解决

# 2. 技术文档更新
# 随着代码变更更新相关文档

# 3. 示例代码维护
# 保持示例代码与最新API同步
```

## 🎯 最佳实践总结

### 核心原则
1. **代码质量优先**：格式化、静态分析、测试覆盖
2. **小步迭代**：频繁提交，每次解决一个问题
3. **自动化一切**：构建、测试、发布自动化
4. **文档伴随**：代码变更时同步更新文档

### 工具使用原则
1. **标准化配置**：团队统一的工具配置
2. **版本控制**：所有配置纳入版本控制
3. **可重复构建**：在任何环境都能一致构建
4. **持续集成**：每次提交都运行完整检查

### 协作原则
1. **明确接口**：模块间清晰的接口定义
2. **代码审查**：所有变更经过同行审查
3. **知识共享**：重要的设计决策文档化
4. **持续学习**：关注新技术和最佳实践

---

**最后更新**: 2026-04-23  
**分析来源**: WinuxCmd, AstraDB 项目深度分析  
**适用场景**: 高性能C++系统开发项目