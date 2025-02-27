#include "pch.h"
#include "CObject.h"
#include "CMeshRender.h"
#include "CTransform.h"
#include "CRoomMgr.h"
#include "CEventMgr.h"

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
		_pObj->m_pParent->EraseChild(_pObj);
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

CObject* CObject::EraseChild(CObject* _pObj)
{
	vector<CObject*>::iterator iter = m_vecChild.begin();

	for (iter; iter != m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			CObject* pTarget = (*iter);
			pTarget->m_pParent = nullptr;
			m_vecChild.erase(iter);

			return pTarget;
		}
	}
	return nullptr;
}

CObject* CObject::EraseChild(const wstring& _strName)
{
	vector<CObject*>::iterator iter = m_vecChild.begin();

	for (iter; iter != m_vecChild.end(); ++iter)
	{
		if ((*iter)->GetName() == _strName)
		{
			CObject* pTarget = (*iter);
			pTarget->m_pParent = nullptr;
			m_vecChild.erase(iter);

			return pTarget;
		}
	}

	return nullptr;
}

CObject* CObject::FindChild(CObject* _pObj)
{
	
	vector<CObject*>::iterator iter = m_vecChild.begin();

	for (iter; iter != m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			return *iter;
		}
	}
	return nullptr;
}

CObject* CObject::FindChild(const wstring& _strName)
{
	vector<CObject*>::iterator iter = m_vecChild.begin();

	for (iter; iter != m_vecChild.end(); ++iter)
	{
		if ((*iter)->GetName() == _strName)
		{
			return *iter;
		}
	}
	return nullptr;
}

bool CObject::DeleteChild(const wstring& _strName)
{
	CObject* pEraseObject = EraseChild(_strName);
	if (pEraseObject != nullptr)
	{
		//찾은 오브젝트의 자식들이 있다면 내쪽으로 편입
		const vector<CObject*>& vecChild = pEraseObject->GetChilds();
		if (vecChild.empty() == false)
		{
			for (CObject* pChild : vecChild)
			{
				AddChild(pChild);
			}
		}
		pEraseObject->m_pParent = nullptr;
		CEventMgr::GetInst()->DeleteObject(pEraseObject);

		return true;
	}
	
	return false;
}

bool CObject::DeleteChild(CObject* _pObj)
{
	return false;
}
