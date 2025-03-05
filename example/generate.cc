#include <cstdio>

#include <hjson/Writer.h>
#include <hjson/FileWriteStream.h>

using namespace json;

int main()
{
    FileWriteStream os(stdout);
    Writer writer(os);

    printf("%d\n", INFINITY == INFINITY);

    writer.StartArray();
    writer.Double(INFINITY);
    writer.Double(NAN);
    writer.Bool(true);
    writer.String("\0蛤蛤蛤\0"sv);
    writer.Bool(true);
    writer.EndArray();
}