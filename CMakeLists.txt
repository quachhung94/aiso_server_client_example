cmake_minimum_required(VERSION 3.10.2)

project(cpp_sample LANGUAGES C CXX VERSION 1.0.0)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" "${CMAKE_CURRENT_SOURCE_DIR}/Logging/spdlog/include")
set($ENV{CMAKE_BUILD_PARALLEL_LEVEL} 4)

# Compiler features
include(SetCompilerFeatures)
include(SetCompilerWarnings)
include(SetPlatformFeatures)
include(SystemInformation)

# external
find_package(OpenSSL REQUIRED)
# module components
add_subdirectory("./modules")
# components
add_subdirectory("./PlatformInterface")
add_subdirectory("./IfRuntime")
add_subdirectory("./NetworkIf")
# add_subdirectory("./Service")
# add_subdirectory("./ServiceTLS")
# add_subdirectory("./Client")
add_subdirectory("./ClientTLS")
add_subdirectory("./lib")
add_subdirectory("./Logging/spdlog")
add_subdirectory("./App")