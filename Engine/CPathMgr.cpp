#include "pch.h"
#include "CPathMgr.h"

void CPathMgr::Init()
{
	SetCurrentDirectoryW(m_strCurrentPath.c_str());

	int iLen = m_strCurrentPath.size();

	for (int i = iLen - 1; i >= 0; --i)
	{
		if (m_strCurrentPath[i] == L'\\')
		{
			m_strCurrentPath[i] = L'\0';
			break;
		}
	}

	m_strCurrentPath += L"\\Release\\content\\";
}
