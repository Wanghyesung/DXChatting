#include "pch.h"
#include "CLoginUI.h"
#include "CRoomMgr.h"
#include "CSpeechBar.h"
CLoginUI::CLoginUI():
	m_pNameBar(nullptr),
	m_bLogin(false)
{

}

CLoginUI::~CLoginUI()
{

}

void CLoginUI::tick()
{
	if (!m_strSpeech.empty())
		Speech(m_strSpeech,false,true,Vector2{40.f,-7.f});

	CUI::tick();
}

void CLoginUI::MouseOn()
{
	CUI::MouseOn();
}

void CLoginUI::MouseLbtnDown()
{
	CUI::MouseLbtnDown();
}

void CLoginUI::MouseLbtnUp()
{

	CUI::MouseLbtnUp();
}

void CLoginUI::MouseRelease()
{
	CUI::MouseRelease();
}

void CLoginUI::MouseLbtnClicked()
{
	if (m_bLogin == true)
	{
		MessageBox(nullptr, L"로그인 요청 실패, 프로세스를 다시 실행해주세요", L"로그인 실패", MB_OK);
		return;
	}
		
	const wstring& strName = m_pNameBar->GetSpeech();
	if (strName.size() == 0)
	{
		MessageBox(nullptr, L"이름을 입력해주세요", L"채팅방 입장 실패", MB_OK);
		return;
	}

	CRoomMgr::GetInst()->SetClientName(strName);

	m_bLogin = true;
	//connect
	CUI::MouseLbtnClicked();
}

