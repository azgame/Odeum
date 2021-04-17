#include "MenuScene.h"

#include <stdlib.h>


MenuScene::MenuScene()
{
	
}

MenuScene::~MenuScene()
{
}

bool MenuScene::Initialize()
{
	return true;
}

void MenuScene::Update(const float deltaTime_)
{
	
}

void MenuScene::UIRender()
{
	ImGui::Begin("Menu UI");
	ImGui::Text("Choose your game mode");
	ImGui::Button("Start Game");
	ImGui::SliderInt(levelString.at(i1), &i1, 1, 9);
	if (ImGui::Button("Select MiniGame")) { LoadLevel(i1); }
	ImGui::Button("Quit Game");
	
	ImGui::End();
}
void MenuScene::LoadLevel(int level_)
{
	int selection;
	switch (level_)
	{
	case 1:
		selection = 5;
		break;
	case 2:
		selection = 7;
		break;
	case 3:
		selection = 8;
		break;
	case 4:
		selection = 12;
		break;
	case 5:
		selection = 13;
		break;
	case 6:
		selection = 11;
		break;
	case 7:
		selection = 14;
		break;
	case 8:
		selection = 15;
		break;
	case 9:
		selection = 16;
		break;
	default:
		break;

	}
	OdeumEngine::Get().SetCurrentScene(selection);
	
}
