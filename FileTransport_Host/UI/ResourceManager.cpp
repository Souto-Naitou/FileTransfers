#include "ResourceManager.h"

#include <Novice.h>

/// 静的メンバ変数の初期化
std::vector<int>			ResourceManager::handles;
std::vector<std::string>	ResourceManager::identifiers;

int ResourceManager::_GetIndex(const char* _name)
{
	int count = 0;
	/// 検索してインデックスを作成
	for (auto& element : identifiers)
	{
		if (strcmp(element.c_str(), _name) == 0)
			break;
		count++;
	}
	return count;
}

void ResourceManager::Regist(const char* _name, const char* _path, bool _isTexture)
{
	identifiers.push_back(std::string(_name));

	if (_isTexture)
	{
		int iHandle = Novice::LoadTexture(_path);
		handles.push_back(iHandle);
	}
	else
	{
		int iHandle = Novice::LoadAudio(_path);
		handles.push_back(iHandle);
	}
}

int ResourceManager::Handle(const char* _name)
{
	int index = _GetIndex(_name);

	/// 見つからなかった場合...
	if (index == identifiers.size())
		return -1;

	return handles[index];
}

void ResourceManager::Unload(const char* _name)
{
	int index = _GetIndex(_name);
	Novice::UnloadTexture(handles[index]);
	handles.erase(handles.begin() + index);
	identifiers.erase(identifiers.begin() + index);
}
