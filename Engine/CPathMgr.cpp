#include "pch.h"
#include "CPathMgr.h"

void CPathMgr::Init()
{
	GetCurrentDirectory(256,m_strCurrentPath);

	
	int iLen = wcslen(m_strCurrentPath);

	for (int i = iLen - 1; i >= 0; --i)
	{
		if (m_strCurrentPath[i] == L'\\')
		{
			m_strCurrentPath[i] = L'\0';
			break;
		}
	}

	wcscat_s(m_strCurrentPath, L"\\Release\\content\\");
}
