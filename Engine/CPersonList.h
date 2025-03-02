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

	void SetStaticPos(const Vector3& _vStaticPos) { m_vStaticPos = _vStaticPos; }
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
	void erase_propile(const wstring& _strName);
	void SortPropile();

	void tick_offsetpos();

	Vector2 find_spawn_pos();

private:
	const Vector2 m_vStartSpawnPos;
	const Vector2 m_vOffsetSpawnPos;
	Vector3 m_vStaticPos;
};

