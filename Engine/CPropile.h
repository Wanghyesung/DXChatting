#pragma once
#include "CSpeechObject.h"
class CPropile : public CSpeechObject
{
public:
	CPropile();
	virtual ~CPropile();

public:
	void Init();

protected:
	virtual void tick()override;

protected:
	virtual void MouseOn();
	virtual void MouseLbtnDown();
	virtual void MouseLbtnUp();
	virtual void MouseLbtnClicked();


};

