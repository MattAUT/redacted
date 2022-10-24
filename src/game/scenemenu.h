#ifndef __SCENEMENU_H__
#define __SCENEMENU_H__

#include <engine/scene.h>
#include <vector2.h>

// Forward Declarations
class SceneManager;
class Sprite;
class MainMenu;

namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

class SceneMenu : public Scene
{

	// Member Methods
public:
	SceneMenu();
	virtual ~SceneMenu();

	virtual bool Initialise(Renderer& renderer, Camera& camera) override;
	virtual void Process(float deltaTime, Input& input) override;
	virtual void Draw(Renderer& renderer) override;
	virtual void Reset() override;
	virtual void SetCurrentLevel(int level) override;
#ifdef _DEBUG
	virtual void DebugDraw(Renderer& renderer) override;
#endif // _DEBUG

protected:

private:
	SceneMenu(const SceneMenu& s);
	SceneMenu& operator=(const SceneMenu& s) {}

	// Member Data
public:
protected:
	MainMenu* m_pMainMenu;
	

private:

};

#endif // __SCENEMENU_H__

