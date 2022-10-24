#include "hud.h"

#include <string>
#include <engine/renderer.h>
#include <engine/sprite.h>

#include "utils.h"
#include "player.h"
#include "game/safearea.h"
#include "abilitymanager.h"
#include "ability.h"

#ifdef _DEBUG
#include <imgui/imgui_impl_sdl.h>
#endif // _DEBUG

HUD::HUD()
	: m_fWinTextAlpha(0.f)
{

}

HUD::~HUD()
{
	delete m_pHPTextSprite;
	m_pHPTextSprite = 0;

	delete m_pHPSprite;
	m_pHPSprite = 0;

	delete m_pSafeAreaItemCountText;
	m_pSafeAreaItemCountText = 0;

	delete m_pItemSprite;
	m_pItemSprite = 0;

	delete m_pObjectiveText;
	m_pObjectiveText = 0;

	delete m_pObjectiveCount;
	m_pObjectiveCount = 0;

	delete m_pWinText;
	m_pWinText = 0;

	delete m_pCoolDownBar;
	m_pCoolDownBar = 0;

	for (Sprite* sprite : m_vCoolDowns)
	{
		delete sprite;
	}
	m_vCoolDowns.clear();

	for (Sprite* sprite : m_vCoolDownIcons)
	{
		delete sprite;
	}
	m_vCoolDownIcons.clear();
}

bool
HUD::Initialise(Renderer& renderer, Player* player, SafeArea* safeArea)
{
	m_pPlayer = player;
	m_pSafeArea = safeArea;

	SDL_Color color = { 255, 255, 255, 255 };

	TTF_Font* font = TTF_OpenFont("assets//fonts//Square.ttf", 32);

	// HP
	m_pHPTextSprite = renderer.CreateStaticTextSprite("Player HP: ", font, color, SpriteOrigin::BOTTOM_LEFT);
	m_pHPSprite = renderer.CreateSprite("assets//sprites//ui//heart.png", SpriteOrigin::BOTTOM_LEFT);

	// Safe Area Items
	m_pSafeAreaItemCountText = renderer.CreateStaticTextSprite("Items Retrieved: ", font, color, SpriteOrigin::TOP_LEFT);
	m_pItemSprite = renderer.CreateSprite("assets//sprites//ui//item.png", SpriteOrigin::TOP_LEFT);

	// Objective
	m_pObjectiveText = renderer.CreateStaticTextSprite("Items Required: ", font, color, SpriteOrigin::TOP_LEFT);
	std::string s = std::to_string(safeArea->GetObjectiveCount());
	m_pObjectiveCount = renderer.CreateStaticTextSprite(s.c_str(), font, color, SpriteOrigin::TOP_LEFT);

	// Player Inventory
	m_pPlayerInventory = renderer.CreateStaticTextSprite("Items on Player: ", font, color, SpriteOrigin::BOTTOM_LEFT);

	// Win Text
	m_pWinText = renderer.CreateStaticTextSprite("You Win!", font, color, SpriteOrigin::CENTER);
	m_pWinText->SetScale(5.f);

	// Ability Bar
	m_pCoolDownBar = renderer.CreateSprite("assets//sprites//ui//bar.png", SpriteOrigin::BOTTOM_LEFT);
	m_vCoolDowns.push_back(renderer.CreateSprite("assets//sprites//ui//processbar.png", SpriteOrigin::BOTTOM_LEFT));
	m_vCoolDowns.push_back(renderer.CreateSprite("assets//sprites//ui//processbar.png", SpriteOrigin::BOTTOM_LEFT));
	m_vCoolDowns.push_back(renderer.CreateSprite("assets//sprites//ui//processbar.png", SpriteOrigin::BOTTOM_LEFT));

	m_vCoolDownIcons.push_back(renderer.CreateStaticTextSprite("Smoke ", font, color, SpriteOrigin::BOTTOM_RIGHT));
	m_vCoolDownIcons.push_back(renderer.CreateStaticTextSprite("Flash ", font, color, SpriteOrigin::BOTTOM_RIGHT));
	m_vCoolDownIcons.push_back(renderer.CreateStaticTextSprite("Tranq ", font, color, SpriteOrigin::BOTTOM_RIGHT));

	return true;
}

