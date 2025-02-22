#pragma once
#include "CUI.h"

class CPorpile;

//멀티스레드 환경

class CPersonList : public CUI
{
	friend class CRoomMgr;

public:
	CPersonList();
	virtual ~CPersonList();

protected:
	virtual void tick()override;

protected:
	virtual void MouseOn();
	virtual void MouseLbtnDown();
	virtual void MouseLbtnUp();
	virtual void MouseLbtnClicked();
	virtual void MouseRelease();
private:
	void add_propile(const wstring& _strName);

	Vector2 find_spawn_pos();

private:
	const Vector2 m_vStartSpawnPos;
	const Vector2 m_vOffsetSpawnPos;
};

