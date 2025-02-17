#include "pch.h"
#include "CSpeechBar.h"
#include "CSpeechBubble.h"
#include "CTimeMgr.h"
#include "CFontMgr.h"
#include "CTransform.h"
#include "CSpeechBubble.h"
#include "CDevice.h"

CSpeechBar::CSpeechBar():
	m_fRepeatTimer(0.5f),
	m_fCurTime(0.f),
	m_bStartBar(false),
	m_pTarget(nullptr)
{
	m_fFontSize = 20.f;
	m_iFontColor = FONT_RGBA(255, 255, 255, 255);
}

CSpeechBar::~CSpeechBar()
{

}

void CSpeechBar::tick()
{
	CSpeechObject::tick();

	if (m_strSpeech.size() == 0)
		repeat_startbar();


}

void CSpeechBar::repeat_startbar()
{
	check_time();

	Vector2 vWindowPos = GetWindowPosition();

	if (m_bStartBar)
		CFontMgr::GetInst()->AddFont(L"|", vWindowPos.x, vWindowPos.y - m_fFontSize/2.f, m_fFontSize, m_iFontColor);

}

void CSpeechBar::check_time()
{
	m_fCurTime += DT;
	if (m_fCurTime >= m_fRepeatTimer)
	{
		m_fCurTime = 0.f;
		m_bStartBar = !m_bStartBar;
	}
}


void CSpeechBar::MouseOn()
{

}

void CSpeechBar::MouseLbtnDown()
{

}

void CSpeechBar::MouseLbtnUp()
{

}

void CSpeechBar::MouseLbtnClicked()
{

}

