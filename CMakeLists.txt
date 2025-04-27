cmake_minimum_required(VERSION 3.31.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})
project(editor VERSION ${PROJECT_VERSION})

option(WITH_GOOGLE_TEST "Whether to build Google test" OFF)

configure_file(version.h.in version.h)

set(SOURCES
    main.cpp
    Processor.cpp
)

set(HEADERS
    Processor.h
    BasicTypes.h
    Primitive.h
    views/FileDialog.h
    views/Viewport.h
)

add_executable(editor_cli ${SOURCES})
add_library(editor STATIC ${SOURCES} ${HEADERS})

set_target_properties(editor_cli editor PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# пути для заголовков
target_include_directories(editor
    PRIVATE 
    "${CMAKE_BINARY_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_include_directories(editor_cli
    PRIVATE 
    "${CMAKE_BINARY_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(editor_cli PRIVATE editor)

# здесь настройка Google Test
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
        "${CMAKE_CURRENT_SOURCE_DIR}"
    )

    target_link_libraries(test_version PRIVATE
        gtest_main
        editor
    )

    add_test(NAME test_version COMMAND test_version)
endif()

if (MSVC)
    target_compile_options(editor_cli PRIVATE /W4)
    target_compile_options(editor PRIVATE /W4)
    if(WITH_GOOGLE_TEST)
        target_compile_options(test_version PRIVATE /W4)
    endif()
else()
    target_compile_options(editor_cli PRIVATE -Wall -Wextra -pedantic -Werror)
    target_compile_options(editor PRIVATE -Wall -Wextra -pedantic -Werror)
    if(WITH_GOOGLE_TEST)
        target_compile_options(test_version PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()
endif()

# установка
install(TARGETS editor_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)