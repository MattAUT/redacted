#ifndef __PAUSESCREEN_H__				 
#define __PAUSESCREEN_H__ 				 

// Class declaration: 			
class Renderer;
class Sprite;
class Input;

class PauseScreen
{
	// Member methods: 								 
public:
	PauseScreen();
	~PauseScreen();
	bool Initialise(Renderer& renderer);
	void Process(float deltaTime, Input& input);
	void Draw(Renderer& renderer);

	bool GetIsPaused();
	void SetIsPaused(bool pause);

#ifdef _DEBUG
	void DebugDraw();
#endif _DEBUG

protected:

private:
	PauseScreen(const PauseScreen& p);
	PauseScreen& operator=(const PauseScreen& p);

	// Member data: 								 
public:

protected:
	Sprite* m_pPauseSprite;

	Sprite* m_pContinueText;
	Sprite* m_pMainMenu;
	Sprite* m_pQuitText;

	Sprite* m_pHover;

	bool isPaused;

	int m_iMenuSelector;

private:

};

#endif // __PAUSESCREEN_H__			 
