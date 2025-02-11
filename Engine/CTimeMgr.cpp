#include "pch.h"
#include "CTimeMgr.h"

float CTimeMgr::m_fDeltaTime = 0.f;

CTimeMgr::CTimeMgr()
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::init()
{

	// 1초당 카운팅 증가량
	QueryPerformanceFrequency(&m_llFrequency);

	QueryPerformanceCounter(&m_llCurCount);
	QueryPerformanceCounter(&m_llPrevCount);
}

void CTimeMgr::tick()
{
	QueryPerformanceCounter(&m_llCurCount);

	// tick 간격 시간
	m_fDeltaTime = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;


	//if (m_fDeltaTime >= (1.f / 60.f))
	//	m_fDeltaTime = 1.f / 60.f;

	if (isnan(m_dTime))
		m_dTime = 0;

	// 누적 시간
	m_dTime += m_fDeltaTime;

	// 함수 호출 횟수
	++m_iCallCount;

	// 이전 카운트 값을 현재 카운트로 갱신
	m_llPrevCount = m_llCurCount;

	// GlobalData 갱신
	g_Global.tDT = m_fDeltaTime;
	g_Global.tAccTime += m_fDeltaTime;
}

void CTimeMgr::render()
{

}
