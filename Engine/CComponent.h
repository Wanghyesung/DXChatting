#pragma once
#include "CEntity.h"
#include "CObject.h"

class CObject;

class CComponent : public CEntity
{
public:
	CComponent(COMPONENT_TYPE _eComponentType);

	virtual ~CComponent() {};

public:
	virtual void begin() {};
	virtual void tick() {};
	virtual void final_tick() = 0;


	template <typename T>
	T* GetComponent(COMPONENT_TYPE _eType);

	COMPONENT_TYPE GetComponentType() { return m_eComponentType; }
public:
	void SetOwner(CObject* _pOwner) { m_pOwner = _pOwner; }
	CObject* GetOwner() { return m_pOwner; }

private:
	CObject* m_pOwner;
	COMPONENT_TYPE m_eComponentType;
};

template<typename T>
inline T* CComponent::GetComponent(COMPONENT_TYPE _eType)
{
	if (m_pOwner != nullptr)
		return m_pOwner->GetComponent<T>(_eType);

	return nullptr;
}
