//
// DOM风格JSON解析示例
//

#include <hjson/Document.h>
#include <hjson/PrettyWriter.h>
#include <hjson/StringWriteStream.h>
#include <iostream>
#include <string>

using namespace json;

void basic_parsing_example() {
    std::cout << "=== 基本解析示例 ===" << std::endl;
    
    const char* json = R"({
        "name": "Alice",
        "age": 25,
        "active": true,
        "score": 95.5,
        "address": null,
        "hobbies": ["reading", "swimming", "coding"]
    })";
    
    Document doc;
    ParseError err = doc.parse(json);
    if (err != PARSE_OK) {
        std::cout << "解析错误: " << parseErrorStr(err) << std::endl;
        return;
    }
    
    // 访问基本类型
    std::cout << "姓名: " << doc["name"].getString() << std::endl;
    std::cout << "年龄: " << doc["age"].getInt32() << std::endl;
    std::cout << "活跃: " << (doc["active"].getBool() ? "是" : "否") << std::endl;
    std::cout << "分数: " << doc["score"].getDouble() << std::endl;
    
    // 检查null值
    if (doc["address"].isNull()) {
        std::cout << "地址: 未填写" << std::endl;
    }
    
    // 遍历数组
    std::cout << "爱好: ";
    auto& hobbies = doc["hobbies"];
    for (size_t i = 0; i < hobbies.getSize(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << hobbies[i].getString();
    }
    std::cout << std::endl;
}

void object_manipulation_example() {
    std::cout << "\n=== 对象操作示例 ===" << std::endl;
    
    // 创建JSON对象
    Document doc;
    doc.setObject();
    
    // 添加各种类型的成员
    doc.addMember("id", Value(12345));
    doc.addMember("username", Value("john_doe"));
    doc.addMember("verified", Value(true));
    doc.addMember("balance", Value(1234.56));
    
    // 添加嵌套对象
    Value profile(TYPE_OBJECT);
    profile.addMember("firstName", Value("John"));
    profile.addMember("lastName", Value("Doe"));
    profile.addMember("email", Value("john.doe@example.com"));
    doc.addMember("profile", std::move(profile));
    
    // 添加数组
    Value tags(TYPE_ARRAY);
    tags.addValue(Value("developer"));
    tags.addValue(Value("javascript"));
    tags.addValue(Value("nodejs"));
    doc.addMember("tags", std::move(tags));
    
    // 输出为格式化JSON
    StringWriteStream os;
    PrettyWriter writer(os);
    doc.writeTo(writer);
    
    std::cout << "生成的JSON:" << std::endl;
    std::cout << os.get() << std::endl;
}

void array_operations_example() {
    std::cout << "\n=== 数组操作示例 ===" << std::endl;
    
    const char* json = R"([
        {"name": "Alice", "score": 95},
        {"name": "Bob", "score": 87},
        {"name": "Charlie", "score": 92}
    ])";
    
    Document doc;
    doc.parse(json);
    
    std::cout << "学生成绩:" << std::endl;
    auto& students = doc.getArray();
    
    double total = 0;
    for (size_t i = 0; i < students.size(); ++i) {
        auto& student = students[i];
        auto name = student["name"].getString();
        auto score = student["score"].getInt32();
        
        std::cout << "  " << name << ": " << score << "分" << std::endl;
        total += score;
    }
    
    std::cout << "平均分: " << (total / students.size()) << std::endl;
}

void nested_structure_example() {
    std::cout << "\n=== 嵌套结构示例 ===" << std::endl;
    
    const char* json = R"({
        "company": {
            "name": "TechCorp",
            "departments": [
                {
                    "name": "Engineering",
                    "employees": [
                        {"name": "Alice", "role": "Senior Developer"},
                        {"name": "Bob", "role": "DevOps Engineer"}
                    ]
                },
                {
                    "name": "Marketing",
                    "employees": [
                        {"name": "Charlie", "role": "Marketing Manager"}
                    ]
                }
            ]
        }
    })";
    
    Document doc;
    doc.parse(json);
    
    auto& company = doc["company"];
    std::cout << "公司: " << company["name"].getString() << std::endl;
    
    auto& departments = company["departments"];
    std::cout << "部门数量: " << departments.getSize() << std::endl;
    
    for (size_t i = 0; i < departments.getSize(); ++i) {
        auto& dept = departments[i];
        std::cout << "\n部门: " << dept["name"].getString() << std::endl;
        
        auto& employees = dept["employees"];
        for (size_t j = 0; j < employees.getSize(); ++j) {
            auto& emp = employees[j];
            std::cout << "  - " << emp["name"].getString() 
                      << " (" << emp["role"].getString() << ")" << std::endl;
        }
    }
}

int main() {
    try {
        basic_parsing_example();
        object_manipulation_example();
        array_operations_example();
        nested_structure_example();
        
        std::cout << "\n所有示例执行完成!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
