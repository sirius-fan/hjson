# add_library(hjson STATIC
#         Exception.h
#         FileReadStream.h FileWriteStream.h
#         StringReadStream.h StringWriteStream.h
#         Reader.h
#         Writer.h
#         Value.h
#         Document.h
#         noncopyable.h
#         PrettyWriter.h)
# install(TARGETS hjson DESTINATION lib)

# set(HEADERS
#         Document.h
#         Exception.h
#         FileReadStream.h
#         FileWriteStream.h
#         noncopyable.h
#         PrettyWriter.h
#         Reader.h
#         StringReadStream.h
#         StringWriteStream.h
#         Value.h
#         Writer.h
#         )
# install(FILES ${HEADERS} DESTINATION include)

# 创建接口库而不是静态库
add_library(hjson INTERFACE)

# 设置头文件目录
target_include_directories(hjson INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# 安装头文件
set(HEADERS
    Document.h
    Exception.h
    FileReadStream.h
    FileWriteStream.h
    noncopyable.h
    PrettyWriter.h
    Reader.h
    StringReadStream.h
    StringWriteStream.h
    Value.h
    Writer.h
)

install(FILES ${HEADERS} DESTINATION include/hjson)
install(TARGETS hjson EXPORT hjson-targets
    INCLUDES DESTINATION include)