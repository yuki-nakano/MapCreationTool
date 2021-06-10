#ifndef MAP_CHIP_H
#define MAP_CHIP_H

#include "DxLib.h"
#include "Definition.h"
#include "Singleton.h"
#include "Manager/InputManager.h"
#include "Manager/TextureManager.h"

#include <iostream>
#include <fstream>

const int Edge_icon = 80;	//画面下部に表示されるアイコンのサイズ
const int MaxBlock = 10;	//画面下部に表示されるテクスチャの最大数

const int MaxTextureNum = 99;

const int col = 100;	//縦一列に配置可能なマップチップの数+1
const int row = 1000;	//横一列に配置可能なマップチップの数+1

enum
{
	kGimmickMode,	//mapDate_Gimmickへの配置
	kBlockMode,		//mapDate_Blockへの配置
	kModeMax,
};

class MapChip : public Singleton<MapChip>
{
public:
	MapChip();
	~MapChip();

	void Exec();
	void Draw();

private:
	//ウィンドウに表示されるマップチップの移動
	void MoveMap();

	//画面下部に表示されるテクスチャの番号の変更
	void CangeTextureNum();

	//マウスのウィンドウ上の座標をマップチップの座標に変更
	void ChangeArrayVec();

	//mapDateの削除
	void DeleteMapDate();

	//ブロックの配置
	void PutBulock();

	//マップの縦横の最大値の変更
	void SetLimit();

	//配置するテクスチャの変更
	void SelectBlock();

	//ウィンドウに表示されるマップチップの数の変更
	void ChangeSize();

	//配置するマップデータの変更
	void ChangeMode();

	//ファイルの書き込み
	void FileOutput();
	//ファイルの読み込み
	void FileInput();

private:
	InputManager* inputManager = nullptr;
	TextureManager* textureManager = nullptr;

	//色データ
	unsigned int cr_Frame;
	unsigned int cr_FirstFrame;
	unsigned int cr_ChoiceBlock;
	unsigned int cr_LimitBlock;

	//マップデータの保存
	int mapDate_Gimmick[100][1000];
	int mapDate_Block[100][1000];

	//テクスチャの読み込みができてるか
	bool successfulDate[kTextureMax];

	//マウスの座標
	Vec2 MouseVec;
	Vec2 ArrayVec;

	//マップデータの左上の座標
	Vec2 Coordinate_UpLeft;

	//マップデータの終わりの座標
	Vec2 Coordinate_Limit;

	//選択中のブロック
	int currentBlock_Gimmick = 0;
	int currentBlock_Block = 0;

	int Edge = 40;		//マップチップ1マスのサイズ
	int Block_col = 14;	//画面に表示される横の数
	int Block_row = 31;	//画面に表示される縦の数

	int textureNumber = 0;	

	int currentMode = 0;
};

#endif // !MAP_CHIP_H
