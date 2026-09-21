cmake_minimum_required(VERSION 3.20)

project(C_Database LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(C_Database src/main.c)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(C_Database PRIVATE -Wall -Wextra -Wpedantic)
endif()
