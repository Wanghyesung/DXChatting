#pragma once
#include "CSingleton.h"

class CConstBuffer;

class CDevice : public CSingleton<CDevice>
{
public:
	CDevice();
	virtual ~CDevice();

	bool init(HWND _hwnd, UINT _iWidth, UINT _iHeight);

public:
	ID3D11Device* GetDevice() { return m_device.Get(); }
	ID3D11DeviceContext* GetContext() { return m_deviceContex.Get(); }

	Vec2 GetResolution() { return m_vRenderResolution; }
private:
	int CreateSwapChain();
	int CreateView();

	int CreateRasterizerState();
	int CreateDepthStencilState();
	int CreateBlendState();
	int CreateSampler();

	void CreateConstBuffer();
private:
	HWND m_hwnd; //������ â �ڵ�

	ComPtr<ID3D11Device> m_device; //gpu �޸� �Ҵ�
	ComPtr<ID3D11DeviceContext> m_deviceContex; //gpu ����, ������

	ComPtr<IDXGISwapChain> m_swapChain; //����ü��

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

