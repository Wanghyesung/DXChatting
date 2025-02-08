#include "pch.h"
#include "CTransform.h"
#include "CConstBuffer.h"
#include "CDevice.h"

CTransform::CTransform(COMPONENT_TYPE _eComponentType) :
	CComponent(_eComponentType),
	m_vecRotation{},
	m_vecPosition{},
	m_vecScale{}
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
	m_matSclae *= XMMatrixScaling(m_vecScale.x, m_vecScale.y, m_vecScale.z);

	
	m_matRotation = XMMatrixIdentity();
	m_matRotation *= XMMatrixRotationX(m_vecRotation.x);
	m_matRotation *= XMMatrixRotationY(m_vecRotation.y);
	m_matRotation *= XMMatrixRotationZ(m_vecRotation.z);

	Matrix m_matPosition = XMMatrixTranslation(m_vecPosition.x, m_vecPosition.y, m_vecPosition.z);

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
