#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

class Game;
class Renderer;
class Camera;
class Scene;
class b2World;

class SceneManager
{
public:
	static SceneManager& GetInstance();
	static void DestroyInstance();
	
	static Scene& GetActiveScene();
	static bool SwitchScenes(unsigned int index);

	bool Initialise(Game& game, Scene** scenes, Camera& camera, Renderer& renderer);

	void DebugDraw();

	void Exit();

	Camera* GetCamera();

protected:
private:
	SceneManager();
	~SceneManager();
	SceneManager(const SceneManager& s);
	SceneManager& operator=(const SceneManager& s) {}

public:
	static const unsigned int sm_uiSceneCount = 2;
	b2World* m_pWorld;
	
protected:
	static SceneManager* sm_pInstance;
	Scene* m_pScenes[sm_uiSceneCount];
	unsigned int m_iActiveScene;

	Game* m_pGame;
	Renderer* m_pRenderer;
	Camera* m_pCamera;


	//ImGUI
	int m_iSceneToChangeToo;

private:
};

#endif // __SCENEMANAGER_H__