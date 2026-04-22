# CMX 工具链配置和集成指南

基于对 WinuxCmd 和 AstraDB 项目的深度分析，总结的完整工具链配置和集成方案。

## 🛠️ 完整工具链配置

### 开发环境配置

#### 基础开发环境
```bash
# 操作系统: Windows 11 / Ubuntu 22.04
# 编译器: Clang 19.1 / MSVC 2022
# 构建系统: CMake 3.28+
# 包管理: vcpkg

# 环境检查脚本
#!/bin/bash
echo "=== 开发环境检查 ==="

# 编译器检查
if command -v clang++ &> /dev/null; then
    clang++ --version | head -1
else
    echo "❌ Clang 未安装"
fi

# CMake检查
if command -v cmake &> /dev/null; then
    cmake --version | head -1
else
    echo "❌ CMake 未安装"
fi

# vcpkg检查
if [ -d "/usr/local/vcpkg" ] || [ -d "$HOME/vcpkg" ]; then
    echo "✅ vcpkg 已安装"
else
    echo "⚠️  vcpkg 未安装"
fi
```

#### IDE 配置
```json
// VS Code 配置 (.vscode/settings.json)
{
    "C_Cpp.default.compilerPath": "clang++",
    "C_Cpp.default.cppStandard": "c++20",
    "C_Cpp.default.intelliSenseMode": "clang-x64",
    
    "editor.formatOnSave": true,
    "editor.codeActionsOnSave": {
        "source.fixAll": "always"
    },
    
    "clangd.path": "clangd",
    "clangd.arguments": [
        "--background-index",
        "--clang-tidy",
        "--completion-style=detailed"
    ],
    
    "cmake.configureArgs": [
        "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
    ]
}
```

### 代码质量工具链

