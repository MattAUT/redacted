#include "mainmenu.h"

#include <engine/sprite.h>
#include <engine/renderer.h>
#include <engine/xinputcontroller.h> 
#include <engine/scenemanager.h>
#include "game/game.h"

MainMenu::MainMenu()
{

}

MainMenu::~MainMenu()
{

}

bool
MainMenu::Initialise(Renderer& renderer)
{
	TTF_Font* smallFont = TTF_OpenFont("assets//fonts//Square.ttf", 32);
	TTF_Font* largeFont = TTF_OpenFont("assets//fonts//Square.ttf", 128);
	SDL_Color color = { 255, 255, 255, 255 };

	m_pTitleText = renderer.CreateStaticTextSprite("Redacted", largeFont, color, SpriteOrigin::BOTTOM_CENTER);

	m_pBackgroundImage = renderer.CreateSprite("assets//sprites//ui//background.png", SpriteOrigin::CENTER);
	m_pBackgroundImage->SetAlpha(0.3f);

	m_pPlayText = renderer.CreateStaticTextSprite("Play", smallFont, color, SpriteOrigin::CENTER);
	m_pControls = renderer.CreateStaticTextSprite("Controls", smallFont, color, SpriteOrigin::CENTER);
	m_pQuitText = renderer.CreateStaticTextSprite("Quit", smallFont, color, SpriteOrigin::CENTER);

	m_pHover = renderer.CreateSprite("assets//sprites//ui//hover.png", SpriteOrigin::CENTER);

	m_iMenuSelector = 0;

	return true;
}

void 
MainMenu::Process(float deltaTime, Input& input)
{
	// Menu Navigation
	if (input.GetController(0)->GetButtonState(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BS_PRESSED)
	{
		if (m_iMenuSelector < 2)
		{
			m_iMenuSelector++;
		}
	}
	if (input.GetController(0)->GetButtonState(SDL_CONTROLLER_BUTTON_DPAD_UP) == BS_PRESSED)
	{
		if (m_iMenuSelector > 0)
		{
			m_iMenuSelector--;
		}
	}

	// Menu Selection
	if (input.GetController(0)->GetButtonState(SDL_CONTROLLER_BUTTON_A) == BS_PRESSED)
	{
		if (m_iMenuSelector == 0)
		{
			SceneManager::GetInstance().SwitchScenes(1);
		}
		else if (m_iMenuSelector == 1)
		{
			// Go to Controls panel
		}
		else if (m_iMenuSelector == 2)
		{
			Game::GetInstance().Quit();
		}
	}
}

void
MainMenu::Draw(Renderer& renderer)
{
	m_pBackgroundImage->Draw(renderer, Vector2(renderer.GetWidth() / 2, renderer.GetHeight() / 2), 0.f, DrawSpace::SCREEN_SPACE);

	m_pTitleText->Draw(renderer, Vector2(renderer.GetWidth() / 2, renderer.GetHeight() / 3), 0.f, DrawSpace::SCREEN_SPACE);

	m_pHover->Draw(renderer, Vector2(renderer.GetWidth() / 2, (renderer.GetHeight() / 3) * 2 + (m_pPlayText->GetHeight() + 32.f) * m_iMenuSelector), 0.f, DrawSpace::SCREEN_SPACE);

	// Texts
	m_pPlayText->Draw(renderer, Vector2(renderer.GetWidth() / 2, (renderer.GetHeight() / 3) * 2), 0.f, DrawSpace::SCREEN_SPACE);
	m_pControls->Draw(renderer, Vector2(renderer.GetWidth() / 2, (renderer.GetHeight() / 3) * 2 + m_pPlayText->GetHeight() + 32.f), 0.f, DrawSpace::SCREEN_SPACE);
	m_pQuitText->Draw(renderer, Vector2(renderer.GetWidth() / 2, (renderer.GetHeight() / 3) * 2 + m_pPlayText->GetHeight() + 98.f), 0.f, DrawSpace::SCREEN_SPACE);
}