#pragma once
#include "CUI.h"
#include "CFontMgr.h"


class CSpeechObject : public CUI
{
public:
	CSpeechObject();
	virtual ~CSpeechObject();

public:
	void SetFontColor(float r, float g, float b, float a) { m_iFontColor = FONT_RGBA(r, g, b, a); }
	void SetFontSize(float _fSize) { m_fFontSize = _fSize; }
	void SetSpeech(const wstring& _strSpeech) { m_strSpeech = _strSpeech; };

	float GetFontSize() { return m_fFontSize; }
protected:
	virtual void tick()override;

protected:
	virtual void MouseOn()override;
	virtual void MouseLbtnDown()override;
	virtual void MouseLbtnUp()override;
	virtual void MouseLbtnClicked()override;


protected:
	Vector2 GetWindowPosition();
	void Speech(const wstring& _strSpeech);
	void Speech(const wstring& _strSpeech, bool _bCenterAlignedX, bool _bCenterAlignedY, const Vector2& _vOffset = Vector2::Zero);

protected:
	wstring m_strSpeech;
	tFontInfo m_tSpeechInfo;

	UINT m_iFontColor;
	float m_fFontSize;
};