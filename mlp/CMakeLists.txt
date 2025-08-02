add_executable(mlp main.cpp helpers.cpp mlp_classifier.cpp)

# Указываем путь к заголовочным файлам eigen
target_include_directories(mlp PRIVATE
    ${CMAKE_SOURCE_DIR}/eigen-3.4.0
)
