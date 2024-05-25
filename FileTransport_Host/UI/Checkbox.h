#pragma once

#include "UI_ToolKit_Defines.h"

class CheckboxObject
{
private:
	CheckboxData	checkboxData;
	SpriteData*		boxSprite;
	SpriteData*		checkSprite;

public:
	
	CheckboxObject(SpriteData* _boxSpr, SpriteData* _checkSpr);
	
	void	SetPosition(Transform _pos);
	void	SetPosition(int _posX, int _posY);
	void	SetSize(Size _size);
	void	SetSize(int _width, int _height);
	void	StatusUpdate();
	bool	IsPressed();
	void	Draw();
};
