#include "common/logger.h"
#include <filesystem>
int main()
{
  std::filesystem::path log_path = std::filesystem::current_path() / "logs";
  if (!std::filesystem::exists(log_path))
  {
    std::filesystem::create_directory(log_path);
  }

  auto &logger = chat::Logger::getInstance();
  logger.setLogLevel(chat::LogLevel::DEBUG);
  logger.setLogFile("server.log");

  LOG_INFO("Server starting up");
  LOG_DEBUG("Debug information: {}", 42);
  LOG_WARNING("Warning: {} happened at {}", "something", "somewhere");
  LOG_ERROR("Error occurred: {}", "connection refused");

  return 0;
}