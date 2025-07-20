add_executable(join_server main.cpp)
add_library(bulk_server  BulkServer.cpp)
add_library(command_processor  CommandProcessor.cpp)
add_library(database  Database.cpp)

#target_link_libraries(bulk_server_cli PRIVATE async)

target_link_libraries(command_processor PRIVATE database)

target_link_libraries(bulk_server PRIVATE 
    command_processor
    Boost::filesystem
    Boost::asio
)

target_link_libraries(join_server PRIVATE
    bulk_server
    Boost::filesystem
    Boost::asio
)

#target_include_directories(bulk_server_cli PRIVATE ${Boost_INCLUDE_DIRS})
#target_include_directories(bulk_server PRIVATE ${Boost_INCLUDE_DIRS})

# --- устанавливаем только бинарь ---------------------------------------------
install(TARGETS join_server
        RUNTIME DESTINATION bin)