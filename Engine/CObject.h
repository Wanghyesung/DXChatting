#pragma once
#include "CEntity.h"

class CComponent;
class CRenderComponent;
class CObject : public CEntity
{
	friend class CLayer;
	friend class CCamera;
	friend class CEventMgr;

public:
	CObject();
	virtual ~CObject();

protected:
	void begin();
	virtual void tick();
	void finaltick();
	void render();

public:
	template <typename T>
	T* GetComponent(COMPONENT_TYPE _eType);

	void SetComponent(CComponent* _pComponent);

	CObject* GetParent() { return m_pParent; }
	void SetLayer(LAYER_TYPE _eLayer) { m_eCurLayer = _eLayer; }
	LAYER_TYPE GetLayer() { return m_eCurLayer; }

protected:
	const vector<CObject*>& GetChilds() { return m_vecChild; }

	void AddChild(CObject* _pObj);
	CObject* EraseChild(CObject* _pObj);
	CObject* EraseChild(const wstring& _strName);

	CObject* FindChild(CObject* _pObj);
	CObject* FindChild(const wstring& _strName);

	bool DeleteChild(const wstring& _strName);
	bool DeleteChild(CObject* _pObj);

protected:
	vector<CObject*> m_vecChild;
	CObject* m_pParent;
	LAYER_TYPE m_eCurLayer;

protected:
	CComponent* m_arrComponent[COMPONENT_TYPE::END];
	CRenderComponent* m_pRenderComponent;
};

template<typename T>
inline T* CObject::GetComponent(COMPONENT_TYPE _eType)
{
	if (m_arrComponent[_eType] != nullptr)
		return static_cast<T*>(m_arrComponent[_eType]);

	return nullptr;
}
