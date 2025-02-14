#pragma once
#include "CComponent.h"

class CObject;
class CCamera : public CComponent
{
	friend class CRenderMgr;
public:
	CCamera();
	virtual ~CCamera();

	virtual void begin();
	virtual void tick() {};
	virtual void final_tick();

public:
	void SetScale(float _fScale) { m_fScale = _fScale; }
	float GetScale() { return m_fScale; }

	void SetFar(float _fFar) { m_fFar = _fFar; }
	float GetFar() { return m_fFar; }

	void SetLayerMask(int _iLayer, bool _Visible);
	void SetLayerMaskAll(bool _Visible);

	void SetFOV(float _Radian) { m_fFOV = _Radian; }
	float GetFOV() { return m_fFOV; }

	void SetOrthoWidth(float _width) { m_OrthoWidth = _width; }
	void SetOrthoHeight(float _height) { m_OrthoHeight = _height; }

	float GetorthoWidth() { return m_OrthoWidth; }
	float GetOrthoHeight() { return m_OrthoHeight; }

	static const Matrix& GetViewMat() { return GView; }
	static const Matrix& GetProjMat() { return GProj; }
private:
	void render();

	void render_object();
	void render_opaque();
	void render_transparent();
	void render_translucent();

	void sort();
	void alpha_sort();
	void z_sort();

	void create_view();
	void create_proj();

private:
	float m_fFOV;
	float m_OrthoHeight;
	float m_OrthoWidth;
	float m_fScale;
	float m_fNear;
	float m_fFar;

	vector< CObject*> m_vecOpaque;
	vector< CObject*> m_vecTranslucent;
	vector< CObject*> m_vecTransparent;

	Matrix m_matView;
	Matrix m_matProj;

	static Matrix GView;
	static Matrix GProj;

	PROJ_TYPE m_eProjType;
};

