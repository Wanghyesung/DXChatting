#include "pch.h"
#include "CCamera.h"
#include "CDevice.h"
#include "CTransform.h"
#include "CRoomMgr.h"
#include "CObject.h"
#include "CMaterial.h"
#include "CMeshRender.h"
Matrix CCamera::GView = Matrix::Identity;
Matrix CCamera::GProj = Matrix::Identity;

bool CompareZSort( CObject* a,  CObject*  b)
{
	float AZ = a->GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM)->GetPosition().z;
	float BZ = b->GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM)->GetPosition().z;

	if (AZ <= BZ)
		return false;
	return true;
}

CCamera::CCamera():
	CComponent(COMPONENT_TYPE::CAMERA),
	m_fFar(1000),
	m_fNear(1),
	m_fFOV(XM_2PI / 6.f),
	m_OrthoHeight(0),
	m_OrthoWidth(0),
	m_fScale(1000.f),
	m_eProjType(PROJ_TYPE::ORTHOGRAPHIC)
{

}

CCamera::~CCamera()
{
}

void CCamera::begin()
{

}

void CCamera::final_tick()
{
	create_view();
	create_proj();
}

void CCamera::render()
{
	GView = m_matView;
	GProj = m_matProj;
	
	sort();
	render_object();
}

void CCamera::render_object()
{
	render_opaque();
	render_translucent();
	render_transparent();
}

void CCamera::render_opaque()
{
	for (int i = 0; i < m_vecOpaque.size(); ++i)
	{
		m_vecOpaque[i]->render();
	}
}

void CCamera::render_transparent()
{
	for (int i = 0; i < m_vecTranslucent.size(); ++i)
	{
		m_vecTranslucent[i]->render();
	}
}

void CCamera::render_translucent()
{
	for (int i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->render();
	}
}

void CCamera::sort()
{
	alpha_sort();
	z_sort();
}

void CCamera::alpha_sort()
{
	m_vecOpaque.clear();
	m_vecTranslucent.clear();
	m_vecTransparent.clear();

	const vector<CLayer*>& vecLayer = CRoomMgr::GetInst()->GetCurLayers();
	for (int i = 0; i < (UINT)LAYER_TYPE::END; ++i)
	{
		for (CObject* pObj : vecLayer[i]->m_vecObject)
		{
			CMeshRender* pMeshRender = pObj->GetComponent<CMeshRender>(COMPONENT_TYPE::MESH_RENDER);
			if (pMeshRender == nullptr)
				continue;

			shared_ptr<CMaterial> pMtrl = pMeshRender->GetMaterial();
			if (pMtrl == nullptr)
				continue;

			RENDER_TYPE eRenderType = pMtrl->GetRenderType();

			switch (eRenderType)
			{
			case RENDER_TYPE::R_OPAQUE:
				m_vecOpaque.push_back(pObj);
				break;
			case RENDER_TYPE::R_TRANSLUCENT:
				m_vecTranslucent.push_back(pObj);
				break;
			case RENDER_TYPE::R_TRANSPARENT:
				m_vecTransparent.push_back(pObj);
				break;
			}
		}	
	}
}

void CCamera::z_sort()
{
	std::sort(m_vecTranslucent.begin(), m_vecTranslucent.end(), CompareZSort);
	std::sort(m_vecTransparent.begin(), m_vecTransparent.end(), CompareZSort);
}

void CCamera::create_view()
{
	m_matView = Matrix::Identity;

	CTransform* pTransform = GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);

	//카메라를 원점(0,0,0)으로 이동시키는 행렬 (카메라의 움직임의 반대로 물체가 움직여야함)
	Vector3 vCamPos = pTransform->GetPosition();
	Matrix matViewTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y,-vCamPos.z); 

	//뷰 행렬의 회전 부분은 "카메라의 축 벡터들"을 기준으로 생성 (카메라가 바라보는 방향)
	Vector3 vUp = pTransform->Up();
	Vector3 vRight = pTransform->Right();
	Vector3 vFoward = pTransform->Foward();

	
	Matrix matViewRotate = Matrix::Identity;
	matViewRotate._11 = vRight.x;	matViewRotate._12 = vUp.x;	matViewRotate._13 = vFoward.x;
	matViewRotate._21 = vRight.y;	matViewRotate._22 = vUp.y;	matViewRotate._23 = vFoward.y;
	matViewRotate._31 = vRight.z;	matViewRotate._32 = vUp.z;	matViewRotate._33 = vFoward.z;
	m_matView = matViewTrans * matViewRotate;
}

void CCamera::create_proj()
{
	Vector2 vResolution = CDevice::GetInst()->GetResolution();
	float fWidth = vResolution.x;
	float fHeight = vResolution.y;

	if (m_eProjType == PROJ_TYPE::ORTHOGRAPHIC)
	{
		//해상도를 기반으로 투영 영역 크기 설정

		float fOrthRatio = m_fScale / 1000.f;
		m_OrthoWidth = fWidth * fOrthRatio;
		m_OrthoHeight = fHeight * fOrthRatio;

		m_matProj = Matrix::CreateOrthographic(m_OrthoWidth, m_OrthoHeight, m_fNear, m_fFar);
	}
	else
	{
		//가로 세로 비율
		float fPerspectiveRatio = fWidth / fHeight;
		m_matProj = Matrix::CreatePerspectiveFieldOfView(m_fFOV, fPerspectiveRatio, m_fNear, m_fFar); 
	}
}