#### clang-format 完整配置
```yaml
# .clang-format (完整配置)
---
Language:        Cpp
BasedOnStyle:    google
AccessModifierOffset: -1
AlignAfterOpenBracket: Align
AlignConsecutiveAssignments:
  Enabled:         false
AlignConsecutiveBitFields:
  Enabled:         false
AlignConsecutiveDeclarations:
  Enabled:         false
AlignEscapedNewlines: Left
AlignOperands:   Align
AlignTrailingComments: true
AllowAllArgumentsOnNextLine: false
AllowAllConstructorInitializersOnNextLine: false
AllowAllParametersOfDeclarationOnNextLine: false
AllowShortBlocksOnASingleLine: Never
AllowShortCaseLabelsOnASingleLine: false
AllowShortFunctionsOnASingleLine: All
AllowShortIfStatementsOnASingleLine: Never
AllowShortLambdasOnASingleLine: All
AllowShortLoopsOnASingleLine: false
AlwaysBreakAfterDefinitionReturnType: None
AlwaysBreakAfterReturnType: None
AlwaysBreakBeforeMultilineStrings: false
AlwaysBreakTemplateDeclarations: MultiLine
BinPackArguments: true
BinPackParameters: true
BraceWrapping:
  AfterCaseLabel:  false
  AfterClass:      false
  AfterControlStatement: Never
  AfterEnum:       false
  AfterFunction:   false
  AfterNamespace:  false
  AfterObjCDeclaration: false
  AfterStruct:     false
  AfterUnion:      false
  AfterExternBlock: false
  BeforeCatch:     false
  BeforeElse:      false
  BeforeLambdaBody: false
  BeforeWhile:     false
  IndentBraces:    false
  SplitEmptyFunction: false
  SplitEmptyRecord: false
  SplitEmptyNamespace: false
BreakBeforeBinaryOperators: None
BreakBeforeBraces: Attach
BreakBeforeInheritanceComma: false
BreakInheritanceList: BeforeColon
BreakBeforeTernaryOperators: true
BreakConstructorInitializers: BeforeColon
BreakConstructorInitializersBeforeComma: false
BreakAfterJavaFieldAnnotations: false
BreakStringLiterals: true
ColumnLimit:     80
CommentPragmas:  '^ IWYU pragma:'
CompactNamespaces: false
ConstructorInitializerAllOnOneLineOrOnePerLine: false
ConstructorInitializerIndentWidth: 4
ContinuationIndentWidth: 4
Cpp11BracedListStyle: true
DeriveLineEnding: true
DerivePointerAlignment: false
DisableFormat:   false
EmptyLineAfterAccessModifier: Never
EmptyLineBeforeAccessModifier: LogicalBlock
ExperimentalAutoDetectBinPacking: false
FixNamespaceComments: true
ForEachMacros:
  - foreach
  - Q_FOREACH
  - BOOST_FOREACH
IncludeBlocks:   Preserve
IncludeCategories:
  - Regex:           '^"(llvm|llvm-c|clang|clang-c)/'
    Priority:        2
  - Regex:           '^(<|"(gtest|gmock|isl|json)/)'
    Priority:        3
  - Regex:           '.*'
    Priority:        1
IncludeIsMainRegex: '(Test)?$'
IncludeIsMainSourceRegex: ''
IndentCaseLabels: false
IndentCaseBlocks: false
IndentGotoLabels: false
IndentPPDirectives: None
IndentExternBlock: AfterExternBlock
IndentWidth:     2
IndentWrappedFunctionNames: false
JavaScriptQuotes: Leave
JavaScriptWrapImports: true
KeepEmptyLinesAtTheStartOfBlocks: false
LambdaBodyIndentation: Signature
MacroBlockBegin: ''
MacroBlockEnd:   ''
MaxEmptyLinesToKeep: 1
NamespaceIndentation: None
ObjCBinPackProtocolList: Auto
ObjCBlockIndentWidth: 2
ObjCSpaceAfterProperty: false
ObjCSpaceBeforeProtocolList: true
PenaltyBreakAssignment: 2
PenaltyBreakBeforeFirstCallParameter: 19
PenaltyBreakComment: 300
PenaltyBreakFirstLessLess: 120
PenaltyBreakString: 1000
PenaltyBreakTemplateDeclaration: 10
PenaltyExcessCharacter: 1000000
PenaltyReturnTypeOnItsOwnLine: 60
PenaltyIndentedWhitespace: 0
PointerAlignment: Left
QualifierAlignment: Leave
ReferenceAlignment: Pointer
ReflowComments:  true
RemoveBracesLLVM: false
RemoveParentheses: Leave
RemoveSemicolon: false
RequiresClausePosition: OwnLine
SeparateDefinitionBlocks: Leave
ShortNamespaceLines: 1
SortIncludes:    CaseSensitive
SortJavaStaticImport: Before
SortUsingDeclarations: true
SpaceAfterCStyleCast: false
SpaceAfterLogicalNot: false
SpaceAfterTemplateKeyword: true
SpaceBeforeAssignmentOperators: true
SpaceBeforeCaseColon: false
SpaceBeforeCpp11BracedList: false
SpaceBeforeCtorInitializerColon: true
SpaceBeforeInheritanceColon: true
SpaceBeforeParens: ControlStatements
SpaceBeforeRangeBasedForLoopColon: true
SpaceBeforeSquareBrackets: false
SpaceInEmptyBlock: false
SpaceInEmptyParentheses: false
SpacesBeforeTrailingComments: 2
SpacesInAngles:  Never
SpacesInConditionalStatement: false
SpacesInContainerLiterals: true
SpacesInCStyleCastParentheses: false
SpacesInLineCommentPrefix:
  Minimum:         1
  Maximum:         -1
SpacesInParentheses: false
SpacesInSquareBrackets: false
Standard:        Latest
StatementAttributeLikeMacros:
  - Q_EMIT
StatementMacros:
  - Q_UNUSED
  - QT_REQUIRE_VERSION
TabWidth:        8
UseCRLF:         false
UseTab:          Never
WhitespaceSensitiveMacros:
  - STRINGIZE
  - PP_STRINGIZE
  - BOOST_PP_STRINGIZE
  - NS_SWIFT_NAME
  - CF_SWIFT_NAME
```

