#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "DxLib.h"
#include "../Definition.h"
#include "../Singleton.h"

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

const int Delete = 0;

class TextureManager : public Singleton<TextureManager>
{
public:
	TextureManager();

	//テクスチャデータの初期化
	//各シーンの最初に呼び出す
	void LoadTexture();

	//テクスチャデータの削除
	//各シーンの最後に呼び出す
	void DeleteTexture();

	//テクスチャデータの取得
	int GetTextureDate(int num);

private:
	int textureDate[kTextureMax];
};

#endif // !TEXTURE_MANAGER_H
