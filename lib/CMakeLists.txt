cmake_minimum_required(VERSION 3.10)

project(platform)

set(CMAKE_CXX_STANDARD 17)
include(files.cmake)
include(compiler.cmake)

include_directories(SYSTEM "${CMAKE_CURRENT_LIST_DIR}/include" ${INCLUDE_DIRECTORIES} )


add_library(${PROJECT_NAME} ${PLATFORM_SOURCES})
target_link_libraries(${PROJECT_NAME} crypto sodium spdlog)
set_compile_options(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
            ${PLATFORM_LIB_ROOT}/platform/include
        )