#### clang-tidy 完整配置
```yaml
# .clang-tidy (完整配置)
---
Checks: >
  -*,  # 禁用所有检查，然后显式启用需要的
  
  # 现代C++特性
  modernize-*,
  
  # Google风格检查
  google-*,
  
  # 可读性检查
  readability-*,
  
  # 性能检查
  performance-*,
  
  # bug检查
  bugprone-*,
  
  # 并发检查
  concurrency-*,
  
  # 安全性检查
  cert-*,
  cppcoreguidelines-*,
  
  # Clang静态分析器
  clang-analyzer-*,
  
  # 其他有用的检查
  misc-*,
  hicpp-*

# 检查选项
CheckOptions:
  - key:   modernize-use-nodiscard.CheckMacros
    value: 'DEBUG_ASSERT'
  - key:   readability-identifier-naming.ClassCase
    value: CamelCase
  - key:   readability-identifier-naming.StructCase
    value: CamelCase
  - key:   readability-identifier-naming.EnumCase
    value: CamelCase
  - key:   readability-identifier-naming.TypedefCase
    value: CamelCase
  - key:   readability-identifier-naming.MemberCase
    value: lower_case
  - key:   readability-identifier-naming.VariableCase
    value: lower_case
  - key:   readability-identifier-naming.ParameterCase
    value: lower_case
  - key:   readability-identifier-naming.ConstantCase
    value: UPPER_CASE
  - key:   readability-identifier-naming.GlobalConstantCase
    value: UPPER_CASE
  - key:   readability-identifier-naming.FunctionCase
    value: lower_case
  - key:   readability-identifier-naming.MethodCase
    value: lower_case
  - key:   readability-identifier-naming.NamespaceCase
    value: lower_case

# 警告为错误
WarningsAsErrors: '*'

# 头文件过滤器
HeaderFilterRegex: ''

# 格式检查
FormatStyle: file  # 使用.clang-format文件

# 系统头文件检查
SystemHeaders: false

# 内联命名空间检查
CheckInlinedNamespaceMembers: false
```

### 构建系统配置

#### CMake 预设配置
```json
// CMakePresets.json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "default",
      "hidden": true,
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build/${presetName}",
      "cacheVariables": {
        "CMAKE_C_COMPILER": "clang",
        "CMAKE_CXX_COMPILER": "clang++",
        "CMAKE_CXX_STANDARD": "20",
        "CMAKE_CXX_STANDARD_REQUIRED": "ON",
        "CMAKE_CXX_EXTENSIONS": "OFF",
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON",
        "CMAKE_BUILD_TYPE": "Debug"
      }
    },
    {
      "name": "debug",
      "inherits": "default",
      "displayName": "Debug Configuration",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_CXX_FLAGS": "-g -O0 -fsanitize=address,undefined"
      }
    },
    {
      "name": "release",
      "inherits": "default",
      "displayName": "Release Configuration",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release",
        "CMAKE_CXX_FLAGS": "-O3 -DNDEBUG -march=native"
      }
    },
    {
      "name": "coverage",
      "inherits": "debug",
      "displayName": "Coverage Configuration",
      "cacheVariables": {
        "CMAKE_CXX_FLAGS": "--coverage",
        "ENABLE_COVERAGE": "ON"
      }
    }
  ],
  "buildPresets": [
    {
      "name": "debug",
      "configurePreset": "debug",
      "jobs": 8
    },
    {
      "name": "release", 
      "configurePreset": "release",
      "jobs": 8
    }
  ],
  "testPresets": [
    {
      "name": "default",
      "configurePreset": "debug",
      "output": {"outputOnFailure": true},
      "execution": {"noTestsAction": "error", "stopOnFailure": false}
    }
  ]
}
```

#### vcpkg 集成配置
```cmake
# vcpkg集成示例
# CMakeLists.txt 中的配置

# 设置vcpkg工具链
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

# 查找包
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(rocksdb CONFIG REQUIRED)

# 使用包
target_link_libraries(my_target PRIVATE
    fmt::fmt
    spdlog::spdlog
    Asio::asio
    rocksdb::rocksdb
)
```

### 自动化脚本

