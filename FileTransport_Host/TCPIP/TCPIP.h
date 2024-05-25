#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <vector>

enum NETWORKMODE
{
	NWMODE_HOST,
	NWMODE_CLIENT,
};

enum SOCKETS
{
	SOCKETS_LISTEN,
	SOCKETS_CLIENT,
};

class TCPIP
{
private:
	static	WSADATA		wsaData;
	static	int			iWSAResult;
	NETWORKMODE	netMode;
	int			iResult;
	SOCKET		parentSocket;
	SOCKET		clientSocket;

	addrinfo* result;
	addrinfo* ptr;
	addrinfo	hints;
	int			isGetConnection;
	timeval		tv;

	std::vector<SOCKET> clients;
	std::vector<int> clientIndex;
	int latestIndex;

	//char buffer[512]{};
	int noOutputMode;

	int GetClientIndex(int _ID);
public:
	static	void		InitWSA();
	static	void		ExitWSA();
	TCPIP(NETWORKMODE _netMode);
	void		SetNoOutput(int _flag);

	void		SelectSocket();
	/// [For Server]
	void		GenerateSocket(const char* _port);
	/// [For Client]
	void		GenerateSocket(const char* _ipaddr, const char* _port);
	/// [For Server]
	void		BindSocket();
	/// [For Client]
	void		ConnectSocket();
	/// [For Server]
	void		BeginListen();
	/// [For Server]
	int			AcceptConnection();
	/// [For Server]
	void		Receive(int _ID, char* _buffer, size_t _bufferSize, int* _bites);
	/// [For Client]
	void		Receive(char* _buffer, size_t _bufferSize, int* _bites);
	/// [For Server]
	void		Send(int _ID, const char* _message);
	/// [For Client]
	void		Send(const char* _message, int* _bites);
	void		Send(const char* _message, int _size, int* _bites);

	int			GetClientID();
	int			GetIsConnection();

	/// <summary>
	/// ソケットを閉じる。
	/// </summary>
	/// <param name="_flag">リッスン用ソケットを残す場合、trueに。</param>
	void		CloseSocket(int _ID, bool _flag);
	void		CloseSocket(void);
	int			GetResult() { return iResult; };

};