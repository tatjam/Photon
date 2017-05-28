#pragma once

#include <unordered_map>


#include "../Engine.h"
#include "../gfx/Shader.h"
#include "../gfx/Model.h"
#include "../gfx/Texture.h"
#include "Logger.h"

#include "../../dep/stb/stb_image.h"

namespace ph
{




	class AssetManager
	{
	private:

	

	public:

		void uploadTexture(Texture* tex);

		// This is deleted from the start of every path to make names
		std::string resourceRoot = "res/";

		Engine* en;

		std::string makeName(std::string path);

		Texture* loadTexture(std::string path, std::string name = "", TextureSettings settings = TextureSettings());
		Shader* loadShader(std::string path, std::string name = "");
		Model* loadModel(std::string path, bool loadTextures = false, std::string name = "");

		std::unordered_map<std::string, Shader> shaders;
		std::unordered_map<std::string, Model> models;
		std::unordered_map<std::string, Texture> textures;

		AssetManager(Engine* en);
		~AssetManager();
	};

}
