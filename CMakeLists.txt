cmake_minimum_required(VERSION 3.15)

# КУДА класть exe и dll
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY         ${CMAKE_BINARY_DIR}/bin CACHE PATH "runtime")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CACHE PATH "runtime")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CACHE PATH "runtime")

project(async_demo LANGUAGES CXX)

# и создаём таргеты
add_subdirectory(libasync)
add_subdirectory(app)