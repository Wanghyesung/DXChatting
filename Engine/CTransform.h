#pragma once
#include "CComponent.h"

class CTransform : public CComponent
{
public:
	CTransform(COMPONENT_TYPE _eComponentType);
	virtual ~CTransform();

public:
	virtual void begin()override;
	virtual void tick() {};
	virtual void final_tick()override;

	void UpdateData();


public:
	const Vector3& GetPosition() { return m_vecPosition; }
	const Vector3& GetRotation() { return m_vecRotation; }
	const Vector3& GetScale() { return m_vecScale; }
	const Matrix& GetWorldMatrix() { return m_matWorld; }

	void SetPostion(const Vector3& _vecPosition) { m_vecPosition = _vecPosition; }
	void SetRotation(const Vector3& _vecLotation) { m_vecRotation = _vecLotation; }
	void SetSostion(const Vector3& _vecScale) { m_vecScale = _vecScale; }


private:
	Vector3 m_vecPosition;
	Vector3 m_vecScale;
	Vector3 m_vecRotation;

	//방향 벡터 X
	Matrix  m_matWorld; // 크기, 회전, 이동 정보를 합쳐놓음
	Matrix  m_matSclae;
	Matrix  m_matRotation;
	Matrix  m_matPosition;
};

