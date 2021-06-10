#include "InputManager.h"

void InputManager::UpdateKeyStatus()
{
	for (int i = 0; i < 256; i++)
	{
		previousKeyStatus[i] = keyStatus[i];
	}

	previousMouseButtonStatus = mouseButtonStatus;

	GetHitKeyStateAll(keyStatus);

	mouseButtonStatus = GetMouseInput() & MOUSE_INPUT_LEFT;
}

bool InputManager::IsKeyPushed(int key)
{
	if (previousKeyStatus[key] == 0 && keyStatus[key] == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputManager::IsKeyHeld(int key)
{
	if (previousKeyStatus[key] == 1 && keyStatus[key] == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputManager::IsKeyRelesed(int key)
{
	if (previousKeyStatus[key] == 1 && keyStatus[key] == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputManager::IsMouseButtonPushed()
{
	if (previousMouseButtonStatus == 0 && mouseButtonStatus != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputManager::IsMouseButtonHeld()
{
	if (previousMouseButtonStatus != 0 && mouseButtonStatus != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputManager::IsMouseButtonRelesed()
{
	if (previousMouseButtonStatus != 0 && mouseButtonStatus == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
