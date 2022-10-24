#ifndef __SOUNDMANAGER_H__
#define __SOUDMANAGER_H__

namespace FMOD
{
	class System;
}

class SoundManager
{
public:
	static FMOD::System& GetInstance();
	static void DestroyInstance();

protected:
	FMOD::System* GetFMODSystem();

private:
	SoundManager();
	~SoundManager();

public:
protected:
	FMOD::System* m_pFMODsystem;
	static SoundManager* sm_pInstance;

private:
};

#endif // __SOUNDMANAGER_H__