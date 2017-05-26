#pragma once

#include "Shader.h"

#include <unordered_map>

namespace ph
{
	// We usually waste a few bytes per property
	// not too bad, but we could use different
	// property maps!
	struct Property
	{
		int ival;
		float fval[4];
	};

	typedef std::unordered_map<std::string, Property> propertyMap;

	class Material
	{
	public:

		Shader* shader;
		
		propertyMap properties;

		void use();

		Material();
		~Material();
	};

}
