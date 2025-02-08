#include "pch.h"
#include "CComponent.h"

CComponent::CComponent(COMPONENT_TYPE _eComponentType):
	m_eComponentType(_eComponentType),
	m_pOwner(nullptr)
{
}
