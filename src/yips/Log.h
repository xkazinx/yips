#pragma once

enum class LogType
{
	Debug,
	Error,
	Out,
};

// One line to the console, coloured by its type (rang, kazilib)
void LogLine(const LogType type, const string & text);

namespace Log
{
	template<typename ... Args>
	void Debug(std::format_string<Args...> fs, Args && ... args)
	{
		LogLine(LogType::Debug, std::format(fs, std::forward<Args>(args)...));
	}

	template<typename ... Args>
	void Error(std::format_string<Args...> fs, Args && ... args)
	{
		LogLine(LogType::Error, std::format(fs, std::forward<Args>(args)...));
	}
}

template<typename ... Args>
void Print(std::format_string<Args...> fs, Args && ... args)
{
	LogLine(LogType::Out, std::format(fs, std::forward<Args>(args)...));
}

template<typename ... Args>
void PrintDebug(std::format_string<Args...> fs, Args && ... args)
{
	LogLine(LogType::Debug, std::format(fs, std::forward<Args>(args)...));
}

template<typename ... Args>
void PrintError(std::format_string<Args...> fs, Args && ... args)
{
	LogLine(LogType::Error, std::format(fs, std::forward<Args>(args)...));
}
