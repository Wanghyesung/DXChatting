#include "pch.h"
#include "CGraphicsShader.h"
#include "CPathMgr.h"
#include "CDevice.h"
#include "CResMgr.h"

CGraphicsShader::CGraphicsShader():
	m_topology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	m_eRSType(RS_TYPE::CULL_BACK),
	m_eDSType(DS_TYPE::LESS),	
	m_eBSType(BS_TYPE::ALPHA_BLEND)
{

}

CGraphicsShader::~CGraphicsShader()
{

}

int CGraphicsShader::CreateVertexShader(const wstring& _strFileName, const string& _strFunctionName)
{
	// Shader 파일 경로
	wstring strShaderFile = L"C:\\Users\\wangh\\source\\source\\Chatting\\Engine\\";
	CPathMgr::GetInst()->GetCurrentPath();
	strShaderFile += _strFileName;

	// VertexShader Compile
	if (D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		 _strFunctionName.c_str(), "vs_5_0", 0, 0, m_VSBlob.GetAddressOf(), m_errorBlob.GetAddressOf()))
	{
		MessageBoxA(nullptr, (const char*)m_errorBlob->GetBufferPointer()
			, "Vertex Shader Compile Failed!!", MB_OK);
	}

	// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
	HRESULT hr = CDevice::GetInst()->GetDevice()->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize()
		, nullptr, m_VS.GetAddressOf());

	if (FAILED(hr))
	{
		assert(nullptr);
	}

	//inputlayout
	const vector<D3D11_INPUT_ELEMENT_DESC>& vecDesc = CResMgr::GetInst()->GetInputLayoutDesc();
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateInputLayout(vecDesc.data(), (UINT)vecDesc.size(),
		m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), m_inputLayout.GetAddressOf())))
	{
		assert(nullptr);
	}

    return S_OK;
}

int CGraphicsShader::CreateHullShader(const wstring& _strFileName, const string& _strFunctionName)
{
	// Shader 파일 경로
	wstring strShaderFile = CPathMgr::GetInst()->GetCurrentPath();
	strShaderFile += _strFileName;

	// VertexShader Compile
	if (D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_strFunctionName.c_str(), "hs_5_0", 0, 0, m_HSBlob.GetAddressOf(), m_errorBlob.GetAddressOf()))
	{
		MessageBoxA(nullptr, (const char*)m_errorBlob->GetBufferPointer()
			, "Hull Shader Compile Failed!!", MB_OK);
	}

	// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
	HRESULT hr = CDevice::GetInst()->GetDevice()->CreateHullShader(m_HSBlob->GetBufferPointer(), m_HSBlob->GetBufferSize()
		, nullptr, m_HS.GetAddressOf());

	if (FAILED(hr))
	{
		assert(nullptr);
	}

    return S_OK;
}

int CGraphicsShader::CreateDomainShader(const wstring& _strFileName, const string& _strFunctionName)
{
	// Shader 파일 경로
	wstring strShaderFile = CPathMgr::GetInst()->GetCurrentPath();
	strShaderFile += _strFileName;

	// VertexShader Compile
	if (D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_strFunctionName.c_str(), "ds_5_0", 0, 0, m_DSBlob.GetAddressOf(), m_errorBlob.GetAddressOf()))
	{
		MessageBoxA(nullptr, (const char*)m_errorBlob->GetBufferPointer()
			, "Domain Shader Compile Failed!!", MB_OK);
	}

	// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
	HRESULT hr = CDevice::GetInst()->GetDevice()->CreateDomainShader(m_DSBlob->GetBufferPointer(), m_DSBlob->GetBufferSize()
		, nullptr, m_DS.GetAddressOf());

	if (FAILED(hr))
	{
		assert(nullptr);
	}

	return S_OK;
}

int CGraphicsShader::CreateGeometryShader(const wstring& _strFileName, const string& _strFunctionName)
{
	// Shader 파일 경로
	wstring strShaderFile = CPathMgr::GetInst()->GetCurrentPath();
	strShaderFile += _strFileName;

	// VertexShader Compile
	if (D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_strFunctionName.c_str(), "gs_5_0", 0, 0, m_GSBlob.GetAddressOf(), m_errorBlob.GetAddressOf()))
	{
		MessageBoxA(nullptr, (const char*)m_errorBlob->GetBufferPointer()
			, "Geometry Shader Compile Failed!!", MB_OK);
	}

	// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
	HRESULT hr = CDevice::GetInst()->GetDevice()->CreateGeometryShader(m_GSBlob->GetBufferPointer(), m_GSBlob->GetBufferSize()
		, nullptr, m_GS.GetAddressOf());

	if (FAILED(hr))
	{
		assert(nullptr);
	}

	return S_OK;
}

int CGraphicsShader::CreatePixelShader(const wstring& _strFileName, const string& _strFunctionName)
{
	// Shader 파일 경로
	wstring strShaderFile = L"C:\\Users\\wangh\\source\\source\\Chatting\\Engine\\"; //CPathMgr::GetInst()->GetCurrentPath();
	strShaderFile += _strFileName;

	// VertexShader Compile
	if (D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_strFunctionName.c_str(), "ps_5_0", 0, 0, m_PSBlob.GetAddressOf(), m_errorBlob.GetAddressOf()))
	{
		MessageBoxA(nullptr, (const char*)m_errorBlob->GetBufferPointer()
			, "Pixel Shader Compile Failed!!", MB_OK);
	}

	// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
	HRESULT hr = CDevice::GetInst()->GetDevice()->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize()
		, nullptr, m_PS.GetAddressOf());
	
	if (FAILED(hr))
	{
		assert(nullptr);
	}

	return S_OK;
}

void CGraphicsShader::UpdateData()
{
	//IA
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_topology);
	CDevice::GetInst()->GetContext()->IASetInputLayout(m_inputLayout.Get());

	//shader
	CDevice::GetInst()->GetContext()->VSSetShader(m_VS.Get(), nullptr, 0);
	CDevice::GetInst()->GetContext()->HSSetShader(m_HS.Get(), nullptr, 0);
	CDevice::GetInst()->GetContext()->DSSetShader(m_DS.Get(), nullptr, 0);
	CDevice::GetInst()->GetContext()->GSSetShader(m_GS.Get(), nullptr, 0);
	CDevice::GetInst()->GetContext()->PSSetShader(m_PS.Get(), nullptr, 0);

	//RS
	CDevice::GetInst()->GetContext()->RSSetState( CDevice::GetInst()->GetRSState(m_eRSType).Get() );
	
	//OM
	CDevice::GetInst()->GetContext()->OMSetDepthStencilState(CDevice::GetInst()->GetDSState(m_eDSType).Get(), 0);
	CDevice::GetInst()->GetContext()->OMSetBlendState(CDevice::GetInst()->GetBSState(m_eBSType).Get(), Vec4(0.f,0.f,0.f,0.f), 0xffffffff);
}
