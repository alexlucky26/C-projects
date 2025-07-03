# app/CMakeLists.txt
add_executable(sample main.cpp)
target_link_libraries(sample PRIVATE async)

install(TARGETS sample
        RUNTIME DESTINATION bin
        COMPONENT runtime)