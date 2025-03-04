# Google Test 手动配置（避免版本兼容性问题）
set(GOOGLETEST_ROOT ${PROJECT_SOURCE_DIR}/third_party/gtest/googletest)

# 检查Google Test是否存在
if(NOT EXISTS "${GOOGLETEST_ROOT}")
    message(FATAL_ERROR "Google Test not found at ${GOOGLETEST_ROOT}")
endif()

# 添加gtest源文件
add_library(gtest STATIC
    ${GOOGLETEST_ROOT}/src/gtest-all.cc
    ${GOOGLETEST_ROOT}/src/gtest_main.cc
)

# 设置gtest包含目录
target_include_directories(gtest PUBLIC
    ${GOOGLETEST_ROOT}/include
    ${GOOGLETEST_ROOT}
)

# 设置gtest编译特性
target_compile_features(gtest PUBLIC cxx_std_17)
target_link_libraries(gtest PUBLIC pthread)

# 禁用gtest的警告
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(gtest PRIVATE 
        -Wno-conversion 
        -Wno-old-style-cast
        -Wno-shadow
        -Wno-unused-parameter
        -Wno-sign-compare
    )
endif()

# 测试可执行文件
add_executable(test_error test_error.cc)
target_link_libraries(test_error PRIVATE hjson gtest)
target_compile_features(test_error PRIVATE cxx_std_17)

add_executable(test_value test_value.cc)
target_link_libraries(test_value PRIVATE hjson gtest)
target_compile_features(test_value PRIVATE cxx_std_17)

add_executable(test_roundtrip test_roundtrip.cc)
target_link_libraries(test_roundtrip PRIVATE hjson gtest)
target_compile_features(test_roundtrip PRIVATE cxx_std_17)

# 为测试可执行文件禁用符号比较警告
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(test_error PRIVATE -Wno-sign-compare)
    target_compile_options(test_value PRIVATE -Wno-sign-compare) 
    target_compile_options(test_roundtrip PRIVATE -Wno-sign-compare)
endif()

# 添加测试
enable_testing()

add_test(NAME test_error 
         COMMAND test_error
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME test_value 
         COMMAND test_value
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME test_roundtrip 
         COMMAND test_roundtrip
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# 设置测试属性
set_tests_properties(test_error test_value test_roundtrip 
    PROPERTIES 
        TIMEOUT 30
        LABELS "unit_tests"
)