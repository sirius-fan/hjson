#include <cstdio>
#include <hjson/Reader.h>
#include <hjson/Writer.h>
#include <hjson/FileReadStream.h>
#include <hjson/FileWriteStream.h>

using namespace json;

int main()
{
    FileReadStream is(stdin);
    FileWriteStream os(stdout);
    Writer writer(os);

    ParseError err = Reader::parse(is, writer);
    if (err != PARSE_OK) {
        printf("%s\n", parseErrorStr(err));
    }
}