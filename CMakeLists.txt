cmake_minimum_required(VERSION 3.31.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(print_ip VERSION ${PROJECT_VERSION})

option(WITH_GOOGLE_TEST "Whether to build Google test" OFF)

configure_file(version.h.in version.h)

add_executable(print_ip main.cpp)

# библиотека как INTERFACE, так как реализация в заголовочном файле
add_library(print_ip_lib INTERFACE)

target_sources(print_ip_lib INTERFACE print_ip_lib.hpp)
target_include_directories(print_ip_lib INTERFACE 
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_BINARY_DIR}"
)

set_target_properties(print_ip PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(print_ip PRIVATE print_ip_lib)

if (WITH_GOOGLE_TEST)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(test_version test_version.cpp)

    target_include_directories(test_version PRIVATE
        ${googletest_SOURCE_DIR}/googletest/include
    )

    target_link_libraries(test_version PRIVATE
        gtest_main
        print_ip_lib
    )

    add_test(NAME test_version COMMAND test_version)
endif()

if (MSVC)
    add_compile_options(/Zc:__cplusplus)
    target_compile_options(print_ip PRIVATE /W4)
    
    if(WITH_GOOGLE_TEST)
        target_compile_options(test_version PRIVATE /W4)
    endif()
else()
    target_compile_options(print_ip PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    
    if(WITH_GOOGLE_TEST)
        target_compile_options(test_version PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()

install(TARGETS print_ip RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)