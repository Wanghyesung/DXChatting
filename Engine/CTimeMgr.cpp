#include "pch.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CFontMgr.h"

float CTimeMgr::m_fDeltaTime = 0.f;

CTimeMgr::CTimeMgr():
	 m_llPrevCount{}
	, m_llCurCount{}
	, m_llFrequency{}
	, m_iCallCount(0)
	, m_dTime(0)
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
	g_Global.tDT = m_fDeltaTime;
	g_Global.tAccTime += m_fDeltaTime;
}

void CTimeMgr::render()
{
	static wchar_t szBuff[256] = {};
	if (m_dTime >= 1)
	{
		m_dTime = 0;
		Vector2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
		swprintf_s(szBuff, L"FPS : %d, DT : %f, KEY X : %f, Y : %f", m_iCallCount, m_fDeltaTime, vMousePos.x, vMousePos.y);
	}

	CFontMgr::GetInst()->AddFont(szBuff, 10, 20, 16, FONT_RGBA(255, 255, 255, 255));
}