#### 项目初始化脚本
```bash
#!/bin/bash
# scripts/init-project.sh

set -e

echo "=== 项目初始化脚本 ==="

# 参数检查
if [ $# -lt 1 ]; then
    echo "用法: $0 <项目名称>"
    exit 1
fi

PROJECT_NAME=$1
PROJECT_DIR=$(pwd)/$PROJECT_NAME

echo "创建项目: $PROJECT_NAME"
echo "目录: $PROJECT_DIR"

# 创建目录结构
mkdir -p $PROJECT_DIR/{src,include,tests,examples,scripts,.github/workflows}
mkdir -p $PROJECT_DIR/src/{core,utils,commands}

# 复制配置文件
cp templates/.clang-format $PROJECT_DIR/
cp templates/.clang-tidy $PROJECT_DIR/
cp templates/.gitignore $PROJECT_DIR/

# 创建CMakeLists.txt
cat > $PROJECT_DIR/CMakeLists.txt << EOF
cmake_minimum_required(VERSION 3.20)
project($PROJECT_NAME VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译器选项
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(
        -Wall
        -Wextra
        -Werror
        -pedantic
    )
endif()

# 添加可执行文件
add_executable($PROJECT_NAME src/main.cpp)

# 安装目标
install(TARGETS $PROJECT_NAME DESTINATION bin)
EOF

# 创建主文件
cat > $PROJECT_DIR/src/main.cpp << EOF
#include <iostream>

int main() {
    std::cout << "Hello, $PROJECT_NAME!" << std::endl;
    return 0;
}
EOF

# 创建README
cat > $PROJECT_DIR/README.md << EOF
# $PROJECT_NAME

## 构建说明

\`\`\`bash
# 配置
cmake -B build

# 构建
cmake --build build

# 运行
./build/$PROJECT_NAME
\`\`\`

## 开发说明

- 代码风格: Google C++ Style
- 构建系统: CMake
- 包管理: vcpkg
EOF

echo "✅ 项目初始化完成"
echo "📁 项目结构:"
tree $PROJECT_DIR -L 2
```

#### 代码质量检查脚本
```bash
#!/bin/bash
# scripts/check-code-quality.sh

set -e

echo "=== 代码质量检查 ==="

# 1. 代码格式化检查
echo "🔍 检查代码格式化..."
if ! clang-format --dry-run --Werror src/**/*.cpp include/**/*.h 2>/dev/null; then
    echo "❌ 代码格式化检查失败"
    echo "运行以下命令修复:"
    echo "  clang-format -i src/**/*.cpp include/**/*.h"
    exit 1
fi
echo "✅ 代码格式化检查通过"

# 2. 静态分析检查
echo "🔍 运行静态分析..."
if ! clang-tidy src/**/*.cpp --checks=* --warnings-as-errors=* 2>/dev/null; then
    echo "❌ 静态分析检查失败"
    exit 1
fi
echo "✅ 静态分析检查通过"

# 3. 编译检查
echo "🔍 检查编译..."
if ! cmake --build build --config Debug 2>/dev/null; then
    echo "❌ 编译检查失败"
    exit 1
fi
echo "✅ 编译检查通过"

# 4. 测试检查
echo "🔍 运行测试..."
if ! ctest --test-dir build --output-on-failure 2>/dev/null; then
    echo "❌ 测试检查失败"
    exit 1
fi
echo "✅ 测试检查通过"

echo "🎉 所有代码质量检查通过！"
```

### CI/CD 配置

#### GitHub Actions 完整配置
```yaml
# .github/workflows/ci.yml
name: CI

on:
  push:
    branches: [ main, develop ]
    tags: [ 'v*' ]
  pull_request:
    branches: [ main ]

jobs:
  format-check:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      
      - name: Check code formatting
        run: |
          find . -name '*.cpp' -o -name '*.h' | xargs clang-format --dry-run --Werror

  static-analysis:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      
      - name: Install clang-tidy
        run: sudo apt-get install -y clang-tidy
        
      - name: Run static analysis
        run: |
          find . -name '*.cpp' | xargs clang-tidy --checks=* --warnings-as-errors=*

  build-linux:
    runs-on: ubuntu-latest
    strategy:
      matrix:
        build_type: [Debug, Release]
    
    steps:
      - uses: actions/checkout@v4
      
      - name: Configure CMake
        run: cmake -B build -DCMAKE_BUILD_TYPE=${{ matrix.build_type }}
        
      - name: Build
        run: cmake --build build --config ${{ matrix.build_type }}
        
      - name: Test
        run: ctest --test-dir build --output-on-failure

  build-windows:
    runs-on: windows-latest
    strategy:
      matrix:
        arch: [x64, arm64]
    
    steps:
      - uses: actions/checkout@v4
      
      - name: Configure CMake
        run: |
          cmake -B build -A ${{ matrix.arch }}
          
      - name: Build
        run: cmake --build build --config Release
        
      - name: Upload artifacts
        uses: actions/upload-artifact@v4
        with:
          name: winuxcmd-${{ matrix.arch }}
          path: build/Release/winuxcmd.exe

  release:
    needs: [format-check, static-analysis, build-linux, build-windows]
    if: startsWith(github.ref, 'refs/tags/v')
    runs-on: ubuntu-latest
    
    steps:
      - uses: actions/checkout@v4
      
      - name: Create Release
        uses: softprops/action-gh-release@v1
        with:
          files: |
            build/winuxcmd
            build/Release/winuxcmd.exe
          generate_release_notes: true
```

