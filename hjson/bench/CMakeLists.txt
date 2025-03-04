# Google Benchmark 手动配置
set(BENCHMARK_ROOT ${PROJECT_SOURCE_DIR}/third_party/benchmark)

# 检查Google Benchmark是否存在
if(NOT EXISTS "${BENCHMARK_ROOT}")
    message(FATAL_ERROR "Google Benchmark not found at ${BENCHMARK_ROOT}")
endif()

# 添加benchmark子目录，但禁用测试和安装
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Enable testing of the benchmark library.")
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "Enable installation of benchmark.")
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "Enable building the unit tests which depend on gtest.")
set(BENCHMARK_DOWNLOAD_DEPENDENCIES OFF CACHE BOOL "Allow the downloading and in-tree building of unmet dependencies")

add_subdirectory(${BENCHMARK_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/benchmark)

# 创建benchmark可执行文件
add_executable(bench_parse bench_parse.cc)

# 设置编译特性
target_compile_features(bench_parse PRIVATE cxx_std_17)

# 链接库
target_link_libraries(bench_parse PRIVATE hjson benchmark::benchmark pthread)

# 禁用一些警告
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(bench_parse PRIVATE 
        -Wno-conversion 
        -Wno-old-style-cast
        -Wno-shadow
        -Wno-unused-parameter
    )
endif()

# 设置优化选项
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(bench_parse PRIVATE -O3 -DNDEBUG)
endif()

# 添加自定义目标来运行benchmark
add_custom_target(run_benchmark
    COMMAND $<TARGET_FILE:bench_parse>
    DEPENDS bench_parse
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Running JSON parsing benchmarks"
)