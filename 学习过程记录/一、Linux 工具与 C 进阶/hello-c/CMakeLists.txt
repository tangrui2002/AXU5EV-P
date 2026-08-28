cmake_minimum_required(VERSION 3.16)

project(hello_c VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(math_utils STATIC src/math_utils.c)
target_include_directories(math_utils PUBLIC include)

add_executable(hello
    src/main.c
)

target_link_libraries(hello PRIVATE math_utils)

