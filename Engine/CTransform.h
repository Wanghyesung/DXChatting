#pragma once
#include "CComponent.h"


class CTransform : public CComponent
{
public:
	CTransform();
	virtual ~CTransform();

public:
	virtual void begin()override;
	virtual void tick() {};
	virtual void final_tick()override;

	void UpdateData();
public:
	const Vector3& GetPosition() { return m_vPosition; }
	const Vector3& GetRotation() { return m_vRotation; }
	const Vector3& GetScale() { return m_vScale; }
	const Matrix& GetWorldMatrix() { return m_matWorld; }

	void SetPostion(const Vector3& _vPosition) { m_vPosition = _vPosition; }
	void SetRotation(const Vector3& _vLotation) { m_vRotation = _vLotation; }
	void SetScale(const Vector3& _vScale) { m_vScale = _vScale; }


	Vector3 Foward() { return m_vFoward; }
	Vector3 Right() { return m_vRight; }
	Vector3 Up() { return m_vUp; }

	void SetScaleMode(SCALE_MODE _eMode) { m_eSclaeMode = _eMode; }
	SCALE_MODE GetScaleMode() { return m_eSclaeMode; }

private:
	void calculate_scale();

private:
	Vector3 m_vPosition;
	Vector3 m_vScale;
	Vector3 m_vRotation;

	Vector3 m_vUp;
	Vector3 m_vFoward;
	Vector3 m_vRight;

	//방향 벡터 X
	Matrix  m_matWorld; // 크기, 회전, 이동 정보를 합쳐놓음
	Matrix  m_matSclae;
	Matrix  m_matSclaeInv;
	Matrix  m_matRotation;
	Matrix  m_matPosition;

	SCALE_MODE m_eSclaeMode;

};

