#pragma once

enum class LogType
{
	Debug,
	Error,
	Out,
};

template<LogType T>
class ConsoleOut
{
public:
	static inline thread_local string kDummy;

	using Type = LogType;

	ConsoleOut(string & str) :
		_str(str), _ptr(nullptr)
	{
	}

	ConsoleOut(const char * str) :
		_str(kDummy), _ptr(str)
	{
	}

	~ConsoleOut();

	string &     _str;
	const char * _ptr;
};

namespace Log
{
	using Type = LogType;

	template<typename ... Args>
	void Debug(std::_Fmt_string<Args...> fs, Args && ...args)
	{
		ConsoleOut<Type::Debug>(std::vformat(fs._Str, std::make_format_args(args...)).c_str());
	}

	template<typename ... Args>
	void Error(std::_Fmt_string<Args...> fs, Args && ...args)
	{
		ConsoleOut<Type::Error>(std::vformat(fs._Str, std::make_format_args(args...)).c_str());
	}
}

template<typename ... Args>
void Print(std::_Fmt_string<Args...> fs, Args && ...args)
{
	ConsoleOut<LogType::Out>(std::vformat(fs._Str, std::make_format_args(args...)).c_str());
}

template<typename ... Args>
void PrintDebug(std::_Fmt_string<Args...> fs, Args && ...args)
{
	ConsoleOut<LogType::Out>(std::vformat(fs._Str, std::make_format_args(args...)).c_str());
}

template<typename ... Args>
void PrintError(std::_Fmt_string<Args...> fs, Args && ...args)
{
	ConsoleOut<LogType::Out>(std::vformat(fs._Str, std::make_format_args(args...)).c_str());
}