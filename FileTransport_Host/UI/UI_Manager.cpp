#include "UI_Manager.h"
#include "CursorManager.h"
#include <Novice.h>
SpriteData UI_Manager::scrollBox = SpriteData();
Scroller* UI_Manager::scroll = new Scroller(&scrollBox);
float t;
Transform curpos;
Transform pos;

void UI_Manager::Init()
{
	scrollBox.srcPos = Transform(0, 0);
	scrollBox.srcSize = Size(1, 1);
	scrollBox.textureHandle = Novice::LoadTexture("white1x1.png");
	scroll->SetPosition(Transform(1250, 100));
	scroll->SetBarSize(Size(40, 520));
	scroll->SetBoxSize(Size(10, 70));
	pos.x = 1000; 
	pos.y = 200;
	
}

void UI_Manager::Update()
{
	CursorManager::GetCursorPos(&curpos);
	scroll->UpdateStatus();
	t = scroll->GetValue();
	pos.y = int(t * -1280.0f + (1.0f - t) * 200.0f);
}

void UI_Manager::Draw()
{
	Novice::DrawBox(740, 0, 525, 720, 0.0f, 0x121212ff, kFillModeSolid);

	Novice::DrawBox(pos.x, pos.y, 120, 120, 0.0f, 0xdfdfdfff, kFillModeSolid);
	
	scroll->Draw();
}
