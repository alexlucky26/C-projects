add_executable(join_client main.cpp)
add_library(bulk_client  BulkClient.cpp)

target_link_libraries(bulk_client PRIVATE 
    Boost::filesystem
    Boost::asio
)

target_link_libraries(join_client PRIVATE
    bulk_client
    Boost::filesystem
    Boost::asio
)

# --- устанавливаем только бинарь ---------------------------------------------
install(TARGETS join_client
        RUNTIME DESTINATION bin)