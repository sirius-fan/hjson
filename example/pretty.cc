#include <hjson/Document.h>
#include <hjson/PrettyWriter.h>
#include <hjson/FileWriteStream.h>

#include "sample.h"

using namespace json;

int main()
{
    Document document;
    ParseError err = document.parse(sample[1]);
    if (err != PARSE_OK) {
        puts(parseErrorStr(err));
        return 1;
    }

    FileWriteStream os(stdout);
    PrettyWriter writer(os);
    document.writeTo(writer);
}