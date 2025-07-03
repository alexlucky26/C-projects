cmake_minimum_required(VERSION 3.15)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY         ${CMAKE_BINARY_DIR}/bin CACHE PATH "runtime")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CACHE PATH "runtime")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CACHE PATH "runtime")

project(async_demo LANGUAGES CXX)

add_subdirectory(libasync)
add_subdirectory(app)

# --------------- CPack --------------------------------------------------------
set(CPACK_PACKAGE_NAME        "async_demo")
set(CPACK_PACKAGE_VERSION     ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT     "alex_mock_email@gmail.com")
set(CPACK_GENERATOR           "DEB")

# компоненты - отдельные .deb
set(CPACK_COMPONENTS_ALL      runtime lib)
set(CPACK_DEB_COMPONENT_INSTALL ON)

set(CPACK_DEBIAN_RUNTIME_PACKAGE_NAME "async-demo")
set(CPACK_DEBIAN_LIB_PACKAGE_NAME     "libasync1")

# runtime зависит от lib
set(CPACK_DEBIAN_RUNTIME_PACKAGE_DEPENDS "libasync1 (>= ${PROJECT_VERSION})")

include(CPack)