#include "ability.h"

#include <imgui/imgui_impl_sdl.h>

Ability::Ability()
	: m_bIsActive(false)
	, m_bIsDebuging(false)
	, m_fCoolDown(0)
	, m_fCoolDownTimer(0)
	, m_fDuration(3)
	, m_fDurationCounter(0)
{
}

Ability::~Ability()
{
	delete m_pUserData;
	m_pUserData = 0;

	delete m_pSprite;
	m_pSprite = 0;

	delete m_pDebugSprite;
	m_pDebugSprite = 0;
}

void Ability::UseAbility(Vector2 pos)
{
}

float Ability::GetCoolDownTimer()
{
	return m_fCoolDownTimer;
}

float Ability::GetCoolDown()
{
	return m_fCoolDown;
}

#ifdef _DEBUG
void Ability::DebugDraw()
{
	ImGui::Text(m_sName.c_str());
	ImGui::Text("Is Active: %i", m_bIsActive);
	ImGui::Text("Duration Counter %f", m_fDurationCounter);
	ImGui::Text("CoolDown: %f / %f", m_fCoolDownTimer, m_fCoolDown);
}

#endif // _DEBUG
