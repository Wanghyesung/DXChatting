#pragma once
#include "CSpeechObject.h"


class CSpeechBar : public CSpeechObject
{
public:
	CSpeechBar();
	virtual ~CSpeechBar();

public:
	void SetStaticPos(Vector3 _vPos) { m_vStaticPos = _vPos; }
	void SetBaseSpeech(const wstring& _strBaseSpeech) { m_strBaseSpeech = _strBaseSpeech; }

protected:
	virtual void tick()override;

protected:
	virtual void MouseOn()override;
	virtual void MouseLbtnDown()override;
	virtual void MouseLbtnUp()override;
	virtual void MouseLbtnClicked()override;
	virtual void MouseRelease()override;
private:
	void check_time();
	void base_speech();
	void repeat_startbar();

	void check_key();
	void enter();

	void send_data();
	void recv_data();

	void tick_offsetpos();

private:
	wstring m_strBaseSpeech;
	wstring m_strPrevBaseSpeech;
	Vector3 m_vStaticPos;

	bool m_bStartBar;
	bool m_bCheckTime;

	float m_fRepeatTimer;
	float m_fCurTime;

	bool test = false;
};

