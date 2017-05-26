#include "Logger.h"


namespace ph
{
	std::string Logger::formMessageBase()
	{
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

		std::string f = "[";
		if (cLevel == INF) { f.append("INF"); }
		if (cLevel == WRN) { f.append("WRN"); }
		if (cLevel == ERR) { f.append("ERR"); }
		if (cLevel == FTL) { f.append("FTL"); }
		if (cLevel == WIN) { f.append("WIN"); }
		f += ']';

		time_t tt = std::chrono::system_clock::to_time_t(now);
		tm utc_tm = *gmtime(&tt);
		tm local_tm = *localtime(&tt);
		f += '(';
		/*f.append(std::to_string(utc_tm.tm_year + 1900));
		f += '-';
		f.append(std::to_string(utc_tm.tm_mon + 1));
		f += '-';
		f.append(std::to_string(utc_tm.tm_mday));
		f += '|';
		*/
		f.append(std::to_string(utc_tm.tm_hour));
		f += ':';
		f.append(std::to_string(utc_tm.tm_min));
		f += ':';
		f.append(std::to_string(utc_tm.tm_sec));
		f += ')';
		f += ' ';

		return f;
	}

	Logger& Logger::operator<<(const std::string& msg)
	{
		std::string f = formMessageBase();

		f.append(msg);
		f += '\n';

		for (std::ostream* s : streams)
		{
			if (s == &std::cout)
			{
				// Special for this...
				std::string ff = formMessageBase();
				*s << ff;

				if (cLevel == INF)
				{
					*s << rang::fg::gray << msg << "\n";
				}
				else if (cLevel == WRN)
				{
					*s << rang::fg::yellow << msg << "\n";
				}
				else if (cLevel == ERR)
				{
					*s << rang::fg::red << msg << "\n";
				}
				else if (cLevel == FTL)
				{
					*s << rang::fg::yellow << rang::bg::red << msg << "\n";
				}
				else if (cLevel == WIN)
				{
					*s << rang::fg::green << msg << "\n";
				}

				*s << rang::style::reset;
			}
			else
			{ 
				*s << f;
			}
		}

		return *this;
	}
	Logger& Logger::operator()(LogLevel verb)
	{
		cLevel = verb;
		return *this;
	}
	Logger::Logger()
	{
		streams.push_back(&std::cout);
	}


	Logger::~Logger()
	{
	}

}