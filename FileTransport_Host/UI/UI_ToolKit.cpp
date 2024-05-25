#include "UI_ToolKit.h"
#include "Button.h"
#include "CursorManager.h"

std::vector<std::string>	GUI_Toolkit::name_button;
std::vector<ButtonObject>	GUI_Toolkit::object_button;

bool GUI_Toolkit::Button(char const* _name, int _posx, int _posy, SpriteData* _sprite, bool _draw)
{
	bool found = false;
	int index = 0;
	for (auto& element : name_button)
	{
		if (strcmp(element.c_str(), _name) == 0)
		{
			found = true;
			break;
		}
		index++;
	}
	if (found == false)
	{
		// ないなら作る
		name_button.push_back(_name);
		object_button.push_back(ButtonObject(_sprite));
	}
	// ボタンの処理ここから
	object_button[index].SetSize(Size(_sprite->trgSize.width, _sprite->trgSize.height));
	object_button[index].SetPosition(Transform(_posx, _posy));
	if (_draw == true)
	{
		object_button[index].Draw();
	}
	object_button[index].StatusUpdate();
	return object_button[index].IsPressed();
}

void GUI_Toolkit::Draw(Component _comp)
{
	switch (_comp)
	{
	case Comp_Button:
		for (auto& element : object_button)
		{
			element.Draw();
		}
		break;
	default:
		break;
	}
}

int GUI_Toolkit::Draw(Component _comp, char const* _name)
{
	bool found = false;
	int index = 0;
	switch (_comp)
	{
	case Comp_Button:
		for (auto& element : name_button)
		{
			if (strcmp(element.c_str(), _name) == 0)
			{
				found = true;
				break;
			}
			index++;
		}
		if (!found) return 1;
		object_button[index].Draw();
		break;
	default:
		break;
	}
	return 0;
}

int GUI_Toolkit::DeleteComponent(Component _comp, const int _count, ...)
{
	va_list args;
	va_start(args, _count);

	bool found = false;
	int index = 0;
	const char* _name = nullptr;

	for (int i = 0; i < _count; i++) {
		_name = va_arg(args, const char*);
		switch (_comp)
		{
		case Comp_Button:
			for (auto& element : name_button)
			{
				if (strcmp(element.c_str(), _name) == 0)
				{
					found = true;
					break;
				}
				index++;
			}
			if (!found)
			{
				va_end(args);
				return 1;
			}
			name_button.erase(name_button.begin() + index);
			object_button.erase(object_button.begin() + index);
			break;
		default:
			break;
		}
	}

	va_end(args);
	return 0;
}
