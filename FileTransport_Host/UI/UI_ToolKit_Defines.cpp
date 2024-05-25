#include "UI_ToolKit_Defines.h"

void Transform::operator=(Vector2 _vec2)
{
    x = int(_vec2.x);
    y = int(_vec2.y);
}

void Transform::operator=(Transform _tf)
{
    x = _tf.x;
    y = _tf.y;
}

void Transform::operator=(int _i)
{
    x = _i;
    y = _i;
}

Transform Transform::operator+(int&& _i)
{
    return Transform(x + _i, y + _i);
}

Transform Transform::operator-(int&& _i)
{
    return Transform(x - _i, y - _i);
}

Transform Transform::operator-=(int&& _i)
{
    x -= _i;
    y -= _i;
    return Transform(x, y);
}

void Size::operator=(int _i)
{
    width = _i;
    height = _i;
}
