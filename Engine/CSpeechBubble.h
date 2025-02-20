#pragma once
#include "CSpeechObject.h"

class CSpeechBubble : public CSpeechObject
{
public:
	CSpeechBubble();
	virtual ~CSpeechBubble();
	
public:
	void init(bool _bOther = false);

protected:
	virtual void tick()override;

protected:
	virtual void MouseOn()override;
	virtual void MouseLbtnDown()override;
	virtual void MouseLbtnUp()override;
	virtual void MouseLbtnClicked()override;


private:
	Vector2 m_vOffset;
};

