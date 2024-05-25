#include "FileTransfer/fileTransfer.h"
#include "FilePathSelector/fpathselctr.h"
#include "structs.h"
#include "PhilliaFunction/Phill.h"

#include <thread>
#include <ImGuiManager.h>
#include <Novice.h>
#include <string>
#include <format>
#include <queue>

#define PORTLEN 8

// Constant 
const int	kWindowWidth						= 1280;					// 画面幅
const int	kWindowHeight						= 720;					// 画面高さ
const char	kWindowTitle[]						= "クライアント(受信)";	// ウィンドウタイトル
const int	kPathLen							= 64;					// パス量

// Classes
FilePathSelector	fps;												// FilePathSelecter
FileTransfer		ft;													// FileTransfer

// Flags
int					exitFlag					= 0;					// アプリケーション終了フラグ
bool				selectwnd					= 0;
bool				connectButton				= 0;
bool				receiveButton				= 0;
bool				isConnect					= 0;
bool				noPreview					= 0;
bool				noGrid						= 0;
bool				initialized					= 0;
bool				initButton					= 0;

// Data
char				port[PORTLEN]				= "2200";				// ポート番号
char				ipaddr[INET_ADDRSTRLEN]		= "127.0.0.1";			// IPv4アドレス
int					imghandle[kPathLen]{};
std::string			statusLine					= "Waiting...";
float				fGridOpacity				= 1.0f;
int					receiptCount				= 0;
int					exitloadCount				= 0;
std::string*		filename					= nullptr;
int					mouseX{};
int					mouseY{};
int					gridOpacity{};
std::queue<std::string>	filenames;

// Bites
int*				bites_all;
int*				targBites;

// Path
//std::string			path[kPathLen];
char				fullPath[PATH_LEN]{};								// フルパス
char				savepath[PATH_LEN]			= "savedImages";		// 保存先

ThreadData th_connect{};
ThreadData th_receive{};

void	ImGuiSettingBegin();
void	ImGuiSettingEnd();
void	ImGuiFullscreen();
void	ImGuiButtons();
void	ImGuiWindow();
void	ImGuiWindow2();
void	ImGuiMainWindow();
void	PathSelect();
void	Connect();
void	Receive();
void	ImgPreview();
void	JoinThread();
void	Initialize();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	char keys[256] = {0};
	char preKeys[256] = {0};

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

		if (connectButton == 1 && th_connect.status == ThreadStatus::DEAD)
		{
			th_connect.thread = std::thread(Connect);
			th_connect.status = ThreadStatus::LIVE;
		}
		if (isConnect && th_receive.status == ThreadStatus::DEAD)
		{
			th_receive.thread = std::thread(Receive);
			th_receive.status = ThreadStatus::LIVE;
		}
		if (initButton)
		{
			Initialize();
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, 0x1e1e1eff, kFillModeSolid);
		ImgPreview();

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
	JoinThread();
	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

void PathSelect()
{
	
	return;
}

void ImGuiSettingBegin()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, 0xff3c7d3c);
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

	
	initButton = ImGui::Button("Init", ImVec2(100, 30));

	if (isConnect)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.2f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.2f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 1.0f, 1.0f, 0.2f));
		ImGui::Button("Connect", ImVec2(100, 30));
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}
	else
	{
		if (ImGui::Button("Connect", ImVec2(100, 30))) connectButton = 1;
	}


	ImGui::End();
}
void ImGuiWindow()
{

	ImGui::SetNextWindowPos(ImVec2(0, 19));
	ImGui::SetNextWindowSize(ImVec2(kWindowWidth, 40));
	ImGui::Begin("inputTexts", (bool*)false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);

	if (ImGui::BeginTable("toptool", 5))
	{
		ImGui::TableNextColumn();
		ImGui::InputText("Ip Address", ipaddr, INET_ADDRSTRLEN);
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
	ImGui::Begin("Status", (bool*)false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
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
		ImGui::Text("Receipt bites");
		ImGui::Text("File Size");

		// 2列目
		ImGui::TableNextColumn();
		ImGui::Text(": %s", ipaddr);
		ImGui::Text(": %s", port);
		if (bites_all && targBites)
		{
			ImGui::Text(": %d", *bites_all);
			ImGui::Text(": %d", *targBites);
		}

		ImGui::EndTable();
	}

	ImGui::End();
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
	strcpy_s(port, token);

	return;
}
void Connect()
{
	initialized = 0;
	ft.SetIpAddr(ipaddr, INET_ADDRSTRLEN);
	ft.SetIpPort(port, PORTLEN);
	statusLine = "Connecting...";
	int tryCount = 0;
	do
	{
		ft.Connect();
		tryCount++;
	} while (ft.GetConnectResult() != 0 && tryCount < 10);
	if (ft.GetConnectResult() == 0)
	{
		statusLine = std::format("[{}:{}] Connect", ipaddr, port);
		isConnect = 1;
	}
	bites_all = ft.GetBitesAll();
	targBites = ft.GetFileSize();

	th_connect.status = ThreadStatus::WAIT_JOIN;
}

void Receive()
{
	ft.SetSavePath(savepath);
	ft.MakeSaveDirectory();
	filename = ft.GetFileName();
	for (int i = 0; i < 64; i++)
	{
		ft.Receive();
		filenames.push(filename->c_str());
		statusLine = std::format("[{}:{}] Receiving... ({} / 64)", ipaddr, port, i+1);
		receiptCount++;
	}
	statusLine = std::format("[{}:{}] Succeeded ({} / 64)", ipaddr, port, receiptCount);
	th_receive.status = ThreadStatus::WAIT_JOIN;
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

	for (int i = 0; i < receiptCount - exitloadCount; i++)
	{
		imghandle[exitloadCount] = Novice::LoadTexture(
			std::format(
				"./{}/{}",
				savepath,
				filenames.front().c_str()
			).c_str()
		);
		filenames.pop();
		exitloadCount++;
	}

	{
		for (int i = 0; i < receiptCount; i++)
		{
			if (!noPreview)
				Phill::DrawQuadPlus(
					startx + i % 8 * width,
					starty + i / 8 * height,
					width,
					height,
					1.0f,
					1.0f,
					0.0f,
					0,
					0,
					srcwidth,
					srcheight,
					imghandle[i],
					0xffffffff,
					PhillDrawMode::DrawMode_LeftTop
				);

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
	if (th_connect.status == ThreadStatus::WAIT_JOIN || th_connect.status == ThreadStatus::LIVE)
	{
		th_connect.thread.join();
		th_connect.status = ThreadStatus::DEAD;
	}
	if (th_receive.status == ThreadStatus::WAIT_JOIN || th_receive.status == ThreadStatus::LIVE)
	{
		th_receive.thread.join();
		th_receive.status = ThreadStatus::DEAD;
	}
}

void Initialize()
{
	if (th_receive.status == ThreadStatus::WAIT_JOIN || th_receive.status == ThreadStatus::LIVE)
	{
		th_receive.thread.join();
		th_receive.status = ThreadStatus::DEAD;
	}
	for (int& handle : imghandle)
	{
		handle = 0;
	}
	if (filename) filename->clear();
	exitloadCount = 0;
	receiptCount = 0;

	initialized = 1;
}