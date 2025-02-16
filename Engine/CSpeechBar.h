#pragma once
#include "CUI.h"

class CSpeechBar : public CUI
{
public:
	CSpeechBar();
	virtual ~CSpeechBar();

protected:
	virtual void tick()override;

protected:
	virtual void MouseOn()override;
	virtual void MouseLbtnDown()override;
	virtual void MouseLbtnUp()override;
	virtual void MouseLbtnClicked()override;

private:
	void check_time();
	void repeat_startbar();
private:
	wstring m_strSpeech;
	tFontInfo m_tSpeechInfo;

	bool m_bStartBar;
	float m_fRepeatTimer;
	float m_fCurTime;
};

