#pragma once
#include "CComponent.h"
#include "CMaterial.h"
#include "CMesh.h"

class CRenderComponent : public CComponent
{
public:
	CRenderComponent(COMPONENT_TYPE _eComponentType);
	virtual ~CRenderComponent();

public:
	virtual void begin()override;
	virtual void tick() {};
	virtual void final_tick()override;

	virtual void render() = 0;

public:
	void SetMesh(std::shared_ptr<CMesh> _pMesh) { m_pMesh = _pMesh; }
	void SetMaterial(std::shared_ptr<CMaterial> _pMaterial) { m_pMaterial = _pMaterial; }
	std::shared_ptr<CMaterial> GetMaterial() { return m_pMaterial; }
	std::shared_ptr<CMesh> GetMesh() { return m_pMesh; }

private:
	shared_ptr<CMaterial> m_pMaterial;
	shared_ptr<CMesh> m_pMesh;
};

