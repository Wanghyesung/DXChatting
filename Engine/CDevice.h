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
	HWND m_hwnd; //������ â �ڵ�


	ComPtr<ID3D11Device> m_device; //gpu �޸� �Ҵ�
	ComPtr<ID3D11DeviceContext> m_deviceContex; //gpu ����, ������

	ComPtr<IDXGISwapChain> m_swapChain; //����ü��
	
	ComPtr<ID3D11SamplerState> m_sampler[3];

	//Rasterizer
	ComPtr<ID3D11DepthStencilState> m_DSState;
	ComPtr<ID3D11BlendState> m_BSState;
	ComPtr<ID3D11RasterizerState> m_RSState;

	D3D11_VIEWPORT m_viewPort;

	Vec2 m_vRenderResolution;


};

