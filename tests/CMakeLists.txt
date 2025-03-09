# Create a test executable
add_executable(logger_test logger_test.cpp)

# Link with the common library
target_link_libraries(logger_test PRIVATE chat_common)

# Register the test with CTest
add_test(NAME LoggerTest COMMAND logger_test)