# app/CMakeLists.txt
add_executable(sample main.cpp)
target_link_libraries(sample PRIVATE libasync)
#target_include_directories(sample
#    PRIVATE ${CMAKE_SOURCE_DIR}/libasync/include)