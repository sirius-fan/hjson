#!/bin/bash

# hjson JSON Parser 构建脚本
# 使用方法: ./build.sh [options]

set -e

# 默认配置
BUILD_TYPE="Release"
BUILD_TESTS=ON
BUILD_BENCHMARKS=OFF
BUILD_EXAMPLES=ON
CLEAN_BUILD=false
VERBOSE=false

# 解析命令行参数
show_help() {
    echo "hjson JSON Parser 构建脚本"
    echo ""
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  -h, --help          显示此帮助信息"
    echo "  -c, --clean         清理构建目录"
    echo "  -d, --debug         Debug构建 (默认: Release)"
    echo "  -t, --tests         启用测试构建 (默认: ON)"
    echo "  -b, --benchmarks    启用基准测试构建 (默认: OFF)"
    echo "  -e, --examples      启用示例构建 (默认: ON)"
    echo "  -v, --verbose       详细输出"
    echo "  --no-tests          禁用测试"
    echo "  --no-examples       禁用示例"
    echo ""
    echo "示例:"
    echo "  $0                  # 默认Release构建"
    echo "  $0 -d -t            # Debug构建，启用测试"
    echo "  $0 -c -b            # 清理构建，启用基准测试"
    echo "  $0 --no-tests       # 禁用测试的构建"
}

while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            exit 0
            ;;
        -c|--clean)
            CLEAN_BUILD=true
            shift
            ;;
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -t|--tests)
            BUILD_TESTS=ON
            shift
            ;;
        --no-tests)
            BUILD_TESTS=OFF
            shift
            ;;
        -b|--benchmarks)
            BUILD_BENCHMARKS=ON
            shift
            ;;
        -e|--examples)
            BUILD_EXAMPLES=ON
            shift
            ;;
        --no-examples)
            BUILD_EXAMPLES=OFF
            shift
            ;;
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        *)
            echo "未知选项: $1"
            show_help
            exit 1
            ;;
    esac
done

# 项目根目录
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"

echo "=========================================="
echo "hjson JSON Parser 构建配置"
echo "=========================================="
echo "构建类型:     ${BUILD_TYPE}"
echo "启用测试:     ${BUILD_TESTS}"
echo "启用基准测试: ${BUILD_BENCHMARKS}"
echo "启用示例:     ${BUILD_EXAMPLES}"
echo "构建目录:     ${BUILD_DIR}"
echo "清理构建:     ${CLEAN_BUILD}"
echo "=========================================="

# 清理构建目录
if [[ "$CLEAN_BUILD" == "true" ]]; then
    echo "🧹 清理构建目录..."
    rm -rf "${BUILD_DIR}"
fi

# 创建构建目录
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

# 配置CMake参数
CMAKE_ARGS=(
    -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"
    -DCMAKE_BUILD_TESTS="${BUILD_TESTS}"
    -DCMAKE_BUILD_BENCHMARK="${BUILD_BENCHMARKS}"
)

if [[ "$BUILD_EXAMPLES" == "OFF" ]]; then
    CMAKE_ARGS+=(-DCMAKE_BUILD_NO_EXAMPLES=ON)
fi

if [[ "$VERBOSE" == "true" ]]; then
    CMAKE_ARGS+=(-DCMAKE_VERBOSE_MAKEFILE=ON)
fi

# 运行CMake配置
echo "🔧 配置项目..."
cmake "${CMAKE_ARGS[@]}" "${PROJECT_ROOT}"

# 构建项目
echo "🔨 构建项目..."
if [[ "$VERBOSE" == "true" ]]; then
    cmake --build . --parallel $(nproc) --verbose
else
    cmake --build . --parallel $(nproc)
fi

echo "✅ 构建完成!"

# 显示构建结果
echo ""
echo "📁 构建产物:"
if [[ -d "${BUILD_DIR}/bin" ]]; then
    echo "可执行文件:"
    ls -la "${BUILD_DIR}/bin/" | grep -E '^-' | awk '{print "  " $9}' || true
fi

if [[ -d "${BUILD_DIR}/lib" ]]; then
    echo "库文件:"
    ls -la "${BUILD_DIR}/lib/" | grep -E '^-' | awk '{print "  " $9}' || true
fi

# 运行测试
if [[ "$BUILD_TESTS" == "ON" ]]; then
    echo ""
    echo "🧪 运行测试..."
    if ctest --test-dir "${BUILD_DIR}" --output-on-failure; then
        echo "✅ 所有测试通过!"
    else
        echo "❌ 测试失败!"
        exit 1
    fi
fi

# 基准测试提示
if [[ "$BUILD_BENCHMARKS" == "ON" ]]; then
    echo ""
    echo "📊 基准测试已构建完成:"
    if [[ -f "${BUILD_DIR}/bin/bench_parse" ]]; then
        echo "正在运行基准测试..."
        echo ""
        # 从项目根目录运行以确保能找到JSON文件
        cd "${PROJECT_ROOT}"
        ${BUILD_DIR}/bin/bench_parse --benchmark_min_time=1s
    else
        echo "  基准测试可执行文件未找到: ${BUILD_DIR}/bin/bench_parse"
        echo "  也可以使用: make run_benchmark"
    fi
fi

echo ""
echo "🎉 构建脚本执行完成!"
