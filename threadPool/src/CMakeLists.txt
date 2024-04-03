aux_source_directory(. SRCLIST)
set(CMAKE_BUILD_TYPE "Debug")

# 指定生成可执行文件
add_executable(threadPool ${SRCLIST})
target_link_libraries(threadPool pthread)

