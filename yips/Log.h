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
	void Debug(Args && ...args)
	{
		ConsoleOut<Type::Debug>(format(forward<Args>(args)...).c_str());
	}

	template<typename ... Args>
	void Error(Args && ...args)
	{
		ConsoleOut<Type::Error>(format(forward<Args>(args)...).c_str());
	}
}

template<typename ... Args>
void Print(Args && ...args)
{
	ConsoleOut<LogType::Out>(format(forward<Args>(args)...).c_str());
}

template<typename ... Args>
void PrintDebug(Args && ...args)
{
	ConsoleOut<LogType::Out>(format(forward<Args>(args)...).c_str());
}

template<typename ... Args>
void PrintError(Args && ...args)
{
	ConsoleOut<LogType::Out>(format(forward<Args>(args)...).c_str());
}