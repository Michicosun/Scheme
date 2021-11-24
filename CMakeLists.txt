project("scheme")
cmake_minimum_required(VERSION 3.0)

file(GLOB SOURCES "*.cpp")
add_library(scheme ${SOURCES})

add_executable(scheme-repl repl/main.cpp)
target_link_libraries(scheme-repl scheme)
