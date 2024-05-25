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

void TCPIP::SelectSocket()
{
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 500;

	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(parentSocket, &readfds);

	int activity = select(0, &readfds, nullptr, nullptr, &timeout);

	if (activity == SOCKET_ERROR)
	{
		isGetConnection = 0;
		return;
	}
	else if (activity == 0)
	{
		isGetConnection = 0;
		return;
	}

	isGetConnection = 1;

	return;
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

		// info get
		int clientInfoLen = int(sizeof(clientInfo));
		getpeername(clients[clients.size() - 1], (struct sockaddr*)&clientInfo, &clientInfoLen);
	}
	return 0;
}

void TCPIP::Receive(int _ID, char* _buffer, size_t _bufferSize, int* _bites)
{
	ZeroMemory(_buffer, _bufferSize);
	if (_bites)
	{
		*_bites = 0;
		*_bites = recv(clients[GetClientIndex(_ID)], _buffer, int(_bufferSize), 0);
	}
	else
	{
		recv(clients[GetClientIndex(_ID)], _buffer, int(_bufferSize), 0);
	}
}

void TCPIP::Receive(char* _buffer, size_t _bufferSize, int* _bites)
{
	ZeroMemory(_buffer, _bufferSize);
	if (_bites)
	{
		*_bites = 0;
		*_bites = recv(parentSocket, _buffer, int(_bufferSize), 0);
	}
	else
	{
		recv(parentSocket, _buffer, int(_bufferSize), 0);
	}
}

void TCPIP::Send(int _ID, const char* _message, size_t _bufferSize)
{
	if (!noOutputMode)
		printf("message : %s, %d\n", _message, int(_bufferSize));
	send(clients[GetClientIndex(_ID)], _message, int(_bufferSize), 0);
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

sockaddr_in TCPIP::GetClientInfo()
{
	return clientInfo;
}

int TCPIP::GetClientID()
{
	return latestIndex;
}

int TCPIP::GetIsConnection()
{
	return isGetConnection;
}

void TCPIP::CloseSocket(int _ID, bool _flag = false)
{
	closesocket(clients[GetClientIndex(_ID)]);
	if (!_flag)
	{
		closesocket(parentSocket);
	}
	return;
}

void TCPIP::CloseSocket(void)
{
	closesocket(parentSocket);
}

void TCPIP::Shutdown(int _ID, bool _flag)
{
	shutdown(clients[GetClientIndex(_ID)], SD_BOTH);
	if (!_flag)
	{
		shutdown(parentSocket, SD_BOTH);
	}
}

void TCPIP::Shutdown(void)
{
	shutdown(parentSocket, SD_BOTH);
}
