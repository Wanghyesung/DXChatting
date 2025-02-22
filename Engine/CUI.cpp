#include "pch.h"
#include "CUI.h"
#include "CComponent.h"
#include "CTransform.h"
#include "CKeyMgr.h"
#include "CRoomMgr.h"
#include "CFontMgr.h"

CUI::CUI():
	m_bMouseOn(false),
	m_bLbntDown(false)
{
}

CUI::~CUI()
{
}

void CUI::tick()
{
	//부모 먼저
	check_mouse();

	CObject::tick();

}

void CUI::MouseOn()
{
	if (m_tUIEvent.pMouseOn != nullptr)
		m_tUIEvent.pMouseUp();
}

void CUI::MouseLbtnDown()
{
	if (m_tUIEvent.pMouseOn != nullptr)
		m_tUIEvent.pMouseDown();
}

void CUI::MouseLbtnUp()
{
	if (m_tUIEvent.pMouseOn != nullptr)
		m_tUIEvent.pMouseUp();
}

void CUI::MouseLbtnClicked()
{
	if (m_tUIEvent.pMouseOn != nullptr)
		m_tUIEvent.pMouseClicked();
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
