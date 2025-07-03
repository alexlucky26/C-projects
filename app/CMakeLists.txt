# app/CMakeLists.txt
add_executable(sample main.cpp)
target_link_libraries(sample PRIVATE libasync)
#target_include_directories(sample
#    PRIVATE ${CMAKE_SOURCE_DIR}/libasync/include)
#чтобы запаковало и /bin/sample как отдельную библиотеку, а не один монолитный deb
install(TARGETS sample RUNTIME DESTINATION bin)