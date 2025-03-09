#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace chat
{

  enum class LogLevel
  {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
  };

  class Logger
  {
  public:
    static Logger &getInstance()
    {
      static Logger instance;
      return instance;
    }

    void setLogLevel(LogLevel level)
    {
      m_logLevel = level;
    }

    void setLogFile(const std::string &filename)
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      if (m_fileStream.is_open())
      {
        m_fileStream.close();
      }
      m_fileStream.open(filename, std::ios::app);
    }

    template <typename... Args>
    void debug(const std::string &format, Args... args)
    {
      log(LogLevel::DEBUG, format, args...);
    }

    template <typename... Args>
    void info(const std::string &format, Args... args)
    {
      log(LogLevel::INFO, format, args...);
    }

    template <typename... Args>
    void warning(const std::string &format, Args... args)
    {
      log(LogLevel::WARNING, format, args...);
    }

    template <typename... Args>
    void error(const std::string &format, Args... args)
    {
      log(LogLevel::ERROR, format, args...);
    }

    template <typename... Args>
    void fatal(const std::string &format, Args... args)
    {
      log(LogLevel::FATAL, format, args...);
    }

  private:
    Logger() : m_logLevel(LogLevel::INFO) {}
    ~Logger()
    {
      if (m_fileStream.is_open())
      {
        m_fileStream.close();
      }
    }

    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    template <typename... Args>
    void log(LogLevel level, const std::string &format, Args... args)
    {
      if (level < m_logLevel)
        return;

      std::string formattedMessage = formatString(format, args...);
      std::string levelStr = logLevelToString(level);
      std::string timestamp = getCurrentTimestamp();

      std::string logMessage = timestamp + " [" + levelStr + "] " + formattedMessage;

      std::lock_guard<std::mutex> lock(m_mutex);
      std::cout << logMessage << std::endl;

      if (m_fileStream.is_open())
      {
        m_fileStream << logMessage << std::endl;
      }
    }

    template <typename T, typename... Args>
    std::string formatString(const std::string &format, T value, Args... args)
    {
      size_t pos = format.find("{}");
      if (pos == std::string::npos)
      {
        return format;
      }

      std::ostringstream ss;
      ss << value;
      std::string newFormat = format.substr(0, pos) + ss.str() + format.substr(pos + 2);
      return formatString(newFormat, args...);
    }

    std::string formatString(const std::string &format)
    {
      return format;
    }

    std::string logLevelToString(LogLevel level)
    {
      switch (level)
      {
      case LogLevel::DEBUG:
        return "DEBUG";
      case LogLevel::INFO:
        return "INFO";
      case LogLevel::WARNING:
        return "WARNING";
      case LogLevel::ERROR:
        return "ERROR";
      case LogLevel::FATAL:
        return "FATAL";
      default:
        return "UNKNOWN";
      }
    }

    std::string getCurrentTimestamp()
    {
      auto now = std::chrono::system_clock::now();
      auto time = std::chrono::system_clock::to_time_t(now);
      auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                    now.time_since_epoch()) %
                1000;

      std::stringstream ss;
      ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
      ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
      return ss.str();
    }

    LogLevel m_logLevel;
    std::ofstream m_fileStream;
    std::mutex m_mutex;
  };

#define LOG_DEBUG(...) chat::Logger::getInstance().debug(__VA_ARGS__)
#define LOG_INFO(...) chat::Logger::getInstance().info(__VA_ARGS__)
#define LOG_WARNING(...) chat::Logger::getInstance().warning(__VA_ARGS__)
#define LOG_ERROR(...) chat::Logger::getInstance().error(__VA_ARGS__)
#define LOG_FATAL(...) chat::Logger::getInstance().fatal(__VA_ARGS__)

} // namespace chat