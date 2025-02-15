#include "pch.h"
#include "CUIMgr.h"
#include "CRoomMgr.h"
#include "CLayer.h"
#include "CKeyMgr.h"

CUIMgr::CUIMgr():
	m_pTargetUI(nullptr)
{

}
CUIMgr::~CUIMgr()
{

}

void CUIMgr::init()
{

}

void CUIMgr::tick()
{
	CUI* pFocuseUI = GetFocuseUI();

	m_pTargetUI = GetTargetUI(pFocuseUI);

	if (m_pTargetUI == nullptr)
		return;

	KEY_STATE LBTN = CKeyMgr::GetInst()->GetKeyState(KEY::LBTN);

	m_pTargetUI->MouseOn();

	if (LBTN == KEY_STATE::TAP)
	{
		m_pTargetUI->m_bLbntDown = true;
		m_pTargetUI->MouseLbtnDown();
	}

	else if (LBTN == KEY_STATE::RELEASE)
	{
		m_pTargetUI->MouseLbtnUp();

		if (m_pTargetUI->m_bLbntDown == true)
		{
			m_pTargetUI->MouseLbtnClicked();
			m_pTargetUI->m_bLbntDown = false;
		}

		m_pTargetUI->m_bMouseOn = false;
	}

}



CUI* CUIMgr::GetFocuseUI()
{
	const vector<CObject*>& vecUI = CRoomMgr::GetInst()->GetUIs();

	CUI* pFocuseUI = nullptr;
	for (int i = 0; i < vecUI.size(); ++i)
	{
		CUI* pParentUI = static_cast<CUI*>(vecUI[i]);
		if (pParentUI->m_bMouseOn)
		{
			pFocuseUI = pParentUI;
		}
	}
	
	return pFocuseUI;
}

CUI* CUIMgr::GetTargetUI(CUI* _pFocuseUI)
{
	if (_pFocuseUI == nullptr)
		return nullptr;

	queue<CUI*> queueUI;
	queueUI.push(_pFocuseUI);

	CUI* pTargetUI = nullptr;

	vector<CUI*> vecReleaseUI;
	while (!queueUI.empty())
	{
		pTargetUI = queueUI.front();
		queueUI.pop();

		for (CUI* pChildUI : pTargetUI->m_vecChildUI)
		{
			if (pChildUI->m_bMouseOn)
			{
				//기존 UI 해제
				vecReleaseUI.push_back(pTargetUI);

				queueUI.push(pChildUI);
			}
			{
				vecReleaseUI.push_back(pChildUI);
			}
		}
	}
	for (int i = 0; i < vecReleaseUI.size(); ++i)
		vecReleaseUI[i]->m_bMouseOn = false;

	return pTargetUI;
}