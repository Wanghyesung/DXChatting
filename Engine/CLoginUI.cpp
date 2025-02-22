#include "pch.h"
#include "CLoginUI.h"
#include "CRoomMgr.h"
#include "CSpeechBar.h"
CLoginUI::CLoginUI():
	m_pNameBar(nullptr)
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
	if (m_pNameBar == nullptr)
		return;

	const wstring& strName = m_pNameBar->GetSpeech();
	if (strName.size() == 0)
	{
		MessageBox(nullptr, L"�̸��� �Է����ּ���", L"ä�ù� ���� ����", MB_OK);
		return;
	}

	CRoomMgr::GetInst()->SetClientName(strName);

	CUI::MouseLbtnClicked();
}

