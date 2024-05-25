#include "Checkbox.h"
#include <Novice.h>
#include "CursorManager.h"

CheckboxObject::CheckboxObject(SpriteData* _boxSpr, SpriteData* _checkSpr)
{
	checkboxData = CheckboxData();
	boxSprite = _boxSpr;
	checkSprite = _checkSpr;
}

void CheckboxObject::SetPosition(Transform _pos)
{
	checkboxData.position = _pos;
	return;
}

void CheckboxObject::SetPosition(int _posX, int _posY)
{
	checkboxData.position.x = _posX;
	checkboxData.position.y = _posY;
	return;
}

void CheckboxObject::SetSize(Size _size)
{
	checkboxData.size = _size;
	return;
}

void CheckboxObject::SetSize(int _width, int _height)
{
	checkboxData.size.width = _width;
	checkboxData.size.height = _height;
	return;
}

void CheckboxObject::StatusUpdate()
{
	checkboxData.isHover	= false;
	checkboxData.isPressed	= false;
	checkboxData.isPressing	= false;

	// ホバー時
	if (CursorManager::Hover(&checkboxData.position, &checkboxData.size))
	{
		checkboxData.isHover = true;
	}
	// 押されている間
	if (CursorManager::Pressing(&checkboxData.position, &checkboxData.size))
	{
		checkboxData.isPressing = true;
	}
	// 押されたとき
	if (CursorManager::Pressed(&checkboxData.position, &checkboxData.size))
	{
		checkboxData.isPressed = true;
	}
}

bool CheckboxObject::IsPressed()
{
	return false;
}

void CheckboxObject::Draw()
{
	unsigned int color_temp = 0xffffffff;
	if (checkboxData.isHover) color_temp = 0xddddddff;
	if (checkboxData.isPressing) color_temp = 0xaaaaaaff;
	Phill::DrawQuadPlus(
		checkboxData.position.x, checkboxData.position.y,
		checkboxData.size.width, checkboxData.size.height,
		1.0f, 1.0f,
		0.0f,
		boxSprite->srcPos.x, boxSprite->srcPos.y,
		boxSprite->srcSize.width, boxSprite->srcSize.height,
		boxSprite->textureHandle,
		color_temp,
		DrawMode_Center
	);
	Phill::DrawQuadPlus(
		checkboxData.position.x, checkboxData.position.y,
		checkboxData.size.width, checkboxData.size.height,
		1.0f, 1.0f,
		0.0f,
		checkSprite->srcPos.x, checkSprite->srcPos.y,
		checkSprite->srcSize.width, checkSprite->srcSize.height,
		checkSprite->textureHandle,
		color_temp,
		DrawMode_Center
	);
}
