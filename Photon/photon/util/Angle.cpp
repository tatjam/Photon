#include "Angle.h"

namespace ph
{
	Angle rad(float x)
	{
		return Angle(x, true);
	}

	Angle deg(float x)
	{
		return Angle(x, false);
	}
}