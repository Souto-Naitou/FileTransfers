#pragma once
#include "PhilliaFunction/Phill.h"
#include <Vector2.h>

struct Transform {
	int x; 
	int y;
	void operator = (Vector2 _vec2);
	void operator = (Transform _tf);
	void operator = (int _i);
	Transform operator + (int&& _i);
	Transform operator - (int&& _i);
	Transform operator -= (int&& _i);
};
struct Size {
	int width; 
	int height;
	void operator = (int _i);
};
struct SpriteData
{
	Transform srcPos;
	Size srcSize;
	Size trgSize;
	int textureHandle;
	PhillDrawMode drawMode;		// ※一部のコンポーネントで無効
};
struct ButtonData
{
	Transform	position;		// ボタンの座標
	Size		size;			// サイズ
	bool		isPressed;		// ボタンが押された
	bool		isHover;		// ボタンにホバーされた
	bool		isPressing;		// ボタンが押されている
};
struct CheckboxData
{
	Transform	position;		// チェックボックスの座標
	Size		size;			// チェックボックスの座標
	bool		isPressed;		// チェックボックスが押された
	bool		isHover;		// チェックボックスにホバーされた
	bool		isPressing;		// チェックボックスが押されている
};
struct SliderData
{
	Transform	position;
	Size		size;
	Transform	cursor;
	// W.I.P.


};
struct ScrollBox
{
	Transform	position;
	Size		size;
	int			y_pre;
	int			y_bottom;
	int			y_top;
	bool		isHover;
	bool		isPressed;
	bool		isPressing;
};
struct ScrollBar
{
	Transform	position;
	Size		size;
};

enum Component
{
	Comp_Button,
};