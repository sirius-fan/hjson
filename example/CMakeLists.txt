add_executable(parse parse.cc)
target_link_libraries(parse hjson)

add_executable(generate generate.cc)
target_link_libraries(generate hjson)

add_executable(print_document print_document.cc)
target_link_libraries(print_document hjson)

add_executable(add_one add_one.cc sample.h)
target_link_libraries(add_one hjson)

add_executable(pretty pretty.cc)
target_link_libraries(pretty hjson)

add_executable(write_document write_document.cc)
target_link_libraries(write_document hjson)

add_executable(struct struct.cc)
target_link_libraries(struct hjson)

# 新增的示例
add_executable(dom_example dom_example.cc)
target_link_libraries(dom_example hjson)

add_executable(sax_example sax_example.cc)
target_link_libraries(sax_example hjson)

add_executable(stream_example stream_example.cc)
target_link_libraries(stream_example hjson)

add_executable(performance_example performance_example.cc)
target_link_libraries(performance_example hjson)
