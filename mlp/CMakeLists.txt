add_executable(mlp main.cpp helpers.cpp mlp_classifier.cpp)
install(TARGETS mlp DESTINATION bin)
target_include_directories(mlp PRIVATE
    ${CMAKE_SOURCE_DIR}/eigen-3.4.0
)