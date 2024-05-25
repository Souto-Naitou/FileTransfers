#pragma once
#include <ShObjIdl.h>

class FilePathSelector
{
private:
	IFileOpenDialog* pFileOpen;
	IShellItem* pItem;
	HRESULT hr;
	PWSTR pszFilePath;
	int result;

public:
	FilePathSelector();
	~FilePathSelector();

	void	OpenDialog();
	int		GetResult();
	PWSTR	GetFilePath();
};