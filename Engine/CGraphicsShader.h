#pragma once
#include "CShader.h"

class CGraphicsShader : public CShader
{
public:
	CGraphicsShader();
	virtual ~CGraphicsShader();

	int CreateVertexShader(const wstring& _strFileName, const string& _strFunctionName);
	int CreateHullShader(const wstring& _strFileName, const string& _strFunctionName);
	int CreateDomainShader(const wstring& _strFileName, const string& _strFunctionName);
	int CreateGeometryShader(const wstring& _strFileName, const string& _strFunctionName);
	int CreatePixelShader(const wstring& _strFileName, const string& _strFunctionName);


public:
	virtual void UpdateData()override;

private:
	//IA
	ComPtr<ID3D11InputLayout> m_inputLayout;
	D3D11_PRIMITIVE_TOPOLOGY m_topology;


	ComPtr<ID3DBlob> m_VSBlob;
	ComPtr<ID3DBlob> m_HSBlob;
	ComPtr<ID3DBlob> m_DSBlob;
	ComPtr<ID3DBlob> m_GSBlob;
	ComPtr<ID3DBlob> m_PSBlob;

	ComPtr<ID3D11VertexShader> m_VS;
	ComPtr<ID3D11HullShader> m_HS;
	ComPtr<ID3D11DomainShader> m_DS;
	ComPtr<ID3D11GeometryShader> m_GS;
	ComPtr<ID3D11PixelShader> m_PS;

	//OM
	DS_TYPE m_eDSType;
	BS_TYPE m_eBSType;
	RS_TYPE m_eRSType;
};

