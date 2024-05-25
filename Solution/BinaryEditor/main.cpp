#include <stdio.h>
#include <Windows.h>
#include <ShObjIdl.h>
#include <filesystem>
#include <locale>

int main() {
	setlocale(LC_ALL, "ja-jp.UTF-8");

	PWSTR pszFilePath = nullptr;
	FILE* fp = nullptr;
	size_t fileSize = 0;
	
	IFileOpenDialog* pFileOpen = nullptr;
	IShellItem* pItem = nullptr;

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	
	hr = CoCreateInstance(
		CLSID_FileOpenDialog, nullptr, CLSCTX_ALL,
		IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen)
	);

	if (SUCCEEDED(hr))
	{
		pFileOpen->Show(nullptr);
	}
	if (SUCCEEDED(hr))
	{
		hr = pFileOpen->GetResult(&pItem);
	}
	else return 1;
	if (SUCCEEDED(hr))
	{
		hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
	}

	if (pszFilePath == nullptr) return -1;
	_wfopen_s(&fp, pszFilePath, L"rb+");

	if (!fp) return 1;
	fileSize = std::filesystem::file_size(pszFilePath);

	int bites = 0;
	int row = 0;
	while (true)
	{
		char data[16]{};
		bites = fread(data, 1, sizeof data, fp);
		printf("%08d  ", int(row * sizeof data));
		for (int i = 0; i < sizeof data; i++)
		{
			printf("%02X ", data[i] & 0x000000ff);
		}
		printf("   "); // spacing
		for (int i = 0; i < sizeof data; i++)
		{
			if (data[i] == '\0' ||
				data[i] == '\n' ||
				data[i] < 0 ||
				data[i] == '\r' ||
				data[i] == '\a' ||
				data[i] == '\b' ||
				data[i] == '\f' ||
				data[i] == '\t' ||
				data[i] == '\v' ||
				data[i] == '\x1b'
				)
				printf("_ ");
			else printf("%c ", data[i]);
		}
		printf("\n");
		if (bites <= 0) break;
		row++;
	}

	int end = 0;
	scanf_s("%d", &end);


	return 0;
}