#include "fileTransfer.h"
#include "ConvertString/ConvertString.h"
#include <filesystem>
#include <synchapi.h>
#include <direct.h>

#pragma region FileTransfer()

void FileTransfer::ExtractFileName()
{
	if (filePath == nullptr)
	{
		return;	//早期リターン
	}

	char* saveptr = nullptr;
	char* token = nullptr;
	char filepath_cpy[PATH_LEN]{};
	
	strcpy_s(filepath_cpy, filePath);
	token = strtok_s(filepath_cpy, "\\\\", &saveptr);
	while (true)
	{
		strcpy_s(fileName, token);
		token = strtok_s(nullptr, "\\\\", &saveptr);
		if (token == nullptr || *token == '\0' || *token == '\n') break;
	}
	return;
}

void FileTransfer::OpenFile(OpenMode _om)
{
	if (filePath == nullptr)
	{
		return;
	}
	switch (_om)
	{
	case OM_READ:
		fopen_s(&fp, filePath, "rb");
		break;
	case OM_WRITE:
		fopen_s(&fp, savepath, "w+b");
		if (fp == nullptr) fopen_s(&fp, savepath, "a+b");
		if (fp == nullptr)
		{
			return;
		}
	}
	return;
}

int FileTransfer::ConnectHost()
{
	tcpip->GenerateSocket(addr, port);
	tcpip->ConnectSocket();
	if (tcpip->GetResult() == SOCKET_ERROR)
	{
		return 1;
	}
	return 0;
}

void FileTransfer::SendHead()
{
	char sendData[HEAD_LEN];
	ZeroMemory(sendData, HEAD_LEN);
	fileSize = int(std::filesystem::file_size(filePath));
	sprintf_s(sendData, HEAD_LEN, "%s,%d", fileName, fileSize);
	tcpip->Send(sendData, nullptr);
	return;
}

int FileTransfer::Listen()
{
	tcpip->GenerateSocket(port);
	tcpip->BindSocket();
	tcpip->BeginListen();

	return 0;
}

void FileTransfer::Accept()
{
	connectionCount = 0;
	tcpip->SelectSocket();
	if (tcpip->GetIsConnection() == 1)
	{
		tcpip->AcceptConnection();
		connectionCount++;
	}
}

FileTransfer::FileTransfer()
{
	// initialization
	ZeroMemory(data, BUFFER_SIZE);
	fp = nullptr;
	ZeroMemory(filePath, PATH_LEN);
	ZeroMemory(fileName, PATH_LEN);
	ZeroMemory(addr, sizeof addr);
	ZeroMemory(port, sizeof port);
	tcpip = nullptr;
	fileSize = 0;
	bites_all = 0;
	connectionCount = 0;
	TCPIP::InitWSA();
	tcpip = new TCPIP(NETWORKMODE::NWMODE_HOST);
	csv = new CSV;
}

void FileTransfer::CloseFile()
{
	fclose(fp);
	fp = nullptr;
}

FileTransfer::FileTransfer(const char* _filePath, size_t _filePathSize)
	: FileTransfer()
{
	memcpy_s(filePath, PATH_LEN, _filePath, _filePathSize);
}

FileTransfer::FileTransfer(const std::string& _filePath)
	: FileTransfer()
{
	memcpy_s(filePath, PATH_LEN, _filePath.c_str(), _filePath.size());
}

FileTransfer::FileTransfer(const wchar_t* _filePath)
	: FileTransfer()
{
	std::string result = ConvertString(_filePath);
	memcpy_s(filePath, PATH_LEN, result.c_str(), result.size());
}

#pragma endregion

FileTransfer::~FileTransfer()
{
	delete tcpip;
	delete csv;
}

#pragma region SetFilePath()

void FileTransfer::SetFilePath(const char* _filePath, size_t _filePathSize)
{
	memcpy_s(filePath, PATH_LEN, _filePath, _filePathSize);
}

void FileTransfer::SetFilePath(const std::string& _filePath)
{
	memcpy_s(filePath, PATH_LEN, _filePath.c_str(), _filePath.size());
}

void FileTransfer::SetFilePath(const wchar_t* _filePath)
{
	std::string result = ConvertString(_filePath);
	memcpy_s(filePath, PATH_LEN, result.c_str(), result.size());
}

#pragma endregion


TCPIP* FileTransfer::GetTCPInstance()
{
	return tcpip;
}

void FileTransfer::SetIpAddr(const char* _addr, size_t _addrSize)
{
	memcpy_s(addr, sizeof(addr), _addr, _addrSize);
}

void FileTransfer::Send()
{
	// データ送信
	if (fp == nullptr) return;
	this->ConnectHost();
	this->ExtractFileName();
	this->SendHead();


	Sleep(1000);
	int bites = 0;
	while (true)
	{
		ZeroMemory(data, sizeof data);
		bites = int(fread(data, 1, sizeof data, fp));
		int test = 0;
		Sleep(10);
		tcpip->Send(data, bites, &test);
		bites += test;
		if (bites <= 0)
		{
			break;
		}
	}

	this->CloseFile();
}

std::string* FileTransfer::GetFileName()
{
	return &filename;
}

void FileTransfer::Receive(int& _sessionID)
{
	// データ受信 (this->Sendに対して)
	int bites = 0;	// 送受信一回あたりのバイト
	do
	{
		tcpip->Receive(_sessionID, data, BUFFER_SIZE, &bites);
	} while (bites == -1 || bites == 0);

	// { filename, filesize }
	separatedData = csv->Parse(data);
	filename = separatedData[0];
	fileSize = atoi(separatedData[1].c_str());

	// fopen
	
	sprintf_s(savepath, "saves/%s", filename.c_str());
	OpenFile(OpenMode::OM_WRITE);

	bites_all = 0;
	bites = 0;

	while (true)
	{
		// { data }
		do
		{
			tcpip->Receive(_sessionID, data, BUFFER_SIZE, &bites);
		} while (bites == -1 || bites == 0);

		fwrite(data, 1, bites, fp);

		bites_all += bites;
		if (bites_all == fileSize) break;
	}
	fclose(fp);

}

int* FileTransfer::GetBitesAll()
{
	return &bites_all;
}

int* FileTransfer::GetConnectionCount()
{
	return &connectionCount;
}

void FileTransfer::Shutdown()
{
	tcpip->CloseSocket();
}

int* FileTransfer::GetFileSize()
{
	return &fileSize;
}

void FileTransfer::SetIpPort(const char* _port, size_t _portSize)
{
	memcpy_s(port, sizeof(port), _port, _portSize);
}