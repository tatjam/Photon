#pragma once

#include <vector>



#include <iostream>
#include <string>
#include <ostream>
#include <chrono>
#include <ctime>

#include "../../dep/rang.h"

namespace ph
{

	enum LogLevel
	{
		// Info
		INF, 
		// Warning
		WRN, 
		// Error
		ERR,
		// Fatal Error (Non-reparable crash)
		FTL,
		// Green message, something succesful
		WIN,
	};

	class Logger
	{

		std::string formMessageBase();

	public:

		LogLevel cLevel;

		std::vector<std::ostream*> streams;

		Logger& operator<<(const std::string& msg);

		Logger& operator()(LogLevel level);

		Logger();
		~Logger();
	};

}
