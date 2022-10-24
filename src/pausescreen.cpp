// This include: 					  
#include "pausescreen.h"  

#include <engine/xinputcontroller.h>
#include <engine/renderer.h>
#include <engine/sprite.h>
#include <engine/scenemanager.h>
#include <game/game.h>

#ifdef _DEBUG
#include <imgui/imgui_impl_sdl.h>
#endif // _DEBUG

PauseScreen::PauseScreen()
{

}

PauseScreen::~PauseScreen()
{
	delete m_pPauseSprite;
	m_pPauseSprite = 0;

	delete m_pContinueText;
	m_pContinueText = 0;

	delete m_pMainMenu;
	m_pMainMenu = 0;

	delete m_pQuitText;
	m_pQuitText = 0;

	delete m_pHover;
	m_pHover = 0;
}

bool 
PauseScreen::Initialise(Renderer& renderer)
{
	m_pPauseSprite = renderer.CreateSprite("assets//sprites//ui//pausescreen.png", SpriteOrigin::CENTER);

	TTF_Font* font = TTF_OpenFont("assets//fonts//Square.ttf", 32);
	SDL_Color color = { 255, 255, 255, 255 };

	m_pContinueText = renderer.CreateStaticTextSprite("Continue", font, color, SpriteOrigin::CENTER);
	m_pMainMenu = renderer.CreateStaticTextSprite("Main Menu", font, color, SpriteOrigin::CENTER);
	m_pQuitText = renderer.CreateStaticTextSprite("Quit", font, color, SpriteOrigin::CENTER);

	m_pHover = renderer.CreateSprite("assets//sprites//ui//hover.png" ,SpriteOrigin::CENTER);

	m_iMenuSelector = 0;

	isPaused = false;

	return true;
}

void 
PauseScreen::Process(float deltaTime, Input& input)
{
	if (input.GetController(0)->GetButtonState(SDL_CONTROLLER_BUTTON_START) == BS_PRESSED)
	{
		isPaused = !isPaused;
		Game::GetInstance().SetPaused(!Game::GetInstance().GetPaused());
	}

	// Handle paused states
	if (isPaused)
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
				isPaused = !isPaused;
				Game::GetInstance().SetPaused(!Game::GetInstance().GetPaused());
			}
			else if (m_iMenuSelector == 1)
			{
				// Go back to main menu
				SceneManager::GetInstance().SwitchScenes(0);
			}
			else if (m_iMenuSelector == 2)
			{
				Game::GetInstance().Quit();
			}
		}
	}
}

void 
PauseScreen::Draw(Renderer& renderer)
{
	if (isPaused)
	{
		m_pPauseSprite->Draw(renderer, Vector2(renderer.GetWidth() / 2, renderer.GetHeight() / 2), 0.f, DrawSpace::SCREEN_SPACE);

		m_pHover->Draw(renderer, Vector2(renderer.GetWidth() / 2, renderer.GetHeight() / 2 + (m_pContinueText->GetHeight() + 32.f) * m_iMenuSelector), 0.f, DrawSpace::SCREEN_SPACE);

		// Texts
		m_pContinueText->Draw(renderer, Vector2(renderer.GetWidth() / 2, renderer.GetHeight() / 2), 0.f, DrawSpace::SCREEN_SPACE);
		m_pMainMenu->Draw(renderer, Vector2(renderer.GetWidth() / 2, renderer.GetHeight() / 2 + m_pContinueText->GetHeight() + 32.f), 0.f, DrawSpace::SCREEN_SPACE);
		m_pQuitText->Draw(renderer, Vector2(renderer.GetWidth() / 2, renderer.GetHeight() / 2 + m_pContinueText->GetHeight() + m_pMainMenu->GetHeight() + 64.f), 0.f, DrawSpace::SCREEN_SPACE);
	}
}

void 
PauseScreen::DebugDraw()
{
	ImGui::Text("Current Menu: %d", m_iMenuSelector);
}

bool 
PauseScreen::GetIsPaused()
{
	return isPaused;
}

void 
PauseScreen::SetIsPaused(bool pause)
{
	isPaused = pause;
}