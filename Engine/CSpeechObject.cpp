#include "pch.h"
#include "CSpeechObject.h"
#include "CEngine.h"
#include "CTransform.h"
#include "CDevice.h"
#include "CRoomMgr.h"
CSpeechObject::CSpeechObject():
	m_strSpeech{},
	m_tSpeechInfo{},
	m_fFontSize(20.f),
	m_iFontColor(FONT_RGBA(0.f, 0.f, 0.f, 255))
{
}

CSpeechObject::~CSpeechObject()
{

}



void CSpeechObject::tick()
{
	CUI::tick();
}

void CSpeechObject::MouseOn()
{
	CUI::MouseOn();
}

void CSpeechObject::MouseLbtnDown()
{
	CUI::MouseLbtnDown();

}

void CSpeechObject::MouseLbtnUp()
{
	CUI::MouseLbtnUp();

}

void CSpeechObject::MouseLbtnClicked()
{
	CUI::MouseLbtnClicked();

}

void CSpeechObject::MouseRelease()
{
	CUI::MouseRelease();
}

Vector2 CSpeechObject::GetWindowPosition()
{
	CTransform* pTrasnform = GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);
	Vector3 vPosition = pTrasnform->GetPosition();

	Vector2 vOffset = Resolution / 2.f;

	float fX = vPosition.x + vOffset.x;
	float fY = vOffset.y - vPosition.y;

	return Vector2{ fX,fY };
}

void CSpeechObject::Speech(const wstring& _strSpeech)
{
	Vector2 vWindowPos = GetWindowPosition();
	vWindowPos -= (m_fFontSize / 2.f);
	vWindowPos.y += CRoomMgr::GetInst()->GetUIOffset();

	CFontMgr::GetInst()->AddFont(_strSpeech, vWindowPos.x , vWindowPos.y , m_fFontSize, m_iFontColor);
}

void CSpeechObject::Speech(const wstring& _strSpeech, bool _bCenterAlignedX, bool _bCenterAlignedY, const Vector2& _vOffset)
{
	Vector3 vTransformScale = GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM)->GetScale();

	Vector2 vWindowPos = GetWindowPosition();

	if (_bCenterAlignedX == false)
		vWindowPos.x -= (vTransformScale.x /2.f);
	if (_bCenterAlignedY == false)
		vWindowPos.y -= (vTransformScale.y / 2.f);
	
	vWindowPos -= (m_fFontSize / 2.f); //center
	vWindowPos += _vOffset;
	vWindowPos.y += CRoomMgr::GetInst()->GetUIOffset();

	CFontMgr::GetInst()->AddFont(_strSpeech, vWindowPos.x, vWindowPos.y, m_fFontSize, m_iFontColor);
}


