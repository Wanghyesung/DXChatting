#include "pch.h"
#include "CMeshRender.h"
#include "CTransform.h"
#include "CAnimator2D.h"
CMeshRender::CMeshRender(COMPONENT_TYPE _eComponentType) :
	CRenderComponent(_eComponentType)
{

}

CMeshRender::~CMeshRender()
{

}

void CMeshRender::begin()
{

}

void CMeshRender::final_tick()
{

}

void CMeshRender::render()
{
	shared_ptr<CMaterial> pMtrl = GetMaterial();
	shared_ptr<CMesh> pMesh = GetMesh();
	if (pMtrl == nullptr || pMesh == nullptr)
		return;

	//tr
	CTransform* pTransform = GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);
	pTransform->UpdateData();

	//anim
	CAnimator2D* pAnimator = GetComponent<CAnimator2D>(COMPONENT_TYPE::ANIMATOR);
	pAnimator->UpdateData();

	pMtrl->UpdateData();
	pMesh->Render();


}
