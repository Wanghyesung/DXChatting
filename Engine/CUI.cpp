#include "pch.h"
#include "CUI.h"
#include "CComponent.h"
#include "CTransform.h"
#include "CKeyMgr.h"
#include "CRoomMgr.h"
#include "CFontMgr.h"

CUI::CUI():
	m_bMouseOn(false),
	m_bLbntDown(false),
	m_bParent(true),
	m_pParentUI(nullptr)
{
}

CUI::~CUI()
{
	for (int i = 0; i < m_vecChildUI.size(); ++i)
	{
		delete m_vecChildUI[i];
		m_vecChildUI[i] = nullptr;
	}
}

void CUI::AddChild(CUI* _pUI)
{
	//부모가 있는지 체크 , 있다면 기존 부모와 연결 해제 후 추가
	//부모가 없다면 기존 uilayer에서 해제
	if (_pUI->m_bParent == false)
	{
		_pUI->m_pParentUI->DeleteChild(_pUI);
	}
	else
	{
		CRoomMgr::GetInst()->EraseUI(_pUI);
	}

	m_vecChildUI.push_back(_pUI);
	_pUI->m_pParentUI = this;

	//나보다 앞에서 보이게
	CTransform* pParentUITransform = GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);
	CTransform* pChildUITransform = _pUI->GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);

	float fParentZ = pParentUITransform->GetPosition().z - 0.1f;
	Vector3 vChildPos = pChildUITransform->GetPosition();
	vChildPos.z = fParentZ;
	pChildUITransform->SetPostion(vChildPos);

}

void CUI::DeleteChild(CUI* _pUI)
{
	vector<CUI*>& vecUI = m_pParentUI->m_vecChildUI;
	vector<CUI*>::iterator iter = vecUI.begin();

	for (iter; iter!= vecUI.end(); ++iter)
	{
		if (*iter == this)
		{
			m_pParentUI = nullptr;
			vecUI.erase(iter);

			return;
		}
	}
	
}

void CUI::tick()
{
	CObject::tick();

	//부모 먼저
	check_mouse();

	for (int i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->tick();
	}
}

void CUI::MouseOn()
{
	int a = 10;
}

void CUI::MouseLbtnDown()
{
	int a = 10;
}

void CUI::MouseLbtnUp()
{
	int a = 10;
}

void CUI::MouseLbtnClicked()
{
	int a = 10;
}


bool CUI::check_mouse()
{
	const Vector2& vMousePos = CKeyMgr::GetInst()->GetMousePos();
	CTransform* pTrasnform = GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);
	
	Vector3 vPos = pTrasnform->GetPosition();
	Vector3 vScale = pTrasnform->GetScale();

	if ((vPos.x - vScale.x / 2.f) <= vMousePos.x && vMousePos.x <= (vPos.x + vScale.x / 2.f) &&
		(vPos.y - vScale.y / 2.f) <= vMousePos.y && vMousePos.y <= (vPos.y + vScale.y / 2.f))
		m_bMouseOn = true;
	else
		m_bMouseOn = false;

	return false;
}
