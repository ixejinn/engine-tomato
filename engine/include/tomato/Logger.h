#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <sstream>
#include <string>

class Logger
{
public:
	enum Level { LOG, WARN, ERR };

	Logger(Level level, const char* file, int line) : level_(level), file_(file), line_(line) {}

	template<typename T>
	Logger& operator<<(const T& msg)
	{
		buffer_ << msg;
		return *this;
	}

	~Logger()
	{
		switch (level_)
		{
			case LOG: std::cout << "[LOG] "; break;
			case WARN: std::cout << "[WARN] " << "(" << file_ << " : " << line_ << ") "; break;
			case ERR: std::cout << "[ERR] " << "(" << file_ << " : " << line_ << ") "; break;
		}
		std::cout << buffer_.str() << std::endl;
	}

private:
	Level level_;
	const char* file_;
	int line_;
	std::ostringstream buffer_;
};

#define TMT_LOG Logger(Logger::LOG, __FILE__, __LINE__)
#define TMT_ERR Logger(Logger::ERR, __FILE__, __LINE__)

#endif