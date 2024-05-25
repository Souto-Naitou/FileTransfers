#include "Button.h"
#include "UI_ToolKit.h"
#include "PhilliaFunction/Phill.h"
#include <Novice.h>
#include "CursorManager.h"

ButtonObject::ButtonObject(SpriteData* _ptrSD)
{
	buttonData	= ButtonData();
	ptrSprData	= _ptrSD;
	return;
}

void ButtonObject::SetPosition(Transform _pos)
{
	buttonData.position = _pos;
	return;
}

void ButtonObject::SetPosition(int _posX, int _posY)
{
	buttonData.position.x = _posX;
	buttonData.position.y = _posY;
	return;
}

void ButtonObject::SetSize(Size _size)
{
	buttonData.size = _size;
	return;
}

void ButtonObject::SetSize(int _width, int _height)
{
	buttonData.size.width = _width;
	buttonData.size.height = _height;
	return;
}

void ButtonObject::StatusUpdate()
{
	buttonData.isPressed	= false;
	buttonData.isHover		= false;
	buttonData.isPressing	= false;

#pragma region lefttop
	/// - - - - - - - - - - マウスオーバー時 - - - - - - - - - - ///
	if (
		CursorManager::Hover(&buttonData.position, &buttonData.size) && 
		ptrSprData->drawMode == DrawMode_LeftTop
		)
	{
		buttonData.isHover = true;
	}

	/// - - - - - - - - - - 長押し時 - - - - - - - - - - ///
	if (
		CursorManager::Pressing(&buttonData.position, &buttonData.size) &&
		ptrSprData->drawMode == DrawMode_LeftTop
		)
	{
		buttonData.isPressing = true;
	}
	

	/// - - - - - - - - - - 押されたとき - - - - - - - - - - ///
	if (
		CursorManager::Pressed(&buttonData.position, &buttonData.size) &&
		ptrSprData->drawMode == DrawMode_LeftTop
		)
	{
		buttonData.isPressed = true;
	}
#pragma endregion

#pragma region center
	/// - - - - - - - - - - マウスオーバー時 - - - - - - - - - - ///
	if (
		CursorManager::Hover(&buttonData.position, &buttonData.size) &&
		ptrSprData->drawMode == DrawMode_Center
		)
	{
		buttonData.isHover = true;
	}

	/// - - - - - - - - - - 長押し時 - - - - - - - - - - ///
	if (
		CursorManager::Pressing(&buttonData.position, &buttonData.size) &&
		ptrSprData->drawMode == DrawMode_Center
		)
	{
		buttonData.isPressing = true;
	}
	

	/// - - - - - - - - - - 押されたとき - - - - - - - - - - ///
	if (
		CursorManager::Pressed(&buttonData.position, &buttonData.size) &&
		ptrSprData->drawMode == DrawMode_Center
		)
	{
		buttonData.isPressed = true;
	}
	
#pragma endregion

}


void ButtonObject::Draw()
{
	unsigned int color_temp = 0xffffffff;
	if (buttonData.isHover) color_temp = 0xddddddff;
	if (buttonData.isPressing) color_temp = 0xaaaaaaff;
	Phill::DrawQuadPlus(
		buttonData.position.x, buttonData.position.y,
		buttonData.size.width, buttonData.size.height,
		1.0f, 1.0f,
		0.0f,
		ptrSprData->srcPos.x, ptrSprData->srcPos.y,
		ptrSprData->srcSize.width, ptrSprData->srcSize.height,
		ptrSprData->textureHandle,
		color_temp,
		ptrSprData->drawMode
	);
	return;
}
