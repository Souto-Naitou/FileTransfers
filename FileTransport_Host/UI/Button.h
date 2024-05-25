#pragma once

#include "UI_ToolKit_Defines.h"

class ButtonObject
{
private:
	ButtonData	buttonData;
	SpriteData* ptrSprData;

public:

	ButtonObject(SpriteData* _ptrSD);

	void	SetPosition(Transform _pos);
	void	SetPosition(int _posX, int _posY);
	void	SetSize(Size _size);
	void	SetSize(int _width, int _height);
	void	StatusUpdate();
	bool	IsPressed() { return buttonData.isPressed; }
	void	Draw();

};