#include "TCPIP/TCPIP.h"
#include <Novice.h>
#include <thread>
#include <Windows.h>
#include <ShObjIdl.h>
#include <locale.h>
#include <vector>
#include "CSV/CSV.h"
#include "structs.h"
#include "ConvertString/ConvertString.h"
#include <filesystem>
#include "FileTransfer/fileTransfer.h"
#include "FilePathSelector/fpathselctr.h"

enum THREADS
{
	TH_FILESELECTER,
	TH_SENDTOHOST,

	TH_ALL
};


const char kWindowTitle[] = "クライアント側プログラム";

constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 180;

FileTransfer ft;
FilePathSelector fps;
PWSTR pszFilePath = nullptr;

size_t fileSize = 0;
char ipaddr[16]{};
char portnum[6]{};
int* sentBites = nullptr;

int fSelectReturn = -5;

int threadExist[TH_ALL] = {};

int FileSelecter();
void SendToHost();

void LoadConnectInfoFromFile();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	setlocale(LC_ALL, "ja-jp.UTF-8");

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, SCREEN_WIDTH, SCREEN_HEIGHT);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	for (int i = 0; i < TH_ALL; i++)
	{
		threadExist[i] = -1;
	}

	std::thread th_fselect(FileSelecter);
	std::thread th_sendToHost;

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

		if (fSelectReturn == 0)
		{
			pszFilePath = fps.GetFilePath();
			ft.SetFilePath(pszFilePath);
			LoadConnectInfoFromFile();
			ft.SetIpAddr(ipaddr, sizeof ipaddr);
			ft.SetIpPort(portnum, sizeof portnum);
			ft.OpenFile(OpenMode::OM_READ);
			th_sendToHost = std::thread(SendToHost);
			fSelectReturn = -10;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		int count = 0;

		Novice::DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0x000000ff, kFillModeSolid);

		Novice::ScreenPrintf(15, 15 + 20 * count++, "%s", ipaddr);
		Novice::ScreenPrintf(15, 15 + 20 * count++, "%s", portnum);
		if (threadExist[TH_SENDTOHOST] == 0)
			Novice::ScreenPrintf(15, 15 + 20 * count++, "Connected", portnum);
		//Novice::ScreenPrintf(15, 15 + 20 * count++, "Buffer : %dB", BUFFER_SIZE);
		//Novice::ScreenPrintf(15, 15 + 20 * count++, "fSelect : %s", threadExist[TH_FILESELECTER] ? "End" : "Processing...");
		//Novice::ScreenPrintf(15, 15 + 20 * count++, "sendHost : %s", threadExist[TH_SENDTOHOST] ? "End" : "Processing...");
		//Novice::ScreenPrintf(15, 15 + 20 * count++, "SentBites : %d", sentBites ? *sentBites : 0);

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

	if (threadExist[TH_FILESELECTER] != -1) th_fselect.join();
	if (threadExist[TH_SENDTOHOST] != -1) th_sendToHost.join();

	// ライブラリの終了
	Novice::Finalize();
	
	return 0;
}

int FileSelecter()
{
	threadExist[TH_FILESELECTER] = 0;

	fps.OpenDialog();
	int result = fps.GetResult();

	if (result != 0)
	{
		threadExist[TH_FILESELECTER] = 1;
		fSelectReturn = 1;
		return 1;
	}

	threadExist[TH_FILESELECTER] = 1;
	fSelectReturn = 0;
	return 0;
}

void SendToHost()
{
	threadExist[TH_SENDTOHOST] = 0;

	sentBites = ft.GetSentBites();
	ft.Send();

	ft.Shutdown();

	threadExist[TH_SENDTOHOST] = 1;

	return;
}

void LoadConnectInfoFromFile()
{
	char* freturn = nullptr;
	char buffer[32]{};
	char* token = nullptr;
	FILE* infoFp = nullptr;
	char* saveptr = nullptr;
	fopen_s(&infoFp, "ip.addr", "rt");
	if (infoFp == nullptr) return;
	freturn = fgets(buffer, int(sizeof buffer), infoFp);
	fclose(infoFp);

	token = strtok_s(buffer, ",", &saveptr);
	strcpy_s(ipaddr, token);
	token = strtok_s(nullptr, ",", &saveptr);
	strcpy_s(portnum, token);

	return;
}
