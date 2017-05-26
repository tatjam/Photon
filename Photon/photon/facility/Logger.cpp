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
		std::stringstream ss;
		ss << 
			std::setw(2) << std::setfill('0') << utc_tm.tm_hour <<
			":" << std::setw(2) << std::setfill('0') << utc_tm.tm_min << 
			":" << std::setw(2) << std::setfill('0') << utc_tm.tm_sec;
		f.append(ss.str());
		f += ')';
		f += ' ';

		return f;
	}
	/*Logger& Logger::operator<<(Logger& log)
	{
		
	}*/


	std::string Logger::end()
	{
		return endl;
	}

	void Logger::finish()
	{
		std::string f = formMessageBase();

		std::string msgs;
		msgs = buff->str();

		f.append(msgs);

		for (std::ostream* s : streams)
		{
			if (s == &std::cout)
			{
				// Special for this...
				std::string ff = formMessageBase();
				*s << ff;

				if (cLevel == INF)
				{
					*s << rang::fg::gray << msgs;
				}
				else if (cLevel == WRN)
				{
					*s << rang::fg::yellow << msgs;
				}
				else if (cLevel == ERR)
				{
					*s << rang::fg::red << msgs;
				}
				else if (cLevel == FTL)
				{
					*s << rang::fg::yellow << rang::bg::red << msgs;
				}
				else if (cLevel == WIN)
				{
					*s << rang::fg::green << msgs;
				}

				*s << rang::style::reset;
			}
			else
			{
				*s << f;
			}
		}

	}

	Logger& Logger::operator<<(const std::string& msg)
	{
		if (msg == endl)
		{
			*buff << "\n";
			finish();
		}
		else
		{
			*buff << msg;
		}

		return *this;
	}
	Logger& Logger::operator()(LogLevel verb)
	{
		cLevel = verb;
		delete buff;
		buff = new std::stringstream();
		return *this;
	}
	Logger::Logger()
	{
		buff = new std::stringstream();
		streams.push_back(&std::cout);
	}


	Logger::~Logger()
	{
	}

}