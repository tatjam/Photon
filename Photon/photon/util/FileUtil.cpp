#include "FileUtil.h"

namespace ph
{
	std::string FileUtil::stripFilename(std::string s)
	{
		int lPos = s.find_last_of("/\\");
		return s.substr(0, lPos + 1);
	}
}
