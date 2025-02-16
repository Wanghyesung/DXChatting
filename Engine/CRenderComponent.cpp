#include "pch.h"
#include "CRenderComponent.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _eComponentType):
	CComponent(_eComponentType)
{
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::begin()
{
}

void CRenderComponent::final_tick()
{

}


shared_ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	m_pDynamicMaterial = shared_ptr<CMaterial>(m_pMaterial->Clone());
	m_pDynamicMaterial->SetName(m_pDynamicMaterial->GetName() + L"_Clone");
	return m_pDynamicMaterial;
}
