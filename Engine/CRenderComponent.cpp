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

shared_ptr<CMaterial> CRenderComponent::GetMaterial()
{
	if (m_pMaterial == nullptr)
		assert(nullptr);

	return m_pMaterial;
}

shared_ptr<CMaterial> CRenderComponent::GetSharedMaterial()
{
	m_pMaterial = m_pSharedMaterial;

	if (m_pDynamicMaterial != nullptr)
		m_pDynamicMaterial.reset();

	return m_pSharedMaterial;
}

shared_ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	if (m_pSharedMaterial == nullptr)
		assert(nullptr);

	if (m_pDynamicMaterial == nullptr)
	{
		
		m_pDynamicMaterial = shared_ptr<CMaterial>(m_pSharedMaterial->Clone());
		m_pDynamicMaterial->SetName(m_pSharedMaterial->GetName() + L"_Clone");
		m_pMaterial = m_pDynamicMaterial;
	}

	return m_pMaterial;
}
