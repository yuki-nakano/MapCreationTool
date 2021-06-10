#include "MapChip.h"

MapChip::MapChip()
{
	inputManager = InputManager::GetInstance();
	textureManager = TextureManager::GetInstance();
	textureManager->LoadTexture();

	for (int i = 1; i < kTextureMax; i++)
	{
		if (textureManager->GetTextureDate(i) == -1)
		{
			successfulDate[i] = false;
		}
		else
		{
			successfulDate[i] = true;
		}
	}

	successfulDate[Delete] = true;

	cr_Frame = GetColor(0, 0, 0);
	cr_FirstFrame = GetColor(0, 0, 255);
	cr_ChoiceBlock = GetColor(255, 0, 0);
	cr_LimitBlock = GetColor(255, 0, 0);

	DeleteMapDate();

	Coordinate_Limit.X = Block_row - 1;
	Coordinate_Limit.Y = Block_col - 1;
}

MapChip::~MapChip()
{

}

void MapChip::Exec()
{
	MoveMap();

	GetMousePoint(&MouseVec.X, &MouseVec.Y);
	
	ChangeArrayVec();

	CangeTextureNum();

	PutBulock();
	
	SelectBlock();

	SetLimit();

	ChangeSize();

	ChangeMode();

	FileOutput();
	FileInput();

	DeleteMapDate();
}

