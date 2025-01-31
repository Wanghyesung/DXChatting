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
    D3D_DRIVER_TYPE eDriverType = D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE;//Direct3D 애플리케이션에서 사용해야 하는 기본 드라이버입니다

    int iFlag = 0;
#ifdef _DEBUG
    iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

    //device 초기화
    if (FAILED(D3D11CreateDevice(pAdapter, eDriverType, nullptr, iFlag,
        nullptr, 0, D3D11_SDK_VERSION, m_device.GetAddressOf(), &iFeatureLevels,
        m_deviceContex.GetAddressOf())))
    {
        MessageBox(nullptr, L"Device, Context 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }

    if(FAILED(CreateSwapChain()))
    {
        MessageBox(nullptr, L"스왚체인 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }
    

    return true;
}

int CDevice::CreateSwapChain()
{
    /*
    DirectX에서 **스왑 체인(Swap Chain)**은 렌더링 결과를 저장하는 백 버퍼(back buffer)를 관리하는 구조입니다.
    스왑 체인은 CPU가 프레임을 렌더링하고 GPU가 이를 화면에 출력하는 과정에서 **더블 버퍼링(double buffering)
    */

    // 스왚체인 설정
    DXGI_SWAP_CHAIN_DESC tDesc = {};

    tDesc.OutputWindow = m_hwnd;     // 출력 윈도우
    tDesc.Windowed = true;           // 창모드, 전체화면 모드

    tDesc.BufferCount = 1; //더블 버퍼링
    tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//이 버퍼를 렌더링 타겟으로 사용
    tDesc.BufferDesc.Width = (UINT)m_vRenderResolution.x;//해상도 설정
    tDesc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
    tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//32비트(8비트씩 4채널) 색상 포맷 (일반적인 8비트 RGBA 색상)
    tDesc.BufferDesc.RefreshRate.Denominator = 1;//주사율(Hz)을 설정 (60Hz로 설정)
    tDesc.BufferDesc.RefreshRate.Numerator = 60;
    tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;//일반적인 방식
    tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    tDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;//이전 버퍼의 내용을 삭제

    tDesc.SampleDesc.Count = 1;//(기본값)
    tDesc.SampleDesc.Quality = 0;
    tDesc.Flags = 0;

    // 스왚체인 생성
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

