#include "soundmanager.h"

#include <cassert>
#include <logmanager.h>
#include <fmod.hpp>

SoundManager* SoundManager::sm_pInstance = nullptr;

FMOD::System& 
SoundManager::GetInstance()
{
	if (sm_pInstance == nullptr)
	{
		sm_pInstance = new SoundManager();
	}

	return *(sm_pInstance->GetFMODSystem());
}

void 
SoundManager::DestroyInstance()
{
	sm_pInstance->GetFMODSystem()->release();
	delete sm_pInstance;
	sm_pInstance = nullptr;
}

SoundManager::SoundManager()
{
	if (FMOD::System_Create(&m_pFMODsystem) != FMOD_OK)
	{
		LogManager::GetInstance().Error("Could not create FMOD instance!");
		assert(0);
	}

	if (m_pFMODsystem->init(512, FMOD_INIT_NORMAL, NULL))
	{
		LogManager::GetInstance().Error("FMOD failed to initalise!");
		assert(0);
	}

	assert(m_pFMODsystem);
	LogManager::GetInstance().Log("FMOD Initialised.");
}

SoundManager::~SoundManager()
{

}

FMOD::System*
SoundManager::GetFMODSystem()
{
	return m_pFMODsystem;
}