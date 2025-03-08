# hjson JSON Parser Makefile
# 这是一个简化的Makefile，实际构建使用CMake

.PHONY: all build clean test benchmark debug release help install

# 默认目标
all: build

# 构建项目 (Release模式)
build:
	@echo "🔨 构建 hjson (Release模式)..."
	@./build.sh

# Debug构建
debug:
	@echo "🔨 构建 hjson (Debug模式)..."
	@./build.sh --debug --tests

# Release构建
release:
	@echo "🔨 构建 hjson (Release模式)..."
	@./build.sh --no-tests

# 清理构建
clean:
	@echo "🧹 清理构建目录..."
	@rm -rf build/
	@echo "✅ 清理完成!"

# 构建并运行测试
test:
	@echo "🧪 构建并运行测试..."
	@./build.sh --debug --tests

# 构建基准测试
benchmark:
	@echo "📊 构建基准测试..."
	@./build.sh --benchmarks

# 完整构建 (包含所有组件)
full:
	@echo "🔨 完整构建 (包含测试、基准测试、示例)..."
	@./build.sh --tests --benchmarks --examples

# 快速测试 (仅构建和运行测试)
quick-test:
	@echo "⚡ 快速测试..."
	@./build.sh --debug --tests --no-examples

# 安装 (需要sudo权限)
install: build
	@echo "📦 安装 hjson..."
	@cd build && sudo cmake --install .
	@echo "✅ 安装完成!"

# 格式化代码 (如果有clang-format)
format:
	@if command -v clang-format >/dev/null 2>&1; then \
		echo "🎨 格式化代码..."; \
		find . -name "*.h" -o -name "*.cc" -o -name "*.cpp" | grep -v third_party | xargs clang-format -i; \
		echo "✅ 代码格式化完成!"; \
	else \
		echo "❌ clang-format 未安装"; \
	fi

# 静态分析 (如果有cppcheck)
analyze:
	@if command -v cppcheck >/dev/null 2>&1; then \
		echo "🔍 运行静态分析..."; \
		cppcheck --enable=all --suppress=missingInclude hjson/ example/ --error-exitcode=1; \
		echo "✅ 静态分析完成!"; \
	else \
		echo "❌ cppcheck 未安装"; \
	fi

# 性能测试 (如果构建了基准测试)
perf: benchmark
	@echo "🚀 运行性能测试..."
	@if [ -f build/bin/bench_parse ]; then \
		cd build && ./bin/bench_parse; \
	else \
		echo "❌ 基准测试未构建，请先运行 'make benchmark'"; \
	fi

# 检查依赖
check-deps:
	@echo "🔍 检查构建依赖..."
	@echo -n "CMake: "; command -v cmake >/dev/null 2>&1 && echo "✅" || echo "❌"
	@echo -n "GCC/G++: "; command -v g++ >/dev/null 2>&1 && echo "✅" || echo "❌"
	@echo -n "Ninja: "; command -v ninja >/dev/null 2>&1 && echo "✅" || echo "❌ (可选)"
	@echo -n "clang-format: "; command -v clang-format >/dev/null 2>&1 && echo "✅" || echo "❌ (可选)"
	@echo -n "cppcheck: "; command -v cppcheck >/dev/null 2>&1 && echo "✅" || echo "❌ (可选)"

# 帮助信息
help:
	@echo "hjson JSON Parser - 可用命令:"
	@echo ""
	@echo "构建命令:"
	@echo "  make build       - 构建项目 (Release模式)"
	@echo "  make debug       - Debug模式构建"
	@echo "  make release     - Release模式构建"
	@echo "  make full        - 完整构建 (包含所有组件)"
	@echo ""
	@echo "测试命令:"
	@echo "  make test        - 构建并运行测试"
	@echo "  make quick-test  - 快速测试"
	@echo "  make benchmark   - 构建基准测试"
	@echo "  make perf        - 运行性能测试"
	@echo ""
	@echo "工具命令:"
	@echo "  make clean       - 清理构建目录"
	@echo "  make install     - 安装到系统"
	@echo "  make format      - 格式化代码"
	@echo "  make analyze     - 静态代码分析"
	@echo "  make check-deps  - 检查构建依赖"
	@echo ""
	@echo "更多选项请查看: ./build.sh --help"

# 显示项目信息
info:
	@echo "================================"
	@echo "hjson JSON Parser"
	@echo "================================"
	@echo "一个高性能的C++ JSON解析库"
	@echo ""
	@echo "特性:"
	@echo "• Header-only设计"
	@echo "• 支持DOM和SAX两种解析方式"
	@echo "• 零拷贝、模板优化"
	@echo "• 完整的单元测试"
	@echo "• 性能基准测试"
	@echo ""
	@echo "快速开始: make test"
	@echo "================================"
