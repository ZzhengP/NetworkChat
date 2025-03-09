set(SERVER_SOURCES main.cpp)

add_executable(chat_server ${SERVER_SOURCES})

target_link_libraries(chat_server PRIVATE chat_common)

