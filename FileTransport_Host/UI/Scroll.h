#pragma once
#include "UI_ToolKit_Defines.h"

class Scroller
{
private:
	SpriteData* sprite;
	ScrollBar	scrBar;
	ScrollBox	scrBox;
	int			startY;
	int			currentY;
	bool		movable;
	float		movedT;
	int			arrowHnd;
public:
	Scroller(SpriteData* _spr);

	void	SetBarSize(Size _size);
	void	SetBoxSize(Size _size);
	void	SetPosition(Transform _pos);
	void	UpdateStatus();
	float	GetValue() { return movedT; }
	void	Draw();
};
