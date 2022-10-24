#ifndef __MAINMENU_H__				 
#define __MAINMENU_H__ 				 

// Class declaration: 			
class Renderer;
class Sprite;
class Input;

class MainMenu
{
	// Member methods: 								 
public:
	MainMenu();
	~MainMenu();
	bool Initialise(Renderer& renderer);
	void Process(float deltaTime, Input& input);
	void Draw(Renderer& renderer);

protected:

private:
	MainMenu(const MainMenu& m);
	MainMenu& operator=(const MainMenu& m);

	// Member data: 								 
public:

protected:
	Sprite* m_pBackgroundImage;
	Sprite* m_pTitleText;

	Sprite* m_pPlayText;
	Sprite* m_pControls;
	Sprite* m_pQuitText;

	Sprite* m_pHover;

	int m_iMenuSelector;

private:

};

#endif // __MAINMENU_H__			 
