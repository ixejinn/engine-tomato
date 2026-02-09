#ifndef TOMATO_LOGGER_H
#define TOMATO_LOGGER_H

#include <iostream>
#include <sstream>
#include <string>
#include <source_location>

class Logger
{
public:
	enum Level { LOG, WARN, ERR };

	Logger(Level level, std::source_location loc = std::source_location::current())
    : level_(level), location_(loc) {}

	template<typename T>
	Logger& operator<<(const T& msg)
	{
		buffer_ << msg;
		return *this;
	}

	~Logger()
	{
        std::cout << Prefix(level_) << buffer_.str();

        if (level_ > Level::LOG)
            std::cout << "\n"
            << location_.file_name() << ":" << location_.line()
            << ": " << location_.function_name();
        std::cout << "\n";
	}

private:
    static const char* Prefix(Level lv)
    {
        switch (lv)
        {
            case Level::LOG:  return "[LOG] ";
            case Level::WARN: return "[WARN] ";
            case Level::ERR:  return "[ERR] ";
        }
        return "";
    }

	Level level_;
    std::source_location location_;

	std::ostringstream buffer_;
};

#define TMT_LOG  Logger(Logger::LOG)
#define TMT_WARN Logger(Logger::WARN)
#define TMT_ERR  Logger(Logger::ERR)

#endif //TOMATO_LOGGER_H