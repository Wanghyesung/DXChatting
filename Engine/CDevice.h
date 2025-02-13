#pragma once
#include "CSingleton.h"

class CConstBuffer;

class CDevice : public CSingleton<CDevice>
{
	friend class CSingleton;

private:
	CDevice();
	virtual ~CDevice();

public:
	bool init(HWND _hwnd, UINT _iWidth, UINT _iHeight);

public:
	ID3D11Device* GetDevice() { return m_device.Get(); }
	ID3D11DeviceContext* GetContext() { return m_deviceContex.Get(); }

	Vec2 GetResolution() { return m_vRenderResolution; }

	ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _eType) { return m_DSState[(UINT)_eType];}
	ComPtr<ID3D11RasterizerState> GetRSState(RS_TYPE _eType) { return m_RSState[(UINT)_eType];}
	ComPtr<ID3D11BlendState> GetBSState(BS_TYPE _eType) { return m_BSState[(UINT)_eType];}

	CConstBuffer* GetConstBuffer(CB_TYPE _eType) { return m_arrConstBuffer[(UINT)_eType]; }

	void Present() { m_swapChain->Present(0, 0); }//백 버퍼(Back Buffer)를 프론트 버퍼(Front Buffer)로 전환
	void Clear();
private:
	int CreateSwapChain();
	int CreateView();

	int CreateRasterizerState();
	int CreateDepthStencilState();
	int CreateBlendState();
	int CreateSampler();

	void CreateConstBuffer();
private:
	HWND m_hwnd; //윈도우 창 핸들

	ComPtr<ID3D11Device> m_device; //gpu 메모리 할당
	ComPtr<ID3D11DeviceContext> m_deviceContex; //gpu 제어, 렌더링

	ComPtr<IDXGISwapChain> m_swapChain; //스왑체인

	ComPtr<ID3D11SamplerState> m_sampler[3];

	//Rasterizer
	ComPtr<ID3D11DepthStencilState> m_DSState[(UINT)DS_TYPE::END];
	ComPtr<ID3D11BlendState> m_BSState[(UINT)BS_TYPE::END];
	ComPtr<ID3D11RasterizerState> m_RSState[(UINT)RS_TYPE::END];

	D3D11_VIEWPORT m_viewPort;


	//constBuffer
	CConstBuffer* m_arrConstBuffer[(UINT)CB_TYPE::END];

	Vec2 m_vRenderResolution;


};

