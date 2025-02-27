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
	m_bCheckTime(false),
	m_strBaseSpeech(L""),
	m_strPrevBaseSpeech(L""),
	m_iMaxSpeechSize(10)
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

	if (m_strSpeech.empty() && m_bStartBar == false)
	{
		base_speech();
	}
	else 
	{
		check_key();
	
		CSpeechObject::Speech(m_strSpeech, false, true, Vector2{ 30.f,0.f });
	}

	CUI::tick();
}

void CSpeechBar::ShowText()
{
	create_text();
}

void CSpeechBar::ShowOtherText(const wstring& _strText)
{
	create_text(_strText);
}

void CSpeechBar::create_text(const wstring& _strText)
{
	CSpeechBubble* pSpeechBubble = new CSpeechBubble();
	if (_strText.empty())
	{
		pSpeechBubble->init(false);
		CEventMgr::GetInst()->CreateChatting(pSpeechBubble, m_strSpeech.c_str(), false);
		m_strSpeech.clear();
	}
	else
	{
		pSpeechBubble->init(true);
		CEventMgr::GetInst()->CreateChatting(pSpeechBubble, _strText.c_str(), true);
	}
}


void CSpeechBar::check_key()
{
	for (UINT i = (UINT)KEY::SPACE; i <= (UINT)KEY::_9; ++i)
	{
		if (KEY_TAP((KEY)i))
		{
			if (m_strSpeech.size() >= m_iMaxSpeechSize)
				continue;

			m_strSpeech += static_cast<WCHAR>(g_arrVK[i]);
		}
	}

	if (KEY_TAP(KEY::BACK_SPACE) && m_strSpeech.empty() == false)
		m_strSpeech.pop_back();

	if (KEY_TAP(KEY::ENTER))
		enter();
}

void CSpeechBar::enter()
{
	if (m_strSpeech.empty() || m_bStartBar == false)
		return;

	//패킷 전송
	if (m_pSendPktFunc)
		m_pSendPktFunc();
}


void CSpeechBar::tick_offsetpos()
{
	CTransform* pTransform = GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);

	float fOffset = CRoomMgr::GetInst()->GetUIOffset();
	Vector3 vPos = m_vStaticPos;
	vPos.y += fOffset;

	pTransform->SetPostion(vPos);

}


void CSpeechBar::base_speech()
{
	//기본말 세팅이 없다면
	if (m_strBaseSpeech.empty())
	{
		repeat_startbar();
	}
	else
	{
		CSpeechObject::Speech(m_strBaseSpeech, false, true, Vector2{30.f,0.f});
	}

}

void CSpeechBar::repeat_startbar()
{
	check_time();

	if(m_bCheckTime)
		CSpeechObject::Speech(L"|", false, true, Vector2{30.f,0.f});
	
}

void CSpeechBar::check_time()
{
	m_fCurTime += DT;
	if (m_fCurTime >= m_fRepeatTimer)
	{
		m_fCurTime = 0.f;
		m_bCheckTime = !m_bCheckTime;
	}
}


void CSpeechBar::MouseOn()
{
	CUI::MouseOn();
}

void CSpeechBar::MouseLbtnDown()
{
	m_bStartBar = true;

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

void CSpeechBar::MouseRelease()
{
	m_bStartBar = false;

	CUI::MouseRelease();
}