void MapChip::Draw()
{
	//マップデータの終わりのライン
	if (Coordinate_Limit.X - Coordinate_UpLeft.X >= 0 &&
		Coordinate_Limit.X - Coordinate_UpLeft.X <= Block_row)
	{
		DrawBox((Coordinate_Limit.X - Coordinate_UpLeft.X + 1) * Edge, 0,
			(Coordinate_Limit.X - Coordinate_UpLeft.X + 2) * Edge, Edge * (Block_col + 1), cr_LimitBlock, TRUE);
	}
	if (Coordinate_Limit.Y - Coordinate_UpLeft.Y >= 0 &&
		Coordinate_Limit.Y - Coordinate_UpLeft.Y <= Block_col - 1)
	{
		DrawBox(0, (Coordinate_Limit.Y - Coordinate_UpLeft.Y + 1) * Edge,
			Edge * (Block_row + 1), (Coordinate_Limit.Y - Coordinate_UpLeft.Y + 2) * Edge, cr_LimitBlock, TRUE);
	}

	//配置されたブロック
	for (int y = 0; y < Block_col; y++)
	{
		for (int x = 0; x < Block_row; x++)
		{
			DrawExtendGraph((x + 1) * Edge, (y + 1) * Edge, (x + 2) * Edge, (y + 2) * Edge,
				textureManager->GetTextureDate(mapDate_Gimmick[Coordinate_UpLeft.Y + y][Coordinate_UpLeft.X + x]), TRUE);

			if (mapDate_Block[Coordinate_UpLeft.Y + y][Coordinate_UpLeft.X + x] == 1 && mapDate_Gimmick[Coordinate_UpLeft.Y + y][Coordinate_UpLeft.X + x] == 0)
			{
				DrawBox((x + 1) * Edge, (y + 1) * Edge, (x + 2) * Edge, (y + 2) * Edge, cr_Frame, TRUE);
			}
			else if (mapDate_Block[Coordinate_UpLeft.Y + y][Coordinate_UpLeft.X + x] == 1 && mapDate_Gimmick[Coordinate_UpLeft.Y + y][Coordinate_UpLeft.X + x] != 0)
			{
				DrawBox((x + 1) * Edge, (y + 1.5f) * Edge, (x + 2) * Edge, (y + 2) * Edge, cr_Frame, TRUE);
			}
		}
	}

	//画面上部の枠
	for (int y = 0; y < Block_col + 1; y++)
	{
		for (int x = 1; x < Block_row + 1; x++)
		{
			if (y == 0)
			{
				DrawBox(x * Edge, 0, (x + 1) * Edge, Edge, cr_FirstFrame, FALSE);

				DrawFormatString((x) * Edge, 0, cr_Frame, "%d", Coordinate_UpLeft.X + x);
			}
			else
			{
				DrawBox(x * Edge, y * Edge, (x + 1) * Edge, (y + 1) * Edge, cr_Frame, FALSE);
			}
		}

		DrawBox(0, y * Edge, Edge, (y + 1) * Edge, cr_FirstFrame, FALSE);

		DrawFormatString(0, (y) * Edge, cr_Frame, "%d", Coordinate_UpLeft.Y + y);
	}

	//ブロックのアイコン
	switch (currentMode)
	{
	case kGimmickMode:
		//ブロックのアイコン
		for (int i = 0; i < MaxBlock; i++)
		{
			DrawExtendGraph(10 + i * (Edge_icon + 10), Height - 10 - Edge_icon, 10 + i * (Edge_icon + 10) + Edge_icon, Height - 10,
				textureManager->GetTextureDate(textureNumber * MaxBlock + i), FALSE);
			DrawBox(10 + i * (Edge_icon + 10), Height - 10 - Edge_icon, 10 + i * (Edge_icon + 10) + Edge_icon, Height - 10, cr_Frame, FALSE);
			DrawFormatString(11 + i * (Edge_icon + 10), Height - 30 - Edge_icon, cr_Frame, "%d", textureNumber * MaxBlock + i);
		}

		//現在のブロック
		DrawExtendGraph(20 + MaxBlock * (Edge_icon + 10), Height - 10 - Edge_icon, 20 + MaxBlock * (Edge_icon + 10) + Edge_icon, Height - 10,
			textureManager->GetTextureDate(currentBlock_Gimmick), FALSE);
		DrawBox(20 + MaxBlock * (Edge_icon + 10), Height - 10 - Edge_icon, 20 + MaxBlock * (Edge_icon + 10) + Edge_icon, Height - 10, cr_Frame, FALSE);
		DrawFormatString(21 + MaxBlock * (Edge_icon + 10), Height - 30 - Edge_icon, cr_Frame, "選択中:%d", currentBlock_Gimmick);

		DrawString(20 + (MaxBlock + 1) * (Edge_icon + 10), Height - 15 - Edge_icon / 2, "ギッミク", cr_Frame);

		break;
	case kBlockMode:
		//ブロックのアイコン
		for (int i = 0; i < 2; i++)
		{
			DrawBox(10 + i * (Edge_icon + 10), Height - 10 - Edge_icon, 10 + i * (Edge_icon + 10) + Edge_icon, Height - 10, cr_Frame, FALSE);
			DrawFormatString(11 + i * (Edge_icon + 10), Height - 30 - Edge_icon, cr_Frame, "%d", textureNumber * MaxBlock + i);
		}
		DrawBox(10 + (Edge_icon + 10), Height - 10 - Edge_icon, 10 + (Edge_icon + 10) + Edge_icon, Height - 10, cr_Frame, TRUE);

		//現在のブロック
		if (currentBlock_Block == 1)
		{
			DrawBox(20 + MaxBlock * (Edge_icon + 10), Height - 10 - Edge_icon, 20 + MaxBlock * (Edge_icon + 10) + Edge_icon, Height - 10, cr_Frame, TRUE);
		}
		DrawBox(20 + MaxBlock * (Edge_icon + 10), Height - 10 - Edge_icon, 20 + MaxBlock * (Edge_icon + 10) + Edge_icon, Height - 10, cr_Frame, FALSE);
		DrawFormatString(21 + MaxBlock * (Edge_icon + 10), Height - 30 - Edge_icon, cr_Frame, "選択中:%d", currentBlock_Block);

		DrawString(20 + (MaxBlock + 1) * (Edge_icon + 10), Height - 15 - Edge_icon / 2, "地面", cr_Frame);

		break;
	default:
		break;
	}


	//切り替えボタン
	DrawBox(20 + (MaxBlock + 1) * (Edge_icon + 10), Height - 10 - Edge_icon, 20 + (MaxBlock + 1) * (Edge_icon + 10) + Edge_icon, Height - 10, cr_Frame, FALSE);
	DrawString(21 + (MaxBlock + 1) * (Edge_icon + 10), Height - 30 - Edge_icon, "切り替え", cr_Frame);
	//拡縮ボタン
	for (int i = 0; i < 2; i++)
	{
		DrawBox(Widht - 10 - Edge_icon * (2 - i), Height - 10 - Edge_icon, Widht - 10 - Edge_icon * (1 - i), Height - 10, cr_Frame, FALSE);
	}
	DrawString(Widht - Edge_icon * 2, Height - 15 - Edge_icon / 2, "縮小", cr_Frame);
	DrawString(Widht - Edge_icon * 1, Height - 15 - Edge_icon / 2, "拡大", cr_Frame);

}

