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

Vector2 CSpeechObject::GetOffsetPosition()
{
	CTransform* pTrasnform = GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);
	Vector3 vPosition = pTrasnform->GetPosition();

	Vector2 vOffset = Resolution / 2.f;

	float fX = vPosition.x + vOffset.x;
	float fY = vOffset.y - vPosition.y;

	Vector2 vWindowPos = Vector2{ fX,fY };
	vWindowPos.y += CRoomMgr::GetInst()->GetUIOffset();
	return vWindowPos;
}

void CSpeechObject::Speech(const wstring& _strSpeech)
{
	Vector2 vWindowPos = GetOffsetPosition();
	vWindowPos -= (m_fFontSize / 2.f);
	
	if (vWindowPos.y < 0)
		return;

	CFontMgr::GetInst()->AddFont(_strSpeech, vWindowPos.x , vWindowPos.y , m_fFontSize, m_iFontColor);
}

void CSpeechObject::Speech(const wstring& _strSpeech, bool _bCenterAlignedX, bool _bCenterAlignedY, const Vector2& _vOffset)
{
	Vector3 vTransformScale = GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM)->GetScale();

	Vector2 vWindowPos = GetOffsetPosition();

	if (_bCenterAlignedX == false)
		vWindowPos.x -= (vTransformScale.x /2.f);
	if (_bCenterAlignedY == false)
		vWindowPos.y -= (vTransformScale.y / 2.f);
	
	vWindowPos -= (m_fFontSize / 2.f); //center
	vWindowPos += _vOffset;
	
	if (vWindowPos.y < 0)
		return;

	CFontMgr::GetInst()->AddFont(_strSpeech, vWindowPos.x, vWindowPos.y, m_fFontSize, m_iFontColor);
}

void CSpeechObject::SubSpeech(const wstring& _strSpeech, float _fFontSize, const Vector2& _vOffset)
{
	Vector2 vWindowPos = GetOffsetPosition();

	vWindowPos += _vOffset;
	vWindowPos -= (_fFontSize / 2.f);

	if (vWindowPos.y < 0)
		return;

	CFontMgr::GetInst()->AddFont(_strSpeech, vWindowPos.x, vWindowPos.y, _fFontSize, m_iFontColor);
}


