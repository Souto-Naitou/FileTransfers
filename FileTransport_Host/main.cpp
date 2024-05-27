#include "TCPIP/TCPIP.h"
#include <Novice.h>
#include <thread>
#include <Windows.h>
#include <ShObjIdl.h>
#include <locale.h>
#include <vector>
#include "CSV/CSV.h"
#include "FileTransfer/fileTransfer.h"

enum THREADS
{
	TH_LISTEN,
	TH_SESSION,

	TH_ALL
};

const char kWindowTitle[] = "ホスト側プログラム";



int bites = 0;

int endCnt = 0;
int threadExist[TH_ALL]{};
int* connectionCount = nullptr;

const char port[] = "2200";

FileTransfer ft;

std::vector<std::thread> th_sessions;
std::vector<std::string> separatedData;
std::string* filename = ft.GetFileName();
int* fileSize = ft.GetFileSize();
int* receiptBites = ft.GetBitesAll();

void GetConnection();
void SessionProc(int _sessionID);
const int kWindowWidth = 640;
const int kWindowHeight = 180;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 640, 180);


	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	std::thread th_listen(GetConnection);

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		int linecount = 0;

		if (fileSize != nullptr && receiptBites != nullptr)
		{
			Novice::ScreenPrintf(15, 15 + linecount++ * 20, "Port : %s", port);

			//Novice::ScreenPrintf(15, 15 + linecount++ * 20, "rcv  : %d", *receiptBites);
			if (*fileSize == 0)
			{
				//Novice::ScreenPrintf(15, 15 + linecount++ * 20, "null");
			}
			else
			{
				Novice::ScreenPrintf(15, 15 + linecount++ * 20, "%d%%", int(float(*receiptBites) / float(*fileSize) * 100.0f));
				Novice::ScreenPrintf(15, 15 + linecount++ * 20, "File name : %s", (*filename).c_str());
				Novice::ScreenPrintf(15, 15 + linecount++ * 20, "File size : %dB", *fileSize);
			}

			//Novice::ScreenPrintf(15, 15 + linecount++ * 20, "Buffer : %dB", BUFFER_SIZE);
			Novice::ScreenPrintf(15, 15 + linecount++ * 20, "Listen : %s", threadExist[TH_LISTEN] ? "Connected" : "Processing...");
			//Novice::ScreenPrintf(15, 15 + linecount++ * 20, "Session : %s", threadExist[TH_SESSION] ? "End" : "Processing...");

			// Progress bar
			Novice::DrawBox(0, 0, int(kWindowWidth * float(*receiptBites) / float(*fileSize)), kWindowHeight, 0.0f, 0x234923ff, kFillModeSolid);
		}

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	endCnt = 1;
	th_listen.join();
	for (auto& element : th_sessions)
	{
		element.join();
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

void GetConnection()
{
	ft.SetIpPort(port, 5);
	ft.Listen();
	while (endCnt == 0)
	{
		ft.Accept();
		connectionCount = ft.GetConnectionCount();
		if (*connectionCount == 1)
		{
			th_sessions.push_back(std::thread(SessionProc, ft.GetTCPInstance()->GetClientID()));
			break;
		}
	}
	threadExist[TH_LISTEN] = 1;
}

void SessionProc(int _sessionID)
{
	
	while (endCnt == 0)
	{
		ft.Receive(_sessionID);
		break;
	}

	threadExist[TH_SESSION] = 1;
}
