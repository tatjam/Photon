#pragma once
#include "glm/glm.hpp"

namespace ph
{


	struct Angle
	{
		bool radians;
		float value;
	
		float asRadians()
		{
			if (radians)
			{
				return value;
			}
			else
			{
				return glm::radians(value);
			}
		}

		float asDegrees()
		{
			if (!radians)
			{
				return value;
			}
			else
			{
				return glm::degrees(value);
			}
		}

		Angle(float value)
		{
			radians = true;
			this->value = value;
		}

		Angle(float value, bool radians)
		{
			this->radians = radians;
			this->value = value;
		}

		Angle()
		{
			this->value = 0.0f;
			this->radians = radians;
		}

	};

	Angle rad(float x);

	Angle deg(float x);

}
