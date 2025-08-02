cmake_minimum_required(VERSION 3.31)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

# выходная директория для exe/so
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY         ${CMAKE_BINARY_DIR}/bin CACHE PATH "runtime")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CACHE PATH "runtime")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CACHE PATH "runtime")

project(mlp_project LANGUAGES CXX)

add_subdirectory(mlp)
#add_subdirectory(eigen-3.4.0)

# -------------------- CPack ---------------------------------------------------
set(CPACK_PACKAGE_NAME        "mlp_project")
set(CPACK_PACKAGE_VERSION     ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT     "alex_mock_email@gmail.com")
set(CPACK_GENERATOR           "DEB")

# one monolithic package (не используем компоненты)
include(CPack)

#enable_testing()

#add_executable(test_client test_client.cpp)
#target_link_libraries(test_client PRIVATE Boost::asio GTest::gtest GTest::gtest_main)
#target_include_directories(test_client PRIVATE ${Boost_INCLUDE_DIRS})

#add_test(NAME test_client COMMAND test_client)