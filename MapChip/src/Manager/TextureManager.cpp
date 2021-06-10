#include "TextureManager.h"

TextureManager::TextureManager()
{
	for (int i = 0; i < kTextureMax; i++)
	{
		textureDate[i] = -1;
	}
}

void TextureManager::LoadTexture()
{
	for (int i = 1; i < kTextureMax; i++)
	{
		std::stringstream ss;
		ss << "res/" << std::setfill('0') << std::setw(3) << std::right << std::to_string(i) << ".png";

		textureDate[i] = LoadGraph(ss.str().c_str());
	}
}

void TextureManager::DeleteTexture()
{
	for (int i = 0; i < kTextureMax; i++)
	{
		DeleteGraph(textureDate[i]);
	}
}

int TextureManager::GetTextureDate(int num)
{
	return textureDate[num];
}
