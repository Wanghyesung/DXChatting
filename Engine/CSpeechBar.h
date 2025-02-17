#pragma once
#include "CSpeechObject.h"

class CSpeechBar : public CSpeechObject
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
	class CSpeechBubble* m_pTarget;

	bool m_bStartBar;
	float m_fRepeatTimer;
	float m_fCurTime;
};

