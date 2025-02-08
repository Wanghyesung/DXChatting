#include "pch.h"
#include "CObject.h"
#include "CMeshRender.h"
#include "CTransform.h"

CObject::CObject():
	m_arrComponent{},
	m_pRenderComponent(nullptr)
{
}

CObject::~CObject()
{
	for (int i = 0; i < COMPONENT_TYPE::END; ++i)
	{
		if (m_arrComponent[i] != nullptr)
		{
			delete m_arrComponent[i];
			m_arrComponent[i] = nullptr;
		}
	}
}

void CObject::begine()
{
	for (int i = 0; i < COMPONENT_TYPE::END; ++i)
	{
		m_arrComponent[i]->begin();
	}
}

void CObject::tick()
{
	for (int i = 0; i < COMPONENT_TYPE::END; ++i)
	{
		m_arrComponent[i]->tick();
	}
}

void CObject::finaltick()
{
	for (int i = 0; i < COMPONENT_TYPE::END; ++i)
	{
		m_arrComponent[i]->final_tick();
	}
}

void CObject::render()
{
	if (m_pRenderComponent != nullptr)
	{
		m_pRenderComponent->render();
	}
}



void CObject::SetComponent(CComponent* _pComponent)
{
	COMPONENT_TYPE eType = _pComponent->GetComponentType();
	if (m_arrComponent[eType] == nullptr)
	{
		m_arrComponent[eType] = _pComponent;
	}
	

	//¸¸¾à ·»´õÄÄÆ÷³ÍÆ®ÀÌ¸é
	if (eType >= COMPONENT_TYPE::MESH_RENDER)
	{
		assert(m_pRenderComponent == nullptr);

		m_pRenderComponent = GetComponent<CRenderComponent>(eType);
	}
}
