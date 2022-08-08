#include "pch.h"
#include "log.h"
#include "rang.hpp"
#include <iostream>

template<LogType T>
ConsoleOut<T>::~ConsoleOut()
{
	// #todo #log Log into files what's printed for release mode if enabled for debugging

	using enum rang::fg;

	switch(T)
	{
	case Type::Debug:
		std::cout << cyan << "Debug: ";
		break;
	case Type::Error:
		std::cout << red << "Error: ";
		break;
	case Type::Out:
		std::cout << green;
		break;
	}

	std::cout << (const char*)(_ptr ? _ptr : _str.c_str()) << "\n";
}

template ConsoleOut<LogType::Debug>::~ConsoleOut<LogType::Debug>();
template ConsoleOut<LogType::Error>::~ConsoleOut<LogType::Error>();
template ConsoleOut<LogType::Out>::~ConsoleOut<LogType::Out>();