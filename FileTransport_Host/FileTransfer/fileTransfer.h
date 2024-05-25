#pragma once
#include "TCPIP/TCPIP.h"
#include "CSV/CSV.h"
#include <string>
#include <vector>

enum OpenMode
{
	OM_READ,
	OM_WRITE
};

#define BUFFER_SIZE 50000
#define PATH_LEN 256
#define HEAD_LEN 256

class FileTransfer
{
private:
	char	data[BUFFER_SIZE];
	FILE*	fp;
	char	filePath[PATH_LEN];
	char	fileName[PATH_LEN];
	char	savepath[PATH_LEN]{};
	std::string filename;
	TCPIP*	tcpip;
	CSV*	csv;
	char	addr[16];
	char	port[8];
	int		fileSize;
	int		bites_all;
	int		connectionCount;
	std::vector<std::string> separatedData;

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
	int*	GetBitesAll();
	int*	GetFileSize();
	int*	GetConnectionCount();
	std::string* GetFileName();

	void	Shutdown();

	void	Send();

	/// <summary>
	/// 接続を待機する。接続要求は保留状態になる。
	/// </summary>
	/// <returns>戻り値</returns>
	int		Listen();

	/// <summary>
	/// 接続要求を承諾する。要求がない場合、定数時間ブロックされる。
	/// </summary>
	void	Accept();
	void	Receive(int& _sessionID);
};