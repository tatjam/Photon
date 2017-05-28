#pragma once

#include "Shader.h"

#include <unordered_map>

#include "Texture.h"

#include <glm/glm.hpp>

namespace ph
{


	enum PropertyType
	{
		INT_PROP,
		F1_PROP,
		F2_PROP,
		F3_PROP,
		F4_PROP,
		TEX_PROP
		// TODO MORE
	};


	// We usually waste a few bytes per property
	// not too bad, but we could use different
	// property maps!
	struct Property
	{
		PropertyType type;
		Texture* tval;
		int ival;
		float fval[4];
	};

	Property iProperty(int val);
	Property fProperty(float val);
	Property v2Property(glm::vec2 val);
	Property v3Property(glm::vec3 val);
	Property tProperty(Texture* val);


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
