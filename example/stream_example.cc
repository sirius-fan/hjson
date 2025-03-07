//
// 流操作示例
//

#include <hjson/Document.h>
#include <hjson/FileReadStream.h>
#include <hjson/FileWriteStream.h>
#include <hjson/StringReadStream.h>
#include <hjson/StringWriteStream.h>
#include <hjson/Writer.h>
#include <hjson/PrettyWriter.h>
#include <iostream>
#include <fstream>
#include <cstdio>

using namespace json;

void string_stream_example() {
    std::cout << "=== 字符串流示例 ===" << std::endl;
    
    // 从字符串读取
    const char* json_input = R"({"name":"Alice","age":25,"city":"Beijing"})";
    std::cout << "输入JSON: " << json_input << std::endl;
    
    StringReadStream input_stream(json_input);
    
    // 解析到Document
    Document doc;
    ParseError err = doc.parseStream(input_stream);
    if (err != PARSE_OK) {
        std::cout << "解析错误: " << parseErrorStr(err) << std::endl;
        return;
    }
    
    // 输出到字符串
    StringWriteStream output_stream;
    PrettyWriter writer(output_stream);
    doc.writeTo(writer);
    
    std::cout << "格式化输出:" << std::endl;
    std::cout << output_stream.get() << std::endl;
}

void file_stream_example() {
    std::cout << "\n=== 文件流示例 ===" << std::endl;
    
    // 创建测试文件
    const char* test_filename = "test_input.json";
    {
        std::ofstream file(test_filename);
        file << R"({
    "users": [
        {"id": 1, "name": "Alice", "active": true},
        {"id": 2, "name": "Bob", "active": false}
    ],
    "count": 2
})";
    }
    
    // 从文件读取
    FILE* input_file = fopen(test_filename, "r");
    if (!input_file) {
        std::cout << "无法打开输入文件" << std::endl;
        return;
    }
    
    FileReadStream file_input(input_file);
    Document doc;
    ParseError err = doc.parseStream(file_input);
    fclose(input_file);
    
    if (err != PARSE_OK) {
        std::cout << "解析错误: " << parseErrorStr(err) << std::endl;
        return;
    }
    
    std::cout << "从文件读取成功!" << std::endl;
    std::cout << "用户数量: " << doc["count"].getInt32() << std::endl;
    
    // 输出到标准输出
    std::cout << "文件内容 (格式化):" << std::endl;
    FileWriteStream console_output(stdout);
    PrettyWriter console_writer(console_output);
    doc.writeTo(console_writer);
    
    // 清理测试文件
    std::remove(test_filename);
}

void stream_copy_example() {
    std::cout << "\n=== 流拷贝示例 ===" << std::endl;
    
    const char* json = R"([1,2,3,{"a":true,"b":null}])";
    std::cout << "原始JSON: " << json << std::endl;
    
    // 输入流
    StringReadStream input_stream(json);
    
    // 输出流
    StringWriteStream output_stream;
    Writer writer(output_stream);
    
    // 直接从输入流解析并写入输出流 (SAX方式)
    ParseError err = Reader::parse(input_stream, writer);
    if (err == PARSE_OK) {
        std::cout << "拷贝结果: " << output_stream.get() << std::endl;
    } else {
        std::cout << "拷贝失败: " << parseErrorStr(err) << std::endl;
    }
}

void mixed_stream_example() {
    std::cout << "\n=== 混合流示例 ===" << std::endl;
    
    // 1. 从字符串读取
    const char* json_data = R"({"title":"示例文档","data":[1,2,3,4,5]})";
    StringReadStream string_input(json_data);
    
    Document doc;
    doc.parseStream(string_input);
    
    // 2. 修改数据
    doc["title"].setString("修改后的文档");
    doc["data"].addValue(Value(6));
    doc["data"].addValue(Value(7));
    
    // 3. 输出到不同流
    std::cout << "输出到控制台:" << std::endl;
    FileWriteStream console_out(stdout);
    PrettyWriter console_writer(console_out);
    doc.writeTo(console_writer);
    
    // 4. 同时输出到字符串
    StringWriteStream string_out;
    Writer compact_writer(string_out);
    doc.writeTo(compact_writer);
    
    std::cout << "\n压缩输出: " << string_out.get() << std::endl;
}

int main() {
    try {
        string_stream_example();
        file_stream_example();
        stream_copy_example();
        mixed_stream_example();
        
        std::cout << "\n所有流示例执行完成!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
