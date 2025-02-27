#include "pch.h"
#include "CServerSession.h"
#include "Enter.pb.h"
#include "CRoomMgr.h"
#include "NetFunc.h"
#include "CServerPacketHandler.h"
#include "Service.h"

CServerSession::CServerSession()
{

}

CServerSession::~CServerSession()
{

}

//로그인 누르고 서버에 접속이 되었다면
void CServerSession::OnConnected()
{
	Protocol::C_ENTER pkt;
	const wstring& strClientName = CRoomMgr::GetInst()->GetClientName();

	pkt.set_name(WstringToString(strClientName));
	shared_ptr<SendBuffer> pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);

	GClientService->GetClientSession()->Send(pSendBuffer);
}

int CServerSession::OnRecvPacket(BYTE* buffer, int len)
{
	shared_ptr<PacketSession> pSession = GetPacketSessionRef();
	CServerPacketHandler::HandlePacket(pSession, buffer, len);

    return len;
}

void CServerSession::OnSend(int len)
{

}

void CServerSession::OnDisConnected()
{
}