### 性能分析工具链

#### 性能分析脚本
```bash
#!/bin/bash
# scripts/performance-analysis.sh

set -e

echo "=== 性能分析 ==="

# 1. 编译性能版本
echo "🔧 编译性能版本..."
cmake -B build-perf -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build-perf --config RelWithDebInfo

# 2. 运行基准测试
echo "📊 运行基准测试..."
if [ -f "build-perf/benchmarks" ]; then
    ./build-perf/benchmarks --benchmark_out=benchmark_results.json
fi

# 3. CPU性能分析
echo "⚡ CPU性能分析..."
if command -v perf &> /dev/null; then
    perf record -g ./build-perf/winuxcmd --benchmark
    perf report --stdio > perf_report.txt
fi

# 4. 内存分析
echo "💾 内存分析..."
if command -v valgrind &> /dev/null; then
    valgrind --tool=massif ./build-perf/winuxcmd --benchmark
    ms_print massif.out.* > massif_report.txt
fi

# 5. 缓存分析
echo "🔍 缓存分析..."
if command -v cachegrind &> /dev/null; then
    valgrind --tool=cachegrind ./build-perf/winuxcmd --benchmark
    cg_annotate cachegrind.out.* > cachegrind_report.txt
fi

echo "✅ 性能分析完成"
echo "📁 分析报告:"
ls -la *_report.txt benchmark_results.json 2>/dev/null || true
```

### 文档生成工具链

