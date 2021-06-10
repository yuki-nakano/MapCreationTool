#ifndef DEFINITION_H
#define DEFINITION_H

//画面サイズ
const int Widht = 1280;
const int Height = 720;

//配置可能なテクスチャの種類の最大数
const int kTextureMax = 1000;

struct Vec2
{
	Vec2() :
		X(0),
		Y(0)
	{
	}

	Vec2(int x, int y) :
		X(x),
		Y(y)
	{
	}

	int X;
	int Y;
};

#endif // !DEFINITION_H
