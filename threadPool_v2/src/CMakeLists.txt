aux_source_directory(. SRCLIST)
set(CMAKE_BUILD_TYPE "Debug")

# 生成动态链接库
add_library(so SHARED ${SRCLIST})
set_target_properties(so PROPERTIES
        OUTPUT_NAME threadpool
        PREFIX Lib
        SUFFIX .so)
SET_TARGET_PROPERTIES(so PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# 生成静态库
add_library(a STATIC ${SRCLIST})
set_target_properties(a PROPERTIES
        OUTPUT_NAME threadpool
        PREFIX Lib
        SUFFIX .a)
SET_TARGET_PROPERTIES(a PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# 指定生成可执行文件
add_executable(threadPool ${SRCLIST})
target_link_libraries(threadPool pthread)

