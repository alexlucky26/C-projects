add_executable(sample main.cpp)
target_link_libraries(sample PRIVATE async)

# --- устанавливаем только бинарь ---------------------------------------------
install(TARGETS sample
        RUNTIME DESTINATION bin)