#include "pch.h"
#include "CPersonList.h"
#include "CPropile.h"
#include "CRoomMgr.h"
#include "CTransform.h"
#include "CEventMgr.h"

CPersonList::CPersonList():
	m_vStartSpawnPos(Vector2{-530.f,170.f}),
	m_vOffsetSpawnPos(Vector2{0.f,-95.f})
{

}

CPersonList::~CPersonList()
{

}

void CPersonList::tick()
{
	CUI::tick();
}

void CPersonList::MouseOn()
{
	CUI::MouseOn();
}

void CPersonList::MouseLbtnDown()
{
	CUI::MouseLbtnDown();
}

void CPersonList::MouseLbtnUp()
{
	CUI::MouseLbtnUp();
}

void CPersonList::MouseLbtnClicked()
{
	CUI::MouseLbtnClicked();
}

void CPersonList::MouseRelease()
{
	CUI::MouseRelease();
}

void CPersonList::add_propile(const wstring& _strName)
{
	CPropile* pPropile = new CPropile();
	pPropile->SetSpeech(_strName);
	pPropile->SetName(_strName);
	pPropile->Init();

	//위치 체크
	Vector2 vPos = find_spawn_pos();
	CTransform* pTransform = pPropile->GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);
	pTransform->SetPostion(Vector3{ vPos.x,vPos.y,0.f });

	AddChild(pPropile);
}

void CPersonList::erase_propile(const wstring& _strName)
{
	DeleteChild(_strName);

	//재정렬
	SortPropile();
}

void CPersonList::SortPropile()
{
	const vector<CObject*> vecChild = GetChilds();
	
	Vector2 vFinalPos = m_vStartSpawnPos;
	
	for (CObject* pChild : vecChild)
	{
		CTransform* pTransform = pChild->GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);
		float fOriginZ = pTransform->GetPosition().z;
		pTransform->SetPostion(Vector3{ vFinalPos.x,vFinalPos.y,fOriginZ });

		vFinalPos.y += m_vOffsetSpawnPos.y;
	}
}

Vector2 CPersonList::find_spawn_pos()
{
	const vector<CObject*> vecChild = GetChilds();
	int iChildCount = vecChild.size();
	if (iChildCount <= 0)
		return m_vStartSpawnPos;

	Vector2 vFinalPos = m_vStartSpawnPos;

	for (int i = 1; i <= vecChild.size(); ++i)
	{
		vFinalPos.y += m_vOffsetSpawnPos.y;
	}

	return vFinalPos;
}
