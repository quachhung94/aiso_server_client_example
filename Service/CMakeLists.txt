# Link libraries
list(APPEND LINKLIBS ${OPENSSL_LIBRARIES})
list(APPEND LINKLIBS cppcommon)
list(APPEND LINKLIBS IFRuntime)
list(APPEND LINKLIBS PlatformInterface)
list(APPEND LINKLIBS TCP)
list(APPEND LINKLIBS TCPSERVER)
list(APPEND LINKLIBS TLS)
list(APPEND LINKLIBS UDP)

# OpenSSL libraries
message(STATUS "OpenSSL version: ${OPENSSL_VERSION} ${OPENSSL_INCLUDE_DIR} ${OPENSSL_LIBRARIES}")

# System directories
include_directories(SYSTEM "${CMAKE_CURRENT_LIST_DIR}/include" ${INCLUDE_DIRECTORIES} "${CMAKE_CURRENT_SOURCE_DIR}/../Logging/spdlog/include")

# # Library
# file(GLOB_RECURSE LIB_HEADER_FILES "include/*.h")
# file(GLOB_RECURSE LIB_INLINE_FILES "include/*.inl")
# file(GLOB_RECURSE LIB_SOURCE_FILES "source/*.cpp")
# add_library(cppserver ${LIB_HEADER_FILES} ${LIB_INLINE_FILES} ${LIB_SOURCE_FILES})
# set_target_properties(cppserver PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS}" FOLDER "libraries")
# target_include_directories(cppserver PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
# target_link_libraries(cppserver ${LINKLIBS} asio)
# list(APPEND INSTALL_TARGETS cppserver)
# list(APPEND LINKLIBS cppserver)


# client
file(GLOB EXAMPLE_HEADER_FILES "include/*.h")
# file(GLOB EXAMPLE_INLINE_FILES "*.inl")
file(GLOB EXAMPLE_SOURCE_FILES "src/*.cpp")

add_executable(demo ${EXAMPLE_SOURCE_FILES})
# set_target_properties(tls_client PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS}" FOLDER "examples")
target_link_libraries(demo ${LINKLIBS})
