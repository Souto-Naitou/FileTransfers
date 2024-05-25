#pragma once
#include "TCPIP/TCPIP.h"
#include <string>

enum OpenMode
{
	OM_READ,
	OM_WRITE
};

#define BUFFER_SIZE 50000
#define PATH_LEN 256
class FileTransfer
{
private:
	char	data[BUFFER_SIZE];
	FILE*	fp;
	char	filePath[PATH_LEN];
	char	fileName[PATH_LEN];
	TCPIP*	tcpip;
	char	addr[16];
	char	port[8];
	int		fileSize;
	int		SentBites;

	/// functions
	void	ExtractFileName();		// ファイル名の抽出
	int		ConnectHost();
	void	SendHead();

public:
	FileTransfer();
	FileTransfer(const char* _filePath, size_t _filePathSize);
	FileTransfer(const std::string& _filePath);
	FileTransfer(const wchar_t* _filePath);
	~FileTransfer();

	void	OpenFile(OpenMode _om);	// ファイルのオープン
	void	CloseFile();
	void	SetFilePath(const char* _filePath, size_t _filePathSize);
	void	SetFilePath(const std::string& _filePath);
	void	SetFilePath(const wchar_t* _filePath);

	void	SetIpAddr(const char* _addr, size_t _addrSize);
	void	SetIpPort(const char* _port, size_t _portSize);

	TCPIP*	GetTCPInstance();
	int*	GetSentBites();

	void	Shutdown();

	void	Send();
	void	Receive();
};