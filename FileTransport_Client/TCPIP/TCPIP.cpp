#include "TCPIP.h"
#include <stdio.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

/// static variables
int			TCPIP::iWSAResult;		// int result
WSADATA		TCPIP::wsaData;			// Winsock API Data


int TCPIP::GetClientIndex(int _ID)
{
	int index = 0;

	for (auto& elm : clientIndex)
	{
		if (elm == _ID) break;
		index++;
	}

	return index;
}

void TCPIP::InitWSA()
{
	// 初期化
	iWSAResult = WSAStartup(WINSOCK_VERSION, &wsaData);

	return;
}

void TCPIP::ExitWSA()
{
	// クリーンアップ
	iWSAResult = WSACleanup();

	return;
}

TCPIP::TCPIP(NETWORKMODE _netMode)
{
	ptr = nullptr;
	result = nullptr;
	isGetConnection = 0;
	iResult = 0;
	ZeroMemory(&hints, sizeof hints);
	netMode = _netMode;
	parentSocket = INVALID_SOCKET;
	clientSocket = INVALID_SOCKET;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	latestIndex = -1;
	noOutputMode = 0;
	return;
}

void TCPIP::SetNoOutput(int _flag)
{
	noOutputMode = _flag;
}

void TCPIP::GenerateSocket(const char* _port)
{
	/// 接続待機用ソケットの生成
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(nullptr, _port, &hints, &result);


	parentSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (parentSocket == INVALID_SOCKET)
	{
		if (!noOutputMode)
			printf("error in generating socket, code is : %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		return;
	}
	return;
}

void TCPIP::GenerateSocket(const char* _ipaddr, const char* _port)
{
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(_ipaddr, _port, &hints, &result);

	parentSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (parentSocket == INVALID_SOCKET)
	{
		if (!noOutputMode)
			printf("error in generating socket, code is : %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		return;
	}
	return;
}

void TCPIP::BindSocket()
{
	if (parentSocket == INVALID_SOCKET)
	{
		if (!noOutputMode)
			printf("listenSocket is not generated yet\n");
		return;
	}

	iResult = bind(parentSocket, result->ai_addr, int(result->ai_addrlen));

	if (iResult == SOCKET_ERROR)
	{
		if (!noOutputMode)
			printf("error in bind(), code is : %ld\n", WSAGetLastError());
		return;
	}

	freeaddrinfo(result);
	return;
}

void TCPIP::ConnectSocket()
{
	iResult = connect(parentSocket, result->ai_addr, int(result->ai_addrlen));
}

void TCPIP::BeginListen()
{
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	int err = setsockopt(parentSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));
	if (err < 0)
	{
		if (!noOutputMode)
			printf("setsockopt error\n");
	}

	if (listen(parentSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		if (!noOutputMode)
			printf("listen failed. code is : %ld\n", WSAGetLastError());
		return;
	}
	isGetConnection = 1;
	return;
}

int TCPIP::AcceptConnection()
{
	if (isGetConnection == 1)
	{
		isGetConnection = 0;
		// Accept
		clients.push_back(
			accept(parentSocket, nullptr, nullptr)
		);
		latestIndex++;
		clientIndex.push_back(latestIndex);

		// when error
		if (clients[clients.size()-1] == INVALID_SOCKET)
		{
			if (!noOutputMode)
				printf("accept failed. code is : %ld\n", WSAGetLastError());
			closesocket(clients[clients.size() - 1]);
			clients.erase(clients.end());
			clientIndex.erase(clientIndex.end());
			return -1;
		}

		// タイムアウト設定
		tv.tv_sec = 10000;
		tv.tv_usec = 0;
		int err = setsockopt(clients[clients.size() - 1], SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));
		if (err < 0)
		{
			if (!noOutputMode)
				printf("setsockopt error\n");
		}
	}
	return 0;
}

void TCPIP::Receive(int _ID, char* _buffer, size_t _bufferSize,int* _bites)
{
	ZeroMemory(_buffer, _bufferSize);
	*_bites = 0;
	*_bites = recv(clients[GetClientIndex(_ID)], _buffer, int(_bufferSize), 0);
	_buffer[strlen(_buffer) + 1] = '\0';
}

void TCPIP::Receive(char* _buffer, size_t _bufferSize, int* _bites)
{
	ZeroMemory(_buffer, _bufferSize);
	*_bites = 0;
	*_bites = recv(parentSocket, _buffer, int(_bufferSize), 0);
	_buffer[strlen(_buffer) + 1] = '\0';
}

void TCPIP::Send(int _ID, const char* _message)
{
	if (!noOutputMode)
		printf("message : %s, %zu\n", _message, sizeof(_message));
	send(clients[GetClientIndex(_ID)], _message, int(sizeof(_message)), 0);
	return;
}

void TCPIP::Send(const char* _message, int* _bites)
{
	if (!noOutputMode)
		printf("message : %s, %zu\n", _message, strlen(_message));
	if (_bites != nullptr)
		*_bites = send(parentSocket, _message, int(strlen(_message)), 0);
	else send(parentSocket, _message, int(strlen(_message)), 0);
	return;
}

void TCPIP::Send(const char* _message, int _size, int* _bites)
{
	if (!noOutputMode)
		printf("message : %s, %d\n", _message, _size);
	if (_bites != nullptr)
		*_bites = send(parentSocket, _message, _size, 0);
	else send(parentSocket, _message, _size, 0);

	return;
}

size_t TCPIP::GetClientID()
{
	return latestIndex;
}

int TCPIP::GetIsConnection()
{
	return isGetConnection;
}

void TCPIP::CloseSocket(int _ID, bool _flag = false)
{
	shutdown(clients[GetClientIndex(_ID)], SD_BOTH);
	closesocket(clients[GetClientIndex(_ID)]);
	if (!_flag)
	{
		closesocket(parentSocket);
	}
	return;
}

void TCPIP::CloseSocket(void)
{
	shutdown(parentSocket, SD_BOTH);
	closesocket(parentSocket);
}
