#pragma once

#include "CTexture.h"

class CResMgr : public CSingleton< CResMgr>
{
public:
	CResMgr();
	virtual ~CResMgr();


public:
	template<typename T>
	std::shared_ptr<T> FindRes(const wstring& _strKey, RESOURCE_TYPE _eType);

	template<typename T>
	void AddRes(const wstring& _strKey, std::shared_ptr<T> _Res, RESOURCE_TYPE _eType);

	template<typename T>
	std::shared_ptr<T> Load(const wstring& _strKey, const wstring& _strPath, RESOURCE_TYPE _eType);

public:
	 std::shared_ptr<CTexture> CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height
		, DXGI_FORMAT _pixelformat, UINT _BindFlag, D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);

	 std::shared_ptr<CTexture> CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D);



private:
	map<wstring, std::shared_ptr<CResource>> m_arrResource[(UINT)RESOURCE_TYPE::END];
};

template<typename T>
inline std::shared_ptr<T> CResMgr::FindRes(const wstring& _strKey, RESOURCE_TYPE _eType)
{
	auto iter = m_arrResource[(UINT)_eType].find(_strKey);
	
	if (iter == m_arrResource[(UINT)_eType].end())
		return nullptr;
	
	return std::static_pointer_cast<T>(iter->second);
}
 
template<typename T>
inline void CResMgr::AddRes(const wstring& _strKey, std::shared_ptr<T> _Res, RESOURCE_TYPE _eType)
{
	assert(FindRes<T>(_strKey, _eType) == nullptr);
	
	m_arrResource[(UINT)_eType].insert(std::make_pair(_strKey, _Res));
	_Res->SetKey(_strKey);
}

template<typename T>
inline std::shared_ptr<T> CResMgr::Load(const wstring& _strKey, const wstring& _strPath, RESOURCE_TYPE _eType)
{
	std::shared_ptr<CResource> pRes = FindRes<T>(_strKey, _eType);
	if (pRes != nullptr)
		return pRes;


	pRes = std::make_shared<T>();
	pRes->SetKey(_strKey);
	pRes->SetRelativePath(_strPath);
	
	if (FAILED(pRes->Load(_strPath)))
	{
		assert(nullptr);
	}
	
	m_arrResource[(UINT)_eType].insert(make_pair(_strKey, pRes));
	return pRes;
}
