#include <hjson/Document.h>
#include <hjson/Writer.h>
#include <hjson/FileWriteStream.h>

#include "sample.h"

using namespace json;

int main()
{
    Document document;
    ParseError err = document.parse(sample[0]);
    if (err != PARSE_OK) {
        puts(parseErrorStr(err));
        return 1;
    }

    FileWriteStream os(stdout);
    Writer writer(os);
    document.writeTo(writer);
}