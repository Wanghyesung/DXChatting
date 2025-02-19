#pragma once
#include "CObject.h"

struct tUIEvent
{
	std::function<void(void)> pMouseOn;
	std::function<void(void)> pMouseDown;
	std::function<void(void)> pMouseUp;
	std::function<void(void)> pMouseClicked;
};

class CUI : public CObject
{
	
	friend class CUIMgr;
public:
	CUI();
	virtual ~CUI();

public:
	void AddChild(CUI* _pUI);
	void DeleteChild(CUI* _pUI);

	void SetMouseOnFunction(std::function<void(void)> _pFunc) { m_tUIEvent.pMouseOn = _pFunc; }
	void SetMouseDownFunction(std::function<void(void)> _pFunc) { m_tUIEvent.pMouseDown = _pFunc; }
	void SetMouseUpFunction(std::function<void(void)> _pFunc) { m_tUIEvent.pMouseUp = _pFunc; }
	void SetMouseClickedFunction(std::function<void(void)> _pFunc) { m_tUIEvent.pMouseClicked = _pFunc; }

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

	tUIEvent m_tUIEvent;
	bool m_bMouseOn;
	bool m_bLbntDown;
	bool m_bParent;
};

