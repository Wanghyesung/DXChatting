#include "pch.h"
#include "CSpeechObject.h"
#include "CEngine.h"
#include "CTransform.h"
#include "CDevice.h"
CSpeechObject::CSpeechObject():
	m_strSpeech{},
	m_tSpeechInfo{},
	m_fFontSize(0.f),
	m_iFontColor(FONT_RGBA(0.f, 0.f, 0.f, 0.f))
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

}

void CSpeechObject::MouseLbtnDown()
{

}

void CSpeechObject::MouseLbtnUp()
{
}

void CSpeechObject::MouseLbtnClicked()
{
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
