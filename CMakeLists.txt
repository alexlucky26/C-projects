cmake_minimum_required(VERSION 3.31.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VERSION})

option(WITH_GOOGLE_TEST "Whether to build Google test" ON)

configure_file(version.h.in version.h)

add_executable(ip_filter_cli main.cpp)
#add_library(ip_filter lib.cpp)
add_library(ip_filter ip_filter.cpp)

set_target_properties(ip_filter_cli ip_filter PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(ip_filter
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(ip_filter_cli PRIVATE
    ip_filter
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

    add_executable(test_ip_filter test_ip_filter.cpp)

    target_include_directories(test_ip_filter PRIVATE
        ${googletest_SOURCE_DIR}/googletest/include
    )

    target_link_libraries(test_ip_filter PRIVATE
        gtest_main
        ip_filter
    )

    add_test(NAME test_ip_filter COMMAND test_ip_filter)
endif()

if (MSVC)
    target_compile_options(ip_filter_cli PRIVATE
        /W4
    )
    target_compile_options(ip_filter PRIVATE
        /W4
    )
    if(WITH_GOOGLE_TEST)
        target_compile_options(test_ip_filter PRIVATE
            /W4
        )
    endif()
else ()
    target_compile_options(ip_filter_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(ip_filter PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    if(WITH_GOOGLE_TEST)
        target_compile_options(test_ip_filter PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()

install(TARGETS ip_filter_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
