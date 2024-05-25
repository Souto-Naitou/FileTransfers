#pragma once

#ifndef PHILL_H
#define PHILL_H
#define _WINSOCKAPI_
#include <WinSock2.h>
#include <WS2tcpip.h>

enum DebugMode {
	kStandardMode, // 通常モード
	kDebugMode     // デバッグモード
};

enum PhillDrawMode
{
	DrawMode_LeftTop,
	DrawMode_Center
};


/// <summary>
/// PhilliaFunction
/// </summary>
class Phill {

// デバッグ機能

public:
	static	void	SetDebugMode(DebugMode _mode);
	static	void	ToggleDebugMode();
	static	bool	isDebug();


public:


	/// ################################ 描画系 ################################ ///


	static	void	Init();
	
	static	void	Rotate(int& _x, int& _y, float _theta);

	/// @param _mode 基準点の指定
	static	void	DrawQuadPlus(
		int _x, int _y, 
		int _horizontal, int _vertical,
		float _scaleX, float _scaleY,
		float _theta,
		int _srcX, int _srcY,
		int _srcW, int _srcH,
		int _textureHandle,
		unsigned int _hexColor,
		PhillDrawMode _drawMode
	);

	// 直線を引きます。ぴーん
	static	void DrawPeenLine(int _x, int _y, int _horizon, int _vertical);

	/// イージング関数
	static	float	ConstantT(int _targetFrame, int currentFrame);
	template <typename T>
	static	T		Lerp(T _begin, T _target, float _t) { return T(float(_target) * _t + float(_begin) * (1.0f - _t)); }

	static	float	EaseInQuart(float _constant);
	static	float	EaseOutQuart(float _constant);
	static	float	EaseInOutQuart(float _constant);
	static	float	EaseOutBounce(float _constant);
	static	float	EaseInElastic(float _constant);


	/// ############################## ネットワーク系 ############################## ///


	static	int		InitWSA(WSADATA* _wsaData);
	static	SOCKET	InitSocket(sockaddr_in* _addr, u_short _port);
	static	int		BindAddress(SOCKET* _socket, sockaddr* addr);
	static	int		Listen(SOCKET* _socket, int _backlog);
	static	SOCKET	WaitAccept(SOCKET* _listen, sockaddr* _client, int* _clientLen);

	/// ################################## 雑多 ################################## ///

	static	unsigned int GetRandColor();

	static	void	shuffle(int* arr, int len);

private:
	static	bool	isDebugMode;

	static	float	LimitConstant(float _constant);
	static	int		callCnt;
	static	int		kWhiteCircleHandle;
};

#endif // PHILL_H