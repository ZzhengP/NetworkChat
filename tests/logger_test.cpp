#include "common/logger.h"
#include <cassert>
#include <fstream>
#include <string>
#include <filesystem>

bool fileContains(const std::string &filename, const std::string &text)
{
  std::ifstream file(filename);
  std::string line;
  while (std::getline(file, line))
  {
    if (line.find(text) != std::string::npos)
    {
      return true;
    }
  }
  return false;
}

int main()
{
  const std::string testLogFile = "test_logger.log";

  // Clean up any existing log file
  std::filesystem::remove(testLogFile);

  auto &logger = chat::Logger::getInstance();
  logger.setLogLevel(chat::LogLevel::DEBUG);
  logger.setLogFile(testLogFile);

  LOG_INFO("Test message with number: {}", 123);
  LOG_ERROR("Test error with text: {}", "something went wrong");

  // Verify log file exists and contains the expected content
  assert(std::filesystem::exists(testLogFile));
  assert(fileContains(testLogFile, "Test message with number: 123"));
  assert(fileContains(testLogFile, "Test error with text: something went wrong"));

  std::cout << "Logger test passed successfully!" << std::endl;
  return 0;
}