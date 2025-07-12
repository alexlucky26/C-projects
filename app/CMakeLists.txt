add_executable(bulk_server_cli main.cpp)
add_library(bulk_server  BulkServer.cpp)

#target_link_libraries(bulk_server_cli PRIVATE async)

target_link_libraries(bulk_server PRIVATE 
    async
    Boost::filesystem
    Boost::asio
)

target_link_libraries(bulk_server_cli PRIVATE
    bulk_server
    async
    Boost::filesystem
    Boost::asio
)

#target_include_directories(bulk_server_cli PRIVATE ${Boost_INCLUDE_DIRS})
#target_include_directories(bulk_server PRIVATE ${Boost_INCLUDE_DIRS})

# --- устанавливаем только бинарь ---------------------------------------------
install(TARGETS bulk_server_cli
        RUNTIME DESTINATION bin)