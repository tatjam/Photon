#pragma once

namespace ph
{
	// RGBA textures
	const int TEX_BPP = 4;

	enum TexWrapping
	{
		REPEAT,
		REPEAT_MIRROR,
		CLAMP_EDGE,
		CLAMP_BORDER
	};

	enum TexFiltering
	{
		LINEAR,
		NEAREST
	};

	struct TextureSettings
	{
		TexWrapping wrapMode = REPEAT;
		TexFiltering upScaleMode = LINEAR;
		TexFiltering downScaleMode = LINEAR;
	};

	struct Texture
	{
		unsigned int glID = 0;
		unsigned char* texData = (unsigned char*)0;
		int width = 0, height = 0;

		TextureSettings settings;
	};
}