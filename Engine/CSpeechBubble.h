#pragma once
#include "CSpeechObject.h"

class CSpeechBubble : public CSpeechObject
{
public:
	CSpeechBubble();
	virtual ~CSpeechBubble();
	
public:
	void init(bool _bOther = false);

	void SetClientName(const wstring& _strName) { m_strClientName = _strName; }

protected:
	virtual void tick()override;

protected:
	virtual void MouseOn()override;
	virtual void MouseLbtnDown()override;
	virtual void MouseLbtnUp()override;
	virtual void MouseLbtnClicked()override;
	virtual void MouseRelease()override;

private:
	Vector2 m_vOffset;
	Vector2 m_vSubOffset;

	wstring m_strClientName;
};

