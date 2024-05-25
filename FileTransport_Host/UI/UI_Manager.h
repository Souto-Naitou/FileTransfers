#pragma once
#include "UI_ToolKit_Defines.h"
#include "Scroll.h"

class UI_Manager
{
private:
	static	SpriteData scrollBox;
	static	Scroller* scroll;

public:
	static	void	Init();
	static	void	Update();
	static	void	Draw();

};
