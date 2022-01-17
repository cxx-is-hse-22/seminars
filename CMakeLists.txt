cmake_minimum_required(VERSION 3.12.0)

project(Seminar02 CXX)
set(CMAKE_CXX_STANDARD 20)
include(cmake/StandardProjectSettings.cmake)
# Link this 'library' to set the c++ standard / compile-time options requested
add_library(default INTERFACE)
target_compile_features(default INTERFACE cxx_std_17)

include(cmake/CompilerWarnings.cmake)
set_project_warnings(default)

include(FetchContent)
FetchContent_Declare(
  fmt
  URL https://github.com/fmtlib/fmt/releases/download/8.1.1/fmt-8.1.1.zip
  URL_HASH
    SHA256=23778bad8edba12d76e4075da06db591f3b0e3c6c04928ced4a7282ca3400e5d)
FetchContent_MakeAvailable(fmt)

enable_testing()
add_subdirectory(external/doctest)
include(external/doctest/doctest.cmake)
add_subdirectory(src)
