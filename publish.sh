#!/bin/bash

# CMX Skills 发布脚本
# 将技能包发布到GitHub

set -e

echo "🚀 CMX Skills 发布脚本"
echo "======================"
echo ""

# 检查gh命令
if ! command -v gh &> /dev/null; then
    echo "❌ GitHub CLI (gh) 未安装"
    echo "请先安装: https://cli.github.com/"
    exit 1
fi

# 检查GitHub认证
echo "🔐 检查GitHub认证..."
if ! gh auth status &> /dev/null; then
    echo "❌ GitHub未认证"
    echo "请运行: gh auth login"
    exit 1
fi

echo "✅ GitHub已认证"

# 设置变量
REPO_NAME="cmx-skills"
REPO_OWNER="caomengxuan666"
REPO_DESC="🧬 我的技术DNA技能包 - 从14万行代码中蒸馏出的编程习惯、代码风格和工具配置"
REPO_URL="https://github.com/$REPO_OWNER/$REPO_NAME"

echo ""
echo "📦 发布信息:"
echo "  仓库: $REPO_OWNER/$REPO_NAME"
echo "  描述: $REPO_DESC"
echo ""

# 检查仓库是否已存在
echo "🔍 检查仓库状态..."
if gh repo view "$REPO_OWNER/$REPO_NAME" &> /dev/null; then
    echo "✅ 仓库已存在，将更新内容"
    UPDATE_EXISTING=true
else
    echo "🆕 创建新仓库"
    UPDATE_EXISTING=false
fi

# 创建临时目录
TEMP_DIR="/tmp/cmx-skills-publish-$(date +%s)"
mkdir -p "$TEMP_DIR"
cd "$TEMP_DIR"

echo ""
echo "📁 准备发布文件..."

# 复制所有文件
cp -r /tmp/cmx-skills-release/* .

# 创建.gitignore
cat > .gitignore << 'GITIGNORE_EOF'
# 构建文件
build/
dist/
*.o
*.so
*.dylib

# 临时文件
*.tmp
*.log
*.pid

# 环境文件
.env
.env.local
.env.*.local

# IDE文件
.vscode/
.idea/
*.swp
*.swo

# 系统文件
.DS_Store
Thumbs.db
GITIGNORE_EOF

# 初始化Git仓库
echo "🔄 初始化Git仓库..."
git init
git config user.name "caomengxuan666"
git config user.email "github@caomengxuan666.com"
git add .
git commit -m "🎉 发布CMX Skills v1.0.0

从14万行代码中蒸馏出的技术DNA技能包
包含：
- Google C++代码风格配置
- 完整工具链配置
- 开发习惯分析
- 性能优化策略

分析来源：
- WinuxCmd (75,579行)
- AstraDB (65,131行)
- 其他项目 (140,710行总计)"

echo "✅ 本地提交完成"

# 推送到GitHub
if [ "$UPDATE_EXISTING" = true ]; then
    echo "🔄 连接到现有仓库..."
    git remote add origin "$REPO_URL.git"
    git branch -M main
    git push -u origin main --force
    echo "✅ 仓库更新完成"
else
    echo "🚀 创建新GitHub仓库..."
    gh repo create "$REPO_NAME" \
        --description "$REPO_DESC" \
        --public \
        --source=. \
        --remote=origin \
        --push
    
    echo "✅ 新仓库创建完成"
fi

# 添加标签和发布
echo "🏷️ 创建版本标签..."
git tag -a "v1.0.0" -m "CMX Skills v1.0.0

第一个正式版本
基于140,710行代码分析
包含完整的技能包配置"
git push origin --tags

echo "📦 创建GitHub Release..."
gh release create "v1.0.0" \
    --title "CMX Skills v1.0.0 - 我的技术DNA" \
    --notes "$(cat << 'RELEASE_NOTES'
## 🎉 CMX Skills v1.0.0 发布！

### 这是什么？
这是我的技术DNA技能包，从14万行真实代码中蒸馏而来。

### 📊 分析基础
- **总代码量**: 140,710 行 C++ 代码
- **主要项目**: WinuxCmd, AstraDB, CMakeHub
- **分析维度**: 代码风格、工具配置、工作习惯、性能策略

### 🎯 核心功能
1. **Google C++代码风格配置** - 我的个性化版本
2. **完整工具链配置** - clang-format, clang-tidy, CMake
3. **开发习惯分析** - 基于我的实际工作模式
4. **性能优化策略** - 从高性能项目中学到的技巧

### 🚀 如何使用
```bash
# 安装到OpenClaw
openclaw skill install https://github.com/caomengxuan666/cmx-skills

# 或直接使用
git clone https://github.com/caomengxuan666/cmx-skills.git
```

### 📁 包含内容
- README.md - 有趣的介绍文档
- SKILL.md - 技能包完整说明
- developer-habits.md - 开发习惯深度分析
- toolchain-configs.md - 工具链完整配置
- config/ - 配置文件
- LICENSE - MIT许可证

### 🤝 贡献
欢迎提交Issue和Pull Request！所有贡献需要符合本技能包定义的代码标准。

---
**分析完成时间**: 2026-04-23  
**版本**: 1.0.0  
**许可证**: MIT  
**作者**: caomengxuan666
RELEASE_NOTES
)" \
    --latest

echo ""
echo "🎉 发布完成！"
echo ""
echo "📊 发布统计:"
echo "  文件数: $(find . -type f | wc -l)"
echo "  总大小: $(du -sh . | cut -f1)"
echo "  提交: 1 个初始提交"
echo "  标签: v1.0.0"
echo ""
echo "🌐 访问地址:"
echo "  GitHub: $REPO_URL"
echo "  Release: $REPO_URL/releases/tag/v1.0.0"
echo ""
echo "🔧 安装命令:"
echo "  openclaw skill install $REPO_URL"
echo ""
echo "📈 下一步:"
echo "  1. 分享给其他开发者"
echo "  2. 收集使用反馈"
echo "  3. 基于反馈持续改进"
echo "  4. 考虑添加到ClawHub技能市场"

# 清理
cd /
rm -rf "$TEMP_DIR"

echo ""
echo "✅ 所有发布任务完成！"