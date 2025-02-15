#pragma once
#include "CObject.h"

class CUI : public CObject
{
	
	friend class CUIMgr;
public:
	CUI();
	virtual ~CUI();

public:
	void AddChild(CUI* _pUI);

protected:
	virtual void tick()override;
	
protected:
	virtual void MouseOn();
	virtual void MouseLbtnDown();
	virtual void MouseLbtnUp();
	virtual void MouseLbtnClicked();

private:
	bool check();
	
private:
	vector<CUI*> m_vecChildUI;

	bool m_bMouseOn;
	bool m_bLbntDown;
	bool m_bParent;
};

