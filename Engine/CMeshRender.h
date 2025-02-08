#pragma once
#include "CRenderComponent.h"

class CMeshRender : public CRenderComponent
{
public:
	CMeshRender(COMPONENT_TYPE _eComponentType);
	virtual ~CMeshRender();

public:
	virtual void begin()override;
	virtual void final_tick()override;

	virtual void render();

private:


};

