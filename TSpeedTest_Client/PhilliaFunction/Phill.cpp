#include "Phill.h"
#include <Novice.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>

int Phill::kWhiteCircleHandle = 0;
int Phill::callCnt = 0;

void Phill::Init()
{
	FILE* fp;
	if (fopen_s(&fp, "whiteCir.png", "rb") == 0) kWhiteCircleHandle = Novice::LoadTexture("./whiteCir.png");
	else kWhiteCircleHandle = Novice::LoadTexture("white1x1.png");
	callCnt = 0;
}

bool Phill::isDebugMode = false;
void Phill::SetDebugMode(DebugMode mode)
{
	switch (mode)
	{
	case kStandardMode:
		Phill::isDebugMode = false;
		break;
	case kDebugMode:
		Phill::isDebugMode = true;
	}
}
void Phill::ToggleDebugMode()
{

	if (callCnt % 2 == 0)
	{
		Phill::SetDebugMode(kDebugMode);
	}
	else
	{
		Phill::SetDebugMode(kStandardMode);
	}

	callCnt++;
}
bool Phill::isDebug()
{
	return isDebugMode;
}


void Phill::Rotate(int& _x, int& _y, float _theta)
{
	int newX = 0;
	int newY = 0;

	newX = static_cast<int>((_x * cosf(_theta)) - (_y * sinf(_theta)));
	newY = static_cast<int>((_x * sinf(_theta)) + (_y * cosf(_theta)));

	_x = newX;
	_y = newY;
}

void Phill::DrawQuadPlus(
	int _x, int _y,
	int _horizontal, int _vertical,
	float _scaleX, float _scaleY,
	float _theta,
	int _srcX, int _srcY,
	int _srcW, int _srcH,
	int _textureHandle,
	unsigned int _hexColor,
	PhillDrawMode _drawMode
)
{
	// scale
	int leftTop[2] = { 0 , 0 };
	int rightTop[2] = { 0 , 0 };
	int leftBottom[2] = { 0 , 0 };
	int rightBottom[2] = { 0 , 0 };

	// translate & draw
	if (_drawMode == PhillDrawMode::DrawMode_Center)
	{
		leftTop[0] = static_cast<int>(_horizontal * _scaleX / -2);
		leftTop[1] = static_cast<int>(_vertical * _scaleY / -2);
		rightTop[0] = static_cast<int>(_horizontal * _scaleX / 2);
		rightTop[1] = static_cast<int>(_vertical * _scaleY / -2);
		leftBottom[0] = static_cast<int>(_horizontal * _scaleX / -2);
		leftBottom[1] = static_cast<int>(_vertical * _scaleY / 2);
		rightBottom[0] = static_cast<int>(_horizontal * _scaleX / 2);
		rightBottom[1] = static_cast<int>(_vertical * _scaleY / 2);
	}
	else if (_drawMode == PhillDrawMode::DrawMode_LeftTop)
	{
		leftTop[0] = 0;
		leftTop[1] = 0;
		rightTop[0] = static_cast<int>(_horizontal * _scaleX);
		rightTop[1] = 0;
		leftBottom[0] = 0;
		leftBottom[1] = static_cast<int>(_vertical * _scaleY);
		rightBottom[0] = static_cast<int>(_horizontal * _scaleX);
		rightBottom[1] = static_cast<int>(_vertical * _scaleY);
	}
	else
	{
		// "leftTopでもcenterでもないよ...
		assert(true);
	}

	// rotate
	Rotate(leftTop[0], leftTop[1], _theta);
	Rotate(rightTop[0], rightTop[1], _theta);
	Rotate(leftBottom[0], leftBottom[1], _theta);
	Rotate(rightBottom[0], rightBottom[1], _theta);



	Novice::DrawQuad(
		_x + leftTop[0], _y + leftTop[1],
		_x + rightTop[0], _y + rightTop[1],
		_x + leftBottom[0], _y + leftBottom[1],
		_x + rightBottom[0], _y + rightBottom[1],
		_srcX, _srcY,
		_srcW, _srcH,
		_textureHandle,
		_hexColor
	);

	// debug
	if (isDebugMode)
	{
		// 4頂点の確認
		Novice::DrawQuad(
			_x + leftTop[0] - 3, _y + leftTop[1] - 3,
			_x + leftTop[0] + 3, _y + leftTop[1] - 3,
			_x + leftTop[0] - 3, _y + leftTop[1] + 3,
			_x + leftTop[0] + 3, _y + leftTop[1] + 3,
			0, 0,
			10, 10,
			kWhiteCircleHandle,
			0x2f76ffff
		);

		Novice::DrawQuad(
			_x + rightTop[0] - 3, _y + rightTop[1] - 3,
			_x + rightTop[0] + 3, _y + rightTop[1] - 3,
			_x + rightTop[0] - 3, _y + rightTop[1] + 3,
			_x + rightTop[0] + 3, _y + rightTop[1] + 3,
			0, 0,
			10, 10,
			kWhiteCircleHandle,
			0x2f76ffff
		);

		Novice::DrawQuad(
			_x + leftBottom[0] - 3, _y + leftBottom[1] - 3,
			_x + leftBottom[0] + 3, _y + leftBottom[1] - 3,
			_x + leftBottom[0] - 3, _y + leftBottom[1] + 3,
			_x + leftBottom[0] + 3, _y + leftBottom[1] + 3,
			0, 0,
			10, 10,
			kWhiteCircleHandle,
			0x2f76ffff
		);

		Novice::DrawQuad(
			_x + rightBottom[0] - 3, _y + rightBottom[1] - 3,
			_x + rightBottom[0] + 3, _y + rightBottom[1] - 3,
			_x + rightBottom[0] - 3, _y + rightBottom[1] + 3,
			_x + rightBottom[0] + 3, _y + rightBottom[1] + 3,
			0, 0,
			10, 10,
			kWhiteCircleHandle,
			0x2f76ffff
		);

		// Novice::DrawEllipse(_x + leftTop[0], _y + leftTop[1], 5, 5, 0.0f, 0x2f76ffff, kFillModeSolid);
		// Novice::DrawEllipse(_x + rightTop[0], _y + rightTop[1], 5, 5, 0.0f, 0x2f76ffff, kFillModeSolid);
		// Novice::DrawEllipse(_x + leftBottom[0], _y + leftBottom[1], 5, 5, 0.0f, 0x2f76ffff, kFillModeSolid);
		// Novice::DrawEllipse(_x + rightBottom[0], _y + rightBottom[1], 5, 5, 0.0f, 0x2f76ffff, kFillModeSolid);

		// 中心点の確認
		Novice::DrawEllipse(_x, _y, 3, 3, 0.0f, 0xff0000ff, kFillModeSolid);

		// 中心点の確認(文字)
		Novice::ScreenPrintf(_x + leftTop[0] - 10, _y + leftTop[1] - 17, "(%4d, %4d)", _x, _y);
	}
}

void Phill::DrawPeenLine(int _x, int _y, int _horizon, int _vertical)
{
	Novice::DrawLine(_x, _y, _x + _horizon, _y + _vertical, 0x121212aa);
}

