#include "pch.h"
#include "Log.h"
#include <rang.hpp>
#include <iostream>

void LogLine(const LogType type, const string & text)
{
	// #todo #log Log into files what's printed for release mode if enabled for debugging

	switch (type)
	{
	case LogType::Debug:
		std::cout << rang::fg::cyan << "Debug: ";
		break;
	case LogType::Error:
		std::cout << rang::fg::red << "Error: ";
		break;
	case LogType::Out:
		std::cout << rang::fg::green;
		break;
	}

	std::cout << text << rang::style::reset << "\n";
}
