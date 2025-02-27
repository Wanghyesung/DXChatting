#include "pch.h"
#include "CEngine.h"
#include "CDevice.h"
#include "CPathMgr.h"
#include "CResMgr.h"
#include "CRoomMgr.h"
#include "CTimeMgr.h"
#include "CRenderMgr.h"
#include "CKeyMgr.h"
#include "CUIMgr.h"
#include "CFontMgr.h"
#include "CEventMgr.h"

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

    CTimeMgr::GetInst()->init();

    CKeyMgr::GetInst()->init();

    CFontMgr::GetInst()->init();

    CResMgr::GetInst()->Init();

    CRoomMgr::GetInst()->init();

    return TRUE;
}

void CEngine::progress()
{
    tick();
    
    render();
    CTimeMgr::GetInst()->render();
    CFontMgr::GetInst()->render();

}

void CEngine::tick()
{
    CTimeMgr::GetInst()->tick();
    CKeyMgr::GetInst()->tick();

    CUIMgr::GetInst()->tick();
    CRoomMgr::GetInst()->tick();
    
    CEventMgr::GetInst()->tick();
}

void CEngine::render()
{
    CRenderMgr::GetInst()->clear();
    CRenderMgr::GetInst()->render();
}


CEngine::CEngine()
{

}
CEngine::~CEngine()
{
    int a = 10;
}
