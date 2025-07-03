cmake_minimum_required(VERSION 3.15)
project(async LANGUAGES CXX)

file(GLOB LIB_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# имя таргета async файл libasync.so
add_library(async SHARED ${LIB_SOURCES})

target_include_directories(async
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_definitions(async PRIVATE LIBASYNC_BUILD)
target_compile_features(async PUBLIC cxx_std_20)

set_target_properties(async PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    VERSION     1.0        # libasync.so.1.0
    SOVERSION   1)         # SONAME=libasync.so.1

# --- устанавливаем только .so -------------------------------------------------
install(TARGETS async
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib)
