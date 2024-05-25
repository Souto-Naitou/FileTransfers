#include "fpathselctr.h"
#include <stdio.h>

FilePathSelector::FilePathSelector()
{
	// initialization
	pFileOpen = nullptr;
	pItem = nullptr;
	hr = 0;
	pszFilePath = nullptr;
	result = 0;
}

FilePathSelector::~FilePathSelector()
{
}

void FilePathSelector::OpenDialog()
{
	hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL,
		IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

	if (SUCCEEDED(hr))
	{
		pFileOpen->Show(nullptr);
	}
	if (SUCCEEDED(hr))
	{
		hr = pFileOpen->GetResult(&pItem);
	}
	else
	{
		result = -1;
		return;
	}
	if (SUCCEEDED(hr))
	{
		hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
	}

	if (pszFilePath == nullptr)
	{
		result = 1;
		return;
	}

	return;
}

int FilePathSelector::GetResult()
{
	return result;
}

PWSTR FilePathSelector::GetFilePath()
{
	return pszFilePath;
}
