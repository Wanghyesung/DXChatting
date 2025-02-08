#pragma once
#include "CEntity.h"

class CComponent;
class CRenderComponent;
class CObject : public CEntity
{
public:
	CObject();
	virtual ~CObject();

	void begine();
	void tick();
	void finaltick();
	void render();

public:
	template <typename T>
	T* GetComponent(COMPONENT_TYPE _eType);

	void SetComponent(CComponent* _pComponent);

private:
	CComponent* m_arrComponent[COMPONENT_TYPE::END];
	CRenderComponent* m_pRenderComponent;
};

template<typename T>
inline T* CObject::GetComponent(COMPONENT_TYPE _eType)
{
	if (m_arrComponent[_eType] != nullptr)
		static_cast<T*>(m_arrComponent[_eType]);

	return nullptr;
}
