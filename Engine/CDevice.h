#pragma once
#include "CSingleton.h"

class CDevice : public CSingleton<CDevice>
{
public:
	CDevice();
	virtual ~CDevice();

	bool init(HWND _hwnd, UINT _iWidth, UINT _iHeight);

private:
	int CreateSwapChain();
	int CreateView();
	
	int CreateRasterizerState();
	int CreateDepthStencilState();
	int CreateBlendState();
	int CreateSampler();

private:
	HWND m_hwnd; //윈도우 창 핸들


	ComPtr<ID3D11Device> m_device; //gpu 메모리 할당
	ComPtr<ID3D11DeviceContext> m_deviceContex; //gpu 제어, 렌더링

	ComPtr<IDXGISwapChain> m_swapChain; //스왑체인
	
	ComPtr<ID3D11SamplerState> m_sampler[3];

	//Rasterizer
	ComPtr<ID3D11DepthStencilState> m_DSState;
	ComPtr<ID3D11BlendState> m_BSState;
	ComPtr<ID3D11RasterizerState> m_RSState;

	D3D11_VIEWPORT m_viewPort;

	Vec2 m_vRenderResolution;


};