void MapChip::MoveMap()
{
	if (inputManager->IsKeyHeld(KEY_INPUT_RIGHT) && Coordinate_UpLeft.X < row - Block_row - 1)
	{
		Coordinate_UpLeft.X++;
	}
	
	if (inputManager->IsKeyHeld(KEY_INPUT_LEFT) && Coordinate_UpLeft.X > 0)
	{
		Coordinate_UpLeft.X--;
	}

	if (inputManager->IsKeyHeld(KEY_INPUT_DOWN) && Coordinate_UpLeft.Y < col - Block_col - 1)
	{
		Coordinate_UpLeft.Y++;
	}
	
	if (inputManager->IsKeyHeld(KEY_INPUT_UP) && Coordinate_UpLeft.Y > 0)
	{
		Coordinate_UpLeft.Y--;
	}
}

void MapChip::CangeTextureNum()
{
	if (currentMode != kGimmickMode) { return; }

	int tmpRot = GetMouseWheelRotVol();

	if (tmpRot < 0 && textureNumber > 0)
	{
		textureNumber--;
	}
	
	if (tmpRot > 0 && textureNumber < MaxTextureNum)
	{
		textureNumber++;
	}
}

void MapChip::ChangeArrayVec()
{
	ArrayVec.X = int(MouseVec.X / Edge);
	ArrayVec.Y = int(MouseVec.Y / Edge);
}

void MapChip::DeleteMapDate()
{
	if (!inputManager->IsKeyPushed(KEY_INPUT_Z)) { return; }

	for (int y = 0; y < col; y++)
	{
		for (int x = 0; x < row; x++)
		{
			mapDate_Gimmick[y][x] = Delete;
			mapDate_Block[y][x] = Delete;
		}
	}
}

void MapChip::PutBulock()
{
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0) { return; }

	switch (currentMode)
	{
	case kGimmickMode:

		if (MouseVec.X > Edge &&
			MouseVec.X < Widht &&
			MouseVec.Y > Edge &&
			MouseVec.Y < Edge * (Block_col + 1))
		{
			if (successfulDate[currentBlock_Gimmick])
			{
				mapDate_Gimmick[Coordinate_UpLeft.Y + ArrayVec.Y - 1][Coordinate_UpLeft.X + ArrayVec.X - 1] = currentBlock_Gimmick;
			}
		}

		break;
	case kBlockMode:

		if (MouseVec.X > Edge &&
			MouseVec.X < Widht &&
			MouseVec.Y > Edge &&
			MouseVec.Y < Edge * (Block_col + 1))
		{
			mapDate_Block[Coordinate_UpLeft.Y + ArrayVec.Y - 1][Coordinate_UpLeft.X + ArrayVec.X - 1] = currentBlock_Block;
		}

		break;
	default:
		break;
	}
}

void MapChip::SetLimit()
{
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0) { return; }

	if (MouseVec.X > 0 &&
		MouseVec.X < Widht &&
		MouseVec.Y > 0 &&
		MouseVec.Y < Edge)
	{
		for (int i = 1; i <= Block_row; i++)
		{
			if (MouseVec.X > i * Edge &&
				MouseVec.X < (i + 1) * Edge)
			{
				Coordinate_Limit.X = i - 1 + Coordinate_UpLeft.X;
			}
		}
	}
	//上限値の設定y
	if (MouseVec.X > 0 &&
		MouseVec.X < Edge &&
		MouseVec.Y > Edge &&
		MouseVec.Y < Edge * (Block_col + 1))
	{
		for (int i = 1; i <= Block_col; i++)
		{
			if (MouseVec.Y > i * Edge &&
				MouseVec.Y < (i + 1) * Edge)
			{
				Coordinate_Limit.Y = i - 1 + Coordinate_UpLeft.Y;
			}
		}
	}
}

