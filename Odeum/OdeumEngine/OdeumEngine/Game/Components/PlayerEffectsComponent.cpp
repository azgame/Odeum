#include "PlayerEffectsComponent.h"



void PlayerEffectsComponent::OnAttach(GameObject* parent)
{
	m_gameObject = parent;
}

void PlayerEffectsComponent::OnStart()
{
}

void PlayerEffectsComponent::CheckForProccedEvents(GameEvent& Event)
{
	Notify(*m_gameObject, Event);
}
