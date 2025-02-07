#include "pch.h"
#include "CEngine.h"
#include "CDevice.h"
#include "CPathMgr.h"
#include "CResMgr.h"
int CEngine::init(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
    m_hWnd = _hWnd;
    m_vResolution = Vector2(_iWidth, _iHeight );

    RECT rt = { 0, 0, (int)_iWidth, (int)_iHeight };
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
    SetWindowPos(m_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);
    ShowWindow(m_hWnd, true);

    CDevice::GetInst()->init(_hWnd, _iWidth, _iHeight);

    if (init_mgr() == FALSE)
        return FALSE;

    return TRUE;
}

int CEngine::init_mgr()
{
    CPathMgr::GetInst()->Init();

    CResMgr::GetInst()->Init();

    return TRUE;
}

void CEngine::progress()
{
    tick();

    render();

}

void CEngine::tick()
{

}

void CEngine::render()
{

}

CEngine::CEngine()
{
}
