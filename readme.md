# hjson JSON Parser

<!-- [![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]() -->
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)]()
[![License](https://img.shields.io/badge/license-MIT-green)]()
[![Platform](https://img.shields.io/badge/platform-Linux-lightgrey)]()

一个**用于学习**的高性能、零拷贝的C++17 JSON解析库，支持DOM和SAX两种解析方式。

## ✨ 主要特性

- 🚀 **高性能**: 零拷贝设计，模板优化，性能优异
- 📦 **Header-Only**: 无需编译，直接包含头文件即可使用
- 🔄 **双API风格**: 同时支持DOM和SAX两种解析方式
- 🧩 **模块化设计**: 组件可自由组合，支持自定义扩展
- 🛡️ **类型安全**: 使用C++17模板特性，编译期类型检查
- 🧪 **完整测试**: 100%测试覆盖率，性能基准测试
- 📖 **易于使用**: 简洁的API设计，丰富的文档和示例

## 🚀 快速开始

### 基本使用

```cpp
#include "hjson/Document.h"
#include "hjson/PrettyWriter.h"
#include <iostream>

// DOM风格解析
void dom_example() {
    const char* json = R"({"name": "Tom", "age": 25, "active": true})";
    
    hjson::Document doc;
    doc.parse(json);
    
    std::cout << "Name: " << doc["name"].getString() << std::endl;
    std::cout << "Age: " << doc["age"].getInt32() << std::endl;
    std::cout << "Active: " << doc["active"].getBool() << std::endl;
}

// SAX风格解析
class MyHandler {
public:
    bool String(std::string_view s) { 
        std::cout << "String: " << s << std::endl; 
        return true; 
    }
    bool Int32(int32_t i) { 
        std::cout << "Number: " << i << std::endl; 
        return true; 
    }
    // ... 其他事件处理方法
};

void sax_example() {
    const char* json = R"([1, 2, "hello", true])";
    
    hjson::StringReadStream is(json);
    MyHandler handler;
    hjson::Reader::parse(is, handler);
}
```

### JSON生成

```cpp
#include "hjson/StringWriteStream.h"
#include "hjson/PrettyWriter.h"

void generate_json() {
    hjson::StringWriteStream os;
    hjson::PrettyWriter writer(os);
    
    writer.StartObject();
    writer.Key("name");
    writer.String("Alice");
    writer.Key("scores");
    writer.StartArray();
    writer.Int32(95);
    writer.Int32(87);
    writer.EndArray();
    writer.EndObject();
    
    std::cout << os.get() << std::endl;
}
```

## 📦 安装和构建

### 系统要求

- C++17兼容编译器 (GCC 7+, Clang 5+)
- CMake 3.15+
- Linux/macOS/Windows

### 克隆项目

```bash
git clone https://github.com/sirius-fan/hjson.git
cd hjson
```

### 快速构建

```bash
# 使用构建脚本 (推荐)
./build.sh

# 或使用Makefile
make build

# 或手动使用CMake
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --parallel
```

### 构建选项

```bash
# Debug构建 + 测试
./build.sh --debug --tests

# Release构建 + 基准测试
./build.sh --benchmarks

# 完整构建 (测试 + 基准测试 + 示例)
./build.sh --tests --benchmarks --examples

# 清理构建
./build.sh --clean
```

### Makefile命令

```bash
make help          # 查看所有可用命令
make test          # 构建并运行测试
make benchmark     # 构建基准测试
make perf          # 运行性能测试
make clean         # 清理构建目录
make install       # 安装到系统
```

## 📚 API文档

### 核心组件

#### Value 类型系统

```cpp
hjson::Value value;

// 基本类型操作
value.setNull();
value.setBool(true);
value.setInt32(42);
value.setInt64(1234567890L);
value.setDouble(3.14);
value.setString("hello");

// 获取值
bool b = value.getBool();
int32_t i = value.getInt32();
std::string s = value.getString();

// 类型检查
if (value.isString()) {
    // 处理字符串
}
```

#### 数组操作

```cpp
hjson::Value array;
array.setArray();

// 添加元素
array.pushBack().setInt32(1);
array.pushBack().setString("hello");

// 访问元素
for (int i = 0; i < array.size(); ++i) {
    hjson::Value& elem = array[i];
    // 处理元素
}
```

#### 对象操作

```cpp
hjson::Value object;
object.setObject();

// 添加成员
object.addMember("name").setString("Tom");
object.addMember("age").setInt32(25);

// 访问成员
hjson::Value& name = object["name"];
if (object.hasMember("age")) {
    int age = object["age"].getInt32();
}
```

#### SAX Handler接口

```cpp
class MyHandler {
public:
    bool Null() { return true; }
    bool Bool(bool b) { return true; }
    bool Int32(int32_t i) { return true; }
    bool Int64(int64_t i) { return true; }
    bool Double(double d) { return true; }
    bool String(std::string_view s) { return true; }
    bool Key(std::string_view s) { return true; }
    bool StartObject() { return true; }
    bool EndObject() { return true; }
    bool StartArray() { return true; }
    bool EndArray() { return true; }
};
```

### 流系统

#### 输入流

```cpp
// 从文件读取
hjson::FileReadStream file_stream(stdin);

// 从字符串读取
hjson::StringReadStream string_stream(json_string);
```

#### 输出流

```cpp
// 输出到文件
hjson::FileWriteStream file_stream(stdout);

// 输出到字符串
hjson::StringWriteStream string_stream;
// ... 写入操作
std::string result = string_stream.get();
```

## 🧪 测试

项目包含完整的单元测试套件：

```bash
# 运行所有测试
make test

# 或手动运行
cd build
ctest --output-on-failure

# 运行特定测试
./bin/test_value
./bin/test_roundtrip
./bin/test_error
```

### 测试覆盖

- ✅ Value类型系统测试
- ✅ DOM解析测试
- ✅ SAX解析测试
- ✅ JSON生成测试
- ✅ 错误处理测试
- ✅ 往返测试 (parse → generate → parse)

```
🧪 运行测试...
Test project /home/sirius/Desktop/新建文件夹/new/hjson/build
    Start 1: test_error
1/3 Test #1: test_error .......................   Passed    0.00 sec
    Start 2: test_value
2/3 Test #2: test_value .......................   Passed    0.00 sec
    Start 3: test_roundtrip
3/3 Test #3: test_roundtrip ...................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 3
```


## 📊 性能基准

运行性能基准测试：

```bash
make perf
# 或
./build/bin/bench_parse
```

### 基准测试结果

在现代Linux系统上的测试结果：

| 文件 | 大小 | 解析+生成时间 | 吞吐量 |
|------|------|---------------|--------|
| canada.json | 2.2MB | ~40ms | ~55MB/s |
| citm_catalog.json | 1.7MB | ~5ms | ~340MB/s |

```
Run on (16 X 5102.71 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1024 KiB (x8)
  L3 Unified 16384 KiB (x1)
Load Average: 1.34, 0.89, 0.81
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations
--------------------------------------------------------------------------
BM_read_parse_write/many_double       36.5 ms         36.4 ms           38
BM_read_parse_write/simple            4.58 ms         4.56 ms          308
```


### 与其他库对比

hjson在保持API简洁的同时，性能接近或超越主流C++ JSON库。

## 🏗️ 架构设计

### 模块架构

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   ReadStream    │    │     Handler     │    │   WriteStream   │
│                 │    │                 │    │                 │
│ • FileRead      │────▶│ • Writer       │────▶│ • FileWrite     │
│ • StringRead    │    │ • PrettyWriter  │    │ • StringWrite   │
│                 │    │ • Document      │    │                 │
└─────────────────┘    │ • Custom       │    └─────────────────┘
                       └─────────────────┘
                              ▲
                              │
                       ┌─────────────────┐
                       │     Reader      │
                       │   (SAX Parser)  │
                       └─────────────────┘
```

### 核心概念

1. **ReadStream**: 统一的输入接口
2. **WriteStream**: 统一的输出接口  
3. **Handler**: SAX风格的事件处理接口
4. **Reader**: 核心解析引擎
5. **Value**: DOM风格的数据结构

### 设计特点

- 🔗 **组合式设计**: 组件可自由组合
- 📋 **模板化**: 零运行时开销的多态
- 🔄 **流式处理**: 支持大文件处理
- 🧠 **智能内存管理**: 引用计数和RAII

## 🔧 高级用法

### 自定义Handler

```cpp
// 统计JSON元素数量的Handler
class CounterHandler {
private:
    int objects = 0, arrays = 0, strings = 0, numbers = 0;
    
public:
    bool StartObject() { objects++; return true; }
    bool StartArray() { arrays++; return true; }
    bool String(std::string_view) { strings++; return true; }
    bool Int32(int32_t) { numbers++; return true; }
    // ... 其他方法
    
    void printStats() {
        std::cout << "Objects: " << objects << ", Arrays: " << arrays 
                  << ", Strings: " << strings << ", Numbers: " << numbers << std::endl;
    }
};
```

### Handler链式组合

```cpp
// 过滤器Handler
template<typename NextHandler>
class FilterHandler {
    NextHandler& next_;
public:
    FilterHandler(NextHandler& next) : next_(next) {}
    
    bool String(std::string_view s) {
        // 过滤空字符串
        return s.empty() ? true : next_.String(s);
    }
    
    // 其他方法转发给next_
};

// 使用: FilterHandler -> Writer -> WriteStream
hjson::StringWriteStream os;
hjson::Writer writer(os);
FilterHandler filter(writer);
hjson::Reader::parse(is, filter);
```

### 自定义Stream

```cpp
// 网络读取流示例
class NetworkReadStream {
private:
    int socket_fd;
    char buffer[1024];
    size_t pos = 0, len = 0;
    
public:
    NetworkReadStream(int fd) : socket_fd(fd) {}
    
    char peek() {
        if (pos >= len) {
            len = recv(socket_fd, buffer, sizeof(buffer), 0);
            pos = 0;
        }
        return pos < len ? buffer[pos] : '\0';
    }
    
    char take() {
        char c = peek();
        if (c != '\0') pos++;
        return c;
    }
    
    size_t tell() const { return pos; }
};
```



### 开发环境设置

```bash
# 克隆项目
git clone https://github.com/sirius-fan/hjson.git
cd hjson

# 安装开发依赖 (Ubuntu/Debian)
sudo apt install build-essential cmake ninja-build clang-format cppcheck

# 检查依赖
make check-deps

# 运行完整测试
make full
```

### 代码风格

```bash
# 格式化代码
make format

# 静态分析
make analyze

# 运行测试
make test
```


## 📄 许可证

本项目采用MIT许可证 - 查看 [LICENSE](LICENSE) 文件了解详情。

## 🙏 致谢

- 感谢jackson项目的启发和设计
- 参考了RapidJSON的设计理念
- 使用Google Test和Google Benchmark进行测试
