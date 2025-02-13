#include "pch.h"
#include "CRenderMgr.h"
#include "CRoomMgr.h"
#include "CCamera.h"
#include "CDevice.h"
CRenderMgr::CRenderMgr()
{

}

CRenderMgr::~CRenderMgr()
{

}

void CRenderMgr::render()
{
	CLayer* pLayer = CRoomMgr::GetInst()->GetCurLayer(LAYER_TYPE::CAMERA);

	for (CObject* pCamera : pLayer->m_vecObject)
	{
		
		pCamera->GetComponent<CCamera>(COMPONENT_TYPE::CAMERA)->render();
	}
}

void CRenderMgr::clear()
{
	CDevice::GetInst()->Clear();
}


