# hjson JSON Parser 使用示例

这个目录包含了hjson JSON解析器的各种使用示例，展示了库的主要功能和最佳实践。

## 示例列表

### 1. DOM风格示例 (`dom_example.cc`)
演示如何使用DOM API进行JSON解析和操作：
- 基本类型的读取和写入
- 对象和数组的操作
- 嵌套结构的处理
- JSON的生成和格式化

**编译运行:**
```bash
make dom_example
./build/bin/dom_example
```

### 2. SAX风格示例 (`sax_example.cc`)
展示SAX风格的事件驱动解析：
- 自定义Handler的实现
- 事件流的处理
- Handler链式组合
- 统计和过滤功能

**编译运行:**
```bash
make sax_example
./build/bin/sax_example
```

### 3. 流操作示例 (`stream_example.cc`)
介绍各种输入输出流的使用：
- 字符串流 (StringReadStream/StringWriteStream)
- 文件流 (FileReadStream/FileWriteStream)
- 流之间的数据拷贝
- 混合流操作

**编译运行:**
```bash
make stream_example
./build/bin/stream_example
```

### 4. 性能和错误处理示例 (`performance_example.cc`)
展示性能优化和错误处理：
- DOM vs SAX性能对比
- 错误类型和处理方式
- 内存使用优化
- 引用计数机制

**编译运行:**
```bash
make performance_example
./build/bin/performance_example
```

### 5. 现有示例

#### `pretty.cc` - 格式化输出
演示如何将JSON美化输出：
```bash
./build/bin/pretty
```

#### `parse.cc` - 基本解析
基本的JSON解析功能：
```bash
./build/bin/parse
```

#### `generate.cc` - JSON生成
演示如何生成JSON：
```bash
./build/bin/generate
```

## 快速开始

### 1. 构建所有示例
```bash
# 使用构建脚本
./build.sh --examples

# 或使用Makefile
make build
```

### 2. 运行特定示例
```bash
# DOM风格示例
./build/bin/dom_example

# SAX风格示例
./build/bin/sax_example

# 流操作示例
./build/bin/stream_example

# 性能测试示例
./build/bin/performance_example
```

## 常用模式

### DOM风格 - 适合小到中型JSON
```cpp
#include <hjson/Document.h>

json::Document doc;
json::ParseError err = doc.parse(json_string);
if (err == json::PARSE_OK) {
    // 访问数据
    std::string name = doc["name"].getString();
    int age = doc["age"].getInt32();
}
```

### SAX风格 - 适合大型JSON或流式处理
```cpp
#include <hjson/Reader.h>
#include <hjson/StringReadStream.h>

class MyHandler {
public:
    bool String(std::string_view s) { /* 处理字符串 */ return true; }
    bool Int32(int32_t i) { /* 处理整数 */ return true; }
    // ... 其他Handler方法
};

json::StringReadStream is(json_string);
MyHandler handler;
json::Reader::parse(is, handler);
```

### JSON生成
```cpp
#include <hjson/Writer.h>
#include <hjson/StringWriteStream.h>

json::StringWriteStream os;
json::Writer writer(os);

writer.StartObject();
writer.Key("name");
writer.String("Alice");
writer.Key("age");
writer.Int32(25);
writer.EndObject();

std::string result = os.get();
```

## 最佳实践

1. **选择合适的API风格**：
   - 小型JSON：使用DOM API
   - 大型JSON：使用SAX API
   - 流式处理：使用SAX API

2. **错误处理**：
   - 总是检查ParseError返回值
   - 使用parseErrorStr()获取错误描述

3. **性能优化**：
   - 重用Document对象
   - 使用string_view避免字符串拷贝
   - 考虑使用SAX API处理大文件

4. **内存管理**：
   - Value对象使用引用计数，可以安全拷贝
   - 避免不必要的深拷贝

## 常见问题

### Q: 如何处理大型JSON文件？
A: 使用SAX API和流式处理，避免将整个文件加载到内存。

### Q: 如何自定义Handler？
A: 继承并实现Handler接口的所有方法，参考sax_example.cc。

### Q: 如何提高解析性能？
A: 使用SAX API，避免不必要的字符串拷贝，重用对象。

### Q: 如何处理解析错误？
A: 检查ParseError返回值，使用parseErrorStr()获取错误信息。

## 更多资源

- [API文档](../README.md)
- [性能基准测试](../hjson/bench/)
- [单元测试](../hjson/test/)
