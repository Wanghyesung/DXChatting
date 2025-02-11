#include "pch.h"
#include "CTransform.h"
#include "CConstBuffer.h"
#include "CDevice.h"

CTransform::CTransform(COMPONENT_TYPE _eComponentType) :
	CComponent(_eComponentType),
	m_vRotation{},
	m_vPosition{},
	m_vScale{}
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

	
	m_matRotation = XMMatrixIdentity();
	m_matRotation *= XMMatrixRotationX(m_vRotation.x);
	m_matRotation *= XMMatrixRotationY(m_vRotation.y);
	m_matRotation *= XMMatrixRotationZ(m_vRotation.z);

	m_vUp = Vector3::TransformNormal(Vector3::Up, m_matRotation);
	m_vFoward = Vector3::TransformNormal(Vector3::Forward, m_matRotation);
	m_vRight = Vector3::TransformNormal(Vector3::Right, m_matRotation);

	Matrix m_matPosition = XMMatrixTranslation(m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = m_matSclae * m_matRotation * m_matPosition;

}

void CTransform::UpdateData()
{
	//상수버퍼 대응 구조체
	tTransform srcTrasnform = {};
	srcTrasnform.matWVP = m_matWorld;

	CConstBuffer* pTrConstBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pTrConstBuffer->SetData(&srcTrasnform, sizeof(Matrix));
	pTrConstBuffer->UpdateData();
}
