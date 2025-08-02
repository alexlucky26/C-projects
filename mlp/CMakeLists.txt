add_executable(mlp main.cpp helpers.cpp mlp_classifier.cpp)

target_include_directories(mlp PRIVATE
    ${CMAKE_SOURCE_DIR}/eigen-3.4.0
)