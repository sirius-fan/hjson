//
// 性能对比和错误处理示例
//

#include <hjson/Document.h>
#include <hjson/Reader.h>
#include <hjson/StringReadStream.h>
#include <hjson/StringWriteStream.h>
#include <hjson/Writer.h>
#include <hjson/PrettyWriter.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>

using namespace json;

// 简单的计时器类
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_;
    
public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}
    
    double elapsed_ms() const {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
        return duration.count() / 1000.0;
    }
};

// 用于性能测试的Handler
class NullHandler {
public:
    bool Null() { return true; }
    bool Bool(bool) { return true; }
    bool Int32(int32_t) { return true; }
    bool Int64(int64_t) { return true; }
    bool Double(double) { return true; }
    bool String(std::string_view) { return true; }
    bool Key(std::string_view) { return true; }
    bool StartObject() { return true; }
    bool EndObject() { return true; }
    bool StartArray() { return true; }
    bool EndArray() { return true; }
};

void performance_comparison() {
    std::cout << "=== 性能对比测试 ===" << std::endl;
    
    // 生成测试数据
    std::string large_json = R"({
        "users": [)";
    
    for (int i = 0; i < 1000; ++i) {
        if (i > 0) large_json += ",";
        large_json += R"({
            "id": )" + std::to_string(i) + R"(,
            "name": "User)" + std::to_string(i) + R"(",
            "email": "user)" + std::to_string(i) + R"(@example.com",
            "active": )" + (i % 2 == 0 ? "true" : "false") + R"(,
            "score": )" + std::to_string(i * 0.95) + R"(
        })";
    }
    large_json += R"(],
        "metadata": {
            "total": 1000,
            "version": "1.0",
            "generated": "2023-12-01"
        }
    })";
    
    std::cout << "测试数据大小: " << large_json.size() << " 字节" << std::endl;
    
    const int iterations = 100;
    
    // DOM方式测试
    {
        Timer timer;
        for (int i = 0; i < iterations; ++i) {
            Document doc;
            ParseError err = doc.parse(large_json);
            if (err != PARSE_OK) {
                std::cout << "DOM解析错误!" << std::endl;
                return;
            }
        }
        double elapsed = timer.elapsed_ms();
        std::cout << "DOM解析 " << iterations << " 次用时: " 
                  << elapsed << " ms (平均: " << elapsed/iterations << " ms)" << std::endl;
    }
    
    // SAX方式测试
    {
        Timer timer;
        for (int i = 0; i < iterations; ++i) {
            StringReadStream is(large_json);
            NullHandler handler;
            ParseError err = Reader::parse(is, handler);
            if (err != PARSE_OK) {
                std::cout << "SAX解析错误!" << std::endl;
                return;
            }
        }
        double elapsed = timer.elapsed_ms();
        std::cout << "SAX解析 " << iterations << " 次用时: " 
                  << elapsed << " ms (平均: " << elapsed/iterations << " ms)" << std::endl;
    }
    
    // JSON生成测试
    {
        Document doc;
        doc.parse(large_json);
        
        Timer timer;
        for (int i = 0; i < iterations; ++i) {
            StringWriteStream os;
            Writer writer(os);
            doc.writeTo(writer);
        }
        double elapsed = timer.elapsed_ms();
        std::cout << "JSON生成 " << iterations << " 次用时: " 
                  << elapsed << " ms (平均: " << elapsed/iterations << " ms)" << std::endl;
    }
}

void error_handling_examples() {
    std::cout << "\n=== 错误处理示例 ===" << std::endl;
    
    struct TestCase {
        const char* name;
        const char* json;
        ParseError expected_error;
    };
    
    TestCase test_cases[] = {
        {"空字符串", "", PARSE_EXPECT_VALUE},
        {"不完整对象", "{", PARSE_MISS_COMMA_OR_CURLY_BRACKET},
        {"不完整数组", "[1,2,", PARSE_MISS_COMMA_OR_SQUARE_BRACKET},
        {"缺少引号", "{name: \"value\"}", PARSE_MISS_KEY},
        {"缺少冒号", "{\"name\" \"value\"}", PARSE_MISS_COLON},
        {"无效数字", "01", PARSE_ROOT_NOT_SINGULAR},
        {"无效转义", "\"\\z\"", PARSE_BAD_STRING_ESCAPE},
        {"未闭合字符串", "\"hello", PARSE_MISS_QUOTATION_MARK},
        {"多余的根值", "null false", PARSE_ROOT_NOT_SINGULAR}
    };
    
    for (const auto& test : test_cases) {
        std::cout << "\n测试: " << test.name << std::endl;
        std::cout << "JSON: " << test.json << std::endl;
        
        Document doc;
        ParseError err = doc.parse(test.json);
        
        std::cout << "结果: " << parseErrorStr(err);
        if (err == test.expected_error) {
            std::cout << " ✓" << std::endl;
        } else {
            std::cout << " ✗ (期望: " << parseErrorStr(test.expected_error) << ")" << std::endl;
        }
    }
}

void memory_usage_example() {
    std::cout << "\n=== 内存使用示例 ===" << std::endl;
    
    // 演示引用计数的内存共享
    std::cout << "创建多个共享字符串的Value..." << std::endl;
    
    std::vector<Value> values;
    const char* shared_str = "这是一个共享的字符串";
    
    // 创建多个Value共享同一个字符串
    for (int i = 0; i < 10; ++i) {
        Value v;
        v.setString(shared_str);
        values.push_back(std::move(v));
    }
    
    std::cout << "创建了 " << values.size() << " 个Value对象" << std::endl;
    std::cout << "它们共享同一个字符串数据 (引用计数机制)" << std::endl;
    
    // 演示数组和对象的拷贝成本
    Value large_array;
    large_array.setArray();
    
    for (int i = 0; i < 1000; ++i) {
        large_array.addValue(Value(i));
    }
    
    std::cout << "\n大数组创建完成 (1000个元素)" << std::endl;
    
    Timer timer;
    Value copy = large_array;  // 浅拷贝，只增加引用计数
    double elapsed = timer.elapsed_ms();
    
    std::cout << "数组拷贝用时: " << elapsed << " ms (引用计数拷贝)" << std::endl;
}

int main() {
    try {
        performance_comparison();
        error_handling_examples();
        memory_usage_example();
        
        std::cout << "\n所有性能和错误处理示例执行完成!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
