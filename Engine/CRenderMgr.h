#pragma once
#include "CSingleton.h"
 
class CRenderMgr : public CSingleton<CRenderMgr>
{
	friend class CSingleton;
	friend class CEngines;

private:
	void render();
	void render_object();
	void sort();

private:
	CRenderMgr();
	virtual ~CRenderMgr();
};

