#include "pch.h"
#include "CMeshRender.h"
#include "CTransform.h"

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


	pMtrl->UpdateData();
	pMesh->Render();


}
