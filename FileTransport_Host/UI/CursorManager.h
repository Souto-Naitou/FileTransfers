#pragma once
#include "./UI/UI_ToolKit_Defines.h"

class CursorManager
{
private:
	static	Transform	cursor;			// カーソル座標
	static	bool		click;			// クリックフラグ
	static	bool		click_pre;		// クリックフラグ_前フレーム

public:
	static	void	UpdateCursorStatus();
	static	bool	GetClickFlag() { return click; }
	static	bool	GetPreClickFlag() { return click_pre; }
	static	void	GetCursorPos(Transform* _pos) { *_pos = cursor; }
	static	void	GetCursorPosX(int* _x) { *_x = cursor.x; }
	static	void	GetCursorPosY(int* _y) { *_y = cursor.y; }
	static	bool	Hover(Transform* const _rectPos, Size* const _rectSize);
	static	bool	Pressing(Transform* const _rectPos, Size* const _rectSize);
	static	bool	Pressed(Transform* const _rectPos, Size* const _rectSize);
};
