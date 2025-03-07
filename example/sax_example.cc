//
// SAX风格JSON解析示例
//

#include <hjson/Reader.h>
#include <hjson/StringReadStream.h>
#include <hjson/Writer.h>
#include <hjson/StringWriteStream.h>
#include <iostream>
#include <vector>
#include <string>

using namespace json;

// 简单的打印Handler
class PrintHandler {
private:
    int depth_ = 0;
    
    void indent() {
        for (int i = 0; i < depth_; ++i) {
            std::cout << "  ";
        }
    }
    
public:
    bool Null() {
        indent();
        std::cout << "NULL" << std::endl;
        return true;
    }
    
    bool Bool(bool b) {
        indent();
        std::cout << "BOOL: " << (b ? "true" : "false") << std::endl;
        return true;
    }
    
    bool Int32(int32_t i) {
        indent();
        std::cout << "INT32: " << i << std::endl;
        return true;
    }
    
    bool Int64(int64_t i) {
        indent();
        std::cout << "INT64: " << i << std::endl;
        return true;
    }
    
    bool Double(double d) {
        indent();
        std::cout << "DOUBLE: " << d << std::endl;
        return true;
    }
    
    bool String(std::string_view s) {
        indent();
        std::cout << "STRING: \"" << s << "\"" << std::endl;
        return true;
    }
    
    bool Key(std::string_view s) {
        indent();
        std::cout << "KEY: \"" << s << "\"" << std::endl;
        return true;
    }
    
    bool StartObject() {
        indent();
        std::cout << "START_OBJECT {" << std::endl;
        depth_++;
        return true;
    }
    
    bool EndObject() {
        depth_--;
        indent();
        std::cout << "END_OBJECT }" << std::endl;
        return true;
    }
    
    bool StartArray() {
        indent();
        std::cout << "START_ARRAY [" << std::endl;
        depth_++;
        return true;
    }
    
    bool EndArray() {
        depth_--;
        indent();
        std::cout << "END_ARRAY ]" << std::endl;
        return true;
    }
};

// 统计Handler
class StatHandler {
public:
    int objects = 0;
    int arrays = 0;
    int strings = 0;
    int numbers = 0;
    int booleans = 0;
    int nulls = 0;
    
    bool Null() { nulls++; return true; }
    bool Bool(bool) { booleans++; return true; }
    bool Int32(int32_t) { numbers++; return true; }
    bool Int64(int64_t) { numbers++; return true; }
    bool Double(double) { numbers++; return true; }
    bool String(std::string_view) { strings++; return true; }
    bool Key(std::string_view) { strings++; return true; }
    bool StartObject() { objects++; return true; }
    bool EndObject() { return true; }
    bool StartArray() { arrays++; return true; }
    bool EndArray() { return true; }
    
    void printStats() {
        std::cout << "统计结果:" << std::endl;
        std::cout << "  对象: " << objects << std::endl;
        std::cout << "  数组: " << arrays << std::endl;
        std::cout << "  字符串: " << strings << std::endl;
        std::cout << "  数字: " << numbers << std::endl;
        std::cout << "  布尔值: " << booleans << std::endl;
        std::cout << "  空值: " << nulls << std::endl;
    }
};

// 过滤Handler - 过滤掉空字符串
template<typename NextHandler>
class FilterHandler {
private:
    NextHandler& next_;
    
public:
    FilterHandler(NextHandler& next) : next_(next) {}
    
    bool Null() { return next_.Null(); }
    bool Bool(bool b) { return next_.Bool(b); }
    bool Int32(int32_t i) { return next_.Int32(i); }
    bool Int64(int64_t i) { return next_.Int64(i); }
    bool Double(double d) { return next_.Double(d); }
    
    bool String(std::string_view s) {
        // 过滤空字符串
        if (s.empty()) {
            return next_.String("(空字符串已过滤)");
        }
        return next_.String(s);
    }
    
    bool Key(std::string_view s) { return next_.Key(s); }
    bool StartObject() { return next_.StartObject(); }
    bool EndObject() { return next_.EndObject(); }
    bool StartArray() { return next_.StartArray(); }
    bool EndArray() { return next_.EndArray(); }
};

void print_parsing_example() {
    std::cout << "=== SAX打印解析示例 ===" << std::endl;
    
    const char* json = R"({
        "user": {
            "id": 123,
            "name": "Alice",
            "active": true,
            "scores": [95, 87, 92]
        }
    })";
    
    StringReadStream is(json);
    PrintHandler handler;
    
    ParseError err = Reader::parse(is, handler);
    if (err != PARSE_OK) {
        std::cout << "解析错误: " << parseErrorStr(err) << std::endl;
    }
}

void stats_parsing_example() {
    std::cout << "\n=== SAX统计解析示例 ===" << std::endl;
    
    const char* json = R"({
        "users": [
            {"name": "Alice", "age": 25, "active": true},
            {"name": "Bob", "age": 30, "active": false},
            {"name": "Charlie", "age": null, "active": true}
        ],
        "metadata": {
            "total": 3,
            "version": "1.0"
        }
    })";
    
    StringReadStream is(json);
    StatHandler handler;
    
    ParseError err = Reader::parse(is, handler);
    if (err == PARSE_OK) {
        handler.printStats();
    } else {
        std::cout << "解析错误: " << parseErrorStr(err) << std::endl;
    }
}

void filter_parsing_example() {
    std::cout << "\n=== SAX过滤解析示例 ===" << std::endl;
    
    const char* json = R"({
        "name": "Test",
        "empty": "",
        "data": ["hello", "", "world"]
    })";
    
    std::cout << "原始JSON事件流:" << std::endl;
    {
        StringReadStream is(json);
        PrintHandler handler;
        Reader::parse(is, handler);
    }
    
    std::cout << "\n过滤后的事件流:" << std::endl;
    {
        StringReadStream is(json);
        PrintHandler print_handler;
        FilterHandler filter(print_handler);
        Reader::parse(is, filter);
    }
}

void sax_to_json_example() {
    std::cout << "\n=== SAX转JSON示例 ===" << std::endl;
    
    const char* json = R"({"name":"Alice","age":25,"hobbies":["reading","coding"]})";
    
    std::cout << "输入JSON (压缩格式):" << std::endl;
    std::cout << json << std::endl;
    
    StringReadStream is(json);
    StringWriteStream os;
    Writer writer(os);
    
    ParseError err = Reader::parse(is, writer);
    if (err == PARSE_OK) {
        std::cout << "\n输出JSON (重新格式化):" << std::endl;
        std::cout << os.get() << std::endl;
    } else {
        std::cout << "解析错误: " << parseErrorStr(err) << std::endl;
    }
}

int main() {
    try {
        print_parsing_example();
        stats_parsing_example();
        filter_parsing_example();
        sax_to_json_example();
        
        std::cout << "\n所有SAX示例执行完成!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
