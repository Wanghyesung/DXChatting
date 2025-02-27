#include "pch.h"

#include "CKeyMgr.h"

#include "CEngine.h"
#include "CObject.h"
#include "CCamera.h"

int g_arrVK[(UINT)KEY::END]
=
{
	 VK_UP,
	 VK_DOWN,
	 VK_LEFT,
	 VK_RIGHT,

	 VK_RETURN,
	 VK_ESCAPE,
	 VK_LMENU,
	 VK_LCONTROL,
	 VK_LSHIFT,
	 VK_TAB,

	 VK_LBUTTON,
	 VK_RBUTTON,

	 VK_SPACE,
	 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
	 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
	 'U', 'V', 'W', 'X', 'Y', 'Z',

	 '0','1','2','3','4','5','6','7','8','9',

	 VK_NUMPAD0,
	 VK_NUMPAD1,
	 VK_NUMPAD2,
	 VK_NUMPAD3,
	 VK_NUMPAD4,
	 VK_NUMPAD5,
	 VK_NUMPAD6,
	 VK_NUMPAD7,
	 VK_NUMPAD8,
	 VK_NUMPAD9,

	 VK_BACK,
};


CKeyMgr::CKeyMgr()
{

}

CKeyMgr::~CKeyMgr()
{

}


void CKeyMgr::init()
{
	for (int i = 0; i < (int)KEY::END; ++i)
	{
		m_vecKey.push_back(tKeyInfo{ (KEY)i  , KEY_STATE::NONE });
	}
	
}

void CKeyMgr::tick()
{
	if (GetFocus())
	{
		

		for (size_t i = 0; i < m_vecKey.size(); ++i)
		{
			if (GetAsyncKeyState(g_arrVK[(UINT)m_vecKey[i].key]) & 0x8000)
			{
				// �������� ������ �ʾҴ�.
				if (false == m_vecKey[i].bPrev)
				{
					m_vecKey[i].state = KEY_STATE::TAP;
					m_vecKey[i].bPrev = true;

					check_key(i);
				}
				else
				{
					// ���ݵ� �����ְ�, ���� �����ӿ����� �����־���.
					m_vecKey[i].state = KEY_STATE::PRESSED;
				}

			}
			else
			{
				// �������� �ʴ�.
				if (false == m_vecKey[i].bPrev)
				{
					m_vecKey[i].state = KEY_STATE::NONE;
				}
				else
				{
					m_vecKey[i].state = KEY_STATE::RELEASE;
					m_vecKey[i].bPrev = false;
				}

				if (i >= (UINT)KEY::Q && i <= (UINT)KEY::_9)
					m_hashOnKey.erase(static_cast<WCHAR>((g_arrVK[(UINT)i])));
			}
		}

		// Mouse ��ġ ����
		m_vPrevMousePos = m_vMousePos;

		POINT ptMousePos = {};
		GetCursorPos(&ptMousePos);
		ScreenToClient(CEngine::GetInst()->GetMainWnd(), &ptMousePos);
		m_vMousePos = Vec2((float)ptMousePos.x, (float)ptMousePos.y);
		Vec3 vMousePos = Vec3((float)ptMousePos.x, (float)ptMousePos.y, 0.f);

		m_vMouseDir = m_vMousePos - m_vPrevMousePos;
		m_vMouseDir.y *= -1;


		RECT tRect = {};
		GetClientRect(CEngine::GetInst()->GetMainWnd(), &tRect);

		float fWidth = tRect.right - tRect.left;
		float fHeight = tRect.bottom - tRect.top;
		Viewport view(0.f, 0.f, fWidth, fHeight);

		
		vMousePos = view.Unproject(vMousePos, CCamera::GetProjMat(), CCamera::GetViewMat(), Matrix::Identity);

		m_vNDCMousePos.x = vMousePos.x;
		m_vNDCMousePos.y = vMousePos.y;
	}

	// Window �� focus ���°� �ƴϴ�
	else
	{
		for (size_t i = 0; i < m_vecKey.size(); ++i)
		{
			if (KEY_STATE::TAP == m_vecKey[i].state || KEY_STATE::PRESSED == m_vecKey[i].state)
			{
				m_vecKey[i].state = KEY_STATE::RELEASE;
			}

			else if (KEY_STATE::RELEASE == m_vecKey[i].state)
			{
				m_vecKey[i].state = KEY_STATE::NONE;
			}
		}
	}


}

void CKeyMgr::check_key(UINT _iKey)
{
	if (_iKey >= (UINT)KEY::Q && _iKey <= (UINT)KEY::_9)
		return;

	WCHAR wcKey = static_cast<WCHAR>(_iKey);
	if (m_hashOnKey.find(wcKey) == m_hashOnKey.end())
		m_hashOnKey[wcKey] = 1;

}
