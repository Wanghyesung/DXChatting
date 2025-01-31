#include "pch.h"
#include "CDevice.h"

CDevice::CDevice()
{

}

CDevice::~CDevice()
{

}

bool CDevice::init(HWND _hwnd, UINT _iWidth, UINT _iHeight)
{
    m_hwnd = _hwnd;
    m_vRenderResolution = Vec2{ _iWidth, _iHeight };

    D3D_FEATURE_LEVEL iFeatureLevels = D3D_FEATURE_LEVEL_11_0;

    IDXGIAdapter* pAdapter = nullptr;
    D3D_DRIVER_TYPE eDriverType = D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE;//Direct3D ���ø����̼ǿ��� ����ؾ� �ϴ� �⺻ ����̹��Դϴ�

    int iFlag = 0;
#ifdef _DEBUG
    iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

    //device �ʱ�ȭ
    if (FAILED(D3D11CreateDevice(pAdapter, eDriverType, nullptr, iFlag,
        nullptr, 0, D3D11_SDK_VERSION, m_device.GetAddressOf(), &iFeatureLevels,
        m_deviceContex.GetAddressOf())))
    {
        MessageBox(nullptr, L"Device, Context ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
        return E_FAIL;
    }

    if(FAILED(CreateSwapChain()))
    {
        MessageBox(nullptr, L"����ü�� ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
        return E_FAIL;
    }
    

    return true;
}

int CDevice::CreateSwapChain()
{
    /*
    DirectX���� **���� ü��(Swap Chain)**�� ������ ����� �����ϴ� �� ����(back buffer)�� �����ϴ� �����Դϴ�.
    ���� ü���� CPU�� �������� �������ϰ� GPU�� �̸� ȭ�鿡 ����ϴ� �������� **���� ���۸�(double buffering)
    */

    // ����ü�� ����
    DXGI_SWAP_CHAIN_DESC tDesc = {};

    tDesc.OutputWindow = m_hwnd;     // ��� ������
    tDesc.Windowed = true;           // â���, ��üȭ�� ���

    tDesc.BufferCount = 1; //���� ���۸�
    tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//�� ���۸� ������ Ÿ������ ���
    tDesc.BufferDesc.Width = (UINT)m_vRenderResolution.x;//�ػ� ����
    tDesc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
    tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//32��Ʈ(8��Ʈ�� 4ä��) ���� ���� (�Ϲ����� 8��Ʈ RGBA ����)
    tDesc.BufferDesc.RefreshRate.Denominator = 1;//�ֻ���(Hz)�� ���� (60Hz�� ����)
    tDesc.BufferDesc.RefreshRate.Numerator = 60;
    tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;//�Ϲ����� ���
    tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    tDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;//���� ������ ������ ����

    tDesc.SampleDesc.Count = 1;//(�⺻��)
    tDesc.SampleDesc.Quality = 0;
    tDesc.Flags = 0;

    // ����ü�� ����
    ComPtr<IDXGIDevice>  pDXGIDevice;
    ComPtr<IDXGIAdapter> pAdapter;
    ComPtr<IDXGIFactory> pFactory;

    HRESULT hr = S_OK;

    hr = m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf());
    hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
    hr = pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

    hr = pFactory->CreateSwapChain(m_device.Get(), &tDesc, m_swapChain.GetAddressOf());

    return hr;
}

int CDevice::CreateView()
{
    return 0;
}

int CDevice::CreateRasterizerState()
{
    return 0;
}

int CDevice::CreateDepthStencilState()
{
    return 0;
}

int CDevice::CreateBlendState()
{
    return 0;
}

int CDevice::CreateSampler()
{
    return 0;
}