void MapChip::SelectBlock()
{
	if (!inputManager->IsMouseButtonPushed()) { return; }

	switch (currentMode)
	{
	case kGimmickMode:
		for (int i = 0; i < MaxBlock; i++)
		{
			if (MouseVec.X > 10 + i * (Edge_icon + 10) &&
				MouseVec.X < 10 + i * (Edge_icon + 10) + Edge_icon &&
				MouseVec.Y > Height - 10 - Edge_icon &&
				MouseVec.Y < Height - 10)
			{
				if (successfulDate[i + MaxBlock * textureNumber])
				{
					currentBlock_Gimmick = i + MaxBlock * textureNumber;
				}

				break;
			}
		}
		break;
	case kBlockMode:
		for (int i = 0; i < 2; i++)
		{
			if (MouseVec.X > 10 + i * (Edge_icon + 10) &&
				MouseVec.X < 10 + i * (Edge_icon + 10) + Edge_icon &&
				MouseVec.Y > Height - 10 - Edge_icon &&
				MouseVec.Y < Height - 10)
			{
				currentBlock_Block = i;

				break;
			}
		}
		break;
	default:
		break;
	}
}

void MapChip::ChangeSize()
{
	if (!inputManager->IsMouseButtonPushed()) { return; }

	if (MouseVec.X > Widht - 10 - Edge_icon * 2 &&
		MouseVec.X < Widht - 10 - Edge_icon &&
		MouseVec.Y > Height - 10 - Edge_icon &&
		MouseVec.Y < Height - 10)
	{
		Edge = 20;
		Block_col = 29;
		Block_row = 63;
	}
	else if (MouseVec.X > Widht - 10 - Edge_icon &&
		MouseVec.X < Widht - 10 &&
		MouseVec.Y > Height - 10 - Edge_icon &&
		MouseVec.Y < Height - 10)
	{
		Edge = 40;
		Block_col = 14;
		Block_row = 31;
	}
}

void MapChip::ChangeMode()
{
	if (!inputManager->IsMouseButtonPushed()) { return; }

	if (MouseVec.X > 20 + (MaxBlock + 1) * (Edge_icon + 10) &&
		MouseVec.X < 20 + (MaxBlock + 1) * (Edge_icon + 10) + Edge_icon &&
		MouseVec.Y > Height - 10 - Edge_icon &&
		MouseVec.Y < Height - 10)
	{
		currentMode++;
		currentMode %= kModeMax;
	}
}

void MapChip::FileOutput()
{
	if (inputManager->IsKeyPushed(KEY_INPUT_RETURN))
	{
		std::ofstream ofs("C:MapDate.txt");

		for (int y = 0; y < Coordinate_Limit.Y; y++)
		{
			for (int x = 0; x < Coordinate_Limit.X; x++)
			{
				ofs << mapDate_Block[y][x];
				ofs << mapDate_Gimmick[y][x] / 100;
				ofs << mapDate_Gimmick[y][x] / 10 % 10;
				ofs << mapDate_Gimmick[y][x] % 10;
				ofs << ",";
			}

			ofs <<std::endl;
		}		
	}
}

void MapChip::FileInput()
{
	if (!inputManager->IsKeyPushed(KEY_INPUT_SPACE))
	{
		return;
	}

	FILE* fp;
	fopen_s(&fp, "C:MapDate.txt", "rb");
	if (fp == NULL)
	{
		return;
	}

	DeleteMapDate();

	char tmpLines[row * 5];
	int count_x = 0;
	int count_y = 0;
	while (fgets(tmpLines, row * 5, fp) != NULL)
	{
		for (int i = 0; i < row * 5; i++)
		{
			if (tmpLines[i] == '\0')
			{
				count_y++;
				count_x = 0;
				break;
			}
			else if (tmpLines[i] == ',')
			{
				count_x++;
			}
			else if (tmpLines[i] >= '0' && tmpLines[i] <= '9')
			{
				mapDate_Block[count_y][count_x] = (tmpLines[i] - '0');

				i++;

				mapDate_Gimmick[count_y][count_x] = (tmpLines[i] - '0') * 100;

				i++;

				mapDate_Gimmick[count_y][count_x] += (tmpLines[i] - '0') * 10;

				i++;

				mapDate_Gimmick[count_y][count_x] += tmpLines[i] - '0';
			}
		}
	}

	fclose(fp);
}
