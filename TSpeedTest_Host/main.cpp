#include <thread>
#include "FileTransfer/fileTransfer.h"
#include "FilePathSelector/fpathselctr.h"
#include "structs.h"
#include "TextureManager.h"
#include "PhilliaFunction/Phill.h"

#include <ImGuiManager.h>
#include <Novice.h>
#include <string>
#include <format>
#include <locale>
#include <Vector2.h>


#define PORTLEN 8

// constant 
const int	kWindowWidth		= 1280;				// 画面幅
const int	kWindowHeight		= 720;				// 画面高さ
const char	kWindowTitle[]		= "ホスト(送信)";		// ウィンドウタイトル
const int	kPathLen			= 64;				// パス量

// Classes
FilePathSelector	fps;							// FilePathSelecter
FileTransfer		ft;								// FileTransfer

// Flags
int					exitFlag		= 0;			// アプリケーション終了フラグ
bool				selectwnd		= 0;
bool				listenFlag		= 0;
bool				isConnect		= 0;
bool				isStartSending	= 0;
bool				noPreview		= 0;
bool				noGrid			= 0;

// Data
char				port[PORTLEN]	= "2200";		// ポート番号
char				ipaddr[INET_ADDRSTRLEN];		// IPv4アドレス
std::string			path[kPathLen];
int					imghandle[kPathLen]{};
std::string			statusLine		= "Waiting...";
int					mouseX{};
int					mouseY{};
int					gridOpacity{};
float				fGridOpacity	= 1.0f;
char				fullPath[PATH_LEN]{};			// フルパス
int					fileCount		= 0;

ThreadData			th_pathSelecter{};
ThreadData			th_listenAndAccept{};
ThreadData			th_send{};

sockaddr_in			clientInfo;

void	ImGuiSettingBegin();
void	ImGuiSettingEnd();
void	ImGuiFullscreen();
void	ImGuiButtons();
void	ImGuiWindow();
void	ImGuiWindow2();
void	ImGuiMainWindow();
void	PathSelect();
void	Listening();
void	SendFiles();

void	ImgPreview();

void	JoinThread();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	setlocale(LC_ALL, "ja_JP.Utf-8");

	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	if (th_pathSelecter.status == ThreadStatus::DEAD)
	{
		th_pathSelecter.thread = std::thread(PathSelect);
		th_pathSelecter.status = ThreadStatus::LIVE;
	}

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {

		Novice::BeginFrame();
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		ImGuiSettingBegin();
		ImGuiFullscreen();
		ImGuiButtons();
		ImGuiWindow();
		ImGuiWindow2();
		if (isConnect)
			ImGuiMainWindow();

		ImGuiSettingEnd();

		///
		/// ↓更新処理ここから
		///
		Novice::GetMousePosition(&mouseX, &mouseY);

		if (listenFlag == 1 && th_listenAndAccept.status == ThreadStatus::DEAD)
		{
			statusLine = "Listening...";
			th_listenAndAccept.thread = std::thread(Listening);
			th_listenAndAccept.status = ThreadStatus::LIVE;
		}

		if (isStartSending == 1)
		{
			statusLine = std::format("[{}:{}] Sending...", ipaddr, port);
			th_send.thread = std::thread(SendFiles);
			th_send.status = ThreadStatus::LIVE;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::ScreenPrintf(60, 60, "%d,%d", mouseX, mouseY);

		Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, 0x1e1e1eff, kFillModeSolid);
		
		ImgPreview();

		///
		/// ↑描画処理ここまで
		///

		if (th_send.status == ThreadStatus::WAIT_JOIN)
		{
			th_send.thread.join();
			th_send.status = ThreadStatus::DEAD;
		}

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	exitFlag = 1;
	JoinThread();

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

void ImGuiSettingBegin()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, 0xcc362bff);
}
void ImGuiSettingEnd()
{
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}
void ImGuiFullscreen()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			ImGui::MenuItem("Set port number", nullptr, nullptr);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
void ImGuiButtons()
{
	ImGui::SetNextWindowSize(ImVec2(120, 340));
	ImGui::SetNextWindowPos(ImVec2(1160, 59));

	ImGui::Begin("Controll", (bool*)false, ImGuiWindowFlags_NoResize);

	if (th_listenAndAccept.status == ThreadStatus::LIVE)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.2f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.2f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 1.0f, 1.0f, 0.2f));
		ImGui::Button("Listen", ImVec2(100, 30));
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}
	else
	{
		listenFlag = 0;
		listenFlag = ImGui::Button("Listen", ImVec2(100, 30));
	}

	if (isConnect)
	{
		if (th_send.status == ThreadStatus::LIVE)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.2f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.2f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 1.0f, 1.0f, 0.2f));
			ImGui::Button("Send", ImVec2(100, 30));
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
		}
		else
		{
			isStartSending = 0;
			isStartSending = ImGui::Button("Send", ImVec2(100, 30));
		}

	}


	ImGui::End();
}
void ImGuiWindow()
{
	//ImGui::ShowDemoWindow();

	ImGui::SetNextWindowPos(ImVec2(0, 19));
	ImGui::SetNextWindowSize(ImVec2(kWindowWidth, 40));
	ImGui::Begin("inputTexts", (bool*)false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);

	if (ImGui::BeginTable("toptool", 5))
	{
		ImGui::TableNextColumn();
		ImGui::InputText("Port", port, PORTLEN);
		ImGui::TableNextColumn();
		ImGui::Checkbox("Disable Preview", &noPreview);
		ImGui::TableNextColumn();
		ImGui::Checkbox("Disable Grid", &noGrid);
		ImGui::TableNextColumn();
		ImGui::SliderFloat("Grid Opacity", &fGridOpacity, 0.0f, 1.0f, "%.2f", 0);
		gridOpacity = int(fGridOpacity * 0xff);

		ImGui::EndTable();
	}

	ImGui::End();
}
void ImGuiWindow2()
{
	ImGui::SetNextWindowPos(ImVec2(0, 670));
	ImGui::SetNextWindowSize(ImVec2(1280, 50));
	ImGui::Begin("Status", (bool*)false, ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoCollapse);
	ImGui::Text(statusLine.c_str());
	ImGui::End();
}
void ImGuiMainWindow()
{

	ImGui::Begin("Connection Info");

	if (ImGui::BeginTable("table1", 2))
	{
		// 1列目
		ImGui::TableNextColumn();
		ImGui::Text("IPv4 Address");
		ImGui::Text("Port");

		// 2列目
		ImGui::TableNextColumn();
		ImGui::Text(": %s", ipaddr);
		ImGui::Text(": %s", port);

		ImGui::EndTable();
	}

	ImGui::End();
}

