cmake_minimum_required(VERSION 3.15)
project(libasync LANGUAGES CXX)

file(GLOB LIB_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_library(libasync SHARED ${LIB_SOURCES})

target_include_directories(libasync
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_definitions(libasync PRIVATE LIBASYNC_BUILD)
target_compile_features(libasync PUBLIC cxx_std_20)
set_target_properties(libasync PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    VERSION 1.0
    SOVERSION 1
)

install(TARGETS libasync
        EXPORT libasyncTargets
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT libasyncTargets
        FILE libasync-config.cmake
        NAMESPACE libasync::
        DESTINATION lib/cmake/libasync)
