#pragma once
#include "CSingleton.h"
#include "CUI.h"

class CUIMgr : public CSingleton<CUIMgr>
{
	friend class CSingleton;

private:
	CUIMgr();
	virtual ~CUIMgr();

public:
	void init();
	void tick();

private:
	CUI* GetTargetUI(CUI* _pFocuseUI);
	CUI* GetFocuseUI();

private:
	CUI* m_pTargetUI;

	CUI* m_pClickUI;
	CUI* m_pPrevClickUI;
};

