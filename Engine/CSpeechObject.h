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
protected:
	virtual void tick()override;

protected:
	virtual void MouseOn()override;
	virtual void MouseLbtnDown()override;
	virtual void MouseLbtnUp()override;
	virtual void MouseLbtnClicked()override;


protected:
	Vector2 GetWindowPosition();

protected:
	wstring m_strSpeech;
	tFontInfo m_tSpeechInfo;

	UINT m_iFontColor;
	float m_fFontSize;
};