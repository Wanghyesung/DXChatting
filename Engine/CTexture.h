#pragma once
#include "CResource.h"

#include <DirectXTex\DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex//DirectXTex_debug")
#else 
#pragma comment(lib, "DirectXTex//DirectXTex")
#endif


class CTexture : public CResource
{
public:
	CTexture();
	virtual ~CTexture();


	void UpdateData(UINT _iRegiserNum, int _iShaderFlag);
	static void Clear(UINT _iRegisterNum);
	static void Clear();
public:
	int CreateTex(UINT _iHeight, UINT _iWidth, DXGI_FORMAT _pixelFormat,
		UINT _iBindFlag, D3D11_USAGE _Usage);
	int CreateTex(ComPtr<ID3D11Texture2D> _tTex);

public:
	UINT GetWidth() { return m_desc.Width; }
	UINT GetHeight() { return m_desc.Height; }


	virtual int Load(const wstring& _strPath)override;
	virtual int Save(const wstring& _strPath)override;
private:
	ComPtr<ID3D11Texture2D> m_2DTexture;
	ComPtr<ID3D11ShaderResourceView> m_SRV;
	ComPtr<ID3D11DepthStencilView> m_DSV;
	ComPtr<ID3D11UnorderedAccessView> m_UAV;
	ComPtr<ID3D11RenderTargetView> m_RTV;

	D3D11_TEXTURE2D_DESC m_desc;


	ScratchImage m_Image;

	static UINT m_iPreRegisterNum;
};

