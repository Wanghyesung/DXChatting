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
	void SetSendPktFunc(std::function<void(void)> _pFunc) { m_pSendPktFunc = _pFunc; }
	
	void SetMaxSpeechSize(UINT _iSize) { m_iMaxSpeechSize = _iSize; }
	UINT GetMaxSpeechSize() { return m_iMaxSpeechSize; }


	void ShowText();
	void ShowOtherText(const wstring& _strText);


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

	void tick_offsetpos();

	void create_text(const wstring& _strText = L"");
private:
	wstring m_strBaseSpeech;
	wstring m_strPrevBaseSpeech;
	Vector3 m_vStaticPos;

	bool m_bStartBar;
	bool m_bCheckTime;

	float m_fRepeatTimer;
	float m_fCurTime;

	UINT m_iMaxSpeechSize;

	std::function<void(void)> m_pSendPktFunc;
};

