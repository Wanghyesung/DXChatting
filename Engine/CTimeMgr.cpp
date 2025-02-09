#include "pch.h"
#include "CTimeMgr.h"

CTimeMgr::CTimeMgr()
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::init()
{

	// 1�ʴ� ī���� ������
	QueryPerformanceFrequency(&m_llFrequency);

	QueryPerformanceCounter(&m_llCurCount);
	QueryPerformanceCounter(&m_llPrevCount);
}

void CTimeMgr::tick()
{
	QueryPerformanceCounter(&m_llCurCount);

	// tick ���� �ð�
	m_fDeltaTime = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;


	//if (m_fDeltaTime >= (1.f / 60.f))
	//	m_fDeltaTime = 1.f / 60.f;

	if (isnan(m_dTime))
		m_dTime = 0;

	// ���� �ð�
	m_dTime += m_fDeltaTime;

	// �Լ� ȣ�� Ƚ��
	++m_iCallCount;

	// ���� ī��Ʈ ���� ���� ī��Ʈ�� ����
	m_llPrevCount = m_llCurCount;

	// GlobalData ����
	g_Glbobal.tDT = m_fDeltaTime;
	g_Glbobal.tAccTime += m_fDeltaTime;
}

void CTimeMgr::render()
{

}
