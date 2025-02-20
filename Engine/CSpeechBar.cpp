#include "pch.h"
#include "CSpeechBar.h"
#include "CTimeMgr.h"
#include "CFontMgr.h"
#include "CTransform.h"
#include "CDevice.h"
#include "CKeyMgr.h"
#include "CEventMgr.h"
#include "CSpeechBubble.h"
#include "CRoomMgr.h"

CSpeechBar::CSpeechBar():
	m_fRepeatTimer(0.5f),
	m_fCurTime(0.f),
	m_bStartBar(false),
	m_strTemWchar(L"")
{
	m_fFontSize = 20.f;
	m_iFontColor = FONT_RGBA(0, 0, 0, 255);
}

CSpeechBar::~CSpeechBar()
{

}

void CSpeechBar::tick()
{
	//카메라 위치와 맞게 y축
	tick_offsetpos();

	check_key();

	if (m_strTemWchar.empty() && m_strSpeech.empty())
	{
		repeat_startbar();
	}

	else
	{
		if (KEY_TAP(KEY::ENTER))
			send_data();
		else
			tick_speech();
	}

	CUI::tick();
}

void CSpeechBar::check_key()
{
	m_strTemWchar.clear();
	for (UINT i = (UINT)KEY::A; i <= (UINT)KEY::_9; ++i)
	{
		if (KEY_TAP((KEY)i))
		{
			m_strTemWchar += static_cast<WCHAR>(g_arrVK[i]);
		}
	}

	if (KEY_TAP(KEY::SPACE))
		test = !test;
}

void CSpeechBar::tick_speech()
{
	if (KEY_TAP(KEY::BACK_SPACE))
		m_strSpeech.pop_back();
	else if (KEY_TAP(KEY::SPACE))
		m_strSpeech.push_back(L' ');


	for (WCHAR ch : m_strTemWchar)
		m_strSpeech += ch;

	CSpeechObject::Speech(m_strSpeech, false, true, Vector2{ 30.f,0.f });
}

void CSpeechBar::send_data()
{
	//패킷 만들고 던지는 용 -> 서버에서 다른 클라로 보냈다고 확인 ack패킷이 오면  recv_data()함수에서 말풍선 추가

	CSpeechBubble* pSpeechBubble = new CSpeechBubble();
	pSpeechBubble->init(test);

	
	CEventMgr::GetInst()->CreateChatting(pSpeechBubble, m_strSpeech.c_str(), test);
	m_strSpeech.clear();
	//m_strSpeech.clear();
}

void CSpeechBar::recv_data()
{

}

void CSpeechBar::tick_offsetpos()
{
	CTransform* pTransform = GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);

	float fOffset = CRoomMgr::GetInst()->GetUIOffset();
	Vector3 vPos = m_vStaticPos;
	vPos.y += fOffset;

	pTransform->SetPostion(vPos);

}


void CSpeechBar::repeat_startbar()
{
	check_time();

	if (m_bStartBar == false)
	{
		CSpeechObject::Speech(L"|", false, true, Vector2{30.f,0.f});
	}
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
	CUI::MouseOn();
}

void CSpeechBar::MouseLbtnDown()
{
	CUI::MouseLbtnDown();
	//m_bStartBar = true;
}

void CSpeechBar::MouseLbtnUp()
{
	CUI::MouseLbtnUp();
}

void CSpeechBar::MouseLbtnClicked()
{
	CUI::MouseLbtnClicked();

}

