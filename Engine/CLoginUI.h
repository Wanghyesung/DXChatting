#pragma once
#include "CSpeechObject.h"

class CSpeechBar;

class CLoginUI : public CSpeechObject
{

public:
	CLoginUI();
	virtual ~CLoginUI();

public:
	void SetSpeechBar(CSpeechBar* _pBar) { m_pNameBar = _pBar; }
	void SetLogin(bool _bLogin) { m_bLogin = _bLogin; }

protected:
	virtual void tick()override;

protected:
	virtual void MouseOn() override;
	virtual void MouseLbtnDown()override;
	virtual void MouseLbtnUp()override;
	virtual void MouseLbtnClicked()override;
	virtual void MouseRelease()override;


private:
	class CSpeechBar* m_pNameBar;

	bool m_bLogin;
};

