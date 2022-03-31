project(TLSServer_lib VERSION 1.0.0)

file(GLOB_RECURSE src_files LIST_DIRECTORIES false CONFIGURE_DEPENDS "*.cpp" "*.h")

add_library(TLSSERVER SHARED ${src_files})

target_include_directories(TLSSERVER PUBLIC "./include" "${CMAKE_CURRENT_SOURCE_DIR}/../../Logging/spdlog/include")

target_link_libraries(TLSSERVER
PUBLIC cppcommon)
