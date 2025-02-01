#include "pch.h"
#include "CTexture.h"
#include "CDevice.h"

CTexture::CTexture():
    m_desc{}
{

}

CTexture::~CTexture()
{

}

int CTexture::UpdateData()
{
    return 0;
}

int CTexture::CreateTex(UINT _iHeight, UINT _iWidth, DXGI_FORMAT _pixelFormat, UINT _iBindFlag, D3D11_USAGE _Usage)
{
    m_desc.Format = _pixelFormat;//DXGI ������ ����, ���� ��� RGBA 8��Ʈ ����

    m_desc.Width = _iWidth;
    m_desc.Height = _iHeight;
    m_desc.ArraySize = 1;//�ؽ�ó �迭 ũ��

    /* 
    D3D11_USAGE_DYNAMIC�� ���: D3D11_CPU_ACCESS_WRITE �� CPU���� ���� ����.
    D3D11_USAGE_STAGING�� ���: D3D11_CPU_ACCESS_READ �� CPU���� �б� ����.
    D3D11_USAGE_DEFAULT�̸� CPU���� ���� �Ұ�.
    */
    m_desc.BindFlags = _iBindFlag;
    m_desc.Usage = _Usage;

    if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == _Usage)
        m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    else if (D3D11_USAGE::D3D11_USAGE_STAGING == _Usage)
        m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;


    m_desc.MipLevels = 1;    // ������ ����
    m_desc.SampleDesc.Count = 1;
    m_desc.SampleDesc.Quality = 0;

    if (FAILED(CDevice::GetInst()->GetDevice()->CreateTexture2D(&m_desc, nullptr, m_2DTexture.GetAddressOf())))
    {
        return E_FAIL;
    }

    if (m_desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
    {
        if (FAILED(CDevice::GetInst()->GetDevice()->CreateDepthStencilView(m_2DTexture.Get(), nullptr, m_DSV.GetAddressOf())))
        {
            return E_FAIL;
        }
    }
    else
    {
        if (m_desc.BindFlags & D3D11_BIND_RENDER_TARGET)
        {
            if (FAILED(CDevice::GetInst()->GetDevice()->CreateRenderTargetView(m_2DTexture.Get(), nullptr, m_RTV.GetAddressOf())))
            {
                return E_FAIL;
            }
        }

        if (m_desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
        {
            if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_2DTexture.Get(), nullptr, m_SRV.GetAddressOf())))
            {
                return E_FAIL;
            }
        }

        if (m_desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
        {
            if (FAILED(CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(m_2DTexture.Get(), nullptr, m_UAV.GetAddressOf())))
            {
                return E_FAIL;
            }
        }
    }

    return S_OK;
}


int CTexture::CreateTex(ComPtr<ID3D11Texture2D> _tTex)
{
    m_2DTexture = _tTex;
    _tTex->GetDesc(&m_desc);

    if (FAILED(CDevice::GetInst()->GetDevice()->CreateTexture2D(&m_desc, nullptr, m_2DTexture.GetAddressOf())))
    {
        return E_FAIL;
    }

    if (m_desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
    {
        if (FAILED(CDevice::GetInst()->GetDevice()->CreateDepthStencilView(m_2DTexture.Get(), nullptr, m_DSV.GetAddressOf())))
        {
            return E_FAIL;
        }
    }
    else
    {
        if (m_desc.BindFlags & D3D11_BIND_RENDER_TARGET)
        {
            if (FAILED(CDevice::GetInst()->GetDevice()->CreateRenderTargetView(m_2DTexture.Get(), nullptr, m_RTV.GetAddressOf())))
            {
                return E_FAIL;
            }
        }

        if (m_desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
        {
            if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_2DTexture.Get(), nullptr, m_SRV.GetAddressOf())))
            {
                return E_FAIL;
            }
        }

        if (m_desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
        {
            if (FAILED(CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(m_2DTexture.Get(), nullptr, m_UAV.GetAddressOf())))
            {
                return E_FAIL;
            }
        }
    }

    return S_OK;
}

int CTexture::Load(const wstring& _strPath)
{
    wchar_t szExt[50] = L"";
    _wsplitpath_s(_strPath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);
    wstring strExt = szExt;

    HRESULT hr = S_OK;
    if (L".dds" == strExt || L".DDS" == strExt)
    {
        // dds, DDS
        hr = LoadFromDDSFile(_strPath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_Image);
    }

    else if (L".tga" == strExt || L".TGA" == strExt)
    {
        // tga, TGA
        hr = LoadFromTGAFile(_strPath.c_str(), nullptr, m_Image);
    }

    else
    {
        // png, jpg, jpeg, bmp
        hr = LoadFromWICFile(_strPath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, m_Image);
    }

    if (FAILED(hr))
    {
        MessageBox(nullptr, L"���ҽ� �ε� ����", L"�ؽ��� �ε� ����", MB_OK);
        return E_FAIL;
    }

    hr = CreateShaderResourceView(CDevice::GetInst()->GetDevice()
        , m_Image.GetImages()
        , m_Image.GetImageCount()
        , m_Image.GetMetadata()
        , m_SRV.GetAddressOf());

    if (FAILED(hr))
    {
        MessageBox(nullptr, L"ShaderResourceView ���� ����", L"�ؽ��� �ε� ����", MB_OK);
        return E_FAIL;
    }

    m_SRV->GetResource((ID3D11Resource**)m_2DTexture.GetAddressOf());
    m_2DTexture->GetDesc(&m_desc);

    return S_OK;
 
}

int CTexture::Save(const wstring& _strPath)
{
    return 0;
}
