#include "pch.h"
#include "CTransform.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CCamera.h"

CTransform::CTransform() :
	CComponent(COMPONENT_TYPE::TRANSFORM),
	m_vRotation(Vector3::Zero),
	m_vPosition(Vector3::Zero),
	m_vScale(Vector3::One),
	m_eSclaeMode(SCALE_MODE::S_NONE)
{

}

CTransform::~CTransform()
{

}

void CTransform::begin()
{

}



void CTransform::final_tick()
{
	m_matSclae = XMMatrixIdentity();
	m_matSclae *= XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);

	//m_matSclaeInv = XMMatrixInverse(nullptr, m_matSclae);
	
	m_matRotation = XMMatrixIdentity();
	m_matRotation *= XMMatrixRotationX(m_vRotation.x);
	m_matRotation *= XMMatrixRotationY(m_vRotation.y);
	m_matRotation *= XMMatrixRotationZ(m_vRotation.z);

	m_vUp = Vector3::TransformNormal(Vector3::Up, m_matRotation);
	m_vFoward = Vector3::TransformNormal(Vector3::Forward, m_matRotation);
	m_vRight = Vector3::TransformNormal(Vector3::Right, m_matRotation);

	Matrix m_matPosition = XMMatrixTranslation(m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = m_matSclae * m_matRotation * m_matPosition;


	CObject* pParent = GetOwner()->GetParent();
	if (pParent != nullptr)
		calculate_scale();
}

void CTransform::UpdateData()
{
	//상수버퍼 대응 구조체
	//srcTrasnform은 지역 변수이므로 함수가 끝나면 스택에서 해제 결과적으로 SetData()가 사용하는 메모리가 유효하지 않게 될 가능성
	//tTransform srcTrasnform = {};
	g_transform.matWorld = m_matWorld;
	g_transform.matView = CCamera::GetViewMat();
	g_transform.matProj = CCamera::GetProjMat();
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;

	CConstBuffer* pTrConstBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pTrConstBuffer->SetData(&g_transform, sizeof(tTransform));
	pTrConstBuffer->UpdateData();
}

void CTransform::calculate_scale()
{
	CObject* pParent = GetOwner()->GetParent();
	CTransform* pParentTransform = pParent->GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);
	
	switch (m_eSclaeMode)
	{
	case SCALE_MODE::S_INHERIT:
	{
		//부모 크기게 영향을 받음
		Matrix matParentWorld = pParentTransform->m_matWorld;
		m_matSclae = pParentTransform->m_matSclae;
		m_matWorld = m_matWorld * matParentWorld;
	}
	break;

	case SCALE_MODE::S_ABSOLUTE:
	{
		Matrix matParentWorld = pParentTransform->m_matWorld;
		Matrix matParentScale = pParentTransform->m_matSclae;
		Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale);

		m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
	}
	break;

	}
}


