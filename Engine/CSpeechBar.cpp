#include "pch.h"
#include "CSpeechBar.h"
#include "CSpeechBubble.h"
#include "CTimeMgr.h"
#include "CFontMgr.h"

CSpeechBar::CSpeechBar() :
	m_strSpeech{},
	m_tSpeechInfo{},
	m_fRepeatTimer(0.5f),
	m_fCurTime(0.f),
	m_bStartBar(false)
{

}

CSpeechBar::~CSpeechBar()
{

}

void CSpeechBar::tick()
{
	if (m_strSpeech.size() == 0)
		repeat_startbar();
}

void CSpeechBar::repeat_startbar()
{
	check_time();

	if (m_bStartBar)
		CFontMgr::GetInst()->AddFont(L"|", 350, 670, 20, FONT_RGBA(255, 255, 255, 255));
	else
		CFontMgr::GetInst()->AddFont(L"", 350, 670, 20, FONT_RGBA(255, 255, 255, 255));
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