#### Doxygen 配置
```doxygen
# Doxyfile
PROJECT_NAME           = "WinuxCmd"
PROJECT_NUMBER         = 0.8.2
PROJECT_BRIEF          = "Windows上的Linux命令实现"

OUTPUT_DIRECTORY       = docs
CREATE_SUBDIRS         = YES
ALLOW_UNICODE_NAMES    = YES
OUTPUT_LANGUAGE        = Chinese
BRIEF_MEMBER_DESC      = YES
REPEAT_BRIEF           = YES
ABBREVIATE_BRIEF       = "The $name class" \
                         "The $name widget" \
                         "The $name file" \
                         is \
                         provides \
                         specifies \
                         contains \
                         represents \
                         a \
                         an \
                         the
ALWAYS_DETAILED_SEC    = NO
INLINE_INHERITED_MEMB  = NO
FULL_PATH_NAMES        = YES
STRIP_FROM_PATH        = 
STRIP_FROM_INC_PATH    = 
SHORT_NAMES            = NO
JAVADOC_AUTOBRIEF      = YES
QT_AUTOBRIEF           = NO
MULTILINE_CPP_IS_BRIEF = NO
INHERIT_DOCS           = YES
SEPARATE_MEMBER_PAGES  = NO
TAB_SIZE               = 2
ALIASES                = 
OPTIMIZE_OUTPUT_FOR_C  = NO
OPTIMIZE_OUTPUT_JAVA   = NO
OPTIMIZE_FOR_FORTRAN   = NO
OPTIMIZE_OUTPUT_VHDL   = NO
EXTENSION_MAPPING      = 
MARKDOWN_SUPPORT       = YES
TOC_INCLUDE_HEADINGS   = 5
AUTOLINK_SUPPORT       = YES
BUILTIN_STL_SUPPORT    = YES
CPP_CLI_SUPPORT        = NO
SIP_SUPPORT            = NO
IDL_PROPERTY_SUPPORT   = YES
DISTRIBUTE_GROUP_DOC   = NO
GROUP_NESTED_COMPOUNDS = NO
SUBGROUPING            = YES
INLINE_GROUPED_CLASSES = NO
INLINE_SIMPLE_STRUCTS  = NO
TYPEDEF_HIDES_STRUCT   = NO
LOOKUP_CACHE_SIZE      = 0

# 输入配置
INPUT                  = src include
FILE_PATTERNS          = *.cpp *.h *.hpp
RECURSIVE              = YES
EXCLUDE                = 
EXCLUDE_SYMLINKS       = NO
EXCLUDE_PATTERNS       = 
EXCLUDE_SYMBOLS        = 
EXAMPLE_PATH           = examples
EXAMPLE_PATTERNS       = *.cpp
EXAMPLE_RECURSIVE      = NO
IMAGE_PATH             = 
INPUT_FILTER           = 
FILTER_PATTERNS        = 
FILTER_SOURCE_FILES    = NO
FILTER_SOURCE_PATTERNS = 
USE_MDFILE_AS_MAINPAGE = README.md

# 源代码浏览
SOURCE_BROWSER         = YES
INLINE_SOURCES         = NO
STRIP_CODE_COMMENTS    = YES
REFERENCED_BY_RELATION = YES
REFERENCES_RELATION    = YES
REFERENCES_LINK_SOURCE = YES
SOURCE_TOOLTIPS        = YES
USE_HTAGS              = NO
VERBATIM_HEADERS       = YES

# 输出配置
GENERATE_HTML          = YES
GENERATE_LATEX         = NO
GENERATE_RTF           = NO
GENERATE_MAN           = NO
GENERATE_DOCBOOK       = NO
GENERATE_AUTOGEN_DEF   = NO
GENERATE_PERLMOD       = NO
ENABLE_PREPROCESSING   = YES
MACRO_EXPANSION        = YES
EXPAND_ONLY_PREDEF     = NO
SEARCH_INCLUDES        = YES
INCLUDE_PATH           = 
INCLUDE_FILE_PATTERNS  = 
PREDEFINED             = 
EXPAND_AS_DEFINED      = 
SKIP_FUNCTION_MACROS   = YES

# 索引配置
ALPHABETICAL_INDEX     = YES
COLS_IN_ALPHA_INDEX    = 5
IGNORE_PREFIX          = 
GENERATE_TREEVIEW      = YES
FULL_SIDEBAR           = NO
ENUM_VALUES_PER_LINE   = 4
TREEVIEW_WIDTH         = 250
EXT_LINKS_IN_WINDOW    = NO
FORMULA_FONTSIZE       = 10
FORMULA_TRANSPARENT    = YES
USE_MATHJAX            = NO
MATHJAX_FORMAT         = HTML-CSS
MATHJAX_RELPATH        = 
MATHJAX_EXTENSIONS     = 
MATHJAX_CODEFILE       = 
SEARCHENGINE           = YES
SERVER_BASED_SEARCH    = NO
EXTERNAL_SEARCH        = NO
SEARCHDATA_FILE        = searchdata.xml
EXTERNAL_SEARCH_ID     = 
EXTRA_SEARCH_MAPPINGS  = 
```

## 🎯 工具链使用指南

### 日常开发流程
```bash
# 1. 初始化项目
./scripts/init-project.sh my-project

# 2. 日常开发
# 代码编写 -> 自动格式化 -> 静态分析

# 3. 提交前检查
./scripts/check-code-quality.sh

# 4. 性能分析（需要时）
./scripts/performance-analysis.sh

# 5. 文档生成
doxygen Doxyfile
```

### 团队协作配置
```bash
# 1. 统一开发环境
# 使用相同的编译器版本和工具版本

# 2. 共享配置
# 将.clang-format、.clang-tidy等纳入版本控制

# 3. 预提交钩子
git config core.hooksPath scripts/hooks

# 4. CI/CD配置
# 使用相同的GitHub Actions配置
```

### 故障排除
```bash
# 常见问题解决

# 1. 编译错误
# 检查编译器版本和C++标准设置

# 2. 链接错误  
# 检查依赖库路径和链接顺序

# 3. 性能问题
# 使用性能分析工具定位瓶颈

# 4. 代码风格问题
# 运行clang-format修复格式问题
```

---

**最后更新**: 2026-04-23  
**适用项目**: 高性能C++系统开发  
**工具版本**: Clang 19.1, CMake 3.28, vcpkg 2024.04.15