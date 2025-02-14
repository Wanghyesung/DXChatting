#include "pch.h"
#include "CMaterial.h"
#include "CConstBuffer.h"
#include "CTexture.h"
#include "CDevice.h"
#include "CGraphicsShader.h"
CMaterial::CMaterial():
	CResource(RESOURCE_TYPE::MATERIAL),
	m_pShader(nullptr),
	m_arrTex{},
	m_mtrlConst{},
	m_eRenderType(RENDER_TYPE::R_OPAQUE)
{

}



CMaterial::~CMaterial()
{

}

void CMaterial::UpdateData()
{
	if (m_pShader == nullptr)
		return;

	m_pShader->UpdateData();

	for (UINT i = 0; i < TEX_PARAM::TEX_END; ++i)
	{
		if (m_arrTex[i] == nullptr)
		{
			m_mtrlConst.arrTex[i] = 0;
			//기존에 다른 오브젝트에서 해당 인덱스 레지스터에 걸어뒀을 수 있음
			CTexture::Clear(i);
			continue;
		}
		else
		{
			m_mtrlConst.arrTex[i] = 1;
			m_arrTex[i]->UpdateData(i,SHADER_FLAG::ALL);
		}
	}

	CConstBuffer* pConstBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pConstBuffer->SetData(&m_mtrlConst, sizeof(tMtrlConst));
	pConstBuffer->UpdateData();
}

void CMaterial::SetMtrlParm(MTRL_PARAM _eParm, void* _src)
{
	switch (_eParm)
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:
		m_mtrlConst.arrInt[_eParm] = *reinterpret_cast<int*>(_src);
		break;

	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
		m_mtrlConst.arrFloat[_eParm - MTRL_PARAM::FLOAT_0] = *reinterpret_cast<float*>(_src);
		break;

	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
		m_mtrlConst.arrV2[_eParm - MTRL_PARAM::VEC2_0] = *reinterpret_cast<Vec2*>(_src);
		break;

	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
		m_mtrlConst.arrV4[_eParm - MTRL_PARAM::VEC4_0] = *reinterpret_cast<Vec4*>(_src);
		break;

	case MAT_0:
	case MAT_1:
	case MAT_2:
	case MAT_3:
		m_mtrlConst.arrMat[_eParm- MTRL_PARAM::MAT_0] = *reinterpret_cast<Matrix*>(_src);
		break;

	}
}
