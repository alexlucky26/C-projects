cmake_minimum_required(VERSION 3.15)
project(async LANGUAGES CXX)

file(GLOB LIB_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_library(async SHARED ${LIB_SOURCES})

target_include_directories(async
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>                             # НЕ будет устанавливаться
)

target_compile_definitions(async PRIVATE LIBASYNC_BUILD)
target_compile_features(async PUBLIC cxx_std_20)

set_target_properties(async PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    VERSION 1.0
    SOVERSION 1
)

install(TARGETS async
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        COMPONENT lib)
