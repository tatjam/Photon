#include "AssetManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../dep/stb/stb_image.h"

namespace ph
{

	std::string AssetManager::makeName(std::string s)
	{
		std::string sub = s.substr(0, resourceRoot.size());
		if (sub == resourceRoot)
		{
			return s.substr(resourceRoot.size());
		}
		else
		{
			return s;
		}
	}

	void AssetManager::uploadTexture(Texture* tex)
	{
		glBindTexture(GL_TEXTURE_2D, tex->glID);
		if (tex->settings.wrapMode == REPEAT)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else if (tex->settings.wrapMode == REPEAT_MIRROR)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		}
		else if (tex->settings.wrapMode == CLAMP_EDGE)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else if (tex->settings.wrapMode == CLAMP_BORDER)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		}

		if(tex->settings.downScaleMode == LINEAR)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		if (tex->settings.upScaleMode == NEAREST)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		if (tex->settings.downScaleMode == LINEAR)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (tex->settings.upScaleMode == NEAREST)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->texData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture* AssetManager::loadTexture(std::string path, std::string name, TextureSettings settings)
	{

		Texture n;
		if (name == "")
		{
			name = makeName(path);
		}

		en->log(INF) << "Loading texture: (" << path << ") as: (" << name << ")" << endlog;

		int width, height, channels;

		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, TEX_BPP);

		if(data != NULL)
		{
			n.width = width;
			n.height = height;
			n.texData = data;
		}
		else
		{
			en->log(ERR) << "Could not load texture!" << endlog;
			return NULL;
		}

		n.settings = settings;

		glGenTextures(1, &n.glID);

		textures[name] = n;

		uploadTexture(&textures[name]);

		en->log(WIN) << "Loaded texture!" << endlog;

		return &textures[name];
	}

	AssetManager::AssetManager(Engine* en)
	{
		this->en = en;
	}


	AssetManager::~AssetManager()
	{
	}

}
