#include "Phill.h"
#include <corecrt_math_defines.h>
#include <corecrt_math.h>

float Phill::ConstantT(int _targetFrame, int currentFrame)
{
	float result = float(currentFrame) / float(_targetFrame);
	return result;
}

float Phill::EaseInQuart(float _constant)
{
	_constant = LimitConstant(_constant);
	return powf(_constant, 4.0f);
}

float Phill::EaseOutQuart(float _constant)
{
	_constant = LimitConstant(_constant);
	return 1.0f - powf(1.0f - _constant, 4.0f);
}

float Phill::EaseInOutQuart(float _constant)
{
	_constant = LimitConstant(_constant);
	if (_constant < 0.5f)
	{
		return 8.0f * powf(_constant, 4.0f);
	}
	else
	{
		return 1.0f - powf(-2.0f * _constant + 2.0f, 4.0f) / 2.0f;
	}
}

float Phill::EaseOutBounce(float _constant)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	_constant = LimitConstant(_constant);
	if (_constant < 1.0f / d1)
	{
		return n1 * _constant * _constant;
	}
	else if (_constant < 2.0f / d1)
	{
		float cst_buf = _constant - (1.5f / d1);
		return n1 * cst_buf * cst_buf + 0.75f;
	}
	else if (_constant < 2.5f / d1)
	{
		float cst_buf = _constant - (2.25f / d1);
		return n1 * cst_buf * cst_buf + 0.9375f;
	}
	else
	{
		float cst_buf = _constant - (2.625f / d1);
		return n1 * cst_buf * cst_buf + 0.984375f;
	}
}

float Phill::EaseInElastic(float _constant)
{
	const float c4 = (2.0f * float(M_PI)) / 3.0f;
	if (_constant == 0.0f) return 0.0f;
	else if (_constant == 1.0f) return 1.0f;
	else return -powf(2.0f, 10.0f * _constant - 10.0f) * sinf((_constant * 10.0f - 10.75f) * c4);
}

float Phill::LimitConstant(float _constant)
{
	if (_constant < 0.0f) return 0.0f;
	else if (_constant > 1.0f) return 1.0f;
	else return _constant;
}
