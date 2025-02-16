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
	void DeleteChild(CUI* _pUI);

protected:
	virtual void tick()override;
	
protected:
	virtual void MouseOn();
	virtual void MouseLbtnDown();
	virtual void MouseLbtnUp();
	virtual void MouseLbtnClicked();

private:
	bool check_mouse();
	
private:
	vector<CUI*> m_vecChildUI;

	CUI* m_pParentUI;

	bool m_bMouseOn;
	bool m_bLbntDown;
	bool m_bParent;
};

