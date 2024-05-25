#pragma once
#include "UI_ToolKit_Defines.h"
#include "Button.h"

#include <vector>
#include <string>
#include <stdarg.h>

class GUI_Toolkit
{
public:
	/// <summary>
	/// ボタンを配置する。
	/// </summary>
	/// <param name="_name">名前</param>
	/// <param name="_posx">x座標</param>
	/// <param name="_posy">y座標</param>
	/// <param name="_sprite">スプライトのデータ</param>
	/// <param name="_draw">このコードのみで描画するかどうか（false時はDraw関数を使って描画する）</param>
	/// <returns>ボタンが押されたらtrueを1F返す</returns>
	static	bool	Button(char const* _name, int _posx, int _posy, SpriteData* _sprite, bool _draw = true);

	static	void	Draw(Component _comp);
	static	int		Draw(Component _comp, char const* _name);

	static	int		DeleteComponent(Component _comp, const int _count, ...);

private:

	static	std::vector<std::string> name_button;
	static	std::vector<ButtonObject> object_button;

};