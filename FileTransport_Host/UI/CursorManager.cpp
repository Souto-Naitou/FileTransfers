#include "CursorManager.h"
#include <Novice.h>

Transform	CursorManager::cursor = Transform(0,0);
bool		CursorManager::click = false;
bool		CursorManager::click_pre = false;

void CursorManager::UpdateCursorStatus()
{
	Novice::GetMousePosition(&cursor.x, &cursor.y);
	click_pre = click;
	click = Novice::IsPressMouse(0);
	return;
}

bool CursorManager::Hover(Transform* const _rectPos, Size* const _rectSize)
{
	if (
		cursor.x > _rectPos->x - _rectSize->width / 2 &&
		cursor.x < _rectPos->x + _rectSize->width / 2 &&
		cursor.y > _rectPos->y - _rectSize->height / 2 &&
		cursor.y < _rectPos->y + _rectSize->height / 2
		)
	{
		return true;
	}
	return false;
}

bool CursorManager::Pressing(Transform* const _rectPos, Size* const _rectSize)
{
	if (click)
	{
		if (
			cursor.x > _rectPos->x - _rectSize->width / 2 &&
			cursor.x < _rectPos->x + _rectSize->width / 2 &&
			cursor.y > _rectPos->y - _rectSize->height / 2 &&
			cursor.y < _rectPos->y + _rectSize->height / 2
			)
		{
			return true;
		}
	}
	return false;
}

bool CursorManager::Pressed(Transform* const _rectPos, Size* const _rectSize)
{
	if (!click && click_pre)
	{
		if (
			cursor.x > _rectPos->x - _rectSize->width / 2 &&
			cursor.x < _rectPos->x + _rectSize->width / 2 &&
			cursor.y > _rectPos->y - _rectSize->height / 2 &&
			cursor.y < _rectPos->y + _rectSize->height / 2
			)
		{
			return true;
		}
	}
	return false;
}
