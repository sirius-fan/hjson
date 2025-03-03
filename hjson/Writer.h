#ifndef TJSON_WRITER_H
#define TJSON_WRITER_H

// #include <string>
#include <cstdint>
#include <vector>
#include <cassert>
#include <cmath>

#include <hjson/Value.h>

namespace json
{

namespace detail
{

//
// fast int to string conversion
// buffer is NOT null terminated!!!
//
unsigned itoa(int32_t val, char* buf);
unsigned itoa(int64_t val, char* buf);

}

template <typename WriteStream>
class Writer: noncopyable
{
public:
    explicit Writer(WriteStream& os):
            os_(os), seeValue_(false)
    {}

    bool Null()
    {
        prefix(TYPE_NULL);
        os_.put("null");
        return true;
    }
    bool Bool(bool b)
    {
        prefix(TYPE_BOOL);
        os_.put(b ? "true" : "false");
        return true;
    }
    bool Int32(int32_t i32)
    {
        prefix(TYPE_INT32);

        char buf[11];
        unsigned cnt = detail::itoa(i32, buf);
        os_.put(std::string_view(buf, cnt));
        return true;
    }
    bool Int64(int64_t i64)
    {
        prefix(TYPE_INT64);

        char buf[20];
        unsigned cnt = detail::itoa(i64, buf);
        os_.put(std::string_view(buf, cnt));
        return true;
    }
    bool Double(double d)
    {
        prefix(TYPE_DOUBLE);

        // fixme: faster conversion please
        char buf[32];


        if (std::isinf(d)) {
            strcpy(buf, "Infinity");
        }
        else if (std::isnan(d)) {
            strcpy(buf, "NaN");
        }
        else {

            int n = sprintf(buf, "%.17g", d);

            // type information loss if ".0" not added
            // "1.0" -> double 1 -> "1"
            assert(n > 0 && n < 32);
            auto it = std::find_if_not(buf, buf + n, isdigit);
            if (it == buf + n) {
                strcat(buf, ".0");
            }
        }

        os_.put(buf);
        return true;
    }
    bool String(std::string_view s)
    {
        prefix(TYPE_STRING);
        os_.put('"');
        for (auto c: s) {
            auto u = static_cast<unsigned>(c);
            switch (u) {
                case '\"': os_.put("\\\""); break;
                case '\b': os_.put("\\b");  break;
                case '\f': os_.put("\\f");  break;
                case '\n': os_.put("\\n");  break;
                case '\r': os_.put("\\r");  break;
                case '\t': os_.put("\\t");  break;
                case '\\': os_.put("\\\\"); break;
                default:
                    if (u < 0x20) {
                        char buf[7];
                        snprintf(buf, 7, "\\u%04X", u);
                        os_.put(buf);
                    }
                    else os_.put(c);
                    break;
            }
        }

        os_.put('"');
        return true;
    }
    bool StartObject()
    {
        prefix(TYPE_OBJECT);
        stack_.emplace_back(false);
        os_.put('{');
        return true;
    }
    bool Key(std::string_view s)
    {
        prefix(TYPE_STRING);
        os_.put('"');
        os_.put(s);
        os_.put('"');
        return true;
    }
    bool EndObject()
    {
        assert(!stack_.empty());
        assert(!stack_.back().inArray);
        stack_.pop_back();
        os_.put('}');
        return true;
    }
    bool StartArray()
    {
        prefix(TYPE_ARRAY);
        stack_.emplace_back(true);
        os_.put('[');
        return true;
    }
    bool EndArray()
    {
        assert(!stack_.empty());
        assert(stack_.back().inArray);
        stack_.pop_back();
        os_.put(']');
        return true;
    }

private:
    void prefix(ValueType type)
    {
        if (seeValue_)
            assert(!stack_.empty() && "root not singular");
        else
            seeValue_ = true;

        if (stack_.empty())
            return;

        auto& top = stack_.back();
        if (top.inArray) {
            if (top.valueCount > 0)
                os_.put(',');
        }
        else {
            if (top.valueCount % 2 == 1)
                os_.put(':');
            else {
                assert(type == TYPE_STRING && "miss quotation mark");
                if (top.valueCount > 0)
                    os_.put(',');
            }
        }
        top.valueCount++;
    }

private:
    struct Level {
        explicit Level(bool inArray_):
                inArray(inArray_), valueCount(0)
        {}
        bool inArray; // in array or object
        int valueCount;
    };

private:
    std::vector<Level> stack_;
    WriteStream& os_;
    bool seeValue_;
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"

inline unsigned countDigits(uint32_t n)
{
    const uint32_t powers_of_10[] = {
            0,
            10,
            100,
            1000,
            10000,
            100000,
            1000000,
            10000000,
            100000000,
            1000000000
    };
    //
    // about magic number:
    //     http://graphics.stanford.edu/~seander/bithacks.html#IntegerLog10
    // __builtin_clz is gcc builtin:
    //     https://en.wikipedia.org/wiki/Bit_Manipulation_Instruction_Sets
    //     https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
    //
    uint32_t t = static_cast<uint32_t>((32 - __builtin_clz(n | 1)) * 1233 >> 12);
    return t - (n < powers_of_10[t]) + 1;
}

inline unsigned countDigits(uint64_t n)
{
    const uint64_t powers_of_10[] = {
            0,
            10,
            100,
            1000,
            10000,
            100000,
            1000000,
            10000000,
            100000000,
            1000000000,
            10000000000,
            100000000000,
            1000000000000,
            10000000000000,
            100000000000000,
            1000000000000000,
            10000000000000000,
            100000000000000000,
            1000000000000000000,
            10000000000000000000U
    };
    uint32_t t = (64 - __builtin_clzll(n | 1)) * 1233 >> 12;
    return t - (n < powers_of_10[t]) + 1;
}



template <typename T>
unsigned itoa_(T val, char* buf)
{
    static_assert(std::is_unsigned<T>::value, "must be unsigned integer");

    const char digits[201] =
            "0001020304050607080910111213141516171819"
            "2021222324252627282930313233343536373839"
            "4041424344454647484950515253545556575859"
            "6061626364656667686970717273747576777879"
            "8081828384858687888990919293949596979899";

    unsigned count = countDigits(val);
    unsigned next = count - 1;

    while (val >= 100) {
        unsigned i = (val % 100) * 2;
        val /= 100;
        buf[next] = digits[i + 1];
        buf[next - 1] = digits[i];
        next -= 2;
    }

    /* Handle last 1-2 digits. */
    if (val < 10) {
        buf[next] = '0' + val;
    }
    else {
        unsigned i = val * 2;
        buf[next] = digits[i + 1];
        buf[next - 1] = digits[i];
    }
    return count;
}
#pragma GCC diagnostic pop


}

namespace json::detail
{


inline unsigned itoa(int32_t val, char* buf)
{
    auto u = static_cast<uint32_t>(val);
    if (val < 0) {
        *buf++ = '-';
        u = ~u + 1;
    }
    return (val < 0) + itoa_(u, buf);
}


inline unsigned itoa(int64_t val, char* buf)
{
    auto u = static_cast<uint64_t>(val);
    if (val < 0) {
        *buf++ = '-';
        u = ~u + 1;
    }
    return (val < 0) + itoa_(u, buf);
}



}

#endif //TJSON_HANDLER_H
