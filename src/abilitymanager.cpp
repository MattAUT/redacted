#include "abilitymanager.h"

#include <engine/input.h>
#include <engine/xinputcontroller.h>
#include <logmanager.h>
#include <imgui/imgui_impl_sdl.h>
#include <engine/renderer.h>
#include "fmod.h"
#include "player.h"
#include "ability.h"

//Ability includes
#include "abilitysmokebomb.h"
#include "abilityflashlight.h"
#include "abilitytranqdart.h"

AbilityManager::AbilityManager()
	: m_bDebugSprites(false)
{
}

AbilityManager::~AbilityManager()
{
	for (Ability* ability : m_abilityVector)
	{
		delete ability;
	}
	m_abilityVector.clear();
}

bool AbilityManager::Initialise(Renderer& renderer, Player* player)
{
	m_pPlayer = player;

	m_pAbilitySmokeBomb = new AbilitySmokeBomb;
	m_pAbilitySmokeBomb->Initialise(renderer, *player, SDL_CONTROLLER_BUTTON_B);
	m_abilityVector.push_back(m_pAbilitySmokeBomb);

	m_pAbilityFlashLight = new AbilityFlashLight;
	m_pAbilityFlashLight->Initialise(renderer, *m_pPlayer, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
	m_abilityVector.push_back(m_pAbilityFlashLight);

	m_pAbilityTranq = new AbilityTranqDart;
	m_pAbilityTranq->Initialise(renderer, *m_pPlayer, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
	m_abilityVector.push_back(m_pAbilityTranq);

	return true;
}

void AbilityManager::Process(float deltaTime, Input& input)
{
	for (Ability* ability : m_abilityVector)
	{
		ability->m_bIsDebuging = m_bDebugSprites;
		ability->Process(deltaTime, input);
	}

}

void AbilityManager::Draw(Renderer& renderer)
{
	for (Ability* ability : m_abilityVector)
	{
		ability->Draw(renderer);
	}
}

Ability * AbilityManager::GetAbility(int i)
{
	if (i < m_abilityVector.size())
	{
		return m_abilityVector[i];
	}
	LogManager::GetInstance().Log("Trying to get ability out of range");
	return nullptr;
}

std::vector<Ability*>* AbilityManager::GetAllAbilities()
{
	return &m_abilityVector;
}

#ifdef _DEBUG
void AbilityManager::DebugDraw()
{
	ImGui::Text("--Abilities--");
	ImGui::Checkbox("Show debug sprites: ", &m_bDebugSprites);
	for (Ability* ability : m_abilityVector)
	{
		ability->DebugDraw();
	}
	ImGui::Text("    ------------    ");
}
#endif _DEBUG