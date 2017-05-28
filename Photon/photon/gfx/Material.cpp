#include "Material.h"


namespace ph
{

	Property iProperty(int val)
	{
		Property out;
		out.ival = val;
		out.type = INT_PROP;
		return out;
	}
	Property fProperty(float val)
	{
		Property out;
		out.fval[0] = val;
		out.type = F1_PROP;
		return out;
	}
	Property v2Property(glm::vec2 val)
	{
		Property out;
		out.fval[0] = val.x; out.fval[1] = val.y;
		out.type = F2_PROP;
		return out;
	}
	Property v3Property(glm::vec3 val)
	{
		Property out;
		out.fval[0] = val.x; out.fval[1] = val.y; out.fval[2] = val.z;
		out.type = F3_PROP;
		return out;
	}
	Property tProperty(Texture* val)
	{
		Property out;
		out.tval = val;
		out.type = TEX_PROP;
		return out;
	}

	
	void Material::use()
	{
		shader->use();

		int texNum = 0;

		for (propertyMap::iterator it = properties.begin(); it != properties.end(); ++it) 
		{
			Property prop = it->second;
			std::string name = it->first;

			if (prop.type == INT_PROP)
			{
				glUniform1i(glGetUniformLocation(shader->pr, name.c_str()), prop.ival);
			}
			else if(prop.type == F1_PROP)
			{
				glUniform1f(glGetUniformLocation(shader->pr, name.c_str()), prop.fval[0]);
			}
			else if (prop.type == F2_PROP)
			{
				glUniform2f(glGetUniformLocation(shader->pr, name.c_str()), prop.fval[0], prop.fval[1]);
			}
			else if (prop.type == F3_PROP)
			{
				glUniform3f(glGetUniformLocation(shader->pr, name.c_str()), prop.fval[0], prop.fval[1], prop.fval[2]);
			}
			else if (prop.type == F4_PROP)
			{
				glUniform4f(glGetUniformLocation(shader->pr, name.c_str()), prop.fval[0], prop.fval[1], prop.fval[2], prop.fval[3]);
			}
			else if(prop.type == TEX_PROP)
			{
				glActiveTexture(GL_TEXTURE0 + texNum);
				glBindTexture(GL_TEXTURE_2D, prop.tval->glID);
				glUniform1i(glGetUniformLocation(shader->pr, name.c_str()), texNum);
				texNum++;
				//glBindTexture(GL_TEXTURE_2D, 0);
			}

		}
	}

	Material::Material()
	{
	}


	Material::~Material()
	{
	}

}
