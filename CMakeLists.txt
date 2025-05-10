cmake_minimum_required(VERSION 3.31.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(bulk_lib VERSION ${PROJECT_VERSION})

option(WITH_GOOGLE_TEST "Whether to build Google test" OFF)

configure_file(version.h.in version.h)

add_executable(bulk main.cpp)
add_library(bulk_lib BulkParser.cpp)

set_target_properties(bulk bulk_lib PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(bulk_lib
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(bulk PRIVATE
    bulk_lib
)

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
        bulk_lib
    )

    add_test(NAME test_version COMMAND test_version)
endif()

if (MSVC)
    target_compile_options(bulk PRIVATE
        /W4
    )
    target_compile_options(bulk_lib PRIVATE
        /W4
    )
    if(WITH_GOOGLE_TEST)
        target_compile_options(test_version PRIVATE
            /W4
        )
    endif()
else ()
    target_compile_options(bulk PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(bulk_lib PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    if(WITH_GOOGLE_TEST)
        target_compile_options(test_version PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()

install(TARGETS bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
