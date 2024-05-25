#include "Scroll.h"
#include "UI_ToolKit_Defines.h"
#include "PhilliaFunction/Phill.h"
#include "CursorManager.h"
#include "ResourceManager.h"

Scroller::Scroller(SpriteData* _spr)
{
	sprite = _spr;
	scrBar = ScrollBar();
	scrBox = ScrollBox();
	startY = 0;
	currentY = 0;
	movable = false;
	movedT = 0;
	arrowHnd = ResourceManager::Handle("stgArrow");
}

void Scroller::SetPosition(Transform _pos)
{
	scrBar.position = _pos;
	scrBox.position = scrBar.position;
	return;
}

void Scroller::SetBarSize(Size _size)
{
	scrBar.size = _size;
	return;
}

void Scroller::Draw()
{

	Phill::DrawQuadPlus(
		scrBox.position.x, scrBox.position.y - scrBox.size.height / 2 - 75,
		48, 96,
		1.0f, 1.0f,
		float(3.1415926535),
		0, 0,
		64, 128,
		arrowHnd,
		0xffffffff,
		PhillDrawMode::DrawMode_Center
	);

	Phill::DrawQuadPlus(
		scrBox.position.x, scrBox.position.y,
		scrBox.size.width, scrBox.size.height,
		1.0f, 1.0f,
		0.0f,
		sprite->srcPos.x, sprite->srcPos.y,
		sprite->srcSize.width, sprite->srcSize.height,
		sprite->textureHandle,
		0xffffffff,
		PhillDrawMode::DrawMode_Center
	);

	Phill::DrawQuadPlus(
		scrBox.position.x, scrBox.position.y + scrBox.size.height / 2 + 75,
		48, 96,
		1.0f, 1.0f,
		0.0f,
		0, 0,
		64, 128,
		arrowHnd,
		0xffffffff,
		PhillDrawMode::DrawMode_Center
	);
}

void Scroller::SetBoxSize(Size _size)
{
	scrBox.size = _size;
	return;
}

void Scroller::UpdateStatus()
{
	scrBox.isHover		= false;
	scrBox.isPressed	= false;
	scrBox.isPressing	= false;

	scrBox.y_top = scrBar.position.y;
	scrBox.y_bottom = scrBar.position.y + scrBar.size.height;

	CursorManager::GetCursorPosY(&currentY);

	if (CursorManager::GetClickFlag()		&&
		!CursorManager::GetPreClickFlag()	&&
		CursorManager::Hover(&scrBox.position, &scrBox.size)
		)
	{
		scrBox.y_pre = scrBox.position.y;
		startY = currentY;
		movable = true;
	}

	if (!CursorManager::GetClickFlag()) movable = false;

	if (movable)
	{
		scrBox.position.y = scrBox.y_pre + (currentY - startY);
	}
	else
	{
		startY = 0;
	}

	if (scrBox.position.y < scrBox.y_top) scrBox.position.y = scrBox.y_top;
	if (scrBox.position.y > scrBox.y_bottom) scrBox.position.y = scrBox.y_bottom;

	movedT = float(scrBox.position.y - scrBar.position.y) / float(scrBox.y_bottom - scrBar.position.y);

	return;
}
