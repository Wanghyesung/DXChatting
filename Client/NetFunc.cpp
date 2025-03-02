#include "pch.h"
#include "NetFunc.h"
#include "Enter.pb.h"
#include "Chatting.pb.h"
#include "CRoomMgr.h"
#include "CServerPacketHandler.h"
#include "Service.h"
#include "CSpeechBar.h"
#include "CRoomMgr.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CResMgr.h"
#include "CLoginUI.h"
#include "CServerSession.h"

void InitChattingBar()
{
	GChattingBar = new CSpeechBar();
	CRoomMgr::GetInst()->AddObject(L"Chatting", LAYER_TYPE::UI, GChattingBar);
	GChattingBar->SetMaxSpeechSize(20);

	CTransform* pTrasnform = new CTransform();
	GChattingBar->SetStaticPos(Vector3{ 100.f,-300.f,-0.2f });
	pTrasnform->SetPostion(Vector3{ 100.f,-300.f,-0.2f });
	pTrasnform->SetScale(Vector3{ 500.f,50.f,1.f });
	GChattingBar->SetComponent(pTrasnform);


	shared_ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DefaultMaterial", RESOURCE_TYPE::MATERIAL);
	shared_ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh", RESOURCE_TYPE::MESH);
	CMeshRender* pMeshRender = new CMeshRender();
	pMeshRender->SetMaterial(pMtrl);
	shared_ptr<CMaterial> pDynamicMtrl = pMeshRender->GetDynamicMaterial();
	pDynamicMtrl->SetTex(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"SpeechBarTex", RESOURCE_TYPE::TEXTURE));
	pMeshRender->SetMaterial(pDynamicMtrl);

	pMeshRender->SetMesh(pMesh);
	GChattingBar->SetComponent(pMeshRender);

	//이벤트 등록
	GChattingBar->SetSendPktFunc(std::bind(SenPkt));
}

void InitLoginBar()
{
	GLoginUI = new CLoginUI();
	GLoginUI->SetName(L"TemObject");
	GLoginUI->SetSpeech(L"채팅방 참가");
	GLoginUI->SetFontSize(50.f);

	//bind 함수를 "고정된 인자"와 함께 저장하여 나중에 실행할 수 있도록 만드는 것.
	GLoginUI->SetMouseClickedFunction(std::bind(ClientLogin)); //콜백함수 connect로 변경

	CRoomMgr::GetInst()->AddObject(L"lobby", LAYER_TYPE::UI, GLoginUI);

	CTransform* pTrasnform = new CTransform();
	pTrasnform->SetPostion(Vector3{ 0.f,100.f,-0.2f });
	pTrasnform->SetScale(Vector3{ 300.f,100.f,1.f });
	GLoginUI->SetComponent(pTrasnform);


	shared_ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"BorderMaterial", RESOURCE_TYPE::MATERIAL);
	shared_ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh", RESOURCE_TYPE::MESH);
	CMeshRender* pMeshRender = new CMeshRender();
	pMeshRender->SetMaterial(pMtrl);
	pMeshRender->SetMesh(pMesh);
	GLoginUI->SetComponent(pMeshRender);

	/*///////////////////////
			SPEECHBAR
	*////////////////////////
	CSpeechBar* pSpeechBar = new CSpeechBar();
	GLoginUI->SetSpeechBar(pSpeechBar);
	pSpeechBar->SetBaseSpeech(L"NAME :");
	pSpeechBar->SetName(L"SpeechBar");
	CRoomMgr::GetInst()->AddObject(L"lobby", LAYER_TYPE::UI, pSpeechBar);
	pSpeechBar->SetFontSize(40.f);

	pTrasnform = new CTransform();
	pSpeechBar->SetStaticPos(Vector3{ 0.f,-100.f,-0.2f });
	pTrasnform->SetScale(Vector3{ 400.f,100.f,1.f });
	pSpeechBar->SetComponent(pTrasnform);

	pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"BorderMaterial", RESOURCE_TYPE::MATERIAL);
	pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh", RESOURCE_TYPE::MESH);
	pMeshRender = new CMeshRender();
	pMeshRender->SetMaterial(pMtrl);
	pMeshRender->SetMesh(pMesh);
	pSpeechBar->SetComponent(pMeshRender);
}

void ClientLogin()
{
	shared_ptr<Session> pSession = GClientService->GetClientSession();
	pSession->Connect();
}


void SenPkt()
{
	Protocol::C_CHATTING pkt;
	const wstring& strname = CRoomMgr::GetInst()->GetClientName();

	pkt.set_text(WstringToString((GChattingBar->GetSpeech())));

	shared_ptr<SendBuffer> pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);

	shared_ptr<CServerSession> pClientSession = static_pointer_cast<CServerSession>(GClientService->GetClientSession());
	pClientSession->Send(pSendBuffer);
}

void Exit()
{
	return;
	//Protocol::C_EXIT pkt;
	//
	//const wstring& strname = CRoomMgr::GetInst()->GetClientName();
	//pkt.set_name(WstringToString(strname));
	//
	//shared_ptr<SendBuffer> pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	//
	//shared_ptr<CServerSession> pClientSession = static_pointer_cast<CServerSession>(GClientService->GetClientSession());
	//pClientSession->SetLastPacket(PACKET_TYPE::C_EXIT);
	//pClientSession->Send(pSendBuffer);
}

wstring StringToWString(const string& str)
{
    size_t llSize = str.size() + 1; //널문자
    std::wstring wstr(llSize, 0);   //크기 미리 할당

    size_t llNumCvt = 0;
    mbstowcs_s(&llNumCvt, &wstr[0], llSize, str.c_str(), llSize - 1);

    wstr.pop_back(); // 널 문자 제거
    return wstr;
}

string WstringToString(const wstring& _str)
{
	int iSize = WideCharToMultiByte(CP_UTF8, 0, _str.c_str(), -1, NULL, 0, NULL, NULL);
	string str(iSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, _str.c_str(), -1, &str[0], iSize, NULL, NULL);
	return str;
}
