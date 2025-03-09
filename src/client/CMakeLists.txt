set(CLIENT_SOURCES main.cpp)

add_executable(chat_client ${CLIENT_SOURCES})

target_link_libraries(chat_client PRIVATE chat_common)


