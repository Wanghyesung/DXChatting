#pragma once
#include "CSpeechObject.h"

class CSpeechBar : public CSpeechObject
{
public:
	CSpeechBar();
	virtual ~CSpeechBar();

public:
	void SetStaticPos(Vector3 _vPos) { m_vStaticPos = _vPos; }
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

	void check_key();
	void tick_speech();

	void send_data();
	void recv_data();

	void tick_offsetpos();

private:

	wstring m_strTemWchar;
	Vector3 m_vStaticPos;

	bool m_bStartBar;
	float m_fRepeatTimer;
	float m_fCurTime;


	bool test = false;
};

