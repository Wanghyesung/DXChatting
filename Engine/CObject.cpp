#include "pch.h"
#include "CObject.h"
#include "CMeshRender.h"
#include "CTransform.h"
#include "CRoomMgr.h"

CObject::CObject():
	m_arrComponent{},
	m_vecChild{},
	m_pRenderComponent(nullptr),
	m_eCurLayer(LAYER_TYPE::END)
{
	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		delete m_vecChild[i];
		m_vecChild[i] = nullptr;
	}
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

void CObject::begin()
{
	for (int i = 0; i < COMPONENT_TYPE::END; ++i)
	{
		if (m_arrComponent[i] != nullptr)
			m_arrComponent[i]->begin();
	}

	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		m_arrComponent[i]->begin();
	}
}

void CObject::tick()
{
	for (int i = 0; i < COMPONENT_TYPE::END; ++i)
	{
		if(m_arrComponent[i] != nullptr)
			m_arrComponent[i]->tick();
	}
	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->tick();
	}
}

void CObject::finaltick()
{
	for (int i = 0; i < COMPONENT_TYPE::END; ++i)
	{
		if (m_arrComponent[i] != nullptr)
			m_arrComponent[i]->final_tick();
	}

	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finaltick();
	}
}

void CObject::render()
{
	if (m_pRenderComponent != nullptr)
	{
		if (m_pRenderComponent != nullptr)
			m_pRenderComponent->render();
	}
	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->render();
	}
}



void CObject::SetComponent(CComponent* _pComponent)
{
	COMPONENT_TYPE eType = _pComponent->GetComponentType();
	if (m_arrComponent[eType] == nullptr)
	{
		m_arrComponent[eType] = _pComponent;
		_pComponent->SetOwner(this);
	}
	

	//만약 렌더컴포넌트이면
	if (eType >= COMPONENT_TYPE::MESH_RENDER)
	{
		assert(m_pRenderComponent == nullptr);

		m_pRenderComponent = GetComponent<CRenderComponent>(eType);
	}
}

void CObject::AddChild(CObject* _pObj)
{
	
	//추가할 오브젝트가 부모 오브젝트가 있다면
	if (_pObj->m_pParent != nullptr)
	{
		_pObj->m_pParent->DeleteChild(_pObj);
	}
	else
	{
		//아직 레이어에 등록되지 않은 오브젝트는 배제
		if(_pObj->GetLayer() != LAYER_TYPE::END)
			CRoomMgr::GetInst()->EraseObject(_pObj, _pObj->GetLayer());
	}
	
	m_vecChild.push_back(_pObj);
	_pObj->m_pParent = this;

	//나보다 앞에서 보이게
	CTransform* pParentTransform = GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);
	CTransform* pChildTransform = _pObj->GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);

	float fParentZ = pParentTransform->GetPosition().z - 0.1f;
	Vector3 vChildPos = pChildTransform->GetPosition();
	vChildPos.z = fParentZ;
	pChildTransform->SetPostion(vChildPos);

}

bool CObject::DeleteChild(CObject* _pObj)
{
	vector<CObject*>& vecChild = _pObj->m_vecChild;
	vector<CObject*>::iterator iter = vecChild.begin();

	for (iter; iter != vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			(*iter)->m_pParent = nullptr;
			vecChild.erase(iter);

			return true;
		}
	}
	return false;
}

bool CObject::DeleteChild(const wstring& _strName)
{
	vector<CObject*>& vecUI = m_pParent->m_vecChild;
	vector<CObject*>::iterator iter = vecUI.begin();

	for (iter; iter != vecUI.end(); ++iter)
	{
		if ((*iter)->GetName() == _strName)
		{
			(*iter)->m_pParent = nullptr;
			vecUI.erase(iter);

			return true;
		}
	}

	return false;
}
