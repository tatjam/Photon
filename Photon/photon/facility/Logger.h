#pragma once

#include <vector>



#include <iostream>
#include <string>
#include <ostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <vector>
#include <iomanip>

#include "../../dep/rang.h"

#define endl "$$END$$"

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

		std::stringstream* buff;

		void finish();

	public:

		LogLevel cLevel;

		std::string end();

		std::vector<std::ostream*> streams;

		//Logger& operator<<(Logger& op);

		Logger& operator<<(const std::string& msg);

		Logger& operator()(LogLevel level);

		Logger();
		~Logger();
	};

}
