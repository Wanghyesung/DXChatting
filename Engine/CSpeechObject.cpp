#include "pch.h"
#include "CSpeechObject.h"
#include "CEngine.h"
#include "CTransform.h"
#include "CDevice.h"
CSpeechObject::CSpeechObject():
	m_strSpeech{},
	m_tSpeechInfo{},
	m_fFontSize(0.f),
	m_iFontColor(FONT_RGBA(255, 255, 255, 255))
{
}

CSpeechObject::~CSpeechObject()
{

}



void CSpeechObject::tick()
{
	CUI::tick();

	if (!m_strSpeech.empty())
		Speech(m_strSpeech);
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
	CFontMgr::GetInst()->AddFont(_strSpeech, vWindowPos.x , vWindowPos.y , m_fFontSize, m_iFontColor);
}

void CSpeechObject::Speech(const wstring& _strSpeech, bool _bCenterAlignedX, bool _bCenterAlignedY, const Vector2& _vOffset)
{
	Vector3 vTransformScale = GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM)->GetScale();

	Vector2 vWindowPos = GetWindowPosition();
	vWindowPos -= (m_fFontSize / 2.f);

	if (_bCenterAlignedX == false)
		vWindowPos.x -= (vTransformScale.x /2.f);
	if (_bCenterAlignedY == false)
		vWindowPos.y -= (vTransformScale.y / 2.f);
	
	vWindowPos += _vOffset;

	CFontMgr::GetInst()->AddFont(_strSpeech, vWindowPos.x, vWindowPos.y, m_fFontSize, m_iFontColor);
}