void
HUD::Process(float deltaTime, Input& input)
{
	for (unsigned int i = 0; i < m_vCoolDowns.size(); i++)
	{
		m_vCoolDowns.at(i)->SetScaleX(Lerp(0, 1 / m_pPlayer->GetAbilityManager()->GetAbility(i)->GetCoolDown(), m_pPlayer->GetAbilityManager()->GetAbility(i)->GetCoolDownTimer()));
		if (m_vCoolDowns.at(i)->GetWidth() == m_pCoolDownBar->GetWidth())
		{
			m_vCoolDowns.at(i)->SetRedTint(0.f);

			m_vCoolDowns.at(i)->SetBlueTint(0.f);
		}
		else
		{
			m_vCoolDowns.at(i)->SetRedTint(1.f);
			m_vCoolDowns.at(i)->SetBlueTint(1.f);
		}
	}
}

void
HUD::Draw(Renderer& renderer)
{
	m_pHPTextSprite->Draw(renderer, Vector2(0.f, renderer.GetHeight() - m_pPlayerInventory->GetHeight()), 0.f, DrawSpace::SCREEN_SPACE);
	DrawPlayerHearts(renderer);

	m_pPlayerInventory->Draw(renderer, Vector2(0.f, renderer.GetHeight()), 0.f, DrawSpace::SCREEN_SPACE);
	DrawPlayerHeldItems(renderer);

	m_pSafeAreaItemCountText->Draw(renderer, Vector2(0.f, m_pObjectiveText->GetHeight()), 0.f, DrawSpace::SCREEN_SPACE);
	DrawRetrievedItems(renderer);

	m_pObjectiveText->Draw(renderer, Vector2(0.f, 0.f), 0.f, DrawSpace::SCREEN_SPACE);
	m_pObjectiveCount->Draw(renderer, Vector2(m_pObjectiveText->GetWidth(), 0.f), 0.f, DrawSpace::SCREEN_SPACE);

	if (m_pSafeArea->isLevelComplete())
	{
		m_pWinText->Draw(renderer, Vector2(renderer.GetWidth() / 2, renderer.GetHeight() / 2), 0.f, DrawSpace::SCREEN_SPACE);
	}

	// Cooldown Bars
	for (unsigned int i = 0; i < m_vCoolDowns.size(); i++)
	{
		m_vCoolDownIcons.at(i)->Draw(renderer, Vector2(renderer.GetWidth() - m_pCoolDownBar->GetWidth(), renderer.GetHeight() - m_pCoolDownBar->GetHeight() * i), 0.f, DrawSpace::SCREEN_SPACE);
		m_vCoolDowns.at(i)->Draw(renderer, Vector2(renderer.GetWidth() - m_pCoolDownBar->GetWidth(), renderer.GetHeight() - m_pCoolDownBar->GetHeight() * i), 0.f, DrawSpace::SCREEN_SPACE);
		m_pCoolDownBar->Draw(renderer, Vector2(renderer.GetWidth() - m_pCoolDownBar->GetWidth(), renderer.GetHeight() - m_pCoolDownBar->GetHeight() * i), 0.f, DrawSpace::SCREEN_SPACE);
	}
}

void
HUD::DrawPlayerHearts(Renderer& renderer)
{
	for (int i = 0; i < m_pPlayer->GetPlayerHP(); i++)
	{
		m_pHPSprite->Draw(renderer, Vector2(m_pHPTextSprite->GetWidth() + (m_pHPSprite->GetWidth() * i), renderer.GetHeight() - m_pSafeAreaItemCountText->GetHeight()), 0.f, DrawSpace::SCREEN_SPACE);
	}
}

void
HUD::DrawRetrievedItems(Renderer& renderer)
{
	for (int i = 0; i < m_pSafeArea->GetStashCount(); i++)
	{
		m_pItemSprite->Draw(renderer, Vector2(m_pSafeAreaItemCountText->GetWidth() + (m_pItemSprite->GetWidth() * i), m_pObjectiveText->GetHeight()), 0.f, DrawSpace::SCREEN_SPACE);
	}
}

void
HUD::DrawPlayerHeldItems(Renderer& renderer)
{
	for (int i = 0; i < m_pPlayer->GetStorageCurrentSize(); i++)
	{
		m_pItemSprite->Draw(renderer, Vector2(m_pSafeAreaItemCountText->GetWidth() + (m_pItemSprite->GetWidth() * i), renderer.GetHeight() - m_pPlayerInventory->GetHeight()), 0.f, DrawSpace::SCREEN_SPACE);
	}
}

void
HUD::DebugDraw()
{
#ifdef _DEBUG
	ImGui::Text("Current Player storage: %d", m_pPlayer->GetStorageCurrentSize());
#endif // _DEBUG
}