void PathSelect()
{
	for (int num = 0; num < kPathLen; num++)
	{
		char numstr[8]{};
		_itoa_s(num, numstr, 10);
		path[num] = std::string("./sendimg/") + std::string(numstr) + std::string(".jpg");
		imghandle[num] = Novice::LoadTexture(path[num].c_str());
	}
	return;
}
void Listening()
{
	ft.SetIpPort(port, PORTLEN);
	ft.Listen();
	do
	{
		ft.Accept();
	} while (*ft.GetConnectionCount() == 0 && exitFlag == 0);
	clientInfo = ft.GetTCPInstance()->GetClientInfo();
	inet_ntop(AF_INET, &clientInfo.sin_addr, ipaddr, INET_ADDRSTRLEN);
	statusLine = std::format("[{}:{}] Connect", ipaddr, port);
	isConnect = 1;
	return;
}
void SendFiles()
{
	int ID = ft.GetTCPInstance()->GetClientID();
	for (fileCount = 0; fileCount < kPathLen; fileCount++)
	{
		statusLine = std::format("[{}:{}] Sending... ({} / 64)", ipaddr, port, fileCount+1);
		ZeroMemory(fullPath, PATH_LEN);
		GetFullPathNameA(path[fileCount].c_str(), PATH_LEN, fullPath, nullptr);
		ft.SetFilePath(fullPath);
		ft.OpenFile(OpenMode::OM_READ);
		ft.Send(ID);
	}
	statusLine = std::format("[{}:{}] Exit sending ({} / 64)", ipaddr, port, fileCount);
	th_send.status = ThreadStatus::WAIT_JOIN;
}

void ImgPreview()
{
	int startx{};
	int starty{};
	const int width = 80;
	const int height = 60;
	const int srcwidth = 456;
	const int srcheight = 342;
	const int spaceWidth = 1160;
	const int spaceHeight = 611;
	const int headerHeight = 59;

	startx = (spaceWidth - width * 8) / 2;
	starty = (spaceHeight - height * 8) / 2 + headerHeight;

	Novice::DrawBox(
		startx,
		starty,
		width * 8,
		height * 8,
		0.0f,
		0x000000aa, 
		kFillModeSolid
	);
	
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (!noPreview)
					Phill::DrawQuadPlus(
						startx + j * width,
						starty + i * height,
						width,
						height,
						1.0f,
						1.0f,
						0.0f,
						0,
						0,
						srcwidth,
						srcheight,
						imghandle[i * 8 + j],
						0xffffffff,
						PhillDrawMode::DrawMode_LeftTop
					);
			}
		}
	}
	if (!noGrid)
	for (int i = 0; i < 7; i++)
	{
		Novice::DrawLine(
			startx,
			starty + (i + 1) * height,
			startx + width * 8,
			starty + (i + 1) * height,
			0xffffff00 + gridOpacity
		);
		Novice::DrawLine(
			startx + width * (i + 1),
			starty,
			startx + width * (i + 1),
			starty + height * 8,
			0xffffff00 + gridOpacity
		);
	}
}

void JoinThread()
{
	if (th_pathSelecter.status == ThreadStatus::LIVE)
	{
		th_pathSelecter.thread.join();
		th_pathSelecter.status = ThreadStatus::DEAD;
	}
	if (th_listenAndAccept.status == ThreadStatus::LIVE)
	{
		th_listenAndAccept.thread.join();
		th_listenAndAccept.status = ThreadStatus::DEAD;
	}
	if (th_send.status == ThreadStatus::LIVE)
	{
		th_send.thread.join();
		th_send.status = ThreadStatus::DEAD;
	}
}