#pragma once
#include "CSingleton.h"
#define DT CTimeMgr::GetInst()->GetDeltaTime()

class CTimeMgr : public CSingleton<CTimeMgr>
{
	friend class CSingleton;
private:
	CTimeMgr();
	virtual ~CTimeMgr();

private:
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llFrequency;

	UINT			m_iCallCount;
	static float	m_fDeltaTime;
	double			m_dTime;

public:
	void init();
	void tick();
	void render();

public:
	FORCEINLINE static float GetDeltaTime() { return m_fDeltaTime; }
};